/*
 * drv_usb.c
 *
 *  Created on: 2020��1��13��
 *      Author: VeiLiang
 */
#include <rthw.h>
#include <rtthread.h>
#include "rtdevice.h"
#include "drivers/usb_common.h"
#include "usb_host_phy.h"
#include "usb_host_bsp.h"
#include "interrupt.h"
static usb_host_handle_t v3s_usb_host_handle;
static usb_host_pipe_t v3s_in_pipe_handle[8],v3s_out_pipe_handle[8];
static uint8_t* interrupt_ep_buf[8];
static uint8_t interrupt_ep_num[8];
static uint8_t interrupt_ep_cnt = 0;
static struct rt_completion urb_completion;
static volatile rt_bool_t connect_status = RT_FALSE;

#define OTG_FS_PORT 1

typedef enum tx_rx_type
{
    illegal_Type = 0,
    iso_Type,
    bulk_Type,
    interrupt_Type
}tx_rx_type_t;



int USB_HostTransaction(usb_host_handle_t *usbHostPointer,
														uint32_t address,
                                                       uint32_t type,
													   usb_host_pipe_t *pipeDescPointer,
                                                       uint8_t *bufPointer,
                                                       uint32_t len)
{
		int i;
		int old_index = HAL_USB_GetIndex(usbHostPointer->pUSB_Otg);
		len = (len > pipeDescPointer->maxPacketSize) ? pipeDescPointer->maxPacketSize : len;
		HAL_USB_SetIndex(usbHostPointer->pUSB_Otg,(pipeDescPointer->pipe_index & 0x0F));
		/*Set address*/
		usb_echo("addr:%x\n",address);
		usb_echo("pi:%d|ep:%d|t:%02x|m:%d\n",pipeDescPointer->pipe_index,pipeDescPointer->endpointAddress,pipeDescPointer->pipeType,pipeDescPointer->maxPacketSize);
		if(type != kTr_In || (pipeDescPointer->pipe_index & 0x0F) == 0)
		{
			usb_echo(">\n");
			HAL_USB_SetAddress(usbHostPointer->pUSB_Otg, (address & 0x7F));
		}
		else
		{
			usb_echo("<\n");
			HAL_USB_SetInReqAddress(usbHostPointer->pUSB_Otg, (address & 0x7F));
		}
		usb_echo("f:%02x|r:%02x\n",usbHostPointer->pUSB_Otg->FAddr,usbHostPointer->pUSB_Otg->RAddr);
		usbHostPointer->pipe_trans_buf = bufPointer;
		usbHostPointer->pipe_trans_len = 0; // clear buf len
        switch (type)
        {
            case kTr_Ctrl:
				if (pipeDescPointer->pipeType == USB_ENDPOINT_CONTROL)
				{
					/*write data to ep0' fifo*/
					HAL_USB_WritePacket(usbHostPointer->pUSB_Otg, bufPointer, USB_TOKEN_TOKENENDPT((uint8_t)pipeDescPointer->pipe_index),  len);
					/*Trigger Setup Token, this will send setup data to USB device*/
					if ((((uint8_t)pipeDescPointer->endpointAddress) & 0x0F) == USB_ENDPOINT_CONTROL)
					{
						HAL_USB_SetCSR0(usbHostPointer->pUSB_Otg, (USB_OTG_CSR0_SETUPPKT | USB_OTG_CSR0_TXPKTRDY));
					}
				}
				usbHostPointer->pipe_trans_len = len;
                break;

            case kTr_In:
                if (pipeDescPointer->pipeType == USB_ENDPOINT_CONTROL)
                {
                    if ((((uint8_t)pipeDescPointer->pipe_index) & 0x0F) == USB_ENDPOINT_CONTROL)
                    {
                        /*request data from usb device*/
                        usb_echo("USB_OTG_CSR0_REQPKT\r\n");
                        HAL_USB_SetCSR0(usbHostPointer->pUSB_Otg, USB_OTG_CSR0_REQPKT);
                    }
                }
                else if (pipeDescPointer->pipeType == USB_ENDPOINT_BULK)
                {
                    /*write txtype register*/
                    HAL_USB_SetRxType(usbHostPointer->pUSB_Otg, ((bulk_Type << 4) | (pipeDescPointer->endpointAddress & 0x0F)));
                    HAL_USB_SetRxIntv(usbHostPointer->pUSB_Otg, 0);
                    HAL_USB_SetRxMaxP(usbHostPointer->pUSB_Otg, pipeDescPointer->maxPacketSize);
                    /*trigger to receive data*/
                    HAL_USB_SetRxCSR1(usbHostPointer->pUSB_Otg, USB_OTG_RXCSR1_REQPKT);

                    usb_echo("Request by Bulk-read \r\n");
                }
                else if (pipeDescPointer->pipeType == USB_ENDPOINT_INTERRUPT)
                {

                    /*write txtype register*/
                    HAL_USB_SetRxType(usbHostPointer->pUSB_Otg, ((interrupt_Type << 4) | (pipeDescPointer->endpointAddress & 0x0F)));
                    //usb_echo(2,"index:0x%02x\n",usbHostPointer->pUSB_Otg->Index);
                    //usb_echo(2,"epaddr:%d\r\n",pipeDescPointer->endpointAddress);
                    HAL_USB_SetRxIntv(usbHostPointer->pUSB_Otg, pipeDescPointer->interval);

                    HAL_USB_SetRxCSR1(usbHostPointer->pUSB_Otg,USB_OTG_RXCSR1_REQPKT);
                }
                else if (pipeDescPointer->pipeType == USB_ENDPOINT_ISOCHRONOUS)
                {

                    /*write txtype register*/
                    HAL_USB_SetRxType(usbHostPointer->pUSB_Otg, ((iso_Type << 4) | (pipeDescPointer->endpointAddress & 0x0F)));

                    HAL_USB_SetRxIntv(usbHostPointer->pUSB_Otg, 2);

                    /*trigger to receive data*/
                    usb_echo("USB_ENDPOINT_ISOCHRONOUS\r\n");
                    HAL_USB_SetRxCSR1(usbHostPointer->pUSB_Otg, USB_OTG_RXCSR1_REQPKT);
                }
                break;

            case kTr_Out:
                if (pipeDescPointer->pipeType == USB_ENDPOINT_CONTROL)
                {
                    HAL_USB_WritePacket(usbHostPointer->pUSB_Otg, bufPointer, USB_TOKEN_TOKENENDPT((uint8_t)pipeDescPointer->pipe_index), len);

                    /*Trigger Setup Token, this will send setup data to USB device*/
                    if ((((uint8_t)pipeDescPointer->endpointAddress) & 0x0F) == USB_ENDPOINT_CONTROL)
                    {
                        usbHostPointer->pUSB_Otg->CtrlSts0_Union.CSR0 |= USB_OTG_CSR0_STATUSPKT | USB_OTG_CSR0_TXPKTRDY;
                    }
                }
                else if (pipeDescPointer->pipeType == USB_ENDPOINT_BULK)
                {

                    /*write data to fifo*/
                   HAL_USB_WritePacket(usbHostPointer->pUSB_Otg, bufPointer, (pipeDescPointer->pipe_index & 0x0F), len);
                    /*write txtype register*/
                    HAL_USB_SetTxType(usbHostPointer->pUSB_Otg, ((bulk_Type << 4) | (pipeDescPointer->endpointAddress & 0x0F)));

                    HAL_USB_SetTxMaxP(usbHostPointer->pUSB_Otg, pipeDescPointer->maxPacketSize);

                    HAL_USB_SetTxIntv(usbHostPointer->pUSB_Otg, 2);

                    usb_echo("Request by Bulk-write \r\n");
                    /*trigger to send data*/
                    HAL_USB_SetTxCSR1(usbHostPointer->pUSB_Otg, USB_OTG_TXCSR1_TXPKTRDY|USBC_TXCSR_H_WZC_BITS);
                }
                else if (pipeDescPointer->pipeType == USB_ENDPOINT_INTERRUPT)
                {
                    usb_echo("int out\r\n");
                    /*write txtype register*/
                    HAL_USB_SetTxType(usbHostPointer->pUSB_Otg, ((interrupt_Type << 4) | (pipeDescPointer->endpointAddress & 0x0F)));

                    HAL_USB_SetTxIntv(usbHostPointer->pUSB_Otg, 2);
                    /*trigger to send data*/
                    HAL_USB_SetTxCSR1(usbHostPointer->pUSB_Otg, USB_OTG_TXCSR1_TXPKTRDY);
                }
                else if (pipeDescPointer->pipeType == USB_ENDPOINT_ISOCHRONOUS)
                {
                    /*write txtype register*/
                    HAL_USB_SetTxType(usbHostPointer->pUSB_Otg, ((iso_Type << 4) | (pipeDescPointer->endpointAddress & 0x0F)));

                    HAL_USB_SetTxIntv(usbHostPointer->pUSB_Otg, 2);

                    /*trigger to send data*/
                    HAL_USB_SetTxCSR1(usbHostPointer->pUSB_Otg, USB_OTG_TXCSR1_TXPKTRDY);
                }
                usbHostPointer->pipe_trans_len = len;
                break;
            default:
                break;
        }
     HAL_USB_SetIndex(usbHostPointer->pUSB_Otg, old_index);
    /*implemented for data-transfer*/
    return 0;
}

