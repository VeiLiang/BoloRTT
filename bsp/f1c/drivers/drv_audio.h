#ifndef _DRV_AUDIO_H_
#define _DRV_AUDIO_H_

#define SUNXI_DAC_DPC		0x00
#define SUNXI_DAC_FIFOC		0x04
#define SUNXI_DAC_FIFOS		0x08
#define SUNXI_DAC_TXDATA	0x0c
#define SUNXI_ADC_FIFOC		0x10
#define SUNXI_ADC_FIFOS		0x14
#define SUNXI_ADC_RXDATA	0x18
#define DAC_MIXER_CTRL		0x20
#define ADC_MIXER_CTRL		0x24
#define ADDA_TUNE		0x28
#define BIAS_DA16_CAL_CTRL0	0x2C
#define BIAS_DA16_CAL_CTRL1	0x34

#define SUNXI_DAC_CNT		0x40
#define SUNXI_ADC_CNT		0x44
#define SUNXI_DAC_DG		0x48
#define SUNXI_ADC_DG		0x4c

#define AC_PR_CFG		0x400

/*AC_DAC_DPC:0x00*/
#define EN_DAC			31
#define MODQU			25
#define DWA			24
#define HPF_EN			18
#define DVOL			12
/*#define HUB_EN		0 */

/*AC_DAC_FIFOC:0x04*/
#define DAC_FS			29
#define FIR_VER			28
#define SEND_LASAT		26
#define FIFO_MODE		24
#define DAC_DRQ_CLR_CNT		21
#define TX_TRIG_LEVEL		8
#define ADDA_LOOP_EN		7
#define DAC_MONO_EN		6
#define TX_SAMPLE_BITS		5
#define DAC_DRQ_EN		4
#define DAC_IRQ_EN		3
#define FIFO_UNDERRUN_IRQ_EN	2
#define FIFO_OVERRUN_IRQ_EN	1
#define FIFO_FLUSH		0

/*AC_ADC_FIFOC:0x10*/
#define ADFS			29
#define EN_AD			28
#define RX_FIFO_MODE		24
#define ADCDFEN			16
#define RX_FIFO_TRG_LEVEL	8
#define ADC_MONO_EN		7
#define RX_SAMPLE_BITS		6
#define ADC_DRQ_EN		4
#define ADC_IRQ_EN		3
#define ADC_OVERRUN_IRQ_EN	1
#define ADC_FIFO_FLUSH		0


/*DAC_MIXER_CTRL: 0x20*/
#define DAC_AG_R_EN		31  /* dac right enable bit */
#define DAC_AG_L_EN		30  /* dac left enable bit */
#define R_MIXER_EN		29  /* right output mixer */
#define L_MIXER_EN		28  /* left output mixer */
#define PH_R_MUTE		27  /* headphone right mute */
#define PH_L_MUTE		26  /* headphone left mute */
#define PH_R_PWR_SLT		25
#define PH_L_PWR_SLT		24
#define PH_COM_FC		22
#define PH_COM_PROTEC		21
#define R_MIXER_MUTE_MIC	20
#define R_MIXER_MUTE_LINEIN	19
#define R_MIXER_MUTE_FM		18
#define R_MIXER_MUTE_R_DAC	17
#define R_MIXER_MUTE_L_DAC	16
#define R_MIXER_MUTE		16
#define HP_POWER_EN		15
#define L_MIXER_MUTE_MIC	12
#define L_MIXER_MUTE_LINEIN	11
#define L_MIXER_MUTE_FM		10
#define L_MIXER_MUTE_R_DAC	9
#define L_MIXER_MUTE_L_DAC	8
#define L_MIXER_MUTE		8
#define L_HP_TO_R_HP_MUTE	7
#define R_HP_TO_L_HP_MUTE	6
#define HP_VOL			0

