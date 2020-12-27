#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <reset-f1c100s.h>
#include <drv_tcon.h>
#include <drv_debe.h>
#include <drv_ccu.h>
#include <io.h>
#include <string.h>
#include "rtthread.h"
#include "drv_clock.h"
#include "drv_gpio.h"
#include "drv_pwm.h"
#include "drv_fb.h"
#include "cursordat.h"
#define gate_clk_enable(virt, shift, invert) write32(virt, (read32(virt) & ~(0x1 << shift)) | ((invert ? 0x0 : 0x1) << shift))
#define gate_clk_disable(virt, shift, invert) write32(virt, (read32(virt) & ~(0x1 << shift)) | ((invert ? 0x1 : 0x0) << shift))
lcd_cfg_Struct lcd_cfg={
	.pixel_clock_hz=9210200,
	.width=PHY_LCD_WIDTH,
	.height=PHY_LCD_HEIGHT,
	.h_front_porch=4,
	.h_back_porch=8,
	.h_sync_len=41,
	.v_front_porch=4,
	.v_back_porch=2,
	.v_sync_len=10,
	.h_sync_active=false,
	.v_sync_active=false,
	.den_active=false,
	.clk_active=false,
	.line_per_pixel=18,
	.rstdefe=46,
	.rstdebe=44,
	.rsttcon=36,
	.backcolor=0x00000000,	
} ;
lcd_layer_Struct lcd_layer0={
	.width=PHY_LCD_WIDTH,
	.height=PHY_LCD_HEIGHT,
	.posx=0,
	.posy=0,
	.priority=0,
	.pipe=0,
	.alpha_enable=1,
	.alpha_value=255,
	.vram=(void *)0x81800000,
	.vram_format = VRAM_FORMAT_ARGB,//VRAM_FORMAT_RGB888,//VRAM_FORMAT_RGB565,	
};
static pwm_t led_pwm_bl =
        {
            .virt = 0x01c21000,
            .duty = 0,
            .period = 1000000,
            .channel = 1,
            .polarity = true,
            .pwm_port = GPIO_PORT_E,
            .pwm_pin = 6,
            .pwm_pin_cfg = IO_FUN_2,
        };
reset_f1c100s_t reset_1 = 
{
	.virt = 0x01c202c0,
	.base = 0,
	.nreset = 32,
};
reset_f1c100s_t reset_2 = 
{
	.virt = 0x01c202c4,
	.base = 32,
	.nreset = 32,
};
reset_f1c100s_t reset_3 = 
{
	.virt = 0x01c202d0,
	.base = 64,
	.nreset = 32,
};
inline static void fb_f1c100s_cfg_gpios(enum gpio_port port, enum gpio_pin pin_start, enum gpio_pin pin_end, uint8_t cfg, enum gpio_pull pull, enum gpio_drv_level drv)
{
	enum gpio_pin pin;
	for(pin = pin_start; pin < pin_end; pin++)
	{
		gpio_set_func(port,pin,cfg);
		gpio_set_pull_mode(port, pin,pull);
		gpio_set_drive_level(port, pin,drv);
	}
}
static void lcd_pin_Init()
{
	fb_f1c100s_cfg_gpios(GPIO_PORT_D, 0, 22, IO_FUN_1, PULL_DISABLE, DRV_LEVEL_3);
}
inline static uint64_t f1c100s_tcon_clk_get_rate()
{	
	return video_get_pll_clk();
}
inline static void f1c100s_clk_defe_enable()
{
	gate_clk_enable(0x01c2010c, 31, false);
	gate_clk_enable(0x01c20064, 14, false);
}
inline static void f1c100s_clk_debe_enable()
{
	gate_clk_enable(0x01c20104, 31, false);
	gate_clk_enable(0x01c20064, 12, false);
}
inline static void f1c100s_clk_tcon_enable()
{
	gate_clk_enable(0x01c20118, 31, false);
	gate_clk_enable(0x01c20064, 4, false);
}
static void clk_divider_set_rate(uint32_t virt, uint32_t width, uint32_t shift, bool onebased, uint64_t prate, uint64_t rate)
{
	uint32_t mask = ((1 << (width)) - 1);
	uint32_t div;
	uint32_t val;

	if(rate == 0)
		rate = prate;

	div = prate / rate;
	if(onebased)
		div--;
	if(div > mask)
		div = mask;

	val = read32(virt);
	val &= ~(mask << shift);
	val |= div << shift;
	write32(virt, val);
}