rt_mailbox_t sem_int_trans;
uint8_t EpIntrBuf[512];
void usb_interrupt_ep_thread_proc(void* parameter)
{
    uint16_t count = 0;
    uint32_t epnum;
    uint32_t i;
    upipe_t upipe = 0;
    sem_int_trans = rt_mb_create("usb_imb",15,RT_IPC_FLAG_FIFO);
    while (1)
    {
    	if(rt_mb_recv(sem_int_trans,&epnum,RT_WAITING_FOREVER) == RT_EOK)
    	{
    		upipe = v3s_in_pipe_handle[epnum].pdata;
    		if(interrupt_ep_buf[epnum])
    		{
    			memcpy(interrupt_ep_buf[epnum],v3s_usb_host_handle.pipe_trans_buf,v3s_usb_host_handle.pipe_trans_len);
    		}
			upipe->status = UPIPE_STATUS_OK;
			if (upipe->callback != RT_NULL)
			{
				upipe->callback(upipe);
			}
    	}
    	else
		{
			upipe = v3s_in_pipe_handle[epnum].pdata;
			upipe->status = UPIPE_STATUS_ERROR;
			if (upipe->callback != RT_NULL)
			{
				upipe->callback(upipe);
			}
		}

    }
}

void HAL_HCD_Connect_Callback(usb_host_handle_t *hhcd)
{
    uhcd_t hcd = (uhcd_t)hhcd->pData;
    if (!connect_status)
    {
        connect_status = RT_TRUE;
        RT_DEBUG_LOG(RT_DEBUG_USB, ("usb connected\n"));
        rt_usbh_root_hub_connect_handler(hcd, OTG_FS_PORT, RT_TRUE);
    }
}

