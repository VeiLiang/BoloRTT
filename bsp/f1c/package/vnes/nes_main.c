#include "nes_common.h"
#include "nes_main.h" 
#include "nes_ppu.h"
#include "nes_mapper.h"
#include "nes_apu.h"
#include "malloc.h" 
#include "string.h"
#include "drv_audio.h"
#include "drv_input.h"

//////////////////////////////////////////////////////////////////////////////////	 
//��������ֲ������ye781205��NESģ��������
//ALIENTEK STM32������
//NES������ ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/7/1
//�汾��V1.0  			  
////////////////////////////////////////////////////////////////////////////////// 	 

u32 RenderType = POST_RENDER;//�˴�Ϊ����ӵģ������е���Ϸ����Ҫѡ��ͬ�Ĵ���ʽ  VeiLiang
u32 framecnt = 0;	//nes֡������ 
int MapperNo;			//map���
int NES_scanline;		//nesɨ����
int VROM_1K_SIZE;
int VROM_8K_SIZE;
u32 NESrom_crc32;

u8 PADdata0 __attribute__ ((aligned (8)));  			//�ֱ�1��ֵ [7:0]��7 ��6 ��5 ��4 Start3 Select2 B1 A0  
u8 PADdata1 __attribute__ ((aligned (8)));   			//�ֱ�2��ֵ [7:0]��7 ��6 ��5 ��4 Start3 Select2 B1 A0  
u8 *NES_RAM __attribute__ ((aligned (8)));			//����1024�ֽڶ���
u8 *NES_SRAM __attribute__ ((aligned (8)));  
NES_header *RomHeader; 	//rom�ļ�ͷ
MAPPER *NES_Mapper;		 
MapperCommRes *MAPx;  


u8* spr_ram __attribute__ ((aligned (8)));			//����RAM,256�ֽ�
ppu_data* ppu;			//ppuָ��
u8* VROM_banks;
u8* VROM_tiles;

apu_t *apu; 			//apuָ��
u16 *wave_buffers; 		
u16 *saibuf1; 			//��Ƶ����֡,ռ���ڴ��� 367*4 �ֽ�@22050Hz
u16 *saibuf2; 			//��Ƶ����֡,ռ���ڴ��� 367*4 �ֽ�@22050Hz

u8* romfile __attribute__ ((aligned (8)));			//nes�ļ�ָ��,ָ������nes�ļ�����ʼ��ַ.
//////////////////////////////////////////////////////////////////////////////////////

 static u8 is_need_free = 0;
