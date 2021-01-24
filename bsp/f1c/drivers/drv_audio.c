#include <rtthread.h>
#include "bolo_io.h"
#include "drv_ccu.h"
#include "drv_audio.h"
#include "drv_dma.h"

#define snd_soc_update_bits(Base_Add,Offset,bit,Clear,Value)	bolo_reg_write(Base_Add+Offset,audio_reg_read(Base_Add+Offset) & (~((unsigned long long)(Clear)<<bit)) | ((unsigned long long)(Value)<<bit)    ) 

// static const DECLARE_TLV_DB_SCALE(dig_vol_tlv, -7424, 0, 0);
// static const DECLARE_TLV_DB_SCALE(headphone_vol_tlv, -6300, 100, 0);
// static const DECLARE_TLV_DB_SCALE(linein_to_l_r_mix_vol_tlv, -450, 150, 0);
// static const DECLARE_TLV_DB_SCALE(mic1_to_l_r_mix_vol_tlv, -450, 150, 0);
// static const DECLARE_TLV_DB_SCALE(spkl_vol_tlv, -4800, 150, 0);
// static const DECLARE_TLV_DB_SCALE(spkr_vol_tlv, -4800, 150, 0);
// static const DECLARE_TLV_DB_SCALE(mic_boost_vol_tlv, 0, 300, 0);
// static const DECLARE_TLV_DB_SCALE(adc_input_gain_tlv, -450, 150, 0);

struct AUDIO_REG_ADDR_BASE_sr {
	unsigned int samplerate;
	int srbit;
};

static const struct AUDIO_REG_ADDR_BASE_sr AUDIO_REG_ADDR_BASE_sr_s[] = {
	{44100, 0},
	{48000, 0},
	{8000, 5},
	{11025, 4},
	{12000, 4},
	{16000, 3},
	{22050, 2},
	{24000, 2},
	{32000, 1},
	{96000, 7},
	{192000, 6},
};

static struct label reg_labels[] = {
	LABEL(SUNXI_DAC_DPC),
	LABEL(SUNXI_DAC_FIFOC),
	LABEL(SUNXI_DAC_FIFOS),
	LABEL(SUNXI_ADC_FIFOC),
	LABEL(SUNXI_ADC_FIFOS),
	LABEL(SUNXI_ADC_RXDATA),
	LABEL(SUNXI_DAC_TXDATA),
	LABEL(SUNXI_DAC_CNT),
	LABEL(SUNXI_ADC_CNT),
	LABEL(SUNXI_DAC_DG),
	LABEL(SUNXI_ADC_DG),
	LABEL_END,
};

/*
*enable the AUDIO_REG_ADDR_BASE function which need to init befor use audio play.
*/
void f1c100s_audio_init(void)
{

    f1c100s_dma_init();
    bolo_reg_write(CCU_BUS_CLK_REG2,audio_reg_read(CCU_BUS_CLK_REG2)|(1)<<0);//audio 
    bolo_reg_write(CCU_BUS_RST_REG2,audio_reg_read(CCU_BUS_RST_REG2)|((1)<<0));//audio
    bolo_set_shift(AUDIO_PLL_CLK_REG,31);
	//while((audio_reg_read(AUDIO_PLL_CLK_REG)&(1<<28))==0);

	//snd_soc_update_bits(codec, DAC_MIXER_CTRL, (0x3F<<HP_VOL),(0x0<<HP_VOL));   /* set hp volume 0*/
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, SUNXI_DAC_FIFOC,DAC_DRQ_CLR_CNT,0x3,0x3);
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, SUNXI_DAC_FIFOC,FIFO_FLUSH,0x1,0x1);
    /*
    *	0:64-Tap FIR
    *	1:32-Tap FIR
    */	
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, SUNXI_DAC_FIFOC,FIR_VER,0x1,0x0);	
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, SUNXI_ADC_FIFOC,ADC_FIFO_FLUSH,0x1,0x1);
    
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_COM_PROTEC,0x1,0x1);
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_COM_FC,0x3,0x3);	
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,HP_VOL,0x3f,0x38);/* set hp volume */
    
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, SUNXI_DAC_DPC,EN_DAC,0x1,0x1);

    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,HP_POWER_EN,0x1,0x1);	/* power hp */
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,L_MIXER_MUTE_R_DAC,0x1,0x1);	
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,L_MIXER_MUTE_L_DAC,0x1,0x1);
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,R_MIXER_MUTE_R_DAC,0x1,0x1);	
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,R_MIXER_MUTE_L_DAC,0x1,0x1);	
    
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_L_MUTE,0x1,0x1);	
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_R_MUTE,0x1,0x1);	

    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,DAC_AG_L_EN,0x1,0x1);	
    snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,DAC_AG_R_EN,0x1,0x1);	
    
    bolo_set_shift(AUDIO_REG_ADDR_BASE+4,4);
    bolo_set_shift(AUDIO_REG_ADDR_BASE+4,26);

		
}

