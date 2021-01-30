#ifndef __NES_MAIN_H
#define __NES_MAIN_H
#include "nes_common.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序移植自网友ye781205的NES模拟器工程
//ALIENTEK STM32开发板
//NES主函数 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/7/1
//版本：V1.0  			  
////////////////////////////////////////////////////////////////////////////////// 	 
 
#define NES_SKIP_FRAME 	2		//定义模拟器跳帧数,默认跳2帧
 
 
#define INLINE 	static inline
#define int8 	char
#define int16 	short
#define int32 	int
#define uint8 	unsigned char
#define uint16 	unsigned short
#define uint32 	unsigned int
#define boolean uint8 

enum RENDERMETHOD { 
	POST_ALL_RENDER = 0, // 执行扫描线的指令后，进行渲染
	PRE_ALL_RENDER  = 1, // 执行渲染后，执行扫描线的指令
	POST_RENDER     = 2, // 执行显示期间的命令后
	PRE_RENDER      = 3, // 执行渲染后，执行显示期间的命令
	TILE_RENDER     = 4  // 平铺渲染
};

//nes信息头结构体
typedef struct
{
	unsigned char id[3]; // 'NES'
	unsigned char ctrl_z; // control-z
	unsigned char num_16k_rom_banks;
	unsigned char num_8k_vrom_banks;
	unsigned char flags_1;
	unsigned char flags_2;
	unsigned char reserved[8];
}NES_header;   

extern u8 nes_frame_cnt;		//nes帧计数器
extern int MapperNo;			//map编号
extern int NES_scanline;		//扫描线
extern NES_header *RomHeader;	//rom文件头 
extern int VROM_1K_SIZE;
extern int VROM_8K_SIZE; 
extern u8 cpunmi;  				//cpu中断标志  在 6502.s里面
extern u8 cpuirq;			
extern u8 PADdata;   			//手柄1键值 
extern u8 PADdata1;   			//手柄1键值 
extern u8 lianan_biao;			//连按标志 
#define  CPU_NMI  cpunmi=1;
#define  CPU_IRQ  cpuirq=1;
#define  NES_RAM_SPEED	0 	 	//1:内存占用小  0:速度快


void cpu6502_init(void);		//在 cart.s
void run6502(u32); 		   		//在 6502.s 
u8 nes_load_rom(void);
void nes_sram_free(void);
u8 nes_sram_malloc(u32 romsize);
u8 nes_load( unsigned char *rom,int len);
void nes_set_window(void);
void nes_get_gamepadval(void);
void nes_emulate_frame(void);
void debug_6502(u16 reg0,u8 reg1);

void nes_sai_dma_tx_callback(void);
int nes_sound_open(int samples_per_sync,int sample_rate);
void nes_sound_close(void);
void nes_apu_fill_buffer(int samples,u16* wavebuf); 

extern u32 get_crc32(u8* buf, u32 len);
#endif







