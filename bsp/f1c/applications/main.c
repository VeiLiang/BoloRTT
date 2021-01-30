#include "rtthread.h"
#include "drv_clock.h"
#include "drv_gpio.h"
#include <rtdevice.h>
#include <dfs_fs.h>
#include <drv_sdio.h>
#include <drv_fb.h>
#include <stdio.h>
#include "board.h"
#include "UartCursor.h"
#include "LVGL_Interface.h"
#include "demo.h"
#include "drv_tvd.h"
#include "bolo_jpeg.h"
#include "logo_data.h"
#include "drv_audio.h"
#include "InfoNES.h"

int fb_num = 0 ;
unsigned int fb_addr[2] = {0x81800000,0x81A00000};

// #include "test_jpg.h"
static unsigned char jpeg_src[1024*1024]__attribute__ ((aligned (4096)));
unsigned char jpeg_ydst[1920*1080]__attribute__ ((aligned (4096)));
unsigned char jpeg_cdst[1920*1080]__attribute__ ((aligned (4096)));
static unsigned char disp_buf[480*272*4]__attribute__ ((aligned (4096)));

void RefleshLcdWithTVD(unsigned char *ydat,unsigned char * cbcr,int w,int h)
{
	int lcdw = 480;
	int i,j;
	fb_num++;
	uint32_t *Tr=(uint32_t *)fb_addr[fb_num&0x01];
	for ( i= 0; i < 272;i++)
	{
		for (j= 0; j < 480;j++)
		{
			int y = ydat[i*w+j];
			int cb128 =  (int)cbcr[(i/4*2)*(w)+(j/2)*2] - 128;
			int cr128 =  (int)cbcr[(i/4*2)*(w)+j/2*2+1] - 128;
			int rdif = cr128 + ((cr128 * 103) >> 8);
			int invgdif = ((cb128 * 88) >> 8) +((cr128 * 183) >> 8);
			int bdif = cb128 +( (cb128*198) >> 8);
 
			int r =  y+ rdif;
			int g  = y- invgdif;
			int b =  y+ bdif;

			if(r > 255)
			{
				r = 255;
			}
			else if(r < 0)
			{
				r = 0;
			}
			if(g < 0)
			{
				g = 0;
			}
			else if(g > 255)
			{
				g = 255;
			}

			if(b > 255)
			{
				b = 255;
			}
			else if(b < 0)
			{
				b  = 0;
			}
			Tr[i*lcdw + j] = 0xFF000000|((int)r<<16)|((int)g<<8)|((int)b<<0);

		}

	}
	Layer_SetFramBuffer(0,Tr);
}

static void Show_Logo()
{
	int w,h,i,j;

	int BLLevel=0;
	uint32_t *Tr=(uint32_t *)lcd_layer0.vram;
	memcpy(jpeg_src,logo_jpeg,sizeof(logo_jpeg));
	jpeg_decode_main(jpeg_src,sizeof(logo_jpeg),jpeg_ydst,jpeg_cdst,&w,&h);
	w = ((w +31)/32)*32;
	h = ((h +31))/32*32;
	jpeg_yuv_mb32_to_rgb888(jpeg_ydst,jpeg_cdst,disp_buf,w,h,LCD_TYPE_ORDER_RGB);
	for(i = 0 ; i < 272;i++)
	{
		for(j = 0 ; j < 480;j++)
		{
			Tr[j+i*480] = 0xff000000+(disp_buf[(j+i*w)*3+0]<<16)+(disp_buf[(j+i*w)*3+1]<<8)+(disp_buf[(j+i*w)*3+2]<<0);
		}

	}
	//memset(Tr,0x00,480*272*4);
	rt_thread_delay(50);
	for(BLLevel=0;BLLevel<=100;BLLevel++)
	{
		lcd_SetBrightness(BLLevel);
		rt_thread_delay(5);
	}	
}
static void Sys_Servs_Init()
{	
	UartCursor_Init();	
}
static void MainThreadCreat()
{ 
	Lvgl_Init();
}

