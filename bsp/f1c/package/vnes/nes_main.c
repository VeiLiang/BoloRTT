#include "nes_common.h"
#include "nes_main.h" 
#include "nes_ppu.h"
#include "nes_mapper.h"
#include "nes_apu.h"
#include "malloc.h" 
#include "string.h"
#include "drv_audio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序移植自网友ye781205的NES模拟器工程
//ALIENTEK STM32开发板
//NES主函数 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/7/1
//版本：V1.0  			  
////////////////////////////////////////////////////////////////////////////////// 	 

u32 RenderType = POST_RENDER;//此处为后面加的，对于有的游戏可能要选择不同的处理方式  VeiLiang
u32 framecnt = 0;	//nes帧计数器 
int MapperNo;			//map编号
int NES_scanline;		//nes扫描线
int VROM_1K_SIZE;
int VROM_8K_SIZE;
u32 NESrom_crc32;

u8 PADdata0 __attribute__ ((aligned (8)));  			//手柄1键值 [7:0]右7 左6 下5 上4 Start3 Select2 B1 A0  
u8 PADdata1 __attribute__ ((aligned (8)));   			//手柄2键值 [7:0]右7 左6 下5 上4 Start3 Select2 B1 A0  
u8 *NES_RAM __attribute__ ((aligned (8)));			//保持1024字节对齐
u8 *NES_SRAM __attribute__ ((aligned (8)));  
NES_header *RomHeader; 	//rom文件头
MAPPER *NES_Mapper;		 
MapperCommRes *MAPx;  


u8* spr_ram __attribute__ ((aligned (8)));			//精灵RAM,256字节
ppu_data* ppu;			//ppu指针
u8* VROM_banks;
u8* VROM_tiles;

apu_t *apu; 			//apu指针
u16 *wave_buffers; 		
u16 *saibuf1; 			//音频缓冲帧,占用内存数 367*4 字节@22050Hz
u16 *saibuf2; 			//音频缓冲帧,占用内存数 367*4 字节@22050Hz

u8* romfile __attribute__ ((aligned (8)));			//nes文件指针,指向整个nes文件的起始地址.
//////////////////////////////////////////////////////////////////////////////////////

 static u8 is_need_free = 0;