static void clk_mux_set_parent(uint32_t virt, uint32_t width, uint32_t shift, uint32_t parent_val)
{
	uint32_t val;
	val = read32(virt);
	val &= ~(((1 << width) - 1) << shift);
	val |= parent_val << shift;
	write32(virt, val);
}
static void f1c100s_clk_debe_init(void)
{
	clk_mux_set_parent(0x01c20104, 3, 24, 0);
	clk_divider_set_rate(0x01c20104, 4, 0, true, 24000000, video_get_pll_clk());
}
static void f1c100s_clk_defe_init(void)
{
	clk_mux_set_parent(0x01c2010c, 3, 24, 0);
	clk_divider_set_rate(0x01c2010c, 4, 0, true, 24000000, video_get_pll_clk());
}
inline static void f1c100s_tcon_disable()
{
	struct f1c100s_tcon_reg_t * tcon = (struct f1c100s_tcon_reg_t *)F1C100S_TCON_BASE;
	uint32_t val;

	write32((uint32_t)&tcon->ctrl, 0);
	write32((uint32_t)&tcon->int0, 0);

	val = read32((uint32_t)&tcon->tcon0_dclk);
	val &= ~(0xf << 28);
	write32((uint32_t)&tcon->tcon0_dclk, val);

	write32((uint32_t)&tcon->tcon0_io_tristate, 0xffffffff);
	write32((uint32_t)&tcon->tcon1_io_tristate, 0xffffffff);
}