lcd_layer_Struct lcd_layer1={
	.width=100,
	.height=100,
	.posx=0,
	.posy=0,
	.priority=1,
	.pipe=1,
	.alpha_enable=1,
	.alpha_value=0xA0,
	.vram=(void *)0x81C00000,
	.vram_format=VRAM_FORMAT_RGB565,	
};
lcd_layer_Struct lcd_layer2={
	.width=100,
	.height=100,
	.posx=50,
	.posy=50,
	.priority=2,
	.pipe=0,
	.alpha_enable=1,
	.alpha_value=0xA0,
	.vram=(void *)0x81D00000,
	.vram_format=VRAM_FORMAT_RGB565,	
};
void TestLayer()
{
	uint32_t i;
	for(i=0;i<100*100;i++)
	{
		((uint16_t *)lcd_layer1.vram)[i]=0xFF00;
		((uint16_t *)lcd_layer2.vram)[i]=0x00FF;
	}
	Layer_Config(1,&lcd_layer1);
	Layer_Config(2,&lcd_layer2);
	Layer_SetEnable(1,LAYER_ENABLE);
	Layer_SetEnable(2,LAYER_ENABLE);
	rt_thread_delay(1000);
	lcd_layer1.priority=2;
	lcd_layer2.priority=1;
	Layer_Config(1,&lcd_layer1);
	Layer_Config(2,&lcd_layer2);	
}


int cmd_jpegdec(int argc, char **argv)
{
    if (argc == 1)
    {
        rt_kprintf("Usage: jpegdec  [OPTION] FileName\n");
        rt_kprintf("decode jpeg and disp to lcd.\n");
    }
    else
    {
        int w,h;
		FILE* fd = fopen(argv[1],"rb+");
		rt_kprintf("read jpeg file %s - 0x%x\n",argv[1],fd);
		int len = fread(jpeg_src, 1, sizeof(jpeg_src),fd);
		//memcpy(jpeg_src,TestJpgData,sizeof(TestJpgData));
		// dump_reg_val();
		jpeg_decode_main(jpeg_src,len,jpeg_ydst,jpeg_cdst,&w,&h);
		w = ((w +31)/32)*32;
		h = ((h +31))/32*32;
		uint32_t *Tr=(uint32_t *)fb_addr[fb_num&0x01];
		jpeg_yuv_mb32_to_rgb888(jpeg_ydst,jpeg_cdst,disp_buf,w,h,LCD_TYPE_ORDER_RGB);
		int i,j;
		rt_kprintf("w:%d,h:%d\n",w,h);
		int dw = 480 > w ? w:480;
		int dh = 272 > h ? h:272;
		for(i = 0 ; i < dh;i++)
		{
			for(j = 0 ; j < dw;j++)
			{
				Tr[j+i*480] = 0xff000000+(disp_buf[(j+i*w)*3+2]<<16)+(disp_buf[(j+i*w)*3+1]<<8)+(disp_buf[(j+i*w)*3+0]<<0);
			}

		}
		fclose(fd);
    }

    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_jpegdec, __cmd_jpegdec, decode jpeg and disp to lcd.);

extern unsigned char tvd_y_buf_disp[720*576];
extern unsigned char tvd_c_buf_disp[720*576];
extern const unsigned char nes_rom[786464];
int main(int argc, char **argv)
{
	rt_kprintf("Start...\n");
	rt_kprintf("periph_get_pll_clk:%d\n", periph_get_pll_clk());
    rt_kprintf("cpu_get_clk:%d\n", cpu_get_clk());
    rt_kprintf("ahb_get_clk:%d\n", ahb_get_clk()); 
    rt_kprintf("apb_get_clk:%d\n", apb_get_clk());
	rt_kprintf("heap start:0x%08x\nheap stop : 0x%08x\n", RT_HW_HEAP_BEGIN,RT_HW_HEAP_END);
	jpeg_decoder_init();
	// Sys_Servs_Init();//Services
	Show_Logo();
	HWCursor_Enable(HWC_ENABLE);
	MainThreadCreat();
	demo_create();
	//TestLayer();
	
	rt_kprintf("Video clk %d \n",video_get_pll_clk());
	if(dfs_mount("sd0", "/mmc", "elm", 0,0) < 0)
    {
        rt_kprintf("dfs_mount sd card failed %d\n",rt_get_errno());
    }
	//tv_in_decode_init(0);
	//i2c_f1c100s_init(0);
	//dvp_camera_init(648,480,16);
	//i2c_debugdump();
	f1c100s_audio_init();
	// InfoNES_Load(nes_rom);
	// InfoNES_Main();//program will floop in this function
	nes_load(nes_rom,sizeof(nes_rom));
	while(1)
	{
		rt_thread_delay(20);
	}


	
    return 0;
}
