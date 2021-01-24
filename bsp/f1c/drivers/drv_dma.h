#ifndef _DRV_DMA_H_
#define _DRV_DMA_H_

/****************Patch for DMA Bug*********************************
 * If we use a pchan for cyclic last time,we can't use it for memcpy
 * until another pchan end, So before we fix the bug,we can avoid it
 * Here is ways to avoid
 */

// /*way1:cyclic and no cyclic use different pchan*/
// #define CYC_MEM_DIFF

/*way2:forbid the memcpy use this kind of pchan*/
/*#define MEM_NOT_FOLLOW_CYC*/

/*way3:assign the pchan by pchan_assign*/
/*#define ASSIGN_PCHAN*/

/*way4:wait the dma end before terminate*/
/*#define TERMINATE_WAIT_DMA*/

#define PCHAN_INNER_BUFFER_LEN		1024

#define DMA_DRQ_TYPE_NONE		0x1F
#define DMA_VCHAN_IS_DEDICATED		0x01
#define DMA_VCHAN_IS_NOT_DEDICATED	0x00
/** Common macros to normal and dedicated DMA registers **/

#define DMA_CFG_LOADING			(1<<31)
#define DMA_IDLE_STATE (1<<30)
#define DMA_CFG_BURST_MODE 	    (1 << 29)
#define DMA_CFG_DST_DATA_WIDTH(width)	((width) << 24)
#define DMA_CFG_DST_BURST_LENGTH(len)	((len) << 23)
#define DMA_CFG_DST_ADDR_MODE(mode)	((mode) << 21)
#define DMA_CFG_DST_DRQ_TYPE(type)	((type) << 16)
#define DMA_CFG_REMAIN_READ_MODE(en)	    ((en) << 15)
#define DMA_CFG_SRC_DATA_WIDTH(width)	((width) << 8)
#define DMA_CFG_SRC_BURST_LENGTH(len)	((len) << 7)
#define DMA_CFG_SRC_ADDR_MODE(mode)	((mode) << 5)
#define DMA_CFG_SRC_DRQ_TYPE(type)	(type)

#define DMA_WIDTH_8BIT (0)
#define DMA_WIDTH_16BIT (1)
#define DMA_WIDTH_32BIT (2)

#define DMA_CFG_SRC_DRQ_TYPE_MASK (0x1f << 0)
#define DMA_CFG_SRC_ADDR_MODE_MASK (0x3 << 5)
#define DMA_CFG_SRC_BURST_LENGTH_MASK (0x01 << 7)
#define DMA_CFG_SRC_DATA_WIDTH_MASK (0x03 << 8)
#define DMA_CFG_REMAIN_MODE_MASK (0x01 << 15)
#define DMA_CFG_DST_DRQ_TYPE_MASK (0x1f << 16)
#define DMA_CFG_DST_ADDR_MODE_MASK (0x03 << 21)
#define DMA_CFG_BURST_LENGTH_MASK (0x01 << 23)
#define DMA_CFG_DST_DATA_WIDTH_MASK (0x03 << 24)

/** Normal DMA register values **/

/* Normal DMA source/destination data request type values */
#define NDMA_DRQ_TYPE_SDRAM			0x11
#define NDMA_DRQ_TYPE_SPI0_TX         0x04
#define NDMA_DRQ_TYPE_SPI1_TX         0x05
#define NDMA_DRQ_TYPE_AUDIO         0x0c
#define NDMA_DRQ_TYPE_LIMIT			(0x1F + 1)

#define DRQ_TYPE_LIMIT				0x18
#define NDMA_SDRAM2SDRAM_CFG			0x82118211
#define DDMA_SDRAM2SDRAM_CFG			0x82018201
/** Normal DMA register layout **/

/* Dedicated DMA source/destination address mode values */
#define NDMA_ADDR_MODE_LINEAR			0
#define NDMA_ADDR_MODE_IO			1

/* Normal DMA configuration register layout */
#define NDMA_CFG_CONT_MODE			(1<<29)
#define NDMA_CFG_WAIT_STATE(n)			((n) << 26)
#define NDMA_CFG_WAIT_STATE_MASK			((7) << 26)
#define NDMA_CFG_BYTE_COUNT_MODE_REMAIN		(1<<15)