void HAL_HCD_Disconnect_Callback(usb_host_handle_t *hhcd)
{
    uhcd_t hcd = (uhcd_t)hhcd->pData;
    if (connect_status)
    {
        connect_status = RT_FALSE;
        RT_DEBUG_LOG(RT_DEBUG_USB, ("usb disconnnect\n"));
        rt_usbh_root_hub_disconnect_handler(hcd, OTG_FS_PORT);
    }
}

void HAL_HCD_HC_NotifyURBChange_Callback(usb_host_handle_t *hhcd)
{
    rt_completion_done(&urb_completion);
}


static int now_dev_speed = 0;
static rt_err_t drv_reset_port(rt_uint8_t port)
{
    RT_DEBUG_LOG(RT_DEBUG_USB, ("reset port\n"));
    unsigned int devCtl;
    devCtl = HAL_USB_GetDevCtrl(v3s_usb_host_handle.pUSB_Otg);
    /*Bus Reset Signal*/
	
    if (devCtl & USB_OTG_DEVCTL_LSDEV)
    {
    	HAL_USB_SetPower(v3s_usb_host_handle.pUSB_Otg, USB_OTG_POWER_RESET);
        /*Wait 25 ms*/
        HAL_DelayMs(500);
    }
    else
    {
        HAL_USB_SetPower(v3s_usb_host_handle.pUSB_Otg, USB_OTG_POWER_SETHS);
	    HAL_USB_SetPower(v3s_usb_host_handle.pUSB_Otg, USB_OTG_POWER_RESET);
        /*Wait 25 ms*/
        HAL_DelayMs(25);
    }
	HAL_USB_ClearPower(v3s_usb_host_handle.pUSB_Otg, USB_OTG_POWER_RESET);

	devCtl = HAL_USB_GetDevCtrl(v3s_usb_host_handle.pUSB_Otg);

	if (devCtl & USB_OTG_DEVCTL_FSDEV)
	{
		if(HAL_USB_PowerIsHs(v3s_usb_host_handle.pUSB_Otg))
		{
			usb_echo("Hight Speed Device connected \r\n");
			now_dev_speed = 2;
		}
		else
		{
			usb_echo("Full Speed Device connected \r\n");
			now_dev_speed = 0;
		}

	}

	if (devCtl & USB_OTG_DEVCTL_LSDEV)
	{
		usb_echo("Low Speed Device connected \r\n");
		now_dev_speed = 1;
        // usb_host_force_to_ls_mode();
        // HAL_USB_ClearPower(v3s_usb_host_handle.pUSB_Otg, USB_OTG_POWER_SETHS);
        // HAL_USB_SetPower(v3s_usb_host_handle.pUSB_Otg, USB_OTG_POWER_RESET);
        // HAL_USB_ClrDevCtrlBit(v3s_usb_host_handle.pUSB_Otg,USB_OTG_DEVCTL_SESSION);
        // sunxi_udc_disable();
        // HAL_DelayMs(500);
        // sunxi_udc_enable();
        // HAL_USB_SetDevCtrlBit(v3s_usb_host_handle.pUSB_Otg,USB_OTG_DEVCTL_SESSION);

        // /*Wait 25 ms*/
        // HAL_USB_SetPower(v3s_usb_host_handle.pUSB_Otg, USB_OTG_POWER_RESET);
        // HAL_DelayMs(500);
        // HAL_USB_ClearPower(v3s_usb_host_handle.pUSB_Otg, USB_OTG_POWER_RESET);

    
	// HAL_USB_ClearPower(v3s_usb_host_handle.pUSB_Otg, USB_OTG_POWER_RESET);
        //HAL_DelayMs(20);
	}
    return RT_EOK;
}