//加载ROM
//返回值:0,成功
//    1,内存错误
//    3,map错误
u8 nes_load_rom(void)
{  
    u8* p;  
	u8 i;
	u8 res=0;
	p=(u8*)romfile;	
	if(strncmp((char*)p,"NES",3)==0)
	{  
		RomHeader->ctrl_z=p[3];
		RomHeader->num_16k_rom_banks=p[4];
		RomHeader->num_8k_vrom_banks=p[5];
		RomHeader->flags_1=p[6];
		RomHeader->flags_2=p[7]; 
		if(RomHeader->flags_1&0x04)p+=512;		//有512字节的trainer:
		if(RomHeader->num_8k_vrom_banks>0)		//存在VROM,进行预解码
		{		
			VROM_banks=p+16+(RomHeader->num_16k_rom_banks*0x4000);
#if	NES_RAM_SPEED==1	//1:内存占用小 0:速度快	 
			VROM_tiles=VROM_banks;	 
#else  
			VROM_tiles=mymalloc(SRAMEX,RomHeader->num_8k_vrom_banks*8*1024);//这里可能申请多达1MB内存!!!
			if(VROM_tiles==0)
			{
				VROM_tiles=VROM_banks;//内存不够用的情况下,尝试VROM_titles与VROM_banks共用内存	
				is_need_free = 0;
			}
			else
			{
				is_need_free = 1;
			}
					
			compile(RomHeader->num_8k_vrom_banks*8*1024/16,VROM_banks,VROM_tiles);  
#endif	
		}
		else 
		{
			VROM_banks=mymalloc(SRAMIN,8*1024);
			VROM_tiles=mymalloc(SRAMEX,8*1024);
			is_need_free = 2;
			if(!VROM_banks||!VROM_tiles)res=1;
		}  	
		VROM_1K_SIZE = RomHeader->num_8k_vrom_banks * 8;
		VROM_8K_SIZE = RomHeader->num_8k_vrom_banks;  
		//VROM_8K_SIZE = RomHeader->num_16k_rom_banks * 2;//看论坛里面说改这个能解决一些rom 问题 --By VeiLiang
		MapperNo=(RomHeader->flags_1>>4)|(RomHeader->flags_2&0xf0);
		if(RomHeader->flags_2 & 0x0E)MapperNo=RomHeader->flags_1>>4;//忽略高四位，如果头看起来很糟糕 
		rt_kprintf("use map:%d\r\n",MapperNo);
		if(MapperNo == 74) MapperNo = 4;//mapper74 其实和mapper4差不多
		for(i=0;i<255;i++)  // 查找支持的Mapper号
		{		
			if (MapTab[i]==MapperNo)break;		
			if (MapTab[i]==-1)res=3; 
		} 
		if(res==0)
		{
			switch(MapperNo)
			{
				case 1:  
					MAP1=mymalloc(SRAMIN,sizeof(Mapper1Res)); 
					if(!MAP1)res=1;
					break;
				case 4:  
				case 6: 
				case 16:
				case 17:
				case 18:
				case 19:
				case 21: 
				case 23:
				case 24:
				case 25:
				case 64:
				case 65:
				case 67:
				case 69:
				case 85:
				case 189:
				default:
					MAPx=mymalloc(SRAMIN,sizeof(MapperCommRes)); 
					if(!MAPx)res=1;
					break;  
			}
		}
		else
		{
			rt_kprintf("Mapper%d not suppost!\r\n",MapperNo);
		}
		
	} 
	return res;	//返回执行结果
} 
//释放内存 
void nes_sram_free(void)
{ 
	myfree(SRAMIN,NES_RAM);		
	myfree(SRAMIN,NES_SRAM);	
	myfree(SRAMIN,RomHeader);	
	myfree(SRAMIN,NES_Mapper);
	myfree(SRAMIN,spr_ram);		
	myfree(SRAMIN,ppu);	
	myfree(SRAMIN,apu);	
	myfree(SRAMIN,wave_buffers);	
	myfree(SRAMIN,saibuf1);	
	myfree(SRAMIN,saibuf2);	 
	myfree(SRAMEX,romfile);	  
	//if((VROM_tiles!=VROM_banks)&&VROM_banks&&VROM_tiles)//如果分别为VROM_banks和VROM_tiles申请了内存,则释放
	if(is_need_free>1)//处理一下，避免释放不存在的内存
	{
		myfree(SRAMIN,VROM_banks);
		myfree(SRAMEX,VROM_tiles);		 
	}
	else if(is_need_free)
	{
		myfree(SRAMEX,VROM_tiles);	
	}
	
	switch (MapperNo)//释放map内存
	{
		case 1: 			//释放内存
			myfree(SRAMIN,MAP1);
			break;	 	
		case 4: 
		case 6: 
		case 16:
		case 17:
		case 18:
		case 19:
		case 21:
		case 23:
		case 24:
		case 25:
		case 64:
		case 65:
		case 67:
		case 69:
		case 74:
		case 85:
		case 189:
		default:
			myfree(SRAMIN,MAPx);break;	 		//释放内存 
	}
	NES_RAM=0;	
	NES_SRAM=0;
	RomHeader=0;
	NES_Mapper=0;
	spr_ram=0;
	ppu=0;
	apu=0;
	wave_buffers=0;
	saibuf1=0;
	saibuf2=0;
	romfile=0; 
	VROM_banks=0;
	VROM_tiles=0; 
	MAP1=0;
	MAPx=0;
} 
//为NES运行申请内存
//romsize:nes文件大小
//返回值:0,申请成功
//       1,申请失败
u8 nes_sram_malloc(u32 romsize)
{
	u16 i=0;
	for(i=0;i<256;i++)//为NES_RAM,查找1024对齐的内存 //用不断申请的方式查找
	{
		NES_SRAM=mymalloc(SRAMIN,i*4);  //rtthread 内存申请对齐为4字节
		NES_RAM=mymalloc(SRAMIN,0X800);	//申请2K字节,必须1024字节对齐
		if((u32)NES_RAM%1024)			//不是1024字节对齐
		{
			myfree(SRAMIN,NES_RAM);		//释放内存,然后重新尝试分配
			myfree(SRAMIN,NES_SRAM); 
		}else 
		{
			myfree(SRAMIN,NES_SRAM); 	//释放内存
			break;
		}
	}	 
 	NES_SRAM=mymalloc(SRAMIN,0X2000);
	RomHeader=mymalloc(SRAMIN,sizeof(NES_header));
	NES_Mapper=mymalloc(SRAMIN,sizeof(MAPPER));
	spr_ram=mymalloc(SRAMIN,0X100);		
	ppu=mymalloc(SRAMIN,sizeof(ppu_data));  
	apu=mymalloc(SRAMIN,sizeof(apu_t));		//sizeof(apu_t)=  12588
	wave_buffers=mymalloc(SRAMIN,APU_PCMBUF_SIZE*2);
	saibuf1=mymalloc(SRAMIN,APU_PCMBUF_SIZE*4+10);
	saibuf2=mymalloc(SRAMIN,APU_PCMBUF_SIZE*4+10);
 	romfile=mymalloc(SRAMEX,romsize);			//申请游戏rom空间,等于nes文件大小 
	if(romfile==NULL)//内存不够?释放主界面占用内存,再重新申请
	{
		//spb_delete();//释放SPB占用的内存
			rt_kprintf("romfile malloc failed\r\n");
		romfile=mymalloc(SRAMEX,romsize);//重新申请
	}
	if(i==64||!NES_RAM||!NES_SRAM||!RomHeader||!NES_Mapper||!spr_ram||!ppu||!apu||!wave_buffers||!saibuf1||!saibuf2||!romfile)
	{
		rt_kprintf("nes mem malloc failed\r\n");
		nes_sram_free();
		return 1;
	}
	memset(NES_SRAM,0,0X2000);				//清零
	memset(RomHeader,0,sizeof(NES_header));	//清零
	memset(NES_Mapper,0,sizeof(MAPPER));	//清零
	memset(spr_ram,0,0X100);				//清零
	memset(ppu,0,sizeof(ppu_data));			//清零
	memset(apu,0,sizeof(apu_t));			//清零
	memset(wave_buffers,0,APU_PCMBUF_SIZE*2);//清零
	memset(saibuf1,0,APU_PCMBUF_SIZE*4+10);	//清零
	memset(saibuf2,0,APU_PCMBUF_SIZE*4+10);	//清零
	memset(romfile,0,romsize);				//清零 
	return 0;
} 
//开始nes游戏
//pname:nes游戏路径
//返回值:
//0,正常退出
//1,内存错误
//2,文件错误
//3,不支持的map
u8 nes_load( unsigned char *rom,int len)
{
	u8 res=0;  
	res=nes_sram_malloc(len);			//申请内存 
	if(res==0)
	{
		memcpy(romfile,rom,len);
		NESrom_crc32=get_crc32(romfile+16, len-16);//获取CRC32的值	
		res=nes_load_rom();						//加载ROM
		if(res==0) 					
		{   
			
			cpu6502_init();						//初始化6502,并复位	 
			Mapper_Init();						//map初始化 	
			PPU_reset();						//ppu复位
			apu_init(); 						//apu初始化
			nes_sound_open(APU_PCMBUF_SIZE,APU_SAMPLE_RATE) ;
			nes_emulate_frame();				//进入NES模拟器主循环 
			nes_sound_close();					//关闭声音输出
		}
	}
	nes_sram_free();	//释放内存
	return res;
}  
u16 nes_xoff=0;	//显示在x轴方向的偏移量(实际显示宽度=256-2*nes_xoff)
u16 nes_yoff=0;	//显示在y轴方向的偏移量