//����ROM
//����ֵ:0,�ɹ�
//    1,�ڴ����
//    3,map����
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
		if(RomHeader->flags_1&0x04)p+=512;		//��512�ֽڵ�trainer:
		if(RomHeader->num_8k_vrom_banks>0)		//����VROM,����Ԥ����
		{		
			VROM_banks=p+16+(RomHeader->num_16k_rom_banks*0x4000);
#if	NES_RAM_SPEED==1	//1:�ڴ�ռ��С 0:�ٶȿ�	 
			VROM_tiles=VROM_banks;	 
#else  
			VROM_tiles=mymalloc(SRAMEX,RomHeader->num_8k_vrom_banks*8*1024);//�������������1MB�ڴ�!!!
			if(VROM_tiles==0)
			{
				VROM_tiles=VROM_banks;//�ڴ治���õ������,����VROM_titles��VROM_banks�����ڴ�	
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
		//VROM_8K_SIZE = RomHeader->num_16k_rom_banks * 2;//����̳����˵������ܽ��һЩrom ���� --By VeiLiang
		MapperNo=(RomHeader->flags_1>>4)|(RomHeader->flags_2&0xf0);
		if(RomHeader->flags_2 & 0x0E)MapperNo=RomHeader->flags_1>>4;//���Ը���λ�����ͷ����������� 
		rt_kprintf("use map:%d\r\n",MapperNo);
		if(MapperNo == 74) MapperNo = 4;//mapper74 ��ʵ��mapper4���
		for(i=0;i<255;i++)  // ����֧�ֵ�Mapper��
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
	return res;	//����ִ�н��
} 
//�ͷ��ڴ� 
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
	//if((VROM_tiles!=VROM_banks)&&VROM_banks&&VROM_tiles)//����ֱ�ΪVROM_banks��VROM_tiles�������ڴ�,���ͷ�
	if(is_need_free>1)//����һ�£������ͷŲ����ڵ��ڴ�
	{
		myfree(SRAMIN,VROM_banks);
		myfree(SRAMEX,VROM_tiles);		 
	}
	else if(is_need_free)
	{
		myfree(SRAMEX,VROM_tiles);	
	}
	
	switch (MapperNo)//�ͷ�map�ڴ�
	{
		case 1: 			//�ͷ��ڴ�
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
			myfree(SRAMIN,MAPx);break;	 		//�ͷ��ڴ� 
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
//ΪNES���������ڴ�
//romsize:nes�ļ���С
//����ֵ:0,����ɹ�
//       1,����ʧ��
u8 nes_sram_malloc(u32 romsize)
{
	u16 i=0;
	for(i=0;i<256;i++)//ΪNES_RAM,����1024������ڴ� //�ò�������ķ�ʽ����
	{
		NES_SRAM=mymalloc(SRAMIN,i*4);  //rtthread �ڴ��������Ϊ4�ֽ�
		NES_RAM=mymalloc(SRAMIN,0X800);	//����2K�ֽ�,����1024�ֽڶ���
		if((u32)NES_RAM%1024)			//����1024�ֽڶ���
		{
			myfree(SRAMIN,NES_RAM);		//�ͷ��ڴ�,Ȼ�����³��Է���
			myfree(SRAMIN,NES_SRAM); 
		}else 
		{
			myfree(SRAMIN,NES_SRAM); 	//�ͷ��ڴ�
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
 	romfile=mymalloc(SRAMEX,romsize);			//������Ϸrom�ռ�,����nes�ļ���С 
	if(romfile==NULL)//�ڴ治��?�ͷ�������ռ���ڴ�,����������
	{
		//spb_delete();//�ͷ�SPBռ�õ��ڴ�
			rt_kprintf("romfile malloc failed\r\n");
		romfile=mymalloc(SRAMEX,romsize);//��������
	}
	if(i==64||!NES_RAM||!NES_SRAM||!RomHeader||!NES_Mapper||!spr_ram||!ppu||!apu||!wave_buffers||!saibuf1||!saibuf2||!romfile)
	{
		rt_kprintf("nes mem malloc failed\r\n");
		nes_sram_free();
		return 1;
	}
	memset(NES_SRAM,0,0X2000);				//����
	memset(RomHeader,0,sizeof(NES_header));	//����
	memset(NES_Mapper,0,sizeof(MAPPER));	//����
	memset(spr_ram,0,0X100);				//����
	memset(ppu,0,sizeof(ppu_data));			//����
	memset(apu,0,sizeof(apu_t));			//����
	memset(wave_buffers,0,APU_PCMBUF_SIZE*2);//����
	memset(saibuf1,0,APU_PCMBUF_SIZE*4+10);	//����
	memset(saibuf2,0,APU_PCMBUF_SIZE*4+10);	//����
	memset(romfile,0,romsize);				//���� 
	return 0;
} 
//��ʼnes��Ϸ
//pname:nes��Ϸ·��
//����ֵ:
//0,�����˳�
//1,�ڴ����
//2,�ļ�����
//3,��֧�ֵ�map
u8 nes_load( unsigned char *rom,int len)
{
	u8 res=0;  
	res=nes_sram_malloc(len);			//�����ڴ� 
	if(res==0)
	{
		memcpy(romfile,rom,len);
		NESrom_crc32=get_crc32(romfile+16, len-16);//��ȡCRC32��ֵ	
		res=nes_load_rom();						//����ROM
		if(res==0) 					
		{   
			
			cpu6502_init();						//��ʼ��6502,����λ	 
			Mapper_Init();						//map��ʼ�� 	
			PPU_reset();						//ppu��λ
			apu_init(); 						//apu��ʼ��
			nes_sound_open(APU_PCMBUF_SIZE,APU_SAMPLE_RATE) ;
			nes_emulate_frame();				//����NESģ������ѭ�� 
			nes_sound_close();					//�ر��������
		}
	}
	nes_sram_free();	//�ͷ��ڴ�
	return res;
}  
u16 nes_xoff=0;	//��ʾ��x�᷽���ƫ����(ʵ����ʾ���=256-2*nes_xoff)
u16 nes_yoff=0;	//��ʾ��y�᷽���ƫ����

//RGB����Ҫ��3������
//����4��,�������㷽��(800*480Ϊ��):
//offset=lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-(i-sx)*2-1)+nes_yoff+LineNo) 
//offset=2*(800*(480+(sx-i)*2-1)+nes_yoff+LineNo)
//      =1600*(480+(sx-i)*2-1)+2*nes_yoff+LineNo*2
//      =766400+3200*(sx-i)+2*nes_yoff+LineNo*2 
//nes_rgb_parm1=766400
//nes_rgb_parm2=3200
//nes_rgb_parm3=nes_rgb_parm2/2

//������,�������㷽��(480*272Ϊ��):
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
static int continue_cnt = 0;
static int need_swap = 0;
//��ȡ��Ϸ�ֱ�����
void nes_get_gamepadval(void)
{   
	int is_aa_press;
	int is_bb_press;
//�ֱ�1��ֵ [7:0]��7 ��6 ��5 ��4 Start3 Select2 B1 A0  
//�ֱ�2��ֵ [7:0]��7 ��6 ��5 ��4 Start3 Select2 B1 A0  
	continue_cnt++;
	if(continue_cnt > 3)
	{
		continue_cnt = 0;
		need_swap = !need_swap;
	}
	is_aa_press = input_key_aa_press();
	is_bb_press = input_key_bb_press();
	PADdata0 = 0;
	PADdata0 |=(input_key_up_press())<<4;
	PADdata0 |=(input_key_do_press())<<5;
	PADdata0 |=(input_key_lf_press())<<6;
	PADdata0 |=(input_key_rg_press())<<7;

	PADdata0 |=(input_key_sa_press())<<0;
	PADdata0 |=(input_key_sb_press())<<1;
	PADdata0 |=(input_key_st_press())<<3;
	PADdata0 |=(input_key_se_press())<<2;

	if(is_aa_press)
	{
		if(need_swap)
		{
			PADdata0 |=1<<0;
		}
		else
		{
			PADdata0 &=1<<0;
		}
	}
	if(is_bb_press)
	{
		if(need_swap)
		{
			PADdata0 |=1<<1;
		}
		else
		{
			PADdata0 &=1<<1;
		}
	}
}    
//nesģ������ѭ��
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
		run6502(113*256);//����1��
		NES_Mapper->HSync(NES_scanline); 

		if(RenderType < POST_RENDER)//���Ҳ�Ǹ���ʵ��rom����� 
		{ 
			NES_scanline=240;  //243??    //ð�յ�2����Ϸ�Ų���������
			run6502(113*256);//����1��
			NES_Mapper->HSync(NES_scanline);
			//
		}

		start_vblank(); 
		if(NMI_enabled()) 
		{ 
			cpunmi=1;
			if(RenderType >= POST_RENDER)//�е�Ҫ����,�е�rom��Ҫ 
			{
				run6502(7*256);//�����ж�
			}
			
		}

		
		// LINES 242-261    
		for(NES_scanline=241;NES_scanline<262;NES_scanline++)
		{
			run6502(113*256);	  
			NES_Mapper->HSync(NES_scanline);		  
		}	   
		end_vblank(); 



		nes_get_gamepadval();//ÿ3֡��ѯһ��USB
		apu_soundoutput();//�����Ϸ����	 
		framecnt++; 	
		nes_frame++;
		if(nes_frame>NES_SKIP_FRAME)nes_frame=0;//��֡ 
	}
}
//��6502.s���汻����
void debug_6502(u16 reg0,u8 reg1)
{
	rt_kprintf("6502 error:%x,%d\r\n",reg0,reg1);
}
////////////////////////////////////////////////////////////////////////////////// 	 
//nes,��Ƶ���֧�ֲ���

//NES����Ƶ���
int nes_sound_open(int samples_per_sync,int sample_rate) 
{
 	f1c100s_audio_config(1,16,sample_rate);
	f1c100s_audio_open(samples_per_sync*2);
	return 1;
}
//NES�ر���Ƶ���
void nes_sound_close(void) 
{ 

} 
//NES��Ƶ�����SAI����
void nes_apu_fill_buffer(int samples,u16* wavebuf)
{	
 	audio_pcm_play((unsigned char*)wavebuf,APU_PCMBUF_SIZE*2);
} 



















