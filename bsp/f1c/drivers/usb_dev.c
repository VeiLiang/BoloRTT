
#include <rtthread.h>
#include <rthw.h>
#include "usb.h"
#include "interrupt.h"
#include "usb_phy.h"
#include "usb_dev.h"
#include <string.h>

static unsigned char current_usb_type = USB_TYPE_DISCONNECT;
unsigned char current_speed = USB_SPEED_UNKNOWN;
unsigned int usb_connect = 0;
int usb_ep0_state = EP0_IDLE;
unsigned char usb_addr = 0;
unsigned char usb_xfer_type = NONE_REQ;
//static unsigned int  is_controller_alive = 0;
enum sunxi_udc_cmd_e {
	SW_UDC_P_ENABLE	= 1,		/* Pull-up enable        */
	SW_UDC_P_DISABLE = 2,		/* Pull-up disable       */
	SW_UDC_P_RESET	= 3,		/* UDC reset, in case of */
};
static int ep_max_len[3] = {64,512,512};
unsigned char usb_tx_buf[512];
int usb_tx_buf_len = 0;
int usb_tx_pos = 0;
unsigned char usb_rx_buf[512];
int usb_rx_buf_len = 0;
int usb_rx_pos = 0;
int usb_device_write_data_ep_pack(int ep,unsigned char * databuf,int len)
{
	//USBC_SelectActiveEp(ep);
	memcpy(usb_tx_buf,databuf,len);
	usb_tx_buf_len = len;
	usbprint("CTRL IN LEN : %d\n",usb_tx_buf_len);
	if(len > ep_max_len[ep])
	{
		USBC_WritePacket(USBC_SelectFIFO(ep), ep_max_len[ep], databuf);
		USBC_Dev_WriteDataStatus(ep ==0 ? USBC_EP_TYPE_EP0 : USBC_EP_TYPE_TX, 0);
		usb_tx_pos = 64;
	}
	else
	{
		USBC_WritePacket(USBC_SelectFIFO(ep), len, databuf);
		USBC_Dev_WriteDataStatus(ep ==0 ? USBC_EP_TYPE_EP0 : USBC_EP_TYPE_TX, 1);
		usb_tx_pos = 0;
	}
	return 0;
}
int usb_device_read_data_ep_pack(int ep,unsigned char * databuf,int len)
{
	USBC_ReadPacket(USBC_SelectFIFO(ep), len, databuf);
	return 0;
}

void usb_device_send_nullpack_ep0(void)
{
	USBC_Dev_WriteDataStatus(USBC_EP_TYPE_EP0, 1);
}

void usb_device_read_data_status_ep0(unsigned char is_complete)
{
	USBC_Dev_ReadDataStatus(USBC_EP_TYPE_EP0, is_complete);
}

int usb_device_write_data(int ep,unsigned char * databuf,int len)
{
	int length = len;
	volatile int Timeout = 1000000000;
	int pack_len = ep_max_len[ep];
	int data_pos = 0;
	if(usb_connect)
	{
		usbprint("usb_device_write_data len:%d\n",len);
		void * fifo = USBC_SelectFIFO(ep);
		USBC_SelectActiveEp(ep);
		while(length > pack_len)
		{
			while((USBC_Dev_IsWriteDataReady(USBC_EP_TYPE_TX))&&(--Timeout));//等待清除写标志
			USBC_WritePacket(fifo, pack_len, databuf + data_pos);
			USBC_Dev_WriteDataStatus(USBC_EP_TYPE_TX,1);
			data_pos += pack_len;
			length -=  pack_len;
			if(Timeout == 0)
			{
				usbprint("usb_device_write_data Time out!\n");
				return -1;
			}
			Timeout = 1000000000;
		}
		while((USBC_Dev_IsWriteDataReady(USBC_EP_TYPE_TX))&&(Timeout--));//等待清除写标志
		USBC_WritePacket(fifo, length, databuf + data_pos);
		USBC_Dev_WriteDataStatus(USBC_EP_TYPE_TX,1);
		return 0;
	}
	else
	{
		usbprint("usb_device_write_data faild! usb not connect!!\n");
		return -1;
	}
}