static void f1c100s_debe_set_mode(lcd_cfg_Struct * pdat)
{
	struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)F1C100S_DEBE_BASE;
	uint32_t val;

	val = read32((uint32_t)&debe->mode);
	val |= (1 << 0);
	write32((uint32_t)&debe->mode, val);

	write32((uint32_t)&debe->disp_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));

	Layer_Config(0,&lcd_layer0);
	
	write32((uint32_t)&debe->backcolor, pdat->backcolor);
	
	val = read32((uint32_t)&debe->reg_ctrl);
	val |= (1 << 0);
	write32((uint32_t)&debe->reg_ctrl, val);

	val = read32((uint32_t)&debe->mode);
	val |= (1 << 1);
	write32((uint32_t)&debe->mode, val);
}
static void f1c100s_tcon_set_mode(lcd_cfg_Struct * pdat)
{
	struct f1c100s_tcon_reg_t * tcon = (struct f1c100s_tcon_reg_t *)F1C100S_TCON_BASE;
	int32_t bp, total;
	uint32_t val;

	val = read32((uint32_t)&tcon->ctrl);
	val &= ~(0x1 << 0);
	write32((uint32_t)&tcon->ctrl, val);

	val = (pdat->v_front_porch + pdat->v_back_porch + pdat->v_sync_len);
	write32((uint32_t)&tcon->tcon0_ctrl, (1 << 31) | ((val & 0x1f) << 4));
	
	val = f1c100s_tcon_clk_get_rate() / pdat->pixel_clock_hz;
	write32((uint32_t)&tcon->tcon0_dclk, (0xf << 28) | (val << 0));
	write32((uint32_t)&tcon->tcon0_timing_active, ((pdat->width - 1) << 16) | ((pdat->height - 1) << 0));

	bp = pdat->h_sync_len + pdat->h_back_porch;
	total = pdat->width + pdat->h_front_porch + bp;
	write32((uint32_t)&tcon->tcon0_timing_h, ((total - 1) << 16) | ((bp - 1) << 0));
	bp = pdat->v_sync_len + pdat->v_back_porch;
	total = pdat->height + pdat->v_front_porch + bp;
	write32((uint32_t)&tcon->tcon0_timing_v, ((total * 2) << 16) | ((bp - 1) << 0));
	write32((uint32_t)&tcon->tcon0_timing_sync, ((pdat->h_sync_len - 1) << 16) | ((pdat->v_sync_len - 1) << 0));

	write32((uint32_t)&tcon->tcon0_hv_intf, 0);
	write32((uint32_t)&tcon->tcon0_cpu_intf, 0);

	if(pdat->line_per_pixel == 18 || pdat->line_per_pixel == 16)
	{
		write32((uint32_t)&tcon->tcon0_frm_seed[0], 0x11111111);
		write32((uint32_t)&tcon->tcon0_frm_seed[1], 0x11111111);
		write32((uint32_t)&tcon->tcon0_frm_seed[2], 0x11111111);
		write32((uint32_t)&tcon->tcon0_frm_seed[3], 0x11111111);
		write32((uint32_t)&tcon->tcon0_frm_seed[4], 0x11111111);
		write32((uint32_t)&tcon->tcon0_frm_seed[5], 0x11111111);
		write32((uint32_t)&tcon->tcon0_frm_table[0], 0x01010000);
		write32((uint32_t)&tcon->tcon0_frm_table[1], 0x15151111);
		write32((uint32_t)&tcon->tcon0_frm_table[2], 0x57575555);
		write32((uint32_t)&tcon->tcon0_frm_table[3], 0x7f7f7777);
		write32((uint32_t)&tcon->tcon0_frm_ctrl, (pdat->line_per_pixel == 18) ? ((1 << 31) | (0 << 4)) : ((1 << 31) | (5 << 4)));
	}

	val = (1 << 28);
	if(!pdat->h_sync_active)
		val |= (1 << 25);
	if(!pdat->h_sync_active)
		val |= (1 << 24);
	write32((uint32_t)&tcon->tcon0_io_polarity, val);
	write32((uint32_t)&tcon->tcon0_io_tristate, 0);
}
inline static void f1c100s_tcon_enable()
{
	struct f1c100s_tcon_reg_t * tcon = (struct f1c100s_tcon_reg_t *)F1C100S_TCON_BASE;
	uint32_t val;

	val = read32((uint32_t)&tcon->ctrl);
	val |= (1 << 31);
	write32((uint32_t)&tcon->ctrl, val);
}
int board_lcd_init()
{
	f1c100s_clk_defe_enable();
	f1c100s_clk_debe_enable();
	f1c100s_clk_tcon_enable();
	reset_f1c100s_deassert(&reset_2, lcd_cfg.rstdefe - reset_2.base);
	reset_f1c100s_deassert(&reset_2, lcd_cfg.rstdebe - reset_2.base);
	reset_f1c100s_deassert(&reset_2, lcd_cfg.rsttcon - reset_2.base);
	for(int i = 0x0800; i < 0x1000; i += 4)
		write32(F1C100S_DEBE_BASE + i, 0);
	f1c100s_clk_debe_init();
	f1c100s_clk_defe_init();
	lcd_pin_Init();
	f1c100s_tcon_disable();
	f1c100s_debe_set_mode(&lcd_cfg);
	f1c100s_tcon_set_mode(&lcd_cfg);
	f1c100s_tcon_enable();
	
	Layer_SetEnable(0,LAYER_ENABLE);
	HWCursor_Init();
	pwm_f1c100s_init(&led_pwm_bl);
	return 0;
}
INIT_DEVICE_EXPORT(board_lcd_init);

