#include <rtthread.h>
#include "bolo_io.h"
#include "drv_ccu.h"
#include "drv_dma.h"
#define AUDIO_REG_DMA_ADDR  (0x01C23C00+0x0C)
#define SPI0_TX_REG_DMA_ADDR  (0x01C05000+0x200)
#define SPI1__TX_REG_DMA_ADDR  (0x01C06000+0x200)
static int is_dma_clk_init = 0;
void f1c100s_dma_init(void)
{
	if(!is_dma_clk_init)
	{
		is_dma_clk_init = 1;
		bolo_reg_write(CCU_BUS_CLK_GATE0_REG,bolo_reg_read(CCU_BUS_CLK_GATE0_REG)|(1)<<6);//DMA clk
		bolo_reg_write(CCU_BUS_SOFT_RST0_REG,bolo_reg_read(CCU_BUS_SOFT_RST0_REG)|((1)<<6)); //DMA Rst
	}


}

void f1c100s_ndma_config_for_audio(int ch,unsigned int src,unsigned int len)
{
    int channel = ch;

	bolo_clr_bit(DMA_REG_ADDR_BASE+DMA_PTY_CFG_REG, ((0x1)<<16) );	
	bolo_reg_write(DMA_REG_ADDR_BASE+DMA_PTY_CFG_REG,0);	
	
	bolo_reg_write(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_SRC_ADDR_REG,src);
	bolo_reg_write(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_DST_ADDR_REG,(unsigned int)(AUDIO_REG_DMA_ADDR));
	bolo_reg_write(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_BYTE_COUNT_REG,len);

	//Continuous mode
	//	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
	//		DMA_CFG_BURST_MODE,DMA_CFG_BURST_MODE);	

	//form dram 
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_SRC_DRQ_TYPE_MASK,DMA_CFG_SRC_DRQ_TYPE(NDMA_DRQ_TYPE_SDRAM));

	//src bit width 0:8 1:16 2:32
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_SRC_DATA_WIDTH_MASK,DMA_CFG_SRC_DATA_WIDTH(DMA_WIDTH_16BIT));

	//audio
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_DST_DRQ_TYPE_MASK,DMA_CFG_DST_DRQ_TYPE(NDMA_DRQ_TYPE_AUDIO));	
	//addr mode
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_DST_ADDR_MODE_MASK,DMA_CFG_DST_ADDR_MODE(NDMA_ADDR_MODE_IO));	
	//dst bit width
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_DST_DATA_WIDTH_MASK,DMA_CFG_DST_DATA_WIDTH(DMA_WIDTH_16BIT));		
		
	//burst len 0:1  1:4
	// bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
	// 				DMA_CFG_BURST_LENGTH_MASK,DMA_CFG_DST_BURST_LENGTH(1));	

	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_REMAIN_MODE_MASK,DMA_CFG_REMAIN_READ_MODE(1));
			
	//设置DMA加载使能
	//bolo_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					// DMA_CFG_LOADING);
}
///dma for spi0 tx
///FIXME:maybe not right....
void f1c100s_ndma_config_for_spi0_tx(int ch,unsigned int src,unsigned int len)
{
    int channel = ch;

	bolo_clr_bit(DMA_REG_ADDR_BASE + DMA_PTY_CFG_REG, ((0x1)<<16) );	
	// bolo_reg_write(DMA_REG_ADDR_BASE+DMA_PTY_CFG_REG,0);
	
	bolo_reg_write(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_SRC_ADDR_REG,src);
	bolo_reg_write(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_DST_ADDR_REG,(unsigned long)(SPI0_TX_REG_DMA_ADDR));
	bolo_reg_write(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_BYTE_COUNT_REG,len);
// NDMA_CFG_DESTINATION_DRQ_TYPE_CONFIDENTIAL_SET(NDMA_CFG_DRQ_SPI1_TX)|NDMA_CFG_SOURCE_DRQ_TYPE_SET(NDMA_CFG_DRQ_SDRAM_MEMORY)|
// NDMA_CFG_SOURCE_ADDRESS_TYPE_SET(NDMA_CFG_ADDRESS_LINEAR_MODE)|NDMA_CFG_DESTINATION_ADDRESS_TYPE_SET(NDMA_CFG_ADDRESS_IO_MODE)|
// NDMA_CFG_REMAIN_BYTE_COUNTER_READ_ENABLE|NDMA_CFG_WAIT_STATE_SET(NDMA_CFG_WAIT_STATE2);
	//Continuous mode
		bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
			DMA_CFG_BURST_MODE,DMA_CFG_BURST_MODE);	
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
		 	NDMA_CFG_WAIT_STATE_MASK,NDMA_CFG_WAIT_STATE(1));	
	//form dram 
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_SRC_DRQ_TYPE_MASK,DMA_CFG_SRC_DRQ_TYPE(NDMA_DRQ_TYPE_SDRAM));
	//addr mode
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_SRC_ADDR_MODE_MASK,DMA_CFG_SRC_ADDR_MODE(NDMA_ADDR_MODE_LINEAR));	
	//src bit width 0:8 1:16 2:32
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_SRC_DATA_WIDTH_MASK,DMA_CFG_SRC_DATA_WIDTH(DMA_WIDTH_8BIT));

	//spi0
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_DST_DRQ_TYPE_MASK,DMA_CFG_DST_DRQ_TYPE(NDMA_DRQ_TYPE_SPI0_TX));	
	//addr mode
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_DST_ADDR_MODE_MASK,DMA_CFG_DST_ADDR_MODE(NDMA_ADDR_MODE_IO));	
	//dst bit width
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_DST_DATA_WIDTH_MASK,DMA_CFG_DST_DATA_WIDTH(DMA_WIDTH_8BIT));		
		
	//burst len 0:1  1:4
	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_BURST_LENGTH_MASK,DMA_CFG_DST_BURST_LENGTH(0));	

	bolo_clr_and_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_REMAIN_MODE_MASK,DMA_CFG_REMAIN_READ_MODE(1));
			
	//设置DMA加载使能
	//bolo_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					// DMA_CFG_LOADING);
}