int usb_device_read_data(int ep,unsigned char * databuf,int len)
{

}

void usb_device_clear_setup_end(void)
{
	USBC_Dev_Ctrl_ClearSetupEnd();
}
void usb_device_set_address(unsigned char addr)
{
	USBC_Dev_SetAddress(addr);
	usb_addr = addr;
}
void usb_device_set_ep0_state(enum ep0_state state)
{
	usb_ep0_state = state;
}
void usb_device_set_xfer_type(enum usb_xfer_type_req type)
{
	usb_xfer_type = type;
}

int usb_dev_bsp_init(int ishost)
{
	//spinlock_t lock;

	/* open usb lock */
	usb_phy_open_clock();//need to open otg and phy clk

//#ifdef  SUNXI_USB_FPGA
//	clear_usb_reg(sunxi_udc_io->usb_vbase);
//fpga_config_use_otg(sunxi_udc_io->usbc.sram_base);
//#endif

	//USBC_EnhanceSignal(sunxi_udc_io->usb_bsp_hdle); //no use
	USBC_PhyConfig();
	USBC_ConfigFIFO_Base();
	if(ishost)
	{
		USBC_ForceId(USBC_ID_TYPE_HOST);
	}
	else
	{
		USBC_EnableDpDmPullUp();
		USBC_ForceId(USBC_ID_TYPE_DEVICE);
	}
	

	USBC_ForceVbusValid( USBC_VBUS_TYPE_HIGH);
	//USB_HOST_MODE();
	
	USBC_SelectBus(USBC_IO_TYPE_PIO, 0, 0);



	
////	USBC_PHY_Clear_Ctl(sunxi_udc_io->usb_vbase, 1);

//	/* config usb fifo */
//	spin_lock_init(&lock);
//	spin_lock_irqsave(&lock, flags);
//	USBC_ConfigFIFO_Base(sunxi_udc_io->usb_bsp_hdle, USBC_FIFO_MODE_8K);
//	spin_unlock_irqrestore(&lock, flags);

	return 0;
}
void clear_all_irq(void)
{
	USBC_INT_ClearEpPendingAll(USBC_EP_TYPE_TX);
	USBC_INT_ClearEpPendingAll(USBC_EP_TYPE_RX);
	USBC_INT_ClearMiscPendingAll();
}
/* mask the useless irq, save disconect, reset, resume, suspend */
u32 filtrate_irq_misc(u32 irq_misc)
{
	u32 irq = irq_misc;

	irq &= ~(USBC_INTUSB_VBUS_ERROR | USBC_INTUSB_SESSION_REQ);
	USBC_INT_ClearMiscPending(USBC_INTUSB_VBUS_ERROR);
	USBC_INT_ClearMiscPending(USBC_INTUSB_SESSION_REQ);
	//USBC_INT_ClearMiscPending(USBC_INTUSB_SOF);

	return irq;
}
static void  usbd_start_work(void)
{
	usbprint("usbd_start_work\n");
	//enable_irq_udc(); //no use
	USBC_Dev_ConectSwitch( USBC_DEVICE_SWITCH_ON);
	//GH_USB_set_DevCtl_W(0x01);
}

static void  usbd_stop_work(void)
{
	usbprint("usbd_stop_work\n");
	//disable_irq_udc();//no use
	USBC_Dev_ConectSwitch(USBC_DEVICE_SWITCH_OFF); // default is pulldown
}