/* config for audio para */
void f1c100s_audio_config(int chanel,int bit_len,int sample_rate)
{
	int s=0,n=0,m=0;	

	switch(sample_rate)
	{
		case 48000:
			s=0;n=0x55;m=0x14;
			break;
		case 44100:
			s=0;n=0x4E;m=0x14;
			break;	
		case 24000:
			s=2;n=0x55;m=0x14;
			break;	
		case 22050:
			s=2;n=0x4E;m=0x14;
			break;
		case 12000:
			s=4;n=0x55;m=0x14;
			break;
		case 192000:
			s=6;n=0x55;m=0x14;
			break;	
		case 96000:
			s=7;n=0x55;m=0x14;
			break;	
		case 32000:
			s=1;n=0x55;m=0x14;
			break;
		case 16000:
			s=3;n=0x55;m=0x14;
			break;			
		case 8000:
			s=5;n=0x55;m=0x14;
			break;
		case 11025:
			s=4;n=0x4E;m=0x14;
			break;	
		default:
			return -1;
	}
	//PLL close
	bolo_clr_shift(CCU_PLL_AUDIO_CTRL_REG,31);
	
	//set n
	bolo_clr_and_set_bit(CCU_PLL_AUDIO_CTRL_REG,0x7f<<8,n<<8);
	//set m
	bolo_clr_and_set_bit(CCU_PLL_AUDIO_CTRL_REG,0x1f<<0,m<<0);


	//PLL enable
	bolo_set_shift(CCU_PLL_AUDIO_CTRL_REG,31);
	//waiting lock
	while((audio_reg_read(CCU_PLL_AUDIO_CTRL_REG)&(1<<28))==0);
			
		
	/*sample period*/
	bolo_clr_and_set_bit(AUDIO_REG_ADDR_BASE+SUNXI_DAC_FIFOC,0xf<<29,s<<29);
	
	
	if(bit_len == 16)
	{
		bolo_clr_shift(AUDIO_REG_ADDR_BASE+SUNXI_DAC_FIFOC,5); //16 bit
		bolo_clr_and_set_bit(AUDIO_REG_ADDR_BASE+SUNXI_DAC_FIFOC,0x3<<24,0x3<<24);
	}
	else if(bit_len == 24)
	{
		bolo_set_shift(AUDIO_REG_ADDR_BASE+SUNXI_DAC_FIFOC,5); //24 bit
		bolo_clr_and_set_bit(AUDIO_REG_ADDR_BASE+SUNXI_DAC_FIFOC,0x3<<24,0<<24);
	}
	else 
	{
		return ;//not suppost
	}
	
	if(chanel == 1)
	{
		bolo_set_shift(AUDIO_REG_ADDR_BASE+SUNXI_DAC_FIFOC,6);//mono
	}
	else
	{
		bolo_clr_shift(AUDIO_REG_ADDR_BASE+SUNXI_DAC_FIFOC,6);//stereo
	}
	bolo_reg_write(AUDIO_REG_ADDR_BASE+SUNXI_DAC_CNT,0);
}



/* adj volume 0-63 */
void f1c100s_audio_vol_set(int val)
{
	if(val > 63)
	{
		val = 63;
	}
	if(val < 0)
	{
		val = 0;
	}
	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,HP_VOL,
	 (0x3F),(val));   /* set hp volume 0*/

}
#define AUDIO_DMA_CH 0
//ping-pang buf
static unsigned short audio_buf[2][0x400*32]__attribute__ ((aligned (4096)));
static int is_n=0,is_first=0;