void f1c100s_ddma_wait_all_complete(int ch)
{
	unsigned int cfg_reg = 0x80000000;
	/* when the dma end, it clear this bit automatically */
	while ((cfg_reg & 0x80000000) ) {
		cfg_reg = read32(DMA_REG_ADDR_BASE+DDMA_CHANNEL_REG_BASE(ch) +
						DDMA_CFG_REG);
	}
}

void f1c100s_ddma_config_for_mem(int ch, void * dst, void *src, unsigned int bytes_cnt)
{
	unsigned int val = 0;

	if (bytes_cnt > 16*1024*1024)
	{
		bytes_cnt = 16*1024*1024;
	}

	/* Set source address */
	write32(DMA_REG_ADDR_BASE+DDMA_CHANNEL_REG_BASE(ch) + DDMA_SRC_ADDR_REG, (unsigned long)src);

	/* Set dst address */
	write32(DMA_REG_ADDR_BASE+DDMA_CHANNEL_REG_BASE(ch) + DDMA_DST_ADDR_REG, (unsigned long)dst);

	/* Set byte number */
	write32(DMA_REG_ADDR_BASE+DDMA_CHANNEL_REG_BASE(ch) + DDMA_BYTE_COUNT_REG, bytes_cnt);

	/* Set config and start */
	val = (1 << 31) | (2 << 24) | (1 << 23) | (0x1 << 16) | (2 << 8) | (1 << 7) | (0x1 << 0);
	// val = NDMA_SDRAM2SDRAM_CFG;
	write32(DMA_REG_ADDR_BASE+DDMA_CHANNEL_REG_BASE(ch) + DDMA_CFG_REG, val);

	/* Wait end */
	//f1c100s_ddma_wait_all_complete(ch);
}
// static int sun3i_dma_interrupt(int irq, void *dev_id)
// {
// 	struct sun3i_dma_dev *priv = dev_id;
// 	struct sun3i_dma_pchan *pchans = priv->pchans, *pchan;
// 	struct sun3i_dma_vchan *vchan;
// 	struct sun3i_dma_contract *contract;
// 	struct sun3i_dma_promise *promise;
// 	unsigned long pendirq, irqs, disableirqs;
// 	int bit, i, free_room, allow_mitigation = 1;

// 	pendirq = readl_relaxed(priv->base + SUN3I_DMA_IRQ_PENDING_STATUS_REG);

// handle_pending:

// 	disableirqs = 0;
// 	free_room = 0;

// 	for_each_set_bit(bit, &pendirq, 32) {
// 		pchan = &pchans[bit >> 1];
// 		vchan = pchan->vchan;
// 		if (!vchan)	/* a terminated channel may still interrupt */
// 			continue;
// 		contract = vchan->contract;

// 		/*
// 		 * Disable the IRQ and free the pchan if it's an end
// 		 * interrupt (odd bit)
// 		 */
// 		if (bit & 1) {
// 			spin_lock(&vchan->vc.lock);

// 			/*
// 			 * Move the promise into the completed list now that
// 			 * we're done with it
// 			 */
// 			list_del(&vchan->processing->list);
// 			list_add_tail(&vchan->processing->list,
// 					  &contract->completed_demands);