static void cfg_udc_command(enum sunxi_udc_cmd_e cmd)
{
	switch (cmd)
	{
	case SW_UDC_P_ENABLE:
		{
				usbd_start_work();
		}
		break;
	case SW_UDC_P_DISABLE:
		{
				usbd_stop_work();
		}
		break;
	case SW_UDC_P_RESET :
		usbprint("ERR: reset is not support\n");
		break;
	default:
		usbprint("ERR: unkown cmd(%d)\n",cmd);
		break;
	}

	return ;
}


void sunxi_udc_disable(void)
{
	usbprint("sunxi_udc_disable\n");
	/* Disable all interrupts */
	USBC_INT_DisableUsbMiscAll();
	USBC_INT_DisableEpAll(USBC_EP_TYPE_RX);
	USBC_INT_DisableEpAll(USBC_EP_TYPE_TX);

	/* Clear the interrupt registers */
	clear_all_irq();

	//USBC_Writew(0xffff, USBC_REG_INTTxE(USBC0_BASE));	
	//USBC_Writew(0xffff, USBC_REG_INTRxE(USBC0_BASE));
	//usb_otg_dump_reg_offset();	
	//cfg_udc_command(SW_UDC_P_DISABLE);


	/* Set speed to unknown */
	//dev->gadget.speed = USB_SPEED_UNKNOWN;
	return;
}