static int drv_pipe_xfer(upipe_t pipe, rt_uint8_t token, void *buffer, int nbytes, int timeouts)
{
    int timeout = timeouts;
    int retrytimes = 10;
    int address = 0;
    int epnum = pipe->ep.bEndpointAddress & 0x0f;
	int is_in = !!(pipe->ep.bEndpointAddress & 0x80);
	rt_base_t level = rt_hw_interrupt_disable();
	if((pipe->ep.bEndpointAddress & 0x80))
	{
		address = v3s_in_pipe_handle[pipe->ep.bEndpointAddress & 0x0f].devAddress;
		v3s_usb_host_handle.ep_state_array[1][epnum] = STATE_WAIT;
	}
	else
	{
		address = v3s_out_pipe_handle[pipe->ep.bEndpointAddress & 0x0f].devAddress;
		v3s_usb_host_handle.ep_state_array[0][epnum] = STATE_WAIT;
	}
	rt_hw_interrupt_enable(level);

    while (1)
    {
        if (!connect_status)
        {
            return -1;
        }
        rt_completion_init(&urb_completion);


        if((pipe->ep.bEndpointAddress & 0x80))
        {


        	if(token == USBH_PID_DATA)
        	{
        		RT_DEBUG_LOG(RT_DEBUG_USB, ("xfer read data\n"));
				if(USB_HostTransaction(&v3s_usb_host_handle,
						address,kTr_In,&v3s_in_pipe_handle[pipe->ep.bEndpointAddress & 0x07],buffer,nbytes)!= kStatus_USB_Success)
				{
					return kStatus_USB_Error;
				}
				if(pipe->ep.bmAttributes == USB_ENDPOINT_INTERRUPT)
				{
					interrupt_ep_buf[pipe->ep.bEndpointAddress&0x07] = buffer;
				}
        	}
        	else
        	{
        		RT_DEBUG_LOG(RT_DEBUG_USB, ("xfer read setup\n"));
        		if(USB_HostTransaction(&v3s_usb_host_handle,
        				address,kTr_Ctrl,&v3s_in_pipe_handle[pipe->ep.bEndpointAddress & 0x07],buffer,nbytes)!= kStatus_USB_Success)
				{
					return kStatus_USB_Error;
				}
        	}
        }
        else
        {

        	if(token == USBH_PID_DATA)
			{
        		RT_DEBUG_LOG(RT_DEBUG_USB, ("xfer write data\n"));
				if(USB_HostTransaction(&v3s_usb_host_handle,
						address,kTr_Out,&v3s_out_pipe_handle[pipe->ep.bEndpointAddress & 0x07],buffer,nbytes)!= kStatus_USB_Success)
				{
					return kStatus_USB_Error;
				}
			}
			else
			{
				RT_DEBUG_LOG(RT_DEBUG_USB, ("xfer write setup\n"));
				if(USB_HostTransaction(&v3s_usb_host_handle,
						address,kTr_Ctrl,&v3s_out_pipe_handle[pipe->ep.bEndpointAddress & 0x07],buffer,nbytes)!= kStatus_USB_Success)
				{
					return kStatus_USB_Error;
				}
			}
        }

        rt_completion_wait(&urb_completion, timeout);

        HAL_DelayMs(1);
        int state = v3s_usb_host_handle.ep_state_array[is_in][epnum];
        if(state == STATE_NAK)
        {
        	if (pipe->ep.bmAttributes == USB_EP_ATTR_INT)
            {
                rt_thread_delay((pipe->ep.bInterval * RT_TICK_PER_SECOND / 1000) > 0 ? (pipe->ep.bInterval * RT_TICK_PER_SECOND / 1000) : 1);
            }
            continue;
        }
        else if(state == STATE_ERR)
        {
        	pipe->status = UPIPE_STATUS_ERROR;
			if (pipe->callback != RT_NULL)
			{
				pipe->callback(pipe);
			}
			return -1;
        }
        else if(state == STATE_STALL)
		{
        	RT_DEBUG_LOG(RT_DEBUG_USB, ("stall\n"));
			pipe->status = UPIPE_STATUS_STALL;
			if (pipe->callback != RT_NULL)
			{
				pipe->callback(pipe);
			}
			return -1;
		}
        else if(state == STATE_OK)
        {


        	size_t size = v3s_usb_host_handle.pipe_trans_len;
        	RT_DEBUG_LOG(RT_DEBUG_USB, ("ok:%d\n",size));
        	pipe->status = UPIPE_STATUS_OK;
			if (pipe->callback != RT_NULL)
			{
				pipe->callback(pipe);
			}

			if (pipe->ep.bEndpointAddress & 0x80)
			{
				return size;
			}
			return nbytes;
        }
        if(!(--retrytimes))
        {
        	pipe->status = UPIPE_STATUS_ERROR;
			if (pipe->callback != RT_NULL)
			{
				pipe->callback(pipe);
			}
			usb_otg_dump_reg_offset(0);
			return -1;
        }
        continue;
    }
}

