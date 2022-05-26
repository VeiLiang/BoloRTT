/*
 * usb_host_phy.c
 *
 *  Created on: 2020Äê4ÔÂ3ÈÕ
 *      Author: VeiLiang
 */

#include "usb_host_phy.h"

#define USB_ENDPOINT_NUMBER_MASK (0x0FU)

#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK (0x80U)
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT (7U)
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT (0U)
#define USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN (0x80U)

#define USB_CONTROL_ENDPOINT (0U)


extern void usb_otg_dump_reg_offset(int sel_idx);



static inline void HAL_USB_DoubleFlushTxFifo(usb_musb_reg_t *pUSB_Otg)
{
    /*In case of double fifo is used*/
    HAL_USB_SetTxCSR1(pUSB_Otg, USB_OTG_TXCSR1_FLUSHFIFO);
    HAL_USB_SetTxCSR1(pUSB_Otg, USB_OTG_TXCSR1_FLUSHFIFO);
}

static inline void HAL_USB_DoubleFlushRxFifo(usb_musb_reg_t *pUSB_Otg)
{
    /*In case of double fifo is used*/
    HAL_USB_SetRxCSR1(pUSB_Otg, USB_OTG_RXCSR1_FLUSHFIFO);
    HAL_USB_SetRxCSR1(pUSB_Otg, USB_OTG_RXCSR1_FLUSHFIFO);
}

#define DYNAMIC_FIFOSIZEOF_TXENDPOINT_X  128
#define DYNAMIC_FIFOSIZEOF_RXENDPOINT_X  128
#define DYNAMIC_TXRXENDPOINT_OFFSET      128
#define DYNAMIC_MAXPKT_SIZE              3

void HAL_USB_SetDynamicSize(usb_musb_reg_t *pUSB_Otg)
{
    uint8_t ep;
    uint16_t addr = 0; //fifo_addr = addr * 8
    uint16_t size = 0;   //fifo_size = 2^ (size + 3)
    uint16_t temp = 0;

    //--<1>-- 512 align
	temp = DYNAMIC_FIFOSIZEOF_TXENDPOINT_X + 511;
	temp &= ~511;
	temp >>= 3;
	temp >>= 1;
	while(temp)
	{
		size++;
		temp >>= 1;
	}

    addr += DYNAMIC_TXRXENDPOINT_OFFSET;
    for (ep = 1; ep < 4; ep ++)
    {
        /*Set index register*/
        pUSB_Otg->Index = ep;
        /*Set Tx FIFO size for endpoint ep*/
        pUSB_Otg->DyTxFIFOAddr = (addr / 8) & 0xFF;

        /*Double Packet Disabled*/
        //pUSB_Otg->DyTxFIFOSize = ((addr / 8) >> 8 & 0x0F) | (DYNAMIC_MAXPKT_SIZE << 5);
        pUSB_Otg->DyTxFIFOSize = size;
        addr += DYNAMIC_FIFOSIZEOF_TXENDPOINT_X;

        /*Set Rx FIFO size for endpoint ep*/
        pUSB_Otg->FIFO_Union.DyRxFIFOAddr = (addr / 8) & 0xFF;
        /*Double Packet Disabled*/
        //pUSB_Otg->DyRxFIFOSize = ((addr / 8) >> 8 & 0x0F) | (DYNAMIC_MAXPKT_SIZE << 5);
        pUSB_Otg->DyRxFIFOSize = size;
        addr += DYNAMIC_FIFOSIZEOF_RXENDPOINT_X;
    }
}

void HAL_USB_FlushTxFifo (usb_musb_reg_t *pUSB_Otg, uint32_t num )
{
    uint8_t ep = 1;

    /*Flush the FIFO of Tx Endpoints, where num is the numeric of endpoint*/
    if (num == 16)
    {
        /*Only for Tx Endpoint, not for Endpoint 0*/
        for (ep = 1; ep < num; ep ++)
        {
            pUSB_Otg->Index = ep;
            HAL_USB_DoubleFlushTxFifo(pUSB_Otg);
        }

        /*Finally flush the endpoint 0 FIFO, but it has no effect while TxPktRdy or RxPktRdy  is not set*/
        pUSB_Otg->Index = 0;
        pUSB_Otg->CtrlSts1_Union.CSR02 = USB_OTG_CSR02_FLUSHFIFO;
    }
    else if (num != 0)
    {
        /*Indexed for a dedicated Endpoint*/
        pUSB_Otg->Index = num;
        HAL_USB_DoubleFlushTxFifo(pUSB_Otg);
    }
    else if (num == 0)
    {

        pUSB_Otg->Index = num;
        pUSB_Otg->CtrlSts1_Union.CSR02 = USB_OTG_CSR02_FLUSHFIFO;
    }
}


