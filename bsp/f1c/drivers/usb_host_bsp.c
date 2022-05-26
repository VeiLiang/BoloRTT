/*
 * usb_host_bsp.c
 *
 *  Created on: 2020Äê4ÔÂ3ÈÕ
 *      Author: VeiLiang
 */

#include "usb_phy.h"
#include "usb_host_bsp.h"
#include "usb_host_phy.h"

usb_musb_reg_t *pDebugOTG = USB_HOST_OTG_ADDR;

void usb_otg_dump_reg_offset(int sel_idx)
{
	int old_idx = pDebugOTG->Index;
	pDebugOTG->Index = sel_idx;

	usb_echo("pDebugOTG->FIFOx|0x%x\n",&pDebugOTG->FIFOx[0]);
	usb_echo("pDebugOTG->Power|%02x:0x%02x\n",(&pDebugOTG->Power)-(&pDebugOTG->FIFOx[0]),pDebugOTG->Power);
	usb_echo("pDebugOTG->DevCtl|%02x:0x%02x\n",(&pDebugOTG->DevCtl)-(&pDebugOTG->FIFOx[0]),pDebugOTG->DevCtl);
	usb_echo("pDebugOTG->Index|%02x:0x%02x\n",(&pDebugOTG->Index)-(&pDebugOTG->FIFOx[0]),pDebugOTG->Index);
	usb_echo("pDebugOTG->VEND0|%02x:0x%02x\n",(&pDebugOTG->VEND0)-(&pDebugOTG->FIFOx[0]),pDebugOTG->VEND0);
	usb_echo("pDebugOTG->IntrTx1|%02x:0x%02x\n",(&pDebugOTG->IntrTx1)-(&pDebugOTG->FIFOx[0]),pDebugOTG->IntrTx1);
	usb_echo("pDebugOTG->IntrTx2|%02x:0x%02x\n",(&pDebugOTG->IntrTx2)-(&pDebugOTG->FIFOx[0]),pDebugOTG->IntrTx2);
	usb_echo("pDebugOTG->IntrRx1|%02x:0x%02x\n",(&pDebugOTG->IntrRx1)-(&pDebugOTG->FIFOx[0]),pDebugOTG->IntrRx1);
	usb_echo("pDebugOTG->IntrRx2|%02x:0x%02x\n",(&pDebugOTG->IntrRx2)-(&pDebugOTG->FIFOx[0]),pDebugOTG->IntrRx2);
	usb_echo("pDebugOTG->InTrTx1E|%02x:0x%02x\n",(&pDebugOTG->InTrTx1E)-(&pDebugOTG->FIFOx[0]),pDebugOTG->InTrTx1E);
	usb_echo("pDebugOTG->InTrTx2E|%02x:0x%02x\n",(&pDebugOTG->InTrTx2E)-(&pDebugOTG->FIFOx[0]),pDebugOTG->InTrTx2E);
	usb_echo("pDebugOTG->InTrRx1E|%02x:0x%02x\n",(&pDebugOTG->InTrRx1E)-(&pDebugOTG->FIFOx[0]),pDebugOTG->InTrRx1E);
	usb_echo("pDebugOTG->InTrRx2E|%02x:0x%02x\n",(&pDebugOTG->InTrRx2E)-(&pDebugOTG->FIFOx[0]),pDebugOTG->InTrRx2E);
	//usb_echo("pDebugOTG->IntrUSB|%02x:0x%02x\n",(&pDebugOTG->IntrUSB)-(&pDebugOTG->FIFOx[0]),pDebugOTG->IntrUSB);
	usb_echo("pDebugOTG->IntrUSBE|%02x:0x%02x\n",(&pDebugOTG->IntrUSBE)-(&pDebugOTG->FIFOx[0]),pDebugOTG->IntrUSBE);
	usb_echo("pDebugOTG->Frame1|%02x:0x%02x\n",(&pDebugOTG->Frame1)-(&pDebugOTG->FIFOx[0]),pDebugOTG->Frame1);
	usb_echo("pDebugOTG->Frame2|%02x:0x%02x\n",(&pDebugOTG->Frame2)-(&pDebugOTG->FIFOx[0]),pDebugOTG->Frame2);
	//usb_echo("pDebugOTG->TxMaxP|%02x:0x%04x\n",(&pDebugOTG->TxMaxP)-(&pDebugOTG->FIFOx[0]),pDebugOTG->TxMaxP);
	usb_echo("pDebugOTG->CtrlSts0_Union.CSR0|%02x:0x%02x\n",(&pDebugOTG->CtrlSts0_Union.CSR0)-(&pDebugOTG->FIFOx[0]),pDebugOTG->CtrlSts0_Union.CSR0);
	usb_echo("pDebugOTG->CtrlSts0_Union.TxCSR1|%02x:0x%02x\n",(&pDebugOTG->CtrlSts0_Union.TxCSR1)-(&pDebugOTG->FIFOx[0]),pDebugOTG->CtrlSts0_Union.TxCSR1);
	usb_echo("pDebugOTG->CtrlSts1_Union.CSR02|%02x:0x%02x\n",(&pDebugOTG->CtrlSts1_Union.CSR02)-(&pDebugOTG->FIFOx[0]),pDebugOTG->CtrlSts1_Union.CSR02);
	usb_echo("pDebugOTG->CtrlSts1_Union.TxCSR2|%02x:0x%02x\n",(&pDebugOTG->CtrlSts1_Union.TxCSR2)-(&pDebugOTG->FIFOx[0]),pDebugOTG->CtrlSts1_Union.TxCSR2);

	//usb_echo("pDebugOTG->RxMaxP|%02x:0x%04x\n",(&pDebugOTG->RxMaxP)-(&pDebugOTG->FIFOx[0]),pDebugOTG->RxMaxP);
	usb_echo("pDebugOTG->RxCSR1|%02x:0x%02x\n",(&pDebugOTG->RxCSR1)-(&pDebugOTG->FIFOx[0]),pDebugOTG->RxCSR1);
	usb_echo("pDebugOTG->RxCSR2|%02x:0x%02x\n",(&pDebugOTG->RxCSR2)-(&pDebugOTG->FIFOx[0]),pDebugOTG->RxCSR2);
	usb_echo("pDebugOTG->Count_Union.Count0|%02x:0x%02x\n",(&pDebugOTG->Count_Union.Count0)-(&pDebugOTG->FIFOx[0]),pDebugOTG->Count_Union.RxCount1);
	usb_echo("pDebugOTG->Count_Union.RxCount1|%02x:0x%02x\n",(&pDebugOTG->Count_Union.RxCount1)-(&pDebugOTG->FIFOx[0]),pDebugOTG->Count_Union.RxCount1);
	usb_echo("pDebugOTG->RxCount2|%02x:0x%02x\n",(&pDebugOTG->RxCount2)-(&pDebugOTG->FIFOx[0]),pDebugOTG->RxCount2);

	usb_echo("pDebugOTG->TxType|%02x:0x%02x\n",(&pDebugOTG->TxType)-(&pDebugOTG->FIFOx[0]),pDebugOTG->TxType);
	usb_echo("pDebugOTG->IntvNAKLmt0_Union.NAKLmt0|%02x:0x%02x\n",(&pDebugOTG->IntvNAKLmt0_Union.NAKLmt0)-(&pDebugOTG->FIFOx[0]),pDebugOTG->IntvNAKLmt0_Union.NAKLmt0);
	usb_echo("pDebugOTG->IntvNAKLmt0_Union.TxIntv|%02x:0x%02x\n",(&pDebugOTG->IntvNAKLmt0_Union.TxIntv)-(&pDebugOTG->FIFOx[0]),pDebugOTG->IntvNAKLmt0_Union.TxIntv);
	usb_echo("pDebugOTG->RxType|%02x:0x%02x\n",(&pDebugOTG->RxType)-(&pDebugOTG->FIFOx[0]),pDebugOTG->RxType);
	usb_echo("pDebugOTG->RxIntv|%02x:0x%02x\n",(&pDebugOTG->RxIntv)-(&pDebugOTG->FIFOx[0]),pDebugOTG->RxIntv);
	usb_echo("pDebugOTG->DyTxFIFOSize|%02x:0x%02x\n",(&pDebugOTG->DyTxFIFOSize)-(&pDebugOTG->FIFOx[0]),pDebugOTG->DyTxFIFOSize);
	usb_echo("pDebugOTG->DyTxFIFOAddr|%02x:0x%02x\n",(&pDebugOTG->DyTxFIFOAddr)-(&pDebugOTG->FIFOx[0]),pDebugOTG->DyTxFIFOAddr);
	usb_echo("pDebugOTG->DyRxFIFOSize|%02x:0x%02x\n",(&pDebugOTG->DyRxFIFOSize)-(&pDebugOTG->FIFOx[0]),pDebugOTG->DyRxFIFOSize);
	usb_echo("pDebugOTG->FIFO_Union.DyRxFIFOAddr|%02x:0x%02x\n",(&pDebugOTG->FIFO_Union.DyRxFIFOAddr)-(&pDebugOTG->FIFOx[0]),pDebugOTG->FIFO_Union.DyRxFIFOAddr);
	usb_echo("pDebugOTG->FIFO_Union.FIFOSize|%02x:0x%02x\n",(&pDebugOTG->FIFO_Union.FIFOSize)-(&pDebugOTG->FIFOx[0]),pDebugOTG->FIFO_Union.FIFOSize);
	usb_echo("pDebugOTG->FAddr|%02x:0x%02x\n",(&pDebugOTG->FAddr)-(&pDebugOTG->FIFOx[0]),pDebugOTG->FAddr);
	usb_echo("pDebugOTG->HADDR0|%02x:0x%02x\n",(&pDebugOTG->rfu9[0])-(&pDebugOTG->FIFOx[0]),pDebugOTG->rfu9[0]);
	usb_echo("pDebugOTG->HPORT0|%02x:0x%02x\n",(&pDebugOTG->rfu9[1])-(&pDebugOTG->FIFOx[0]),pDebugOTG->rfu9[1]);
	usb_echo("pDebugOTG->RAddr|%02x:0x%02x\n",(&pDebugOTG->RAddr)-(&pDebugOTG->FIFOx[0]),pDebugOTG->RAddr);
	usb_echo("pDebugOTG->RXHADDRx|%02x:0x%02x\n",(&pDebugOTG->rfu10[0])-(&pDebugOTG->FIFOx[0]),pDebugOTG->rfu10[0]);
	usb_echo("pDebugOTG->RXHPORTx|%02x:0x%02x\n",(&pDebugOTG->rfu10[1])-(&pDebugOTG->FIFOx[0]),pDebugOTG->rfu10[1]);
	pDebugOTG->Index = old_idx;
}