void lcd_SetBrightness(int32_t brightness)
{
	pwm_f1c100s_set_duty(&led_pwm_bl, brightness);
}
/*Layer Function*/
void Layer_Config(uint8_t LayerNum,lcd_layer_Struct * pdat)
{
	struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)F1C100S_DEBE_BASE;
	write32((uint32_t)&debe->layer0_size+4*LayerNum, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
	write32((uint32_t)&debe->layer0_pos+4*LayerNum, ((pdat->posy) << 16) | (((pdat->posx)) << 0));
	write32((uint32_t)&debe->layer0_addr_low32b+4*LayerNum, (uint32_t)(pdat->vram) << 3);
	write32((uint32_t)&debe->layer0_addr_high4b+4*LayerNum, (uint32_t)(pdat->vram) >> 29);
	if(pdat->vram_format==VRAM_FORMAT_ARGB)
	{
		write32((uint32_t)&debe->layer0_attr1_ctrl+4*LayerNum, (0x09 << 8)|(FB_RGB_ORDER<<2));
		write32((uint32_t)&debe->layer0_stride+4*LayerNum, pdat->width*32);
	}
	else if(pdat->vram_format==VRAM_FORMAT_RGB565)
	{
		write32((uint32_t)&debe->layer0_attr1_ctrl+4*LayerNum, (0x05 << 8)|(FB_RGB_ORDER<<2));
		write32((uint32_t)&debe->layer0_stride+4*LayerNum, pdat->width*16);
	}
	else 
	{
		write32((uint32_t)&debe->layer0_attr1_ctrl+4*LayerNum, (0x0B << 8) |(FB_RGB_ORDER<<2));
		write32((uint32_t)&debe->layer0_stride+4*LayerNum, pdat->width*24);	
	}
	write32((uint32_t)&debe->layer0_attr0_ctrl+4*LayerNum,(pdat->alpha_value<<24)|(pdat->pipe<<15)|(pdat->priority<<10)|(pdat->alpha_enable<<0));				
}
void Layer_SetPos(uint8_t LayerNum,uint32_t px,uint32_t py)
{
	struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)F1C100S_DEBE_BASE;
	write32((uint32_t)&debe->layer0_pos+4*LayerNum, (py << 16) | (px << 0));
}
void Layer_SetAlpha(uint8_t LayerNum,uint32_t Alpha)
{
	uint32_t val;
	struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)F1C100S_DEBE_BASE;
	val = read32((uint32_t)&debe->layer0_attr0_ctrl+4*LayerNum);	
	val &=0x00ffffff;
	val|=(Alpha<<24);
	write32((uint32_t)&debe->layer0_attr0_ctrl+4*LayerNum,val);
}
void Layer_SetFramBuffer(uint8_t LayerNum,uint32_t BufferAddr)
{
	struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)F1C100S_DEBE_BASE;
	write32((uint32_t)&debe->layer0_addr_low32b+4*LayerNum, BufferAddr<< 3);
	write32((uint32_t)&debe->layer0_addr_high4b+4*LayerNum, BufferAddr>> 29);	
}
void Layer_SetEnable(uint8_t LayerNum,enum LAYER_ENABLE Enable)
{
	uint32_t *p=(uint32_t *)(F1C100S_DEBE_BASE+0x800);
	if(LayerNum>3) return;
	if(Enable)
	{
		*p|=1<<(8+LayerNum);
	}
	else
	{
		*p&=~(1<<(8+LayerNum));
	}
}
//HWC
void HWCursor_SetPos(uint32_t px,uint32_t py)
{	
	*(uint32_t *)(F1C100S_DEBE_BASE+0x8D8)=(py<<16)|(px<<0);
}
void HWCursor_Enable(enum HWC_ENABLE Enable)
{	
	if(Enable)
	{
		*(uint32_t *)(F1C100S_DEBE_BASE+0x800)|=0x00010000;
	}
	else
	{
		*(uint32_t *)(F1C100S_DEBE_BASE+0x800)&=0xFFFEFFFF;
	}
}
void HWCursor_SetColorTable(uint32_t *ColorTable,uint32_t Len)
{
	uint32_t *p=(uint32_t *)(F1C100S_DEBE_BASE+0x1600);
	uint32_t i=0;
	if(Len>64) return;
	for(i=0;i<Len;i++)
	{
		*p=*ColorTable;
		p++;
		ColorTable++;
	}
}
void HWCursor_SetColorIndex(uint32_t *CIndex,uint32_t Len)
{
	uint32_t *p=(uint32_t *)(F1C100S_DEBE_BASE+0x1400);
	uint32_t i=0;
	if(Len>512) return;
	for(i=0;i<Len;i++)
	{
		*p=*CIndex;
		p++;
		CIndex++;
	}	
} 
void HWCursor_Init()
{
	HWCursor_SetColorTable((uint32_t *)HWC_ColorTab,sizeof(HWC_ColorTab)/4);
	HWCursor_SetColorIndex((uint32_t *)HWC_ColorIndex,sizeof(HWC_ColorIndex)/4);
	HWCursor_SetPos(lcd_cfg.width/2,lcd_cfg.height/2);
	*(uint32_t *)(F1C100S_DEBE_BASE+0x8E0)=0x02;
}