void HAL_USB_FlushRxFifo(usb_musb_reg_t *pUSB_Otg, uint32_t num)
{
    uint8_t ep = 1;

    /*Flush the FIFO of Rx Endpoints, where num is the numeric of endpoint*/
    if (num == 16)
    {
        /*Only for Rx Endpoint, not for Endpoint 0*/
        for (ep = 1; ep < num; ep ++)
        {
            pUSB_Otg->Index = ep;
            HAL_USB_DoubleFlushRxFifo(pUSB_Otg);
        }

        /*Finally flush the endpoint 0 FIFO, but it has no effect while TxPktRdy or RxPktRdy  is not set*/
        pUSB_Otg->Index = 0;
        pUSB_Otg->CtrlSts1_Union.CSR02 = USB_OTG_CSR02_FLUSHFIFO;
    }
    else if (num != 0)
    {
        /*Indexed for a dedicated Endpoint*/
        pUSB_Otg->Index = num;
        HAL_USB_DoubleFlushRxFifo(pUSB_Otg);
    }
    else if (num == 0)
    {
        pUSB_Otg->Index = num;
        pUSB_Otg->CtrlSts1_Union.CSR02 = USB_OTG_CSR02_FLUSHFIFO;
    }
}

void HAL_USB_ClearEndpointRxPktRdy(usb_musb_reg_t *pUSB_Otg, uint8_t ep)
{
    /*Indexed for a dedicated Endpoint*/
    pUSB_Otg->Index = ep & 0x0F;

    if (ep == 0)
    {
        pUSB_Otg->CtrlSts0_Union.CSR0 |= USB_OTG_CSR0_SERVRXPKTRDY;
    }
    else
    {
        pUSB_Otg->RxCSR1 &= ~(USB_OTG_RXCSR1_RXPKTRDY);
    }
}

void HAL_USB_SetEndpointxTxPktRdy(usb_musb_reg_t *pUSB_Otg, uint8_t ep, uint8_t dataend)
{
    pUSB_Otg->Index = ep & 0x0F;

    /* Control Endpoint 0 */
    if ((ep & 0x0F) == 0)
    {
        if (dataend)
        {
            /*it's the end of transfer*/
            pUSB_Otg->CtrlSts0_Union.CSR0 |= (USB_OTG_CSR0_TXPKTRDY | USB_OTG_CSR0_DATAEND);
        }
        else
        {
            /*trigger to send data*/
            pUSB_Otg->CtrlSts0_Union.CSR0 |= USB_OTG_CSR0_TXPKTRDY;
        }
    }
    /*Tx Endpoints x other than Endpoint 0*/
    else
    {
        /* trigger to send data, now it will trigger next interrupt */
        pUSB_Otg->CtrlSts0_Union.TxCSR1 |= USB_OTG_TXCSR1_TXPKTRDY;
    }
}

void HAL_USB_WritePacket(usb_musb_reg_t *pUSB_Otg, uint8_t *src, uint8_t ep, uint16_t len)
{
    uint16_t i = 0U;
    uint8_t *debug = src;

    /* Write index register */
    pUSB_Otg->Index = ep & 0x0F;

    for (i = 0U; i < len; i++, src += 1U)
    {
       /*write data to the FIFO*/
       pUSB_Otg->FIFOx[ep * 4] = *((uint8_t *)src);
    }
}

uint8_t HAL_USB_ReadPacket(usb_musb_reg_t *pUSB_Otg, uint8_t *dest, uint8_t ep, uint16_t len)
{
    uint16_t i=0U;
    uint8_t error = 0;
    uint8_t *debug = dest;
    /* Write index register */
    pUSB_Otg->Index = ep & 0x0F;

    if (len != ((pUSB_Otg->RxCount2) << 8 | pUSB_Otg->Count_Union.RxCount1))
    {
        error = 1;
        return error;
    }

    for ( i = 0U; i < len; i++, dest += 1U )
    {
        *(uint8_t *)dest = pUSB_Otg->FIFOx[ep * 4];
    }
    return error;
}


void HAL_USB_InitializeDevice(usb_musb_reg_t *pUSB_Otg)
{
	int i=0;

    HAL_USB_FlushTxFifo(pUSB_Otg, 0x10);
    HAL_USB_FlushRxFifo(pUSB_Otg, 0x10);


    USB_CLEAR_AND_DISABLE_INTERRUPTS;

    /*Interrupts for USB Device or Host, now only implement that for Device*/
    pUSB_Otg->IntrUSBE |= (USB_OTG_INTRUSBE_RESUME | USB_OTG_INTRUSBE_RSTBABL |\
                       USB_OTG_INTRUSBE_SOF | USB_OTG_INTRUSBE_CON | USB_OTG_INTRUSBE_DISCON);

    /*just for debugging*/
    pUSB_Otg->InTrTx1E |= 0x0F;
    pUSB_Otg->InTrRx1E |= 0x0F;


	/*Set dynamic size for USB FIFO*/
	HAL_USB_SetDynamicSize(pUSB_Otg);
    usb_otg_dump_reg_offset(0);


    /*Start a session*/
    pUSB_Otg->DevCtl |= USB_OTG_DEVCTL_SESSION;
}