/* power down chip */
void f1c100s_audio_close(void)
{
	//if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) 
	{
		snd_soc_update_bits(AUDIO_REG_ADDR_BASE, SUNXI_DAC_FIFOC, FIFO_FLUSH,
			(0x1), (0x1));/* flush tx fifo */
		snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,L_MIXER_MUTE_R_DAC,
		    (0x1), (0x0));
		snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,R_MIXER_MUTE_R_DAC,
			(0x1), (0x0));
		snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_L_MUTE,
		 	(0x1),(0x0));
		snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_R_MUTE,
			(0x1),(0x0));
		snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,DAC_AG_L_EN,
		 	(0x1),(0x0));
		snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,DAC_AG_R_EN,
			(0x1),(0x0));
	} 
	// else 
	// {
	// 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, SUNXI_ADC_FIFOC,EN_AD,
	// 		(0x1),(0x0));
	// 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, SUNXI_ADC_FIFOC,ADC_FIFO_FLUSH,
	// 		(0x1),(0x1));
	// 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, ADC_MIXER_CTRL,ADC_EN ,
	// 		(0x1),(0x0));
	// }
	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_COM_PROTEC,
	 		(0x1),(0x0));
	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_COM_FC,
	 		(0x3),(0x0));
}

/* power down chip */
void f1c100s_audio_open(int len)
{
	// //if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) 
	// {
	// 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, SUNXI_DAC_FIFOC, FIFO_FLUSH,
	// 		(0x1), (0x1));/* flush tx fifo */
	// 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,L_MIXER_MUTE_R_DAC,
	// 	    (0x1), (0x1));
	// 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,R_MIXER_MUTE_R_DAC,
	// 		(0x1), (0x1));
	// 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_L_PWR_SLT,
	// 		(0x1), (0x0));
	// 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_R_PWR_SLT,
	// 	 	(0x1),(0x0));
	// 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_L_MUTE,
	// 	 	(0x1),(0x1));
	// 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_R_MUTE,
	// 		(0x1),(0x1));
	// 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,DAC_AG_L_EN,
	// 	 	(0x1),(0x1));
	// 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,DAC_AG_R_EN,
	// 		(0x1),(0x1));
	// } 
	// // else 
	// // {
	// // 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, SUNXI_ADC_FIFOC,EN_AD,
	// // 		(0x1),(0x1));
	// // 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, SUNXI_ADC_FIFOC,ADC_FIFO_FLUSH,
	// // 		(0x1),(0x1));
	// // 	snd_soc_update_bits(AUDIO_REG_ADDR_BASE, ADC_MIXER_CTRL,ADC_EN ,
	// // 		(0x1),(0x1));
	// // }
	// snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_COM_PROTEC,
	//  		(0x1),(0x1));
	// snd_soc_update_bits(AUDIO_REG_ADDR_BASE, DAC_MIXER_CTRL,PH_COM_FC,
	//  		(0x3),(0x3));
	//f1c100s_audio_vol_set(50);
	f1c100s_ndma_disable(AUDIO_DMA_CH);
	memset(audio_buf[1],0,len);
	f1c100s_ndma_config_for_audio(AUDIO_DMA_CH,(unsigned int)audio_buf[1],len);
	f1c100s_ndma_enable(AUDIO_DMA_CH);
	bolo_set_shift(AUDIO_CODEC_CLK_REG,31);	
	is_n = 0;

}

// dat : pcm data 
// len : pcm data size ,how manny bytes
void audio_pcm_play(unsigned char * dat,int len)
{
  	int time_out = 100;
	unsigned short *buf = (unsigned short *)dat;
	// while((!f1c100s_ndma_is_all_complete(AUDIO_DMA_CH))&&(--time_out))
	// {
	// 	rt_thread_delay(1);
	// }
	while(!f1c100s_ndma_is_all_complete(AUDIO_DMA_CH));
	memcpy(audio_buf[is_n],buf,len);
	f1c100s_ndma_clear_all_complete(AUDIO_DMA_CH);	
	f1c100s_ndma_disable(AUDIO_DMA_CH);
	f1c100s_ndma_config_for_audio(AUDIO_DMA_CH,(unsigned int)audio_buf[is_n],len);
	f1c100s_ndma_enable(AUDIO_DMA_CH);
	is_n=!is_n;

}

void dump_audio_reg(void)
{
	int i = 0;
	int reg_group = 0;
	rt_kprinf("dump audio reg:\n");
	while (reg_labels[i].name != NULL) 
    {
		if (reg_labels[i].value == 0)
			reg_group++;
		if (reg_group == 1) 
        {
			rt_kprinf("%s 0x%p: 0x%x\n",
				reg_labels[i].name, (AUDIO_REG_ADDR_BASE +
				reg_labels[i].value), readl(AUDIO_REG_ADDR_BASE
				+ reg_labels[i].value));
		}
		i++;
	}
}
