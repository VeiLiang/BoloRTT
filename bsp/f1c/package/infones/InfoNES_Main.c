
#include <rtthread.h>
#include <dfs_posix.h>
#include "InfoNES.h"
#include "InfoNES_System.h"
#include "InfoNES_pAPU.h"
#include "drv_audio.h"
/*-------------------------------------------------------------------*/
/*  ROM image file information                                       */
/*-------------------------------------------------------------------*/

// #define VBOX_SIZE    7 
// #define VERSION      "InfoNES v0.96J"


DWORD * FrameBuffer = (DWORD *)0x81800000;
#ifdef DISP_USE_ARGB_32
DWORD NesPalette[64]={
 0xff000000|( 0x80<<16 ) | (0x80<<8)|( 0x80<<0),
	0xff000000|( 0x00<<16 ) | (0x00<<8)|( 0xBB<<0),
	0xff000000|( 0x37<<16 ) | (0x00<<8)|( 0xBF<<0),
	0xff000000|( 0x84<<16 ) | (0x00<<8)|( 0xA6<<0),
	0xff000000|( 0xBB<<16 ) | (0x00<<8)|( 0x6A<<0),
	0xff000000|( 0xB7<<16 ) | (0x00<<8)|( 0x1E<<0),
	0xff000000|( 0xB3<<16 ) | (0x00<<8)|( 0x00<<0),
	0xff000000|( 0x91<<16 ) | (0x26<<8)|( 0x00<<0),
	0xff000000|( 0x7B<<16 ) | (0x2B<<8)|( 0x00<<0),
	0xff000000|( 0x00<<16 ) | (0x3E<<8)|( 0x00<<0),
	0xff000000|( 0x00<<16 ) | (0x48<<8)|( 0x0D<<0),
	0xff000000|( 0x00<<16 ) | (0x3C<<8)|( 0x22<<0),
	0xff000000|( 0x00<<16 ) | (0x2F<<8)|( 0x66<<0),
	0xff000000|( 0x00<<16 ) | (0x00<<8)|( 0x00<<0),
	0xff000000|( 0x05<<16 ) | (0x05<<8)|( 0x05<<0),
	0xff000000|( 0x05<<16 ) | (0x05<<8)|( 0x05<<0),
	0xff000000|( 0xC8<<16 ) | (0xC8<<8)|( 0xC8<<0),
	0xff000000|( 0x00<<16 ) | (0x59<<8)|( 0xFF<<0),
	0xff000000|( 0x44<<16 ) | (0x3C<<8)|( 0xFF<<0),
	0xff000000|( 0xB7<<16 ) | (0x33<<8)|( 0xCC<<0),
	0xff000000|( 0xFF<<16 ) | (0x33<<8)|( 0xAA<<0),
	0xff000000|( 0xFF<<16 ) | (0x37<<8)|( 0x5E<<0),
	0xff000000|( 0xFF<<16 ) | (0x37<<8)|( 0x1A<<0),
	0xff000000|( 0xD5<<16 ) | (0x4B<<8)|( 0x00<<0),
	0xff000000|( 0xC4<<16 ) | (0x62<<8)|( 0x00<<0),
	0xff000000|( 0x3C<<16 ) | (0x7B<<8)|( 0x00<<0),
	0xff000000|( 0x1E<<16 ) | (0x84<<8)|( 0x15<<0),
	0xff000000|( 0x00<<16 ) | (0x95<<8)|( 0x66<<0),
	0xff000000|( 0x00<<16 ) | (0x84<<8)|( 0xC4<<0),
	0xff000000|( 0x11<<16 ) | (0x11<<8)|( 0x11<<0),
	0xff000000|( 0x09<<16 ) | (0x09<<8)|( 0x09<<0),
	0xff000000|( 0x09<<16 ) | (0x09<<8)|( 0x09<<0),
	0xff000000|( 0xFF<<16 ) | (0xFF<<8)|( 0xFF<<0),
	0xff000000|( 0x00<<16 ) | (0x95<<8)|( 0xFF<<0),
	0xff000000|( 0x6F<<16 ) | (0x84<<8)|( 0xFF<<0),
	0xff000000|( 0xD5<<16 ) | (0x6F<<8)|( 0xFF<<0),
	0xff000000|( 0xFF<<16 ) | (0x77<<8)|( 0xCC<<0),
	0xff000000|( 0xFF<<16 ) | (0x6F<<8)|( 0x99<<0),
	0xff000000|( 0xFF<<16 ) | (0x7B<<8)|( 0x59<<0),
	0xff000000|( 0xFF<<16 ) | (0x91<<8)|( 0x5F<<0),
	0xff000000|( 0xFF<<16 ) | (0xA2<<8)|( 0x33<<0),
	0xff000000|( 0xA6<<16 ) | (0xBF<<8)|( 0x00<<0),
	0xff000000|( 0x51<<16 ) | (0xD9<<8)|( 0x6A<<0),
	0xff000000|( 0x4D<<16 ) | (0xD5<<8)|( 0xAE<<0),
	0xff000000|( 0x00<<16 ) | (0xD9<<8)|( 0xFF<<0),
	0xff000000|( 0x66<<16 ) | (0x66<<8)|( 0x66<<0),
	0xff000000|( 0x0D<<16 ) | (0x0D<<8)|( 0x0D<<0),
	0xff000000|( 0x0D<<16 ) | (0x0D<<8)|( 0x0D<<0),
	0xff000000|( 0xFF<<16 ) | (0xFF<<8)|( 0xFF<<0),
	0xff000000|( 0x84<<16 ) | (0xBF<<8)|( 0xFF<<0),
	0xff000000|( 0xBB<<16 ) | (0xBB<<8)|( 0xFF<<0),
	0xff000000|( 0xD0<<16 ) | (0xBB<<8)|( 0xFF<<0),
	0xff000000|( 0xFF<<16 ) | (0xBF<<8)|( 0xEA<<0),
	0xff000000|( 0xFF<<16 ) | (0xBF<<8)|( 0xCC<<0),
	0xff000000|( 0xFF<<16 ) | (0xC4<<8)|( 0xB7<<0),
	0xff000000|( 0xFF<<16 ) | (0xCC<<8)|( 0xAE<<0),
	0xff000000|( 0xFF<<16 ) | (0xD9<<8)|( 0xA2<<0),
	0xff000000|( 0xCC<<16 ) | (0xE1<<8)|( 0x99<<0),
	0xff000000|( 0xAE<<16 ) | (0xEE<<8)|( 0xB7<<0),
	0xff000000|( 0xAA<<16 ) | (0xF7<<8)|( 0xEE<<0),
	0xff000000|( 0xB3<<16 ) | (0xEE<<8)|( 0xFF<<0),
	0xff000000|( 0xDD<<16 ) | (0xDD<<8)|( 0xDD<<0),
	0xff000000|( 0x11<<16 ) | (0x11<<8)|( 0x11<<0),
	0xff000000|( 0x11<<16 ) | (0x11<<8)|( 0x11<<0)
};
#else
WORD NesPalette[64]={
  0x738E,0x88C4,0xA800,0x9808,0x7011,0x1015,0x0014,0x004F,
  0x0148,0x0200,0x0280,0x11C0,0x59C3,0x0000,0x0000,0x0000,
  0xBDD7,0xEB80,0xE9C4,0xF010,0xB817,0x581C,0x015B,0x0A59,
  0x0391,0x0480,0x0540,0x3C80,0x8C00,0x0000,0x0000,0x0000,
  0xFFDF,0xFDC7,0xFC8B,0xFC48,0xFBDE,0xB39F,0x639F,0x3CDF,
  0x3DDE,0x1690,0x4EC9,0x9FCB,0xDF40,0x0000,0x0000,0x0000,
  0xFFDF,0xFF15,0xFE98,0xFE5A,0xFE1F,0xDE1F,0xB5DF,0xAEDF,
  0xA71F,0xA7DC,0xBF95,0xCFD6,0xF7D3,0x0000,0x0000,0x0000,
};
#endif
/* Menu screen */
int InfoNES_Menu()
{
    return 0;
}