/*ADC_MIXER_CTRL: 0x24*/
#define ADC_EN			31  /* adc enable bit */
/* mic in boost stage to L or R output mixer gain control */
#define MIC_GAIN_CTL		24
#define LINEIN_VOL		21  /* right output mixer */
#define ADC_IN_GAIN_CTL		16  /* adc input gain control */
#define COS_SLOP_TM		14  /* COS slop time control for Anti-pop */
#define ADC_MIX_MUTE_MIC	13
#define ADC_MIX_MUTE_FML	12
#define ADC_MIX_MUTE_FMR	11
#define ADC_MIX_MUTE_LINEIN	10
#define ADC_MIX_MUTE_L		9
#define ADC_MIX_MUTE_R		8
#define ADC_MIX_MUTE		8  /* ADC mixer mute control */
#define PA_SPEED_SLT		7  /* PA speed select->0: normal 1: fast */
#define FM_TO_MIX_GAIN		4  /* FMin to mixer gain control */
#define MIC_BST_AMP_EN		3  /* MIC boost AMP enable */
#define MIC_BOST_GAIN		0  /* MIC boast AMP gain control */

/*AC_ADC_TXDATA:0x20*/
#define TX_DATA			0

/*AC_DAC_CNT:0x40*/
#define TX_CNT			0

/*AC_ADC_CNT:0x44*/
#define RX_CNT			0

/*AC_DAC_DG:0x48*/
/*
*	DAC Modulator Debug
*	0:DAC Modulator Normal Mode
*	1:DAC Modulator Debug Mode
*/
#define DAC_MODU_SELECT		11
/*
*	DAC Pattern Select
*	00:Normal(Audio sample from TX fifo)
*	01: -6 dB sin wave
*	10: -60 dB sin wave
*	11: silent wave
*/
#define DAC_PATTERN_SELECT	9
/*
*	CODEC Clock Source Select
*	0:codec clock from PLL
*	1:codec clock from OSC(for debug)
*/
#define CODEC_CLK_SELECT	8
/*
*	DAC output channel swap enable
*	0:disable
*	1:enable
*/
#define DA_SWP			6

/*AC_ADC_DG:0x4c*/
#define AD_SWP			24

/*AC_PR_CFG:0x400*/
#define AC_PR_RST		28
#define AC_PR_RW		24
#define AC_PR_ADDR		16
#define ADDA_PR_WDAT		8
#define ADDA_PR_RDAT		0

#define AUDIO_REG_ADDR_BASE 0x01C23C00

//define ccu dma reg

#define CCU_REG_ADDR_BASE     0x01C20000
#define CCU_BUS_CLK_REG0 		(CCU_REG_ADDR_BASE+0x060)
#define CCU_BUS_CLK_REG1 		(CCU_REG_ADDR_BASE+0x064)
#define CCU_BUS_CLK_REG2 		(CCU_REG_ADDR_BASE+0x068)
#define CCU_BUS_RST_REG0 			(CCU_REG_ADDR_BASE+0x2C0)
#define CCU_BUS_RST_REG1 			(CCU_REG_ADDR_BASE+0x2C4)
#define CCU_BUS_RST_REG2 			(CCU_REG_ADDR_BASE+0x2D0)

//AUDIO CLK
#define AUDIO_PLL_CLK_REG 						(CCU_REG_ADDR_BASE+0x008)
#define AUDIO_CODEC_CLK_REG					(CCU_REG_ADDR_BASE+0x140)
#define AUDIO_BIAS_CFG_REG						(CCU_REG_ADDR_BASE+0x224)
#define AUDIO_PAT_CTRL_REG 	                    (CCU_REG_ADDR_BASE+0x284)


#define AUDIO_REG_ADDR_BASE  0x01C23C00


#define  audio_reg_read(reg)	                    (*(volatile unsigned long *)(reg))

#define  audio_reg_write(reg, value)	            (*(volatile unsigned long *)(reg) = (value))



struct label {
	const char *name;
	int value;
};

#define LABEL(constant) { #constant, constant }
#define LABEL_END { NULL, -1 }

void f1c100s_audio_init(void);
void f1c100s_audio_config(int chanel,int bit_len,int sample_rate);
void f1c100s_audio_open(int len);
void f1c100s_audio_vol_set(int val);
void audio_pcm_play(unsigned char * dat,int len);



#endif