static rt_uint16_t pipe_index = 0;
static rt_uint8_t  drv_get_free_pipe_index(void)
{
    rt_uint8_t idx;
    for (idx = 1; idx < 16; idx++)
    {
        if (!(pipe_index & (0x01 << idx)))
        {
            pipe_index |= (0x01 << idx);
            return idx;
        }
    }
    return 0xff;
}

static void drv_free_pipe_index(rt_uint8_t index)
{
    pipe_index &= ~(0x01 << index);
}

static rt_err_t drv_open_pipe(upipe_t pipe)
{

	if(pipe->ep.bEndpointAddress & 0x0f)
	{
		pipe->pipe_index = drv_get_free_pipe_index();
	}
	else
	{
		pipe->pipe_index = 0;
	}
    int epnum = pipe->ep.bEndpointAddress & 0x0f;
    if((pipe->ep.bEndpointAddress & 0x80))
    {
    	usb_echo("drv_open_pipe R addr:%02x\r\n",pipe->inst->address);

		v3s_in_pipe_handle[epnum].devAddress = pipe->inst->address;
		v3s_in_pipe_handle[epnum].pipe_index = pipe->pipe_index;
		v3s_in_pipe_handle[epnum].usbHostHandle = &v3s_usb_host_handle;
		v3s_in_pipe_handle[epnum].pdata = pipe;
		v3s_in_pipe_handle[epnum].pipeType = pipe->ep.bmAttributes;
		v3s_in_pipe_handle[epnum].direction = 1;
		v3s_in_pipe_handle[epnum].endpointAddress = pipe->ep.bEndpointAddress&0x7f;
		v3s_in_pipe_handle[epnum].interval =  pipe->ep.bInterval;
		v3s_in_pipe_handle[epnum].maxPacketSize = pipe->ep.wMaxPacketSize;
		v3s_in_pipe_handle[epnum].nakCount = USB_HOST_CONFIG_MAX_NAK;
		v3s_in_pipe_handle[epnum].nextdata01 = 0;
		v3s_in_pipe_handle[epnum].int_ep_callback = pipe->callback;
    }
    else
    {
    	usb_echo("drv_open_pipe W addr:%02x\r\n",pipe->inst->address);
    	v3s_out_pipe_handle[epnum].pipe_index = pipe->pipe_index;
    	v3s_out_pipe_handle[epnum].devAddress = pipe->inst->address;
		v3s_out_pipe_handle[epnum].usbHostHandle = &v3s_usb_host_handle;
		v3s_in_pipe_handle[epnum].pdata = pipe;
		v3s_out_pipe_handle[epnum].pipeType = pipe->ep.bmAttributes;
		v3s_out_pipe_handle[epnum].direction = 1;
		v3s_out_pipe_handle[epnum].endpointAddress = pipe->ep.bEndpointAddress&0x7f;
		v3s_out_pipe_handle[epnum].interval =  pipe->ep.bInterval;
		v3s_out_pipe_handle[epnum].maxPacketSize = pipe->ep.wMaxPacketSize;
		v3s_out_pipe_handle[epnum].nakCount = USB_HOST_CONFIG_MAX_NAK;
		v3s_out_pipe_handle[epnum].nextdata01 = 0;
		v3s_out_pipe_handle[epnum].int_ep_callback = pipe->callback;
    }

    return RT_EOK;
}

static rt_err_t drv_close_pipe(upipe_t pipe)
{
	int epnum = pipe->ep.bEndpointAddress & 0x0f;
	if(pipe->ep.bEndpointAddress > 0x7f)
	{
		memset(&v3s_in_pipe_handle[epnum],0,sizeof(v3s_in_pipe_handle[epnum]));
	}
	else
	{
		memset(&v3s_in_pipe_handle[epnum],0,sizeof(v3s_in_pipe_handle[epnum]));
	}
    drv_free_pipe_index(pipe->pipe_index);
    return RT_EOK;
}

static struct uhcd_ops _uhcd_ops =
{
    drv_reset_port,
    drv_pipe_xfer,
    drv_open_pipe,
    drv_close_pipe,
};