// 			/*
// 			 * Cyclic DMA transfers are special:
// 			 * - There's always something we can dispatch
// 			 * - We need to run the callback
// 			 * - Latency is very important, as this is used by audio
// 			 * We therefore just cycle through the list and dispatch
// 			 * whatever we have here, reusing the pchan. There's
// 			 * no need to run the thread after this.
// 			 *
// 			 * For non-cyclic transfers we need to look around,
// 			 * so we can program some more work, or notify the
// 			 * client that their transfers have been completed.
// 			 */
// 			if (vchan->terminating) {
// 				disableirqs |= BIT(bit);
// 			} else if (contract->is_cyclic) {
// 				promise = get_next_cyclic_promise(contract);
// 				vchan->processing = promise;
// 				configure_pchan(pchan, promise);
// 				sun3i_vchan_cyclic_callback(&contract->vd);
// 			} else {
// 				vchan->processing = NULL;
// 				vchan->pchan = NULL;
// 				free_room = 1;
// 				disableirqs |= BIT(bit);
// 				release_pchan(priv, pchan);
// 				release_pchan_cyclic(priv, pchan);
// 			}

// 			spin_unlock(&vchan->vc.lock);
// 		} else {
// 			/* Half done interrupt */
// 			if (vchan->terminating)
// 				disableirqs |= BIT(bit);
// 			else if (contract->is_cyclic)
// 				sun3i_vchan_cyclic_callback(&contract->vd);
// 			else
// 				disableirqs |= BIT(bit);
// 		}
// 	}

// 	/* Disable the IRQs for events we handled */
// 	spin_lock(&priv->lock);
// 	irqs = readl_relaxed(priv->base + SUN3I_DMA_IRQ_ENABLE_REG);
// 	writel_relaxed(irqs & ~disableirqs,
// 			   priv->base + SUN3I_DMA_IRQ_ENABLE_REG);
// 	spin_unlock(&priv->lock);

// 	/* Writing 1 to the pending field will clear the pending interrupt */
// 	writel_relaxed(pendirq, priv->base + SUN3I_DMA_IRQ_PENDING_STATUS_REG);

// 	/*
// 	 * If a pchan was freed, we may be able to schedule something else,
// 	 * so have a look around
// 	 */
// 	if (free_room) {
// 		for (i = 0; i < SUN3I_DMA_NR_MAX_VCHANS; i++) {
// 			vchan = &priv->vchans[i];
// 			spin_lock(&vchan->vc.lock);
// 			__execute_vchan_pending(priv, vchan);
// 			spin_unlock(&vchan->vc.lock);
// 		}
// 	}

// 	/*
// 	 * Handle newer interrupts if some showed up, but only do it once
// 	 * to avoid a too long a loop
// 	 */
// 	if (allow_mitigation) {
// 		pendirq = readl_relaxed(priv->base +
// 					SUN3I_DMA_IRQ_PENDING_STATUS_REG);
// 		if (pendirq) {
// 			allow_mitigation = 0;
// 			goto handle_pending;
// 		}
// 	}

// 	return IRQ_HANDLED;
// }
int f1c100s_ndma_read_remain_len(int ch)
{
	return bolo_reg_read(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(ch)+NDMA_BYTE_COUNT_REG);
}

int f1c100s_ndma_is_all_complete(int channel)
{
	return bolo_reg_read(DMA_REG_ADDR_BASE+DMA_IRQ_PENDING_STATUS_REG)&((1<<channel)+1);
}

int f1c100s_ndma_is_half_complete(int channel)
{
	return bolo_reg_read(DMA_REG_ADDR_BASE+DMA_IRQ_PENDING_STATUS_REG)&((1<<channel)+0);
}

void f1c100s_ndma_clear_all_complete(int channel)
{
	bolo_set_bit(DMA_REG_ADDR_BASE+DMA_IRQ_PENDING_STATUS_REG,((1<<channel)+1));
}

void f1c100s_ndma_clear_half_complete(int channel)
{
	bolo_set_bit(DMA_REG_ADDR_BASE+DMA_IRQ_PENDING_STATUS_REG,((1<<channel)+0));
}
//dma disable
void f1c100s_ndma_disable(int channel)
{
	bolo_clr_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_LOADING);
}

//dma enable
void f1c100s_ndma_enable(int channel)
{
	bolo_set_bit(DMA_REG_ADDR_BASE+NDMA_CHANNEL_REG_BASE(channel)+NDMA_CFG_REG,
					DMA_CFG_LOADING);
}