/** Dedicated DMA register values **/
/* Dedicated DMA source/destination address mode values */
#define DDMA_ADDR_MODE_LINEAR				0
#define DDMA_ADDR_MODE_IO				1

/* Dedicated DMA source/destination data request type values */
#define DDMA_DRQ_TYPE_SDRAM				(0x1)
#define DDMA_DRQ_TYPE_LIMIT				(0x1F + 1)

/** Dedicated DMA register layout **/
/* Dedicated DMA configuration register layout */
#define DDMA_CFG_BUSY				(1<<30)
#define DDMA_CFG_CONT_MODE			(1<<29)
#define DDMA_CFG_DST_NON_SECURE			(1<<28)
#define DDMA_CFG_BYTE_COUNT_MODE_REMAIN		(1<<15)

/* Dedicated DMA parameter register layout */
#define DDMA_PARA_DST_DATA_BLK_SIZE(n)		(((n) - 1) << 24)
#define DDMA_PARA_DST_WAIT_CYCLES(n)		(((n) - 1) << 16)
#define DDMA_PARA_SRC_DATA_BLK_SIZE(n)		(((n) - 1) << 8)
#define DDMA_PARA_SRC_WAIT_CYCLES(n)		(((n) - 1) << 0)

/** DMA register offsets **/
/* General register offsets */
#define DMA_IRQ_ENABLE_REG			0x0
#define DMA_IRQ_PENDING_STATUS_REG		0x4
#define DMA_PTY_CFG_REG             0x8

/* Normal DMA register offsets */
#define NDMA_CHANNEL_REG_BASE(n)			(0x100 + (n) * 0x20)
#define NDMA_CFG_REG				0x0
#define NDMA_SRC_ADDR_REG			0x4
#define NDMA_DST_ADDR_REG			0x8
#define NDMA_BYTE_COUNT_REG			0xC

/* Dedicated DMA register offsets */
#define DDMA_CHANNEL_REG_BASE(n)			(0x300 + (n) * 0x20)
#define DDMA_CFG_REG				0x0
#define DDMA_SRC_ADDR_REG			0x4
#define DDMA_DST_ADDR_REG			0x8
#define DDMA_BYTE_COUNT_REG			0xC
#define DDMA_PARA_REG				0x18
#define DDMA_GEN_DATA				0x1C

/** DMA Driver **/
/*
 * Normal DMA has 8 channels, and Dedicated DMA has another 8, so
 * that's 16 channels. As for endpoints, there's 29 and 21
 * respectively. Given that the Normal DMA endpoints (other than
 * SDRAM) can be used as tx/rx, we need 78 vchans in total
 */
#define NDMA_NR_MAX_CHANNELS	4
#define DDMA_NR_MAX_CHANNELS	4
#define DMA_NR_MAX_CHANNELS	\
	(NDMA_NR_MAX_CHANNELS + DDMA_NR_MAX_CHANNELS)
#define NDMA_NR_MAX_VCHANS	(29 * 2 - 1)
#define DDMA_NR_MAX_VCHANS	21
#define DMA_NR_MAX_VCHANS	\
	(NDMA_NR_MAX_VCHANS + DDMA_NR_MAX_VCHANS)

/* This set of DDMA timing parameters were found experimentally while
 * working with the SPI driver and seem to make it behave correctly */
#define DDMA_MAGIC_SPI_PARAMETERS		  \
	(DDMA_PARA_DST_DATA_BLK_SIZE(1)		| \
	 DDMA_PARA_SRC_DATA_BLK_SIZE(1)		| \
	 DDMA_PARA_DST_WAIT_CYCLES(2)		| \
	 DDMA_PARA_SRC_WAIT_CYCLES(2))


#define DMA_REG_ADDR_BASE 0x01C02000



void f1c100s_dma_init(void);
void f1c100s_ndma_config_for_audio(int ch,unsigned int src,unsigned int len);
void f1c100s_ndma_config_for_spi(int ch,unsigned int src,unsigned int len);
int f1c100s_ndma_is_all_complete(int channel);
int f1c100s_ndma_is_half_complete(int channel);
void f1c100s_ndma_clear_all_complete(int channel);
void f1c100s_ndma_clear_half_complete(int channel);
void f1c100s_ndma_disable(int channel);
void f1c100s_ndma_enable(int channel);
int f1c100s_ndma_read_remain_len(int ch);
#endif