void USB_HostIsrFunction(int vector,void *hostHandle)
{
	usb_host_handle_t *usbHostPointer;

    uint8_t status = 0;
    uint8_t epnum = 0;

    uint16_t ep_intr = 0;
    uint16_t epint = 0;
    uint16_t cnt = 0;
    //usb_echo("USB_HostIsrFunction \r\n");
    if (hostHandle == NULL)
    {
        return;
    }
    usbHostPointer = hostHandle;

    usbHostPointer->errsts = 0;
    {
        /*read to clear status*/
        status = HAL_USB_GetDeviceIntrStatus(usbHostPointer->pUSB_Otg);
		//usb_echo(2,"usb_irq=0x%02x \n",status);
        /* Handle Resume Interrupt */
        if (status & USB_OTG_INTRUSB_RESUME)
        {
			usb_echo("USB_OTG_INTRUSB_RESUME \r\n");
        }

        /* Handle Suspend Interrupt */
        if (status & USB_OTG_INTRUSB_SUSPEND)
        {
            usb_echo("USB_OTG_INTRUSB_SUSPEND \r\n");
        }

        if (status & USB_OTG_INTRUSB_DISCON)
        {
            usb_echo("Disconnect \r\n");
            usbHostPointer->deviceAttached = 0;
            HAL_HCD_Disconnect_Callback(usbHostPointer);
        }

        /* Handle Reset Interrupt */
        if (status & USB_OTG_INTRUSB_RSTBABL)
        {
            /*reset event, should only happen when disconnect*/
            //in host ,that is BABBLE
            // HAL_USB_SetPower(v3s_usb_host_handle.pUSB_Otg, USB_OTG_POWER_RESET);
            // volatile int time_dly = 1000;
            usb_echo("babble happen  \r\n");
            // HAL_USB_ClrDevCtrlBit(usbHostPointer->pUSB_Otg,USB_OTG_DEVCTL_SESSION);
            // sunxi_udc_disable();
            // while(time_dly--);
            // sunxi_udc_enable();
            // HAL_USB_SetDevCtrlBit(usbHostPointer->pUSB_Otg,USB_OTG_DEVCTL_SESSION);

            /*Bus Reset Signal*/
            //
            //HAL_DelayMs(25);
            // HAL_USB_ClearPower(v3s_usb_host_handle.pUSB_Otg, USB_OTG_POWER_RESET);

			/*FIXME: maybe do somethiing*/
        }

        /* Handle SOF Interrupt */
        if (status & USB_OTG_INTRUSB_SOF)
        {
        	//usbHostPointer->sof_frame++;
        }

        /*Handle the Device Connection Interrupt*/
        if (status & USB_OTG_INTRUSB_CON)
        {
			usb_echo("USB_OTG_INTRUSB_CON \r\n");
			usbHostPointer->deviceAttached = 1;
            /*Flash All FIFOs of all endpoints */
            HAL_USB_FlushTxFifo(usbHostPointer->pUSB_Otg, 0x10U);
            HAL_HCD_Connect_Callback(usbHostPointer);
        }

        /* Read in the device interrupt bits */
        ep_intr = USB_ReadDevAllTxEpInterrupt(usbHostPointer->pUSB_Otg);

        epnum = 0U;
        int old_idex = usbHostPointer->pUSB_Otg->Index;
        while ( ep_intr )
        {
            if (ep_intr & 0x1U) /* out ITR */
            {
                epint = HAL_USB_ReadDevTxEPInterrupt(usbHostPointer->pUSB_Otg, epnum);
				usb_echo("\nout epnum=%d,epint=0x%02x\n",epnum,epint);
                if (epnum == 0)/*just handle endpoint 0*/
                {
                	usbHostPointer->ep_state_array[0][0] = STATE_OK;
                	usbHostPointer->ep_state_array[1][0] = STATE_OK;
                    /*Clear the SETUPPKT flag*/
                    if ((epint & USB_OTG_CSR0_SETUPPKT) == USB_OTG_CSR0_SETUPPKT)
                    {
						usb_echo("SETUPPKT\n");
                        HAL_USB_ClearCSR0(usbHostPointer->pUSB_Otg, USB_OTG_CSR0_SETUPPKT);

                        /*Set limit0 for NAK, may be other value than 9*/
                        HAL_USB_SetNakLmt0(usbHostPointer->pUSB_Otg, 200);

                    }

                    /*����0�˿ڶ����� ���������������*/
                    if ((epint & USB_OTG_CSR0_RXPKTRDY) == USB_OTG_CSR0_RXPKTRDY)
                    {
						//usb_echo("RXPKTRDY\n");
						
                        cnt = HAL_USB_GetOutEpFifoCnt(usbHostPointer->pUSB_Otg);
                        /*Read the data from FIFO first !!!*/
						usb_echo("cnt=%d\n",cnt);
                        HAL_USB_ReadPacket(usbHostPointer->pUSB_Otg, usbHostPointer->pipe_trans_buf, 0, cnt);
                        usbHostPointer->pipe_trans_len = cnt;
                        usbHostPointer->ep_state_array[0][0] = STATE_OK;
						usbHostPointer->ep_state_array[1][0] = STATE_OK;
                        HAL_USB_ClearCSR0(usbHostPointer->pUSB_Otg, USB_OTG_CSR0_RXPKTRDY);
                    }

                    /*clear the status flag*/
                    if ((epint & USB_OTG_CSR0_STATUSPKT) == USB_OTG_CSR0_STATUSPKT)
                    {
						usb_echo("USB_OTG_CSR0_STATUSPKT\n");

                        HAL_USB_ClearCSR0(usbHostPointer->pUSB_Otg, USB_OTG_CSR0_STATUSPKT);
                    }

                    /*check send transaction status*/
                    if ((epint  & USB_OTG_CSR0_ERR) == USB_OTG_CSR0_ERR)
                    {
						usb_echo("ERR no NAK\n");
                        HAL_USB_ClearCSR0(usbHostPointer->pUSB_Otg, USB_OTG_TXCSR1_ERR);
                        usbHostPointer->ep_state_array[0][0] = STATE_ERR;
                        usbHostPointer->ep_state_array[1][0] = STATE_ERR;
                    }

                    if ((epint  & USB_OTG_CSR0_RXSTALL) == USB_OTG_CSR0_RXSTALL)
                    {
						usb_echo("RXSTALL\n");

                        HAL_USB_ClearCSR0(usbHostPointer->pUSB_Otg, USB_OTG_CSR0_RXSTALL);
                        usbHostPointer->ep_state_array[0][0] = STATE_STALL;
						usbHostPointer->ep_state_array[1][0] = STATE_STALL;
                    }

                    /* atom transaction done - token done */
                    HAL_HCD_HC_NotifyURBChange_Callback(&v3s_usb_host_handle);
                }
                else /*just handle endpointx other than 0 Tx*/
                {
                	int is_ok = 1;
                    if ((epint & USB_OTG_TXCSR1_ERR) == USB_OTG_TXCSR1_ERR)
                    {
                        usb_echo("ERR no NAK\n");
                        usbHostPointer->errsts |= USB_OTG_TXCSR1_ERR;

                        HAL_USB_ClearTxCSR1(usbHostPointer->pUSB_Otg, USB_OTG_TXCSR1_ERR);
                        usbHostPointer->ep_state_array[0][epnum] = STATE_ERR;
                        is_ok = 0;
                    }

                    if ((epint & USB_OTG_TXCSR1_RXSTALL) == USB_OTG_TXCSR1_RXSTALL)
                    {
                        usb_echo("RXSTALL\n");
                        usbHostPointer->errsts |= USB_OTG_TXCSR1_RXSTALL;

                        HAL_USB_ClearTxCSR1(usbHostPointer->pUSB_Otg, USB_OTG_TXCSR1_RXSTALL);
                        usbHostPointer->ep_state_array[0][epnum] = STATE_STALL;
                        is_ok = 0;
                    }

                    if ((epint & USB_OTG_TXCSR1_NAKTMOUT) == USB_OTG_TXCSR1_NAKTMOUT)
                    {
                        usb_echo("3 : 0x%x \n", epint);
                        usbHostPointer->errsts |= USB_OTG_TXCSR1_NAKTMOUT;
                        //usbHostPointer->pUSB_Otg->CtrlSts0_Union.TxCSR1 &= ~(USB_OTG_TXCSR1_NAKTMOUT);
                        HAL_USB_ClearTxCSR1(usbHostPointer->pUSB_Otg, USB_OTG_TXCSR1_NAKTMOUT);
                        usbHostPointer->ep_state_array[0][epnum] = STATE_NAK;
                        is_ok = 0;
                    }
                    if(is_ok)
                    {
                    	usbHostPointer->ep_state_array[0][epnum] = STATE_OK;
                    }
                    /* atom transaction done - token done */
                    HAL_HCD_HC_NotifyURBChange_Callback(&v3s_usb_host_handle);
                }
            }

            epnum++;
            ep_intr >>= 1U;
        }

        epnum = 0U;
        /* Read in the device interrupt bits */
        ep_intr = USB_ReadDevAllRxEpInterrupt(usbHostPointer->pUSB_Otg);

        while ( ep_intr )
        {
            if (ep_intr & 0x1U)
            {
            	int is_ok = 1;
                /*Actually In Epoint can not handle endpoint 0, here*/
                epint = HAL_USB_ReadDevRxEPInterrupt(usbHostPointer->pUSB_Otg, epnum);
                usb_echo("\nin epnum=%d,epint=0x%02x\n",epnum,epint);
                /*Endpoint 0 has different senses from Endpoint 1 ~ 15*/
                if (epnum != 0)
                {
                    if ((epint & USB_OTG_RXCSR1_RXPKTRDY) == USB_OTG_RXCSR1_RXPKTRDY)
                    {
                        cnt = HAL_USB_GetOutEpFifoCnt(usbHostPointer->pUSB_Otg);

                        HAL_USB_ReadPacket(usbHostPointer->pUSB_Otg, usbHostPointer->pipe_trans_buf, (epnum & 0x0F), cnt);
                        usbHostPointer->pipe_trans_len = cnt;

                        HAL_USB_ClearRxCSR1(usbHostPointer->pUSB_Otg, USB_OTG_RXCSR1_RXPKTRDY);

                        usbHostPointer->ep_state_array[1][epnum] = STATE_OK;
                        if(v3s_in_pipe_handle[epnum].pipeType == USB_ENDPOINT_INTERRUPT)
                        {
                        	HAL_USB_SetRxCSR1(usbHostPointer->pUSB_Otg,USB_OTG_RXCSR1_REQPKT);// request next transform
                        	rt_mb_send(sem_int_trans,epnum);
                        }
                        is_ok = 1;
                    }

                    if ((epint & USB_OTG_RXCSR1_ERR) == USB_OTG_RXCSR1_ERR)
                    {
                        usb_echo("ERR no NAK\n", epint);
                        usbHostPointer->errsts |= USB_OTG_RXCSR1_ERR;

                        HAL_USB_ClearRxCSR1(usbHostPointer->pUSB_Otg, USB_OTG_RXCSR1_ERR);
                        usbHostPointer->ep_state_array[1][epnum] = STATE_ERR;
                        is_ok = 0;
                    }

                    if ((epint &  USB_OTG_RXCSR1_ERRTMOUT) ==  USB_OTG_RXCSR1_ERRTMOUT)
                    {
                        usb_echo("ERRTMOUT\n");
                        usbHostPointer->errsts |= USB_OTG_RXCSR1_ERRTMOUT;
                        HAL_USB_ClearRxCSR1(usbHostPointer->pUSB_Otg, USB_OTG_RXCSR1_ERRTMOUT);
                        usbHostPointer->ep_state_array[1][epnum] = STATE_NAK;
                        is_ok = 0;
                    }

                    if ((epint &  USB_OTG_RXCSR1_RXSTALL) ==  USB_OTG_RXCSR1_RXSTALL)
                    {
                        usb_echo("RXSTALL\n");
                        HAL_USB_ClearRxCSR1(usbHostPointer->pUSB_Otg, USB_OTG_RXCSR1_RXSTALL);
                        usbHostPointer->ep_state_array[1][epnum] = STATE_STALL;
                        is_ok = 0;
                    }
                    /* atom transaction done - token done */
                    HAL_HCD_HC_NotifyURBChange_Callback(usbHostPointer);
                }
            }

            epnum++;
            ep_intr >>= 1U;
        }
        usbHostPointer->pUSB_Otg->Index = old_idex;
    }
}