void usb_handle_ep0(void)
{
	//printf("usb ep 0 interupt!!\r\n");
	/* We make the assumption that sunxi_udc_UDC_IN_CSR1_REG equal to
		 * sunxi_udc_UDC_EP0_CSR_REG when index is zero */
	int ep0csr;
	int len;
	unsigned char databuf[8];
	USBC_SelectActiveEp(0);
	/* clear stall status */
	if (USBC_Dev_IsEpStall(USBC_EP_TYPE_EP0))
	{
		usbprint("ERR: ep0 stall\n");
		USBC_Dev_EpClearStall(USBC_EP_TYPE_EP0);
		usb_ep0_state = EP0_IDLE;
		return;
	}
	/* clear setup end */
	if (USBC_Dev_Ctrl_IsSetupEnd())
	{
		usbprint("handle_ep0: ep0 setup end\n");
		//sunxi_udc_nuke(dev, ep, 0);
		USBC_Dev_Ctrl_ClearSetupEnd();
		usb_ep0_state = EP0_IDLE;
	}
	usbprint("usb_ep0_state:%d\n",usb_ep0_state);
	usbprint("ep0csr:0x%02x\n",USBC_Dev_Read_EP0_CSR());
	if (USBC_Dev_IsReadDataReady(USBC_EP_TYPE_EP0))//if that is setup data ,turn to idle state
	{
		usb_ep0_state = EP0_IDLE;
	}
	switch(usb_ep0_state)
	{
		case EP0_IDLE:
			{
				if (!USBC_Dev_IsReadDataReady(USBC_EP_TYPE_EP0))
				{
					usbprint("ERR: data is ready, can not read data.\n");
					return;
				}
				len = USBC_ReadLenFromFifo(USBC_EP_TYPE_EP0);
				usbprint("ep0 len :%d.\n",len);
				if(len != 8)
				{
					int timeout = 16;
					while((timeout--)&&(len != 8))
					{
						len = USBC_ReadLenFromFifo(USBC_EP_TYPE_EP0);
					}
					usbprint("ep0 len wait :%d.\n",len);
				}
				if(len == 8)
				{
					usb_device_read_data_ep_pack(0,databuf,len);
					usb_device_read_data_status_ep0(0);
					//GD_USB_Set_CSR0L(0x40);
					if(databuf[0]&0x80)
					{
						usb_ep0_state = EP0_IN_DATA_PHASE;
					}
					else
					{
						usb_ep0_state = EP0_OUT_DATA_PHASE;
					}
//					usbprint("d[0] :0x%02x    data[1] :0x%02x    data[2] :0x%02x    data[3] :0x%02x    data[4] :0x%02x    data[5] :0x%02x    data[6] :0x%02x    data[7] :0x%02x.\n",
//							databuf[0],databuf[1],databuf[2],databuf[3],databuf[4],databuf[5],
//							databuf[6],databuf[7]);
					if(current_usb_type == USB_TYPE_USB_HID)
					{
						usb_hid_setup_handle(databuf,len);
					}
					else if(current_usb_type == USB_TYPE_USB_COM)
					{
						usb_cdc_setup_handle(databuf,len);
					}
					else
					{
						usbprint("ERR: usb class %d not suppost!!!\n",current_usb_type);
					}
				}
				else
				{
					usbprint("ERR: data len = %d, not setup data.\n",len);
					USBC_Dev_ReadDataStatus(USBC_EP_TYPE_EP0, 0);
					USBC_Dev_EpSendStall( USBC_EP_TYPE_EP0);
					return;
				}
		}break;
		case EP0_IN_DATA_PHASE:
			if (!usb_tx_pos)
			{
						//sunxi_udc_write_fifo(ep, req);
				usbprint("in data has sended!\n");
				usb_ep0_state = EP0_IDLE;
			}
			else
			{
				usbprint("in data is not write ok!\n");
				usbprint("usb_tx_pos : %d\n",usb_tx_pos);
				if(usb_tx_buf_len - usb_tx_pos > 64)
				{
					USBC_WritePacket(USBC_SelectFIFO(0), ep_max_len[0], usb_tx_buf + usb_tx_pos);
					USBC_Dev_WriteDataStatus(USBC_EP_TYPE_EP0 , 0);
				}
				else
				{
					USBC_WritePacket(USBC_SelectFIFO(0), usb_tx_buf_len - usb_tx_pos, usb_tx_buf + usb_tx_pos);
					USBC_Dev_WriteDataStatus(USBC_EP_TYPE_EP0 , 1);
					usb_tx_pos = 0;
					usb_ep0_state = EP0_IDLE;
				}
			}
			break;
		case EP0_OUT_DATA_PHASE:
			if (!USBC_Dev_IsReadDataReady(USBC_EP_TYPE_EP0))
			{
				usbprint("ep0_out_intr: data is ready, can not read data.\n");
				return;
			}
			len = USBC_ReadLenFromFifo(USBC_EP_TYPE_EP0);
			usbprint("ep0_out_intr len :%d.\n",len);
			usb_device_read_data_ep_pack(0,databuf,len);
			usb_device_read_data_status_ep0(0);
			usb_ep0_state = EP0_IDLE;
			usbprint("d[0] :0x%02x    data[1] :0x%02x    data[2] :0x%02x    data[3] :0x%02x    data[4] :0x%02x    data[5] :0x%02x    data[6] :0x%02x    data[7] :0x%02x.\n",
			databuf[0],databuf[1],databuf[2],databuf[3],databuf[4],databuf[5],
			databuf[6],databuf[7]);
			break;
		case EP0_END_XFER:
			if(usb_xfer_type == SET_ADDR)//no use
			{
				//USBC_SelectActiveEp(0);
				//usbprint("usb_xfer_type:SET_ADDR0x%02x\n",usb_addr);

				//USBC_Dev_Ctrl_ClearSetupEnd();
				//USBC_Dev_SetAddress(usb_addr);
//				usb_device_send_nullpack_ep0();

			}
			else if(usb_xfer_type == SET_CONFIG)
			{

			}
			usb_xfer_type = NONE_REQ;
			usb_ep0_state = EP0_IDLE;
			break;
		default:
			usb_ep0_state = EP0_IDLE;break;
	}
		return;
}