//void usb_ehci_dump_reg_offset(void)
//{
//
//	usb_echo("CAPLENGTH|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->CAPLENGTH)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->CAPLENGTH);
//	usb_echo("HCIVERSION|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->HCIVERSION)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->HCIVERSION);
//	usb_echo("HCSPARAMS|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->HCSPARAMS)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->HCSPARAMS);
//	usb_echo("HCCPARAMS|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->HCCPARAMS)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->HCCPARAMS);
//	usb_echo("HCSPPORTROUTE|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->HCSPPORTROUTE)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->HCSPPORTROUTE);
//	usb_echo("USBCMD|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->USBCMD)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->USBCMD);
//	usb_echo("USBSTS|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->USBSTS)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->USBSTS);
//	usb_echo("USBINTR|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->USBINTR)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->USBINTR);
//	usb_echo("FRINDEX|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->FRINDEX)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->FRINDEX);
//	usb_echo("CTRLDSSEGMENT|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->CTRLDSSEGMENT)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->CTRLDSSEGMENT);
//	usb_echo("PERIODICLISTBASE|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->PERIODICLISTBASE)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->PERIODICLISTBASE);
//	usb_echo("CTRLDSSEGMENT|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->CTRLDSSEGMENT)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->CTRLDSSEGMENT);
//	usb_echo("ASYNCLISTADDR|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->ASYNCLISTADDR)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->ASYNCLISTADDR);
//	usb_echo("CONFIGFLAG|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->CONFIGFLAG)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->CONFIGFLAG);
//	usb_echo("PORTSC1|%02x:0x%08x\n",(unsigned int)&(((USBHS_Type *)(0x01C1A000))->PORTSC1)-0x01C1A000,((USBHS_Type *)(0x01C1A000))->PORTSC1);
//}
int USB_HostInit(usb_host_handle_t * handle)
{

	int retval=0;
	handle->pUSB_Otg = USB_HOST_OTG_ADDR;
	retval = usb_dev_bsp_init(1);
	if (retval != 0)
	{
		usb_echo("ERR: usb host bsp_init failed\n");
		return -1;
	}
    HAL_USB_InitializeDevice(handle->pUSB_Otg);
    return retval;
}