//RGB屏需要的3个参数
//扩大4倍,参数计算方法(800*480为例):
//offset=lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-(i-sx)*2-1)+nes_yoff+LineNo) 
//offset=2*(800*(480+(sx-i)*2-1)+nes_yoff+LineNo)
//      =1600*(480+(sx-i)*2-1)+2*nes_yoff+LineNo*2
//      =766400+3200*(sx-i)+2*nes_yoff+LineNo*2 
//nes_rgb_parm1=766400
//nes_rgb_parm2=3200
//nes_rgb_parm3=nes_rgb_parm2/2

//不扩大,参数计算方法(480*272为例):
//offset=lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-(i-sx)-1)+nes_yoff+LineNo*2) 
//offset=2*(480*(272+sx-i-1)+nes_yoff+LineNo*2)
//      =960*(272+sx-i-1)+2*nes_yoff+LineNo*4
//      =260160+960*(sx-i)+2*nes_yoff+LineNo*4 
//nes_rgb_parm1=260160
//nes_rgb_parm2=960
//nes_rgb_parm3=nes_rgb_parm2/2

u32 nes_rgb_parm1;
u16 nes_rgb_parm2;
u16 nes_rgb_parm3;

extern void KEYBRD_FCPAD_Decode(uint8_t *fcbuf,uint8_t mode);
//读取游戏手柄数据
void nes_get_gamepadval(void)
{  
	// u8 *pt;
	// while((usbx.bDeviceState&0XC0)==0X40)//USB设备插入了,但是还没连接成功,猛查询.
	// {
	// 	usbapp_pulling();	//轮询处理USB事务
	// }
	// usbapp_pulling();		//轮询处理USB事务
	// if(usbx.hdevclass==4)	//USB游戏手柄
	// {	
	// 	PADdata0=fcpad.ctrlval;
	// 	PADdata1=0;
	// }else if(usbx.hdevclass==3)//USB键盘模拟手柄
	// {
	// 	KEYBRD_FCPAD_Decode(pt,0);
	// 	PADdata0=fcpad.ctrlval;
	// 	PADdata1=fcpad1.ctrlval; 
	// }	
}    
//nes模拟器主循环
void nes_emulate_frame(void)
{  
	u8 nes_frame=0; 
	while(1)
	{	
		//rt_kprintf("%d\r\n",nes_frame);
		// LINES 0-239
		PPU_start_frame();
		for(NES_scanline = 0; NES_scanline< 240; NES_scanline++)
		{
			run6502(113*256);
			NES_Mapper->HSync(NES_scanline);

			if(nes_frame==0)scanline_draw(NES_scanline);
			else do_scanline_and_dont_draw(NES_scanline); 
		}  
#ifdef FRAME_IRQ	  
	  if(!(frame_irq_enabled & 0xC0))
	  {
		 CPU_IRQ;//cpu->DoPendingIRQ();
	  }
#endif	 

		NES_Mapper->VSync();
		NES_scanline=240;
		run6502(113*256);//运行1线
		NES_Mapper->HSync(NES_scanline); 

		if(RenderType < POST_RENDER)//这个也是根据实际rom区别的 
		{ 
			NES_scanline=240;  //243??    //冒险岛2，马戏团不运行这线
			run6502(113*256);//运行1线
			NES_Mapper->HSync(NES_scanline);
			//
		}

		start_vblank(); 
		if(NMI_enabled()) 
		{ 
			cpunmi=1;
			if(RenderType >= POST_RENDER)//有的要运行,有的rom不要 
			{
				run6502(7*256);//运行中断
			}
			
		}

		
		// LINES 242-261    
		for(NES_scanline=241;NES_scanline<262;NES_scanline++)
		{
			run6502(113*256);	  
			NES_Mapper->HSync(NES_scanline);		  
		}	   
		end_vblank(); 



		nes_get_gamepadval();//每3帧查询一次USB
		apu_soundoutput();//输出游戏声音	 
		framecnt++; 	
		nes_frame++;
		if(nes_frame>NES_SKIP_FRAME)nes_frame=0;//跳帧 
	}
}
//在6502.s里面被调用
void debug_6502(u16 reg0,u8 reg1)
{
	rt_kprintf("6502 error:%x,%d\r\n",reg0,reg1);
}
////////////////////////////////////////////////////////////////////////////////// 	 
//nes,音频输出支持部分

//NES打开音频输出
int nes_sound_open(int samples_per_sync,int sample_rate) 
{
 	f1c100s_audio_config(1,16,sample_rate);
	f1c100s_audio_open(samples_per_sync*2);
	return 1;
}
//NES关闭音频输出
void nes_sound_close(void) 
{ 

} 
//NES音频输出到SAI缓存
void nes_apu_fill_buffer(int samples,u16* wavebuf)
{	
 	audio_pcm_play((unsigned char*)wavebuf,APU_PCMBUF_SIZE*2);
} 



