void usb_handle_epn_in(int ep)
{
	u32 old_ep_idx;
	u32 idx = ep;
	usbprint("USB in ep%d irq\n", ep);

	old_ep_idx = USBC_GetActiveEp();
	USBC_SelectActiveEp(idx);

	if (USBC_Dev_IsEpStall(USBC_EP_TYPE_TX))
	{
		usbprint("ERR: tx ep(%d) is stall\n", idx);
		USBC_Dev_EpClearStall(USBC_EP_TYPE_TX);
	}
	if (!USBC_Dev_IsWriteDataReady(USBC_EP_TYPE_TX))
	{
		usbprint("tx ep(%d) data ready!\n", idx);
		if(current_usb_type == USB_TYPE_USB_HID)
		{
			usb_hid_in_ep_callback();
			//USBC_Dev_WriteDataStatus(USBC_EP_TYPE_TX,1);
			//sunxi_udc_write_fifo(ep, req);
		}
	}
	USBC_SelectActiveEp(old_ep_idx);
}
void usb_handle_epn_out(int ep)
{
	u32 old_ep_idx;
	u32 idx = ep;
	u32 len;
	usbprint("USB out ep%d irq\n", ep);
	old_ep_idx = USBC_GetActiveEp();
	USBC_SelectActiveEp(idx);
	if (USBC_Dev_IsEpStall(USBC_EP_TYPE_RX))
	{
		usbprint("ERR: rx ep(%d) is stall\n", idx);
		USBC_Dev_EpClearStall(USBC_EP_TYPE_RX);
	}
	if (USBC_Dev_IsReadDataReady(USBC_EP_TYPE_RX))
	{
		len = USBC_ReadLenFromFifo(USBC_EP_TYPE_RX);
		usbprint("rx ep(%d) data ready Len:%d!\n", idx,len);
		usb_device_read_data_ep_pack(idx,usb_rx_buf,len);
		usb_rx_buf_len = len;
		if(current_usb_type == USB_TYPE_USB_HID)
		{
			usb_hid_out_ep_callback(usb_rx_buf,usb_rx_buf_len);
		}
		else if(current_usb_type == USB_TYPE_USB_COM)
		{
			usb_cdc_out_ep_callback(usb_rx_buf,usb_rx_buf_len);
		}
		USBC_Dev_ReadDataStatus(USBC_EP_TYPE_RX, 1);
//		usbprint("rx ep(%d) data ready data[0]:%d!\n", idx,usb_rx_buf[0]);
//		usb_device_write_data_ep_pack(idx,usb_rx_buf,usb_rx_buf_len);

	}
	USBC_SelectActiveEp(old_ep_idx);
}