void HAL_USB_DeInitializeDevice(usb_musb_reg_t *pUSB_Otg)
{
    USB_CLEAR_AND_DISABLE_INTERRUPTS;

    /*Disable corresponding USB interrupt flags*/
    pUSB_Otg->IntrUSBE &= ~(USB_OTG_INTRUSBE_SUSPEND | USB_OTG_INTRUSBE_RESUME | USB_OTG_INTRUSBE_RSTBABL |\
                            USB_OTG_INTRUSBE_SOF | USB_OTG_INTRUSBE_CON | USB_OTG_INTRUSBE_DISCON |\
                            USB_OTG_INTRUSBE_SESSION | USB_OTG_INTRUSBE_VBUSERR);
}


void HAL_USB_EnableEndpoints(usb_musb_reg_t *pUSB_Otg, uint8_t ep)
{

    uint8_t endpoint = (ep & USB_ENDPOINT_NUMBER_MASK);
    uint8_t direction = (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) >>
                        USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT;

    pUSB_Otg->Index = endpoint;

    if (direction == 1U)
    {
        /*Configure the endpoint as In endpoint*/
        pUSB_Otg->CtrlSts1_Union.TxCSR2 |= USB_OTG_TXCSR2_MODE;

        /*Enable the endpoint's interrupt of Tx endpoint*/
        if (endpoint > 7)
        {
            pUSB_Otg->InTrTx2E |= 1 << (endpoint - 7);
        }
        else
        {
           pUSB_Otg->InTrTx1E |= 1 << (endpoint);
        }
    }
    else
    {
        /*Configure the endpoint as Out endpoint*/
        pUSB_Otg->CtrlSts1_Union.TxCSR2 &= ~(USB_OTG_TXCSR2_MODE);

        /*Enable the endpoint's interrupt of Rx endpoint*/
        if (endpoint > 7)
        {
            pUSB_Otg->InTrRx2E |= 1 << (endpoint - 7);
        }
        else
        {
           pUSB_Otg->InTrRx1E |= 1 << (endpoint);
        }
    }
}

void HAL_USB_DisableEndpoints(usb_musb_reg_t *pUSB_Otg, uint8_t ep)
{

}

void HAL_USB_EndpointsStall(usb_musb_reg_t *pUSB_Otg, uint8_t ep)
{
    uint8_t endpoint = (ep & USB_ENDPOINT_NUMBER_MASK);
    uint8_t direction = (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) >>
                        USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT;

    pUSB_Otg->Index = endpoint;

    /*Control Endpoint*/
    if (USB_CONTROL_ENDPOINT != endpoint)
    {
        if (direction == 1)
        {
            pUSB_Otg->CtrlSts0_Union.TxCSR1 |= USB_OTG_TXCSR1_SENDSTLL;
        }
        else
        {
            pUSB_Otg->RxCSR1 |= USB_OTG_RXCSR1_SENDSTLL;
        }
    }
    else
    {
        pUSB_Otg->CtrlSts0_Union.CSR0 |= USB_OTG_CSR0_SENDSTALL; // | USB_OTG_CSR0_SERVRXPKTRDY;
    }
}

void HAL_USB_EndpointsUnstall(usb_musb_reg_t *pUSB_Otg, uint8_t ep)
{
    uint8_t endpoint = (ep & USB_ENDPOINT_NUMBER_MASK);
    uint8_t direction = (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) >>
                        USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT;

    pUSB_Otg->Index = endpoint;

    if (endpoint != USB_CONTROL_ENDPOINT)
    {
        if (direction == 1U)
        {
            /*Send STALL condition when as a Tx endpoint*/
            pUSB_Otg->CtrlSts0_Union.TxCSR1 &= ~(USB_OTG_TXCSR1_SENTSTLL);
            pUSB_Otg->CtrlSts0_Union.TxCSR1 |= USB_OTG_TXCSR1_CLRDATATOG; /* DATA0 */
        }
        else
        {
            /*Send STALL condition when as a RX endpoint*/
            pUSB_Otg->RxCSR1 &= ~(USB_OTG_RXCSR1_SENTSTLL);
            pUSB_Otg->RxCSR1 |= USB_OTG_RXCSR1_CLRDATATOG; /* DATA0 */
        }
    }
    else
    {
        pUSB_Otg->CtrlSts0_Union.CSR0&= ~(USB_OTG_CSR0_SENTSTALL);
    }
}


uint32_t HAL_USB_ReadDevRxEPInterrupt (usb_musb_reg_t *pUSB_Otg, uint8_t ep)
{
    uint16_t intr = 0;

    pUSB_Otg->Index = ep;
    if (ep != 0)
    {
        intr = (pUSB_Otg->RxCSR2) << 8 | pUSB_Otg->RxCSR1;
    }

    return intr;
}


uint32_t HAL_USB_ReadDevTxEPInterrupt (usb_musb_reg_t *pUSB_Otg, uint8_t ep)
{
    uint16_t intr;

    pUSB_Otg->Index = ep;

    if (ep == 0)
    {
        intr = pUSB_Otg->CtrlSts0_Union.CSR0;
    }
    else
    {
        intr = (pUSB_Otg->CtrlSts1_Union.TxCSR2) << 8 | pUSB_Otg->CtrlSts0_Union.TxCSR1;
    }

    return intr;

}