static void USB_HostIsrInit(void* host_handle)
{
    /* Install isr, set priority, and enable IRQ. */

	rt_hw_interrupt_install(USB_OTG_INTERRUPT,USB_HostIsrFunction,(void*)host_handle,"host_otg");
	rt_hw_interrupt_umask(USB_OTG_INTERRUPT);

//	rt_hw_interrupt_install(IRQ_USB_OTG_EHCI0,HAL_USB_IRQHandler,(void*)1,"v3s_host_echi");
//	rt_hw_interrupt_umask(IRQ_USB_OTG_EHCI0);
}
static rt_err_t v3s_hcd_init(rt_device_t device)
{
	//usb_dev_bsp_init(1);
	usb_host_handle_t *hhcd = (usb_host_handle_t *)device->user_data;

	rt_err_t ret = RT_EOK;
	ret = USB_HostInit(hhcd);
	if (ret != RT_EOK)
	{
		usb_echo("host init error\r\n");
		return ret;
	}

	USB_HostIsrInit(hhcd);

	usb_echo("host init done\r\n");
	rt_thread_t t;
		t = rt_thread_create("ep_int",usb_interrupt_ep_thread_proc,NULL,10*1024,9,20);

		if (t)
		{
			rt_kprintf("\033[0;33m create thread 1 is success \033[0m \n");
			rt_thread_startup(t);
		}
	return ret;
}


static int v3s_usbh_register(void)
{
    rt_err_t res = -RT_ERROR;

    uhcd_t uhcd = (uhcd_t)rt_malloc(sizeof(struct uhcd));
    if (uhcd == RT_NULL)
    {
        rt_kprintf("uhcd malloc failed\r\n");
        return -RT_ERROR;
    }
    rt_memset((void *)uhcd, 0, sizeof(struct uhcd));

    uhcd->parent.type = RT_Device_Class_USBHost;
    uhcd->parent.init = v3s_hcd_init;
    uhcd->parent.user_data = &v3s_usb_host_handle;

    uhcd->ops = &_uhcd_ops;
    uhcd->num_ports = 1;
    v3s_usb_host_handle.pData = uhcd;

    res = rt_device_register(&uhcd->parent, "usbh", RT_DEVICE_FLAG_DEACTIVATE);
    if (res != RT_EOK)
    {
        rt_kprintf("register usb host failed res = %d\r\n", res);
        return -RT_ERROR;
    }
    rt_kprintf("uhcd host init\r\n");
    rt_usb_host_init();

    return RT_EOK;
}
INIT_DEVICE_EXPORT(v3s_usbh_register);