/* Read ROM image file */
int InfoNES_ReadRom(unsigned char *dat)
{
/*
 *  Read ROM image file
 *
 *  Parameters
 *    const char *pszFileName          (Read)
 *
 *  Return values
 *     0 : Normally
 *    -1 : Error
 */


  /* Read ROM Header */
  BYTE * rom = (BYTE*)dat;
  memcpy( &NesHeader, rom, sizeof(NesHeader));
  if ( memcmp( NesHeader.byID, "NES\x1a", 4 ) != 0 )
  {
    /* not .nes file */
    return -1;
  }
  rom += sizeof(NesHeader);

  /* Clear SRAM */
  memset( NES_SRAM, 0, SRAM_SIZE );

  /* If trainer presents Read Triner at 0x7000-0x71ff */
  if ( NesHeader.byInfo1 & 4 )
  {
    //memcpy( &SRAM[ 0x1000 ], rom, 512);
	rom += 512;
  }

  /* Allocate Memory for ROM Image */
  NES_ROM = rom;
  rom += NesHeader.byRomSize * 0x4000;

  if ( NesHeader.byVRomSize > 0 )
  {
    /* Allocate Memory for VROM Image */
	VROM = (BYTE*)rom;
	rom += NesHeader.byVRomSize * 0x2000;
  }

  /* Successful */
  return 0;
}