void usb_irq_handle(int arg)
{
#if 1
		int usb_irq	= 0;
		int tx_irq	= 0;
		int rx_irq	= 0;
		int i		= 0;
		int dma_irq	= 0;
		u32 old_ep_idx  = 0;
	/* Save index */
		old_ep_idx = USBC_GetActiveEp();

		/* Read status registers */
		usb_irq = USBC_INT_MiscPending();
		tx_irq  = USBC_INT_EpPending(USBC_EP_TYPE_TX);
		rx_irq  = USBC_INT_EpPending(USBC_EP_TYPE_RX);
		//dma_irq = USBC_Readw(USBC_REG_DMA_INTS(dev->sunxi_udc_io->usb_vbase));

		usb_irq = filtrate_irq_misc(usb_irq);
		usbprint("_______________________________\n");
		usbprint("\nirq: usb_irq=%02x, tx_irq=%02x, rx_irq=%02x, dma_irq:%x\n", usb_irq, tx_irq, rx_irq, dma_irq);
		//usbprint(">usb addr:0x%02x\n",GH_USB_get_FAddr());
		//usbprint(">>usb frame:%d\n",GH_USB_get_Frame());
		//usbprint(">>>Time:%d\n",gkosGetTicks());
		/*
		 * Now, handle interrupts. There's two types :
		 * - Reset, Resume, Suspend coming -> usb_int_reg
		 * - EP -> ep_int_reg
		 */

		/* RESET */
		if (usb_irq & USBC_INTUSB_RESET)
		{
			usbprint("IRQ: reset\n");
			USBC_INT_ClearMiscPending(USBC_INTUSB_RESET);
			clear_all_irq();
			usb_connect = 1;
			usb_ep0_state = EP0_IDLE;
			USBC_SelectActiveEp(0);
			USBC_Dev_SetAddress_default();
			return;
		}

		/* RESUME */
		if (usb_irq & USBC_INTUSB_RESUME)
		{
			usbprint("IRQ: resume\n");
			/* clear interrupt */
			USBC_INT_ClearMiscPending(USBC_INTUSB_RESUME);
		}

		/* SUSPEND */
		if (usb_irq & USBC_INTUSB_SUSPEND)
		{
			usbprint("IRQ: suspend\n");

			/* clear interrupt */
			USBC_INT_ClearMiscPending(USBC_INTUSB_SUSPEND);
			usb_connect = 0;
			usb_ep0_state = EP0_IDLE;
		}

		/* DISCONNECT */
		if (usb_irq & USBC_INTUSB_DISCONNECT)
		{
			usbprint("IRQ: disconnect\n");

			USBC_INT_ClearMiscPending( USBC_INTUSB_DISCONNECT);
			usb_ep0_state = EP0_IDLE;
			usb_connect = 0;
		}
//		/* SOF */
//		if (usb_irq & USBC_INTUSB_SOF)
//		{
//			usbprint("IRQ: start of frame intr\n");
//			USBC_INT_ClearMiscPending( USBC_INTUSB_SOF);
//		}
		/* EP */
		/* control traffic */
		/* check on ep0csr != 0 is not a good idea as clearing in_pkt_ready
		 * generate an interrupt
		 */
		if (tx_irq & USBC_INTTx_FLAG_EP0)
		{
			usbprint("USB ep0 irq\n");
			/* Clear the interrupt bit by setting it to 1 */
			USBC_INT_ClearEpPending(USBC_EP_TYPE_TX, 0);
			if (current_speed == USB_SPEED_UNKNOWN)
			{
						if (USBC_Dev_QueryTransferMode() == USBC_TS_MODE_HS)
						{
							current_speed = USB_SPEED_HIGH_DEV;
							ep_max_len[1] = 512;
							ep_max_len[2] = 512;
							usbprint("\n+++++++++++++++++++++++++++++++++++++\n");
							usbprint(" usb enter high speed.\n");
							usbprint("\n+++++++++++++++++++++++++++++++++++++\n");
						}
						else
						{
							current_speed = USB_SPEED_FULL_DEV;
							ep_max_len[1] = 64;
							ep_max_len[2] = 64;
							usbprint("\n+++++++++++++++++++++++++++++++++++++\n");
							usbprint(" usb enter full speed.\n");
							usbprint("\n+++++++++++++++++++++++++++++++++++++\n");
						}
			}

			usb_handle_ep0();
			//USB_OUT_Intr_handle_Ep0();
		}

		/* firstly to get data */

		/* rx endpoint data transfers */
		for (i = 1; i <= SW_UDC_EPNUMS; i++)
		{
			u32 tmp = 1 << i;
			if (rx_irq & tmp)
			{

				//usbprint("USB rx ep%d irq\n", i);
				/* Clear the interrupt bit by setting it to 1 */
				USBC_INT_ClearEpPending( USBC_EP_TYPE_RX, i);
				//sunxi_udc_handle_ep(&dev->ep[ep_fifo_out[i]]);
				usb_handle_epn_out(i);

			}
		}

		/* tx endpoint data transfers */
		for (i = 1; i <= SW_UDC_EPNUMS; i++)
		{
			u32 tmp = 1 << i;
			if (tx_irq & tmp)
			{

				//usbprint("USB tx ep%d irq\n", i);
				/* Clear the interrupt bit by setting it to 1 */
				USBC_INT_ClearEpPending(USBC_EP_TYPE_TX, i);
				//sunxi_udc_handle_ep(&dev->ep[ep_fifo_in[i]]);
				usb_handle_epn_in(i);
			}
		}

		/* Restore old index */
		USBC_SelectActiveEp(old_ep_idx);
#endif 
	
}