/* Release a memory for ROM */
void InfoNES_ReleaseRom()
{
}


/* Transfer the contents of work frame on the screen */
void InfoNES_LoadFrame()
{

    unsigned int x,y;
    
#ifdef DISP_USE_ARGB_32
    unsigned int *src,*dst;
    src = (unsigned int *)WorkFrame;
#else
    unsigned short *src,*dst;
    src = (unsigned short *)WorkFrame;
#endif
    for(x=0;x<NES_DISP_HEIGHT;x++)
    {
        for(y=0;y<NES_DISP_WIDTH;y++)
        {
            FrameBuffer[x*480+y+160] = *src;
			src++;
        }
    }
    // for(x=0;x<NES_DISP_HEIGHT;x++)
    // {
    //     dst = FrameBuffer + (NES_DISP_HEIGHT-1-x);
    //     for(y=0;y<NES_DISP_WIDTH;y++)
    //     {
    //         *dst = *src;
    //         dst += 480;
    //         src++;
    //     }
    // }
}


/* Get a joypad state */
void InfoNES_PadState( DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem )
{
	//add key ctrl in here...
}


/* memcpy */
void *InfoNES_MemoryCopy( void *dest, const void *src, int count )
{
    return memcpy(dest,src,count);
}


/* memset */
void *InfoNES_MemorySet( void *dest, int c, int count )
{
    return memset(dest,c,count);
}


/* Print debug message */
void InfoNES_DebugPrint( char *pszMsg )
{
}


/* Wait */
void InfoNES_Wait()
{
}


/* Sound Initialize */
void InfoNES_SoundInit( void )
{
	
}


/* Sound Open */
int InfoNES_SoundOpen( int samples_per_sync, int sample_rate )
{
	f1c100s_audio_config(1,16,sample_rate);
	f1c100s_audio_open(samples_per_sync*2);
    return 0;
}


/* Sound Close */
void InfoNES_SoundClose( void )
{
}

WORD final_wave[367*5];

/* Sound Output 5 Waves - 2 Pulse, 1 Triangle, 1 Noise, 1 DPCM */
void InfoNES_SoundOutput(int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4, BYTE *wave5)
{
	int i;
	for (i = 0; i < samples; i++) 
	{
		final_wave[i] = (wave1[i]*32 + wave2[i]*32 + wave3[i]*32 + wave4[i]*32 + wave5[i]*32);
	}
	audio_pcm_play(final_wave,samples*2);
}
  