static u32 fifo_offset = 0; // global

void usb_ep0_init(int epidx,int maxpack)
{
	u32 old_ep_idx  = 0;
	/* Save index */
	old_ep_idx = USBC_GetActiveEp();
	USBC_SelectActiveEp(epidx);
	u32 fifo_size = maxpack * 2;	// double buffer
	USBC_Dev_ConfigEp(1,USBC_EP_TYPE_EP0,1,maxpack);

}

void usb_config_ep0_init(int epidx,int maxpack,int interval)
{
	  u32 fifo_size = maxpack * 2;  // double buffer
	  //USBC_Dev_ConfigEp(type,USBC_EP_TYPE_TX,0,maxpack);

	  USBC_Host_ConfigEp(USBC_EP_TYPE_EP0,epidx,maxpack,interval);
	  USBC_ConfigFifo(USBC_EP_TYPE_EP0, 0, fifo_size, fifo_offset);
}

void usb_config_ep_in(int epidx,int maxpack,int interval)
{
	 u32 old_ep_idx  = 0;
	 if(epidx)
	 {
	  //usbprint("config int ep %d:%d , %d",epidx,maxpack,type);
	  /* Save index */
	  old_ep_idx = USBC_GetActiveEp();
	  USBC_SelectActiveEp(epidx);
	  u32 fifo_size = maxpack * 2;  // double buffer
	  //USBC_Dev_ConfigEp(type,USBC_EP_TYPE_TX,0,maxpack);

	  USBC_Host_ConfigEp(USBC_EP_TYPE_TX,epidx,maxpack,interval);
	  USBC_ConfigFifo(USBC_EP_TYPE_TX, 0, fifo_size, fifo_offset);
	  fifo_offset += fifo_size;
	  USBC_INT_EnableEp( USBC_EP_TYPE_TX, epidx);
	  USBC_SelectActiveEp(old_ep_idx);
	  //ep_max_len[epidx] = maxpack;
	 }
}
void usb_config_ep_out(int epidx,int maxpack,int interval)
{
	 u32 old_ep_idx  = 0;
	 if(epidx)
	 {
	  //usbprint("config out ep %d:%d , %d",epidx,maxpack,type);
	  /* Save index */
	  old_ep_idx = USBC_GetActiveEp();
	  USBC_SelectActiveEp(epidx);
	  u32 fifo_size = maxpack * 1;  // double buffer
	  //USBC_Dev_ConfigEp(type,USBC_EP_TYPE_RX,0,maxpack);
	  USBC_Host_ConfigEp(USBC_EP_TYPE_RX,epidx,maxpack,interval);
	  USBC_ConfigFifo(USBC_EP_TYPE_RX, 0, fifo_size, fifo_offset);
	  fifo_offset += fifo_size;
	  USBC_INT_EnableEp( USBC_EP_TYPE_RX, epidx);
	  USBC_SelectActiveEp(old_ep_idx);
	  //ep_max_len[epidx] = maxpack;
	 }
}

#if 0
void usb_config_ep_in(int epidx,int maxpack,int type)
{
	u32 old_ep_idx  = 0;
	if(epidx)
	{
		usbprint("config int ep %d:%d , %d\n",epidx,maxpack,type);
		/* Save index */
		old_ep_idx = USBC_GetActiveEp();
		USBC_SelectActiveEp(epidx);
		USBC_Dev_ConfigEp(type,USBC_EP_TYPE_TX,1,maxpack);
		USBC_INT_EnableEp( USBC_EP_TYPE_TX, epidx);
		USBC_SelectActiveEp(old_ep_idx);
		ep_max_len[epidx] = maxpack;
	}
}
void usb_config_ep_out(int epidx,int maxpack,int type)
{
	u32 old_ep_idx  = 0;
	if(epidx)
	{
		usbprint("config out ep %d:%d , %d\n",epidx,maxpack,type);
		/* Save index */
		old_ep_idx = USBC_GetActiveEp();
		USBC_SelectActiveEp(epidx);
		USBC_Dev_ConfigEp(type,USBC_EP_TYPE_RX,1,maxpack);
		USBC_INT_EnableEp( USBC_EP_TYPE_RX, epidx);
		USBC_SelectActiveEp(old_ep_idx);
		ep_max_len[epidx] = maxpack;
	}
}

#endif

void usb_device_clear_ep_halt(int epaddr)
{
	int old_ep_idx;
	int ep = epaddr & 0x7f;

	/* Save index */
	old_ep_idx = USBC_GetActiveEp();
	USBC_SelectActiveEp(ep);
	if(epaddr  & 0x80)
	{
		USBC_Clear_Feature_HALT(USBC_EP_TYPE_TX);
	}
	else
	{
		USBC_Clear_Feature_HALT(USBC_EP_TYPE_RX);
	}
	USBC_SelectActiveEp(old_ep_idx);
}

void sunxi_udc_enable()
{
	usbprint("sunxi_udc_enable called\n");

	/* dev->gadget.speed = USB_SPEED_UNKNOWN; */
	current_speed = USB_SPEED_UNKNOWN;

	usbprint("CONFIG_USB_GADGET_DUALSPEED: USBC_TS_MODE_HS\n");

	USBC_Dev_ConfigTransferMode(USBC_TS_TYPE_BULK, USBC_TS_MODE_HS);
#if 0
	/* Enable reset and suspend interrupt interrupts */
	USBC_INT_EnableUsbMiscUint( USBC_BP_INTUSB_SUSPEND);
	USBC_INT_EnableUsbMiscUint( USBC_BP_INTUSB_RESUME);
	USBC_INT_EnableUsbMiscUint( USBC_BP_INTUSB_RESET);
	USBC_INT_EnableUsbMiscUint( USBC_BP_INTUSB_DISCONNECT);
	USBC_INT_EnableUsbMiscUint( USBC_INTUSB_SOF);
	USBC_INT_EnableUsbMiscUint( USBC_INTUSB_CONNECT);
#endif	
	USBC_INT_EnableUsbMiscUint( 0x3E);
	/* Enable ep0 interrupt */
	USBC_INT_EnableEp( USBC_EP_TYPE_TX, 0);

	//cfg_udc_command(SW_UDC_P_ENABLE);

	return ;
}
int usb_device_init(unsigned char type)
{
	int		   	retval  = 0;
	if(current_usb_type != type)
	{
		usbprint("sunxi_usb_device_enable start\n");
		current_usb_type = type;
		retval = usb_dev_bsp_init(0);
		if (retval != 0)
		{
			usbprint("ERR: sunxi_udc_bsp_init failed\n");
			return -1;
		}
		//usbhid_usb_phy_init(41);
		sunxi_udc_disable();
	//	GD_USB_Intr_Set_CallBack(usb_irq_handle);
		//retval = request_irq(IRQ_USBOTG, usb_irq_handle,0);
		rt_hw_interrupt_install(USB_OTG_INTERRUPT,usb_irq_handle,(void*)0,"v3s_usb");
		rt_hw_interrupt_umask(USB_OTG_INTERRUPT);
//		if (retval != 0)
//		{
//			usbprint("ERR: cannot get irq %i, err %d\n", IRQ_USBOTG, retval);
//			retval = -1;
//			//goto err;
//		}
		sunxi_udc_enable();
		//cfg_udc_command(SW_UDC_P_ENABLE);
		usbprint("sunxi_usb_device_enable end\n");

		return retval;
	}
	return retval;
}
void usb_reg_debugdump(void)
{
	USBC_CoreRegDump();
}





