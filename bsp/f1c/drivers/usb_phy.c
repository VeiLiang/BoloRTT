#include "usb.h"
#include "usb_phy.h"
#include <rtthread.h>


/* vbus, id, dpdm, these bit is set 1 to clear, so we clear these bit when operate other bits */
static unsigned int USBC_WakeUp_ClearChangeDetect(unsigned int reg_val)
{
	unsigned int temp = reg_val;

	temp &= ~(1 << USBC_BP_ISCR_VBUS_CHANGE_DETECT);
	temp &= ~(1 << USBC_BP_ISCR_ID_CHANGE_DETECT);
	temp &= ~(1 << USBC_BP_ISCR_DPDM_CHANGE_DETECT);

	return temp;
}
void USBC_EnableDpDmPullUp(void)
{
//	usbc_otg_t *usbc_otg = (usbc_otg_t *)hUSB;
	u32 reg_val = 0;

	/* vbus, id, dpdm, these bit is set 1 to clear, so we clear these bit when operate other bits */
	reg_val = USBC_Readl(USBC_REG_ISCR(USBC0_BASE));
	reg_val |=  (1 << USBC_BP_ISCR_DPDM_PULLUP_EN);

	reg_val |= 3<<USBC_BP_ISCR_VBUS_VALID_SRC;
	reg_val = USBC_WakeUp_ClearChangeDetect(reg_val);
	USBC_Writel(reg_val, USBC_REG_ISCR(USBC0_BASE));


	//USBC_REG_set_bit_l(USBC_BP_ISCR_DPDM_PULLUP_EN,USBC_REG_ISCR(USBC0_BASE));//Pull up dp dm
}

void USBC_EnableIdPullUp(void)
{
	u32 reg_val = 0;

	/* vbus, id, dpdm, these bit is set 1 to clear, so we clear these bit when operate other bits */
	reg_val = USBC_Readl(USBC_REG_ISCR(USBC0_BASE));
	reg_val |= (1 << USBC_BP_ISCR_ID_PULLUP_EN);
	reg_val = USBC_WakeUp_ClearChangeDetect(reg_val);
	USBC_Writel(reg_val, USBC_REG_ISCR(USBC0_BASE));
}

void USBC_ForceIdToLow(void )
{
	u32 reg_val = 0;
	/* first write 00, then write 10 */
	reg_val = USBC_Readl(USBC_REG_ISCR(USBC0_BASE));
	reg_val &= ~(0x03 << USBC_BP_ISCR_FORCE_ID);
	reg_val |= (0x02 << USBC_BP_ISCR_FORCE_ID);
	reg_val = USBC_WakeUp_ClearChangeDetect(reg_val);
	USBC_Writel(reg_val, USBC_REG_ISCR(USBC0_BASE));
}

static void USBC_ForceIdToHigh(void )
{
	u32 reg_val = 0;

	/* first write 00, then write 10 */
	reg_val = USBC_Readl(USBC_REG_ISCR(USBC0_BASE));
	//reg_val &= ~(0x03 << USBC_BP_ISCR_FORCE_ID);
	reg_val |= (0x03 << USBC_BP_ISCR_FORCE_ID);
	reg_val = USBC_WakeUp_ClearChangeDetect(reg_val);
	USBC_Writel(reg_val, USBC_REG_ISCR(USBC0_BASE));
}

static void USBC_ForceIdDisable(void)
{
	u32 reg_val = 0;

	/* vbus, id, dpdm, these bit is set 1 to clear, so we clear these bit when operate other bits */
	reg_val = USBC_Readl(USBC_REG_ISCR(USBC0_BASE));
	reg_val &= ~(0x03 << USBC_BP_ISCR_FORCE_ID);
	reg_val = USBC_WakeUp_ClearChangeDetect(reg_val);
	USBC_Writel(reg_val, USBC_REG_ISCR(USBC0_BASE));
}

/* force id to (id_type) */
void USBC_ForceId(u32 id_type)
{

	switch(id_type)
	{
		case USBC_ID_TYPE_HOST:
			USBC_ForceIdToLow();
			break;

		case USBC_ID_TYPE_DEVICE:
			USBC_ForceIdToHigh();
			break;

		default:
			USBC_ForceIdDisable();
	}
}

static void USBC_ForceVbusValidDisable(void )
{
	u32 reg_val = 0;

	/* first write 00, then write 10 */
	reg_val = USBC_Readl(USBC_REG_ISCR(USBC0_BASE));
	reg_val &= ~(0x03 << USBC_BP_ISCR_FORCE_VBUS_VALID);
	reg_val = USBC_WakeUp_ClearChangeDetect(reg_val);
	USBC_Writel(reg_val, USBC_REG_ISCR(USBC0_BASE));
}

static void USBC_ForceVbusValidToLow(void )
{
	u32 reg_val = 0;

	/* first write 00, then write 10 */
	reg_val = USBC_Readl(USBC_REG_ISCR(USBC0_BASE));
	reg_val &= ~(0x03 << USBC_BP_ISCR_FORCE_VBUS_VALID);
	reg_val |= (0x02 << USBC_BP_ISCR_FORCE_VBUS_VALID);
	reg_val = USBC_WakeUp_ClearChangeDetect(reg_val);
	USBC_Writel(reg_val, USBC_REG_ISCR(USBC0_BASE));
}

static void USBC_ForceVbusValidToHigh(void )
{
	u32 reg_val = 0;

	/* first write 00, then write 11 */
	reg_val = USBC_Readl(USBC_REG_ISCR(USBC0_BASE));
	//reg_val &= ~(0x03 << USBC_BP_ISCR_FORCE_VBUS_VALID);
	reg_val |= (0x03 << USBC_BP_ISCR_FORCE_VBUS_VALID);
	reg_val = USBC_WakeUp_ClearChangeDetect(reg_val);
	USBC_Writel(reg_val, USBC_REG_ISCR(USBC0_BASE));
}

/* force vbus valid to (id_type) */
void USBC_ForceVbusValid(u32 vbus_type)
{
	switch(vbus_type)
	{
	case USBC_VBUS_TYPE_LOW:
		USBC_ForceVbusValidToLow();
		break;

	case USBC_VBUS_TYPE_HIGH:
		USBC_ForceVbusValidToHigh();
		break;

	default:
		USBC_ForceVbusValidDisable();
	}
	return ;
}

/*
 * select the bus way for data transfer
 * @hUSB:     handle return by USBC_open_otg, include the key data which USBC need
 * @io_type:  bus type, pio or dma
 * @ep_type:  ep type, rx or tx
 * @ep_index: ep index
 *
 */
void USBC_SelectBus(u32 io_type, u32 ep_type, u32 ep_index)
{
	u32 reg_val = 0;

	reg_val = USBC_Readb(USBC_REG_VEND0(USBC0_BASE));
	if (io_type == USBC_IO_TYPE_DMA) {
		if (ep_type == USBC_EP_TYPE_TX) {
			reg_val |= ((ep_index - 0x01) << 1) << USBC_BP_VEND0_DRQ_SEL;  //drq_sel
			reg_val |= 0x1<<USBC_BP_VEND0_BUS_SEL;   //io_dma
		} else {
			reg_val |= ((ep_index << 1) - 0x01) << USBC_BP_VEND0_DRQ_SEL;
			reg_val |= 0x1<<USBC_BP_VEND0_BUS_SEL;
		}
	} else {
		//reg_val &= ~(0x1 << USBC_BP_VEND0_DRQ_SEL);  //clear drq_sel, select pio
		reg_val &= 0x00;  // clear drq_sel, select pio // V3s no use
	}

	/* in 1667 1673 and later ic, FIFO_BUS_SEL bit(bit24 of reg0x40 for host/device)
	 * is fixed to 1, the hw guarantee that it's ok for cpu/inner_dma/outer_dma transfer */

	//reg_val |= 0x1<<USBC_BP_VEND0_BUS_SEL;  //for 1663 set 1: enable dma, set 0: enable fifo // V3s use this

	USBC_Writeb(reg_val, USBC_REG_VEND0(USBC0_BASE));
}
static void usb_phy_write(int addr, int data, int len)
{
	int j = 0, usbc_bit = 0;
	void *dest = (void *)USBC_REG_CSR(USBC0_BASE);

//#ifdef CONFIG_MACH_SUN8I_A33
//	/* CSR needs to be explicitly initialized to 0 on A33 */
//	writel(0, dest);
//#endif

	usbc_bit = 1 << (0 * 2);
	for (j = 0; j < len; j++)
	{
		/* set the bit address to be written */
		USBC_ClrBit_Mask_l(dest, 0xff << 8);
		USBC_SetBit_Mask_l(dest, (addr + j) << 8);

		USBC_ClrBit_Mask_l(dest, usbc_bit);
		/* set data bit */
		if (data & 0x1)
			USBC_SetBit_Mask_l(dest, 1 << 7);
		else
			USBC_ClrBit_Mask_l(dest, 1 << 7);

		USBC_SetBit_Mask_l(dest, usbc_bit);

		USBC_ClrBit_Mask_l(dest, usbc_bit);

		data >>= 1;
	}
}
void USBC_PhyConfig(void)
{
	/* The following comments are machine
	 * translated from Chinese, you have been warned!
	 */

	/* Regulation 45 ohms */
	//if (phy->id == 0)
		usb_phy_write( 0x0c, 0x01, 1);

	/* adjust PHY's magnitude and rate */
	usb_phy_write(0x20, 0x14, 5);

	/* threshold adjustment disconnect */
#if defined CONFIG_MACH_SUN5I || defined CONFIG_MACH_SUN7I
	usb_phy_write(0x2a, 2, 2);
#else
	usb_phy_write( 0x2a, 3, 2);
#endif

	return;
}
void USBC_ConfigFIFO_Base(void)
{
	u32 reg_value;

	/* config usb fifo, 8kb mode */
	reg_value = USBC_Readl(SUNXI_SRAMC_BASE + 0x04);
	reg_value &= ~(0x03 << 0);
	reg_value |= (1 << 0);
	USBC_Writel(reg_value, SUNXI_SRAMC_BASE + 0x04);
}

u32  usb_phy_open_clock(void)
{
	volatile int i;
	int reg_value = 0;

	usbprint("open_usb_clock\n");

	USBC_REG_set_bit_l(USBPHY_CLK_GAT_BIT, USBPHY_CLK_REG);
   USBC_REG_set_bit_l(USBPHY_CLK_RST_BIT, USBPHY_CLK_REG);

	USBC_REG_set_bit_l(BUS_CLK_USB_BIT,BUS_CLK_GATE0_REG);
	USBC_REG_set_bit_l(BUS_RST_USB_BIT,BUS_CLK_RST_REG);


	USBC_REG_set_bit_l(BUS_CLK_ECHI_BIT,BUS_CLK_GATE0_REG);
		USBC_REG_set_bit_l(BUS_RST_ECHI_BIT,BUS_CLK_RST_REG);
	//USBC_REG_set_bit_l(USBDRAM_ECHI_CLK_BIT,USBDRAM_CLK_REG);



	//reg_value = USBC_Readl(USBC0_BASE+ 0x420);
	//reg_value |= (0x01);
	//USBC_Writel(reg_value, (USBC0_BASE + 0x420));



/*
	reg_value = USBC_Readl(USBC0_BASE+ 0x420);
	reg_value &= ~(0x01);
	USBC_Writel(reg_value, (USBC0_BASE + 0x420));


	reg_value = USBC_Readl(USBC0_BASE+ 0x810);
	reg_value &= ~(0x01<<1);
	USBC_Writel(reg_value, (USBC0_BASE + 0x810));
*/
   // USBC_Writel(0x0043031a,USBC_REG_PMU_IRQ(USBC0_BASE));
	
	//temp_reg_set();
	return 0;
}

u32 close_usb_clock()
{
	usbprint("close_usb_clock\n");


	return 0;
}


/* close a usb misc's interrupt */
void USBC_INT_DisableUsbMiscUint(u32 mask)
{
	u32 reg_val = 0;

	reg_val = USBC_Readb(USBC_REG_INTUSBE(USBC0_BASE));
	reg_val &= ~mask;
	USBC_Writeb(reg_val, USBC_REG_INTUSBE(USBC0_BASE));
}

/* close all usb misc's interrupt */
void USBC_INT_DisableUsbMiscAll(void)
{
	USBC_Writeb(0, USBC_REG_INTUSBE(USBC0_BASE));
}

/* get tx ep's interrupt flag */
static u32 USBC_INT_TxPending(void)
{
	return (USBC_Readw(USBC_REG_INTTx(USBC0_BASE)));
}

/* clear tx ep's interrupt flag */
static void USBC_INT_ClearTxPending( u8 ep_index)
{
	USBC_Writew((1 << ep_index), USBC_REG_INTTx(USBC0_BASE));
}

/* clear all the tx ep's interrupt flag */
static void USBC_INT_ClearTxPendingAll(void)
{
	USBC_Writew(0xffff, USBC_REG_INTTx(USBC0_BASE));
}

/* get rx ep's interrupt flag */
static u32 USBC_INT_RxPending(void)
{
	return (USBC_Readw(USBC_REG_INTRx(USBC0_BASE)));
}

/* clear rx ep's interrupt flag */
static void USBC_INT_ClearRxPending(u8 ep_index)
{
	USBC_Writew((1 << ep_index), USBC_REG_INTRx(USBC0_BASE));
}

/* clear all the rx ep's interrupt flag */
static void USBC_INT_ClearRxPendingAll(void)
{
	USBC_Writew(0xffff, USBC_REG_INTRx(USBC0_BASE));
}

/* open a tx ep's interrupt */
static void USBC_INT_EnableTxEp(u8 ep_index)
{
	USBC_REG_set_bit_w(ep_index, USBC_REG_INTTxE(USBC0_BASE));
}

/* open a rx ep's interrupt */
static void USBC_INT_EnableRxEp(u8 ep_index)
{
	USBC_REG_set_bit_w(ep_index, USBC_REG_INTRxE(USBC0_BASE));
}

/* close a tx ep's interrupt */
static void USBC_INT_DisableTxEp(u8 ep_index)
{
	USBC_REG_clear_bit_w(ep_index, USBC_REG_INTTxE(USBC0_BASE));
}

/* close a rx ep's interrupt */
static void USBC_INT_DisableRxEp(u8 ep_index)
{
	USBC_REG_clear_bit_w(ep_index, USBC_REG_INTRxE(USBC0_BASE));
}

/* close all tx ep's interrupt */
static void USBC_INT_DisableTxAll(void)
{
	USBC_Writew(0, USBC_REG_INTTxE(USBC0_BASE));
}

/* close all rx ep's interrupt */
static void USBC_INT_DisableRxAll(void)
{
	USBC_Writew(0, USBC_REG_INTRxE(USBC0_BASE));
}

/* get ep's interrupt flag */
u32 USBC_INT_EpPending(u32 ep_type)
{
	switch(ep_type) {
	case USBC_EP_TYPE_EP0:
	case USBC_EP_TYPE_TX:
		return USBC_INT_TxPending();

	case USBC_EP_TYPE_RX:
		return USBC_INT_RxPending();

	default:
		return 0;
	}
}

/* clear ep's interrupt flag */
void USBC_INT_ClearEpPending(u32 ep_type, u8 ep_index)
{
	switch(ep_type)
	{
	case USBC_EP_TYPE_EP0:
	case USBC_EP_TYPE_TX:
		USBC_INT_ClearTxPending(ep_index);
		break;

	case USBC_EP_TYPE_RX:
		USBC_INT_ClearRxPending(ep_index);
		break;

	default:
		break;
	}

	return ;
}

/* clear ep's interrupt flag */
void USBC_INT_ClearEpPendingAll(u32 ep_type)
{
	switch(ep_type) {
	case USBC_EP_TYPE_EP0:
	case USBC_EP_TYPE_TX:
		USBC_INT_ClearTxPendingAll();
		break;

	case USBC_EP_TYPE_RX:
		USBC_INT_ClearRxPendingAll();
		break;

	default:
		break;
	}

	return ;
}

void USB_HOST_MODE(void)
{
	//unsigned char reg;
	//reg=USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE));
	//rt_kprintf("USBC_REG_DEVCTL reg=0x%x\n",reg);
	//reg=0x0d;
	//USBC_Writeb(reg, USBC_REG_DEVCTL(USBC0_BASE));
	USBC_Writeb(0x80, USBC_REG_HSEOF(USBC0_BASE));
}
void usb_host_force_to_ls_mode(void)
{
	//unsigned char reg;
	//reg=USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE));
	//rt_kprintf("USBC_REG_DEVCTL reg=0x%x\n",reg);
	//reg=0x0d;
	//USBC_Writeb(reg, USBC_REG_DEVCTL(USBC0_BASE));
	USBC_Writeb(0xD0, USBC_REG_LSEOF(USBC0_BASE));
}
/* get usb misc's interrupt flag */
u32 USBC_INT_MiscPending(void)
{
	return (USBC_Readb(USBC_REG_INTUSB(USBC0_BASE)));
}

/* clear usb misc's interrupt flag */
void USBC_INT_ClearMiscPending(u32 mask)
{
	USBC_Writeb(mask, USBC_REG_INTUSB(USBC0_BASE));
}

/* clear all the usb misc's interrupt flag */
void USBC_INT_ClearMiscPendingAll(void)
{
	USBC_Writeb(0xff, USBC_REG_INTUSB(USBC0_BASE));
}

/* open a ep's interrupt */
void USBC_INT_EnableEp(u32 ep_type, u8 ep_index)
{
	switch(ep_type) {
	case USBC_EP_TYPE_TX:
		USBC_INT_EnableTxEp(ep_index);
		break;

	case USBC_EP_TYPE_RX:
		USBC_INT_EnableRxEp(ep_index);
		break;

	default:
		break;
	}

	return ;
}

/* open a usb misc's interrupt */
void USBC_INT_EnableUsbMiscUint(u32 mask)
{
	u32 reg_val = 0;
	reg_val = USBC_Readb(USBC_REG_INTUSBE(USBC0_BASE));
	reg_val |= mask;
	USBC_Writeb(reg_val, USBC_REG_INTUSBE(USBC0_BASE));
}

/* close a ep's interrupt */
void USBC_INT_DisableEp(u32 ep_type, u8 ep_index)
{
	switch(ep_type) {
	case USBC_EP_TYPE_TX:
		USBC_INT_DisableTxEp(ep_index);
		break;

	case USBC_EP_TYPE_RX:
		USBC_INT_DisableRxEp(ep_index);
		break;

	default:
		break;
	}

	return;
}


/* get current active ep */
u32 USBC_GetActiveEp(void)
{
	return USBC_Readb(USBC_REG_EPIND(USBC0_BASE));
}

/* set the active ep */
void USBC_SelectActiveEp(u8 ep_index)
{
	USBC_Writeb(ep_index, USBC_REG_EPIND(USBC0_BASE));
}

/*
 * the switch to communicate with PC
 * @hUSB:     handle return by USBC_open_otg, include the key data which USBC need
 * @is_on:    1 - open the switch, 0 - close the switch
 *
 */
void USBC_Dev_ConectSwitch(u32 is_on)
{
	if (is_on == USBC_DEVICE_SWITCH_ON)
	{
		USBC_REG_set_bit_b(USBC_BP_POWER_D_SOFT_CONNECT, USBC_REG_PCTL(USBC0_BASE));
	}
	else
	{
		USBC_REG_clear_bit_b(USBC_BP_POWER_D_SOFT_CONNECT, USBC_REG_PCTL(USBC0_BASE));
	}
}

/*
 * select the usb transfer type, eg control/iso/interrupt/bulk transfer
 */

static void __USBC_Dev_TsType_default(void)
{
	//disable all transfer type
	USBC_REG_clear_bit_b(USBC_BP_POWER_D_ISO_UPDATE_EN, USBC_REG_PCTL(USBC0_BASE));
}

static void __USBC_Dev_TsType_Ctrl(void)
{
	//--<1>--disable other transfer type
	USBC_REG_clear_bit_b(USBC_BP_POWER_D_ISO_UPDATE_EN, USBC_REG_PCTL(USBC0_BASE));

	//--<2>--select Ctrl type
	/* donot need config */
}

static void __USBC_Dev_TsType_Iso(void)
{
	//--<1>--disable other transfer type
	/* donot need config */

	//--<2>--select Ctrl type
	USBC_REG_set_bit_b(USBC_BP_POWER_D_ISO_UPDATE_EN, USBC_REG_PCTL(USBC0_BASE));
}

static void __USBC_Dev_TsType_Int(void)
{
	//--<1>--disable other transfer type
	USBC_REG_clear_bit_b(USBC_BP_POWER_D_ISO_UPDATE_EN, USBC_REG_PCTL(USBC0_BASE));

	//--<2>--select Ctrl type
	/* donot need config */
}

static void __USBC_Dev_TsType_Bulk(void)
{
	//--<1>--disable other transfer type
	USBC_REG_clear_bit_b(USBC_BP_POWER_D_ISO_UPDATE_EN, USBC_REG_PCTL(USBC0_BASE));

	//--<2>--select Ctrl type
	/* donot need config */
}

/*
 * select the usb speed type, eg high/full/low
 */

static void __USBC_Dev_TsMode_default(void)
{
	USBC_REG_clear_bit_b(USBC_BP_POWER_D_HIGH_SPEED_EN, USBC_REG_PCTL(USBC0_BASE));
}

static void __USBC_Dev_TsMode_Hs(void)
{
	USBC_REG_set_bit_b(USBC_BP_POWER_D_HIGH_SPEED_EN, USBC_REG_PCTL(USBC0_BASE));
}

static void __USBC_Dev_TsMode_Fs(void)
{
	USBC_REG_clear_bit_b(USBC_BP_POWER_D_HIGH_SPEED_EN, USBC_REG_PCTL(USBC0_BASE));
}

static void __USBC_Dev_TsMode_Ls(void)
{
	// hw not support ls, so default select fs
	__USBC_Dev_TsMode_Fs();
}

/*
 * config the device's transfer type and speed mode
 * @hUSB:       handle return by USBC_open_otg, include the key data which USBC need
 * @ts_type:    transfer type
 * @speed_mode: speed mode
 */
void USBC_Dev_ConfigTransferMode(u8 ts_type, u8 speed_mode)
{
	//--<1>--select the transfer type
	//default bulk transfer
	switch(ts_type)
	{
			case USBC_TS_TYPE_CTRL:
				__USBC_Dev_TsType_Ctrl();
				break;

			case USBC_TS_TYPE_ISO:
				__USBC_Dev_TsType_Iso();
				break;

			case USBC_TS_TYPE_INT:
				__USBC_Dev_TsType_Int();
				break;

			case USBC_TS_TYPE_BULK:
				__USBC_Dev_TsType_Bulk();
				break;

			default:
				__USBC_Dev_TsType_default();
	}

	//--<2>--select the transfer speed
	switch(speed_mode)
	{
			case USBC_TS_MODE_HS:
				__USBC_Dev_TsMode_Hs();
				break;

			case USBC_TS_MODE_FS:
				__USBC_Dev_TsMode_Fs();
				break;

			case USBC_TS_MODE_LS:
				__USBC_Dev_TsMode_Ls();
				break;

			default:
				__USBC_Dev_TsMode_default();
	}
}
u32 USBC_Dev_QueryTransferMode(void)
{

	if (USBC_REG_test_bit_b(USBC_BP_POWER_D_HIGH_SPEED_FLAG, USBC_REG_PCTL(USBC0_BASE)))
	{
		return USBC_TS_MODE_HS;
	}
	else
	{
		return USBC_TS_MODE_FS;
	}
}
/* close all ep's interrupt */
void USBC_INT_DisableEpAll(u32 ep_type)
{
	switch(ep_type)
	{
		case USBC_EP_TYPE_TX:
			USBC_INT_DisableTxAll();
			break;

		case USBC_EP_TYPE_RX:
			USBC_INT_DisableRxAll();
			break;

		default:
			break;
	}
	return;
}

/*
 * clear the address allocated by host for device
 * @hUSB: handle return by USBC_open_otg, include the key data which USBC need
 */
void USBC_Dev_SetAddress_default(void)
{
	USBC_Writeb(0x00, USBC_REG_FADDR(USBC0_BASE));
}

/*
 * set the address
 * @hUSB: handle return by USBC_open_otg, include the key data which USBC need
 */
void USBC_Dev_SetAddress(u8 address)
{
	USBC_Writeb(address, USBC_REG_FADDR(USBC0_BASE));
}

static u32 __USBC_Dev_ep0_IsEpStall(void)
{
	return USBC_REG_test_bit_w(USBC_BP_CSR0_D_SENT_STALL, USBC_REG_CSR0(USBC0_BASE));
}

static void __USBC_Dev_ep0_SendStall(void)
{
	USBC_REG_set_bit_w(USBC_BP_CSR0_D_SEND_STALL, USBC_REG_CSR0(USBC0_BASE));
}

static void __USBC_Dev_ep0_ClearStall(void)
{
	USBC_REG_clear_bit_w(USBC_BP_CSR0_D_SEND_STALL, USBC_REG_CSR0(USBC0_BASE));
	USBC_REG_clear_bit_w(USBC_BP_CSR0_D_SENT_STALL, USBC_REG_CSR0(USBC0_BASE));
}

static void __USBC_Dev_Tx_SendStall(void)
{
	//send stall, and fifo is flushed automaticly
	USBC_REG_set_bit_w(USBC_BP_TXCSR_D_SEND_STALL, USBC_REG_TXCSR(USBC0_BASE));
}

static u32 __USBC_Dev_Tx_IsEpStall(void)
{
	return USBC_REG_test_bit_w(USBC_BP_TXCSR_D_SENT_STALL, USBC_REG_TXCSR(USBC0_BASE));
}


static void __USBC_Dev_Tx_ClearStall(void)
{
	u32 reg_val;

	reg_val = USBC_Readw(USBC_REG_TXCSR(USBC0_BASE));
	reg_val &= ~((1 << USBC_BP_TXCSR_D_SENT_STALL)|(1 << USBC_BP_TXCSR_D_SEND_STALL));
	USBC_Writew(reg_val, USBC_REG_TXCSR(USBC0_BASE));
}

static void __USBC_Dev_Rx_SendStall(void)
{
	USBC_REG_set_bit_w(USBC_BP_RXCSR_D_SEND_STALL, USBC_REG_RXCSR(USBC0_BASE));
}

static u32 __USBC_Dev_Rx_IsEpStall(void)
{
	return USBC_REG_test_bit_w(USBC_BP_RXCSR_D_SENT_STALL, USBC_REG_RXCSR(USBC0_BASE));
}

static void __USBC_Dev_Rx_ClearStall(void)
{
	USBC_REG_clear_bit_w(USBC_BP_RXCSR_D_SEND_STALL, USBC_REG_RXCSR(USBC0_BASE));
	USBC_REG_clear_bit_w(USBC_BP_RXCSR_D_SENT_STALL, USBC_REG_RXCSR(USBC0_BASE));
}

/*
 * check if ep is stalled
 * @hUSB:       handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type:    ep type
 *
 * return: 0 - success, !0 - failed
 */
int USBC_Dev_IsEpStall(u32 ep_type)
{
	switch(ep_type) {
	case USBC_EP_TYPE_EP0:
		__USBC_Dev_ep0_IsEpStall();
		break;

	case USBC_EP_TYPE_TX:
		__USBC_Dev_Tx_IsEpStall();
		break;

	case USBC_EP_TYPE_RX:
		__USBC_Dev_Rx_IsEpStall();
		break;

	default:
		return -1;
	}

	return 0;
}

/*
 * let ep enter stall status
 * @hUSB:       handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type:    ep type
 *
 * return: 0 - success, !0 - failed
 */
int USBC_Dev_EpSendStall(u32 ep_type)
{
	switch(ep_type)
	{
		case USBC_EP_TYPE_EP0:
			__USBC_Dev_ep0_SendStall();
			break;

		case USBC_EP_TYPE_TX:
			__USBC_Dev_Tx_SendStall();
			break;

		case USBC_EP_TYPE_RX:
			__USBC_Dev_Rx_SendStall();
			break;

		default:
			return -1;
	}

	return 0;
}
/*
 * clear the ep's stall status
 * @hUSB:       handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type:    ep type
 *
 * return: 0 - success, !0 - failed
 */
int USBC_Dev_EpClearStall(u32 ep_type)
{
	switch(ep_type) {
	case USBC_EP_TYPE_EP0:
		__USBC_Dev_ep0_ClearStall();
		break;

	case USBC_EP_TYPE_TX:
		__USBC_Dev_Tx_ClearStall();
		break;

	case USBC_EP_TYPE_RX:
		__USBC_Dev_Rx_ClearStall();
		break;

	default:
		return -1;
	}

	return 0;
}
static int __USBC_Dev_ep0_IsSetupEnd(void)
{
	return USBC_REG_test_bit_w(USBC_BP_CSR0_D_SETUP_END, USBC_REG_CSR0(USBC0_BASE));
}
/*
 * check if ep0 is SetupEnd
 * @hUSB:       handle return by USBC_open_otg, include the key data which USBC need
 *
 */
int USBC_Dev_Ctrl_IsSetupEnd(void)
{
	return __USBC_Dev_ep0_IsSetupEnd();
}

/*
 * clear the ep0's SetupEnd status
 * @hUSB:       handle return by USBC_open_otg, include the key data which USBC need
 *
 */
void USBC_Dev_Ctrl_ClearSetupEnd(void)
{
	USBC_REG_set_bit_w(USBC_BP_CSR0_D_SERVICED_SETUP_END, USBC_REG_CSR0(USBC0_BASE));
}

int USBC_Dev_Read_EP0_CSR(void)
{
	return USBC_Readw(USBC_REG_CSR0(USBC0_BASE));
}


void * USBC_SelectFIFO(u32 ep_index)
{
	return (void*)(USBC_REG_EPFIFOx(USBC0_BASE, ep_index));
}

static void __USBC_ConfigFifo_TxEp_Default(void)
{
	USBC_Writew(0x00, USBC_REG_TXFIFOAD(USBC0_BASE));
	USBC_Writeb(0x00, USBC_REG_TXFIFOSZ(USBC0_BASE));
}

/*
 * set tx ep's fifo address and size
 * @hUSB:           handle return by USBC_open_otg, include the key data which USBC need
 * @is_double_fifo: if use hardware double fifo
 * @fifo_size:      fifo size = 2 ^ fifo_size
 * @fifo_addr:      fifo start addr = fifo_addr * 8
 *
 */
static void __USBC_ConfigFifo_TxEp(u32 is_double_fifo, u32 fifo_size, u32 fifo_addr)
{
	u32 temp = 0;
	u32 size = 0;   //fifo_size = 2^ (size + 3)
	u32 addr = 0;   //fifo_addr = addr * 8

	//--<1>-- 512 align
	temp = fifo_size + 511;
	temp &= ~511;
	temp >>= 3;
	temp >>= 1;
	while(temp)
	{
		size++;
		temp >>= 1;
	}

	//--<2>-- calculate addr
	addr = fifo_addr >> 3;

	//--<3>--config fifo addr
	USBC_Writew(addr, USBC_REG_TXFIFOAD(USBC0_BASE));

	//--<4>--config fifo size
	USBC_Writeb((size & 0x0f), USBC_REG_TXFIFOSZ(USBC0_BASE));
	if (is_double_fifo) {
		USBC_REG_set_bit_b(USBC_BP_TXFIFOSZ_DPB, USBC_REG_TXFIFOSZ(USBC0_BASE));
	}
}

static void __USBC_ConfigFifo_RxEp_Default(void)
{
	USBC_Writew(0x00, USBC_REG_RXFIFOAD(USBC0_BASE));
	USBC_Writeb(0x00, USBC_REG_RXFIFOSZ(USBC0_BASE));
}

/*
 * set rx ep's fifo address and size
 * @hUSB:           handle return by USBC_open_otg, include the key data which USBC need
 * @is_double_fifo: if use hardware double fifo
 * @fifo_size:      fifo size = 2 ^ fifo_size
 * @fifo_addr:      fifo start addr = fifo_addr * 8
 *
 */
static void __USBC_ConfigFifo_RxEp(u32 is_double_fifo, u32 fifo_size, u32 fifo_addr)
{
	u32 temp = 0;
	u32 size = 0;   //fifo_size = 2 ^ (size + 3)
	u32 addr = 0;   //fifo_addr = addr * 8

	//--<1>-- 512 align
	temp = fifo_size + 511;
	temp &= ~511;
	temp >>= 3;
	temp >>= 1;
	while(temp) {
		size++;
		temp >>= 1;
	}

	//--<2>--calculate addr
	addr = fifo_addr >> 3;

	//--<3>--config fifo addr
	USBC_Writew(addr, USBC_REG_RXFIFOAD(USBC0_BASE));

	//--<2>--config fifo size
	USBC_Writeb((size & 0x0f), USBC_REG_RXFIFOSZ(USBC0_BASE));
	if (is_double_fifo)
	{
		USBC_REG_set_bit_b(USBC_BP_RXFIFOSZ_DPB, USBC_REG_RXFIFOSZ(USBC0_BASE));
	}
}

/*
 * config ep's fifo addr and size
 * @hUSB:     handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type:  ep type
 *
 */
void USBC_ConfigFifo_Default(u32 ep_type)
{
	switch(ep_type)
	{
		case USBC_EP_TYPE_EP0:
			//not support
			break;
		case USBC_EP_TYPE_TX:
			__USBC_ConfigFifo_TxEp_Default();
			break;
		case USBC_EP_TYPE_RX:
			__USBC_ConfigFifo_RxEp_Default();
			break;
		default:
			break;
	}
}

/*
 * config ep's fifo addr and size
 * @hUSB:           handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type:        ep type
 * @is_double_fifo: if use hardware double fifo
 * @fifo_size:      fifo size = 2 ^ fifo_size
 * @fifo_addr:      fifo start addr = fifo_addr * 8
 *
 */
void USBC_ConfigFifo(u32 ep_type, u32 is_double_fifo, u32 fifo_size, u32 fifo_addr)
{
	switch(ep_type)
	{
		case USBC_EP_TYPE_EP0:
			//not support
			break;
		case USBC_EP_TYPE_TX:
			__USBC_ConfigFifo_TxEp( is_double_fifo, fifo_size, fifo_addr);
			break;
		case USBC_EP_TYPE_RX:
			__USBC_ConfigFifo_RxEp( is_double_fifo, fifo_size, fifo_addr);
			break;
		default:
			break;
	}
}

static void __USBC_Host_ep0_ConfigEp(u32 interval)
{
	u16 csr0 = 0;
	u32 reg_val = 0;
	//--<1>--config ep0 csr
	USBC_Writew(1<<USBC_BP_CSR0_H_FlushFIFO, USBC_REG_CSR0(USBC0_BASE));
	//__USBC_Host_ep0_DisablePing(USBC0_BASE);
	
	csr0 = USBC_Readw(USBC_REG_CSR0(USBC0_BASE));
	csr0 |= (1 << USBC_BP_CSR0_H_DisPing);
	USBC_Writew(csr0, USBC_REG_CSR0(USBC0_BASE));

	//--<2>--config polling interval	NAK超时间隔
	USBC_Writeb(interval, USBC_REG_NAKLIMIT0(USBC0_BASE));

	/* config ep0 transfer type */		//高速模式
	//__USBC_Host_ep0_EpType(USBC0_BASE, ts_mode);
	reg_val |= 0x01 << USBC_BP_RXTYPE_SPEED;
	USBC_Writeb(reg_val, USBC_REG_EP0TYPE(USBC0_BASE));
}


static void __USBC_Host_Tx_ConfigEp(u32 ep_index,u32 ep_MaxPkt,u32 interval)
{
	u32 reg_val = 0;
	u16 temp = 0;

	//--<1>--config tx_csr
	USBC_Writew((1 << USBC_BP_TXCSR_H_MODE) | (1 << USBC_BP_TXCSR_H_CLEAR_DATA_TOGGLE)
		| (1 << USBC_BP_TXCSR_H_FLUSH_FIFO),
	USBC_REG_TXCSR(USBC0_BASE));
/*
	if (is_double_fifo) 	双fifo
	{
		USBC_Writew((1 << USBC_BP_TXCSR_H_MODE) | (1 << USBC_BP_TXCSR_H_CLEAR_DATA_TOGGLE)
			| (1 << USBC_BP_TXCSR_H_FLUSH_FIFO),
		USBC_REG_TXCSR(USBC0_BASE));
	}
*/
	//--<2>--config tx ep max packet
	reg_val = USBC_Readw(USBC_REG_TXMAXP(USBC0_BASE));
	temp    = ep_MaxPkt & ((1 << USBC_BP_TXMAXP_PACKET_COUNT) - 1);
	reg_val |= temp;
	USBC_Writew(reg_val, USBC_REG_TXMAXP(USBC0_BASE));

	//--<3>--config ep transfer type
	//__USBC_Host_Tx_EpType(usbc_base_addr, ep_index, ts_mode, ts_type);
	reg_val=0;
	reg_val |= 0x01 << USBC_BP_TXTYPE_SPEED;		//高速传输
	reg_val |= 0x3 << USBC_BP_TXTYPE_PROROCOL;		//中断收发
	
	reg_val |= ep_index;							//端口
	USBC_Writeb(reg_val, USBC_REG_TXTYPE(USBC0_BASE)); 

	//--<4>--config polling interval
	USBC_Writeb(interval, USBC_REG_TXINTERVAL(USBC0_BASE));		//间隔时间
}


static void __USBC_Host_Rx_ConfigEp(u32 ep_index,u32 ep_MaxPkt,u32 interval)
{
	u32 reg_val = 0;
	u16 temp = 0;

	//--<1>--config rx_csr
	USBC_Writew((1 << USBC_BP_RXCSR_H_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_RXCSR_H_FLUSH_FIFO),
	USBC_REG_RXCSR(USBC0_BASE));
/*
	if (is_double_fifo) 
	{
		USBC_Writew((1 << USBC_BP_RXCSR_H_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_RXCSR_H_FLUSH_FIFO),
		USBC_REG_RXCSR(USBC0_BASE));
	}
*/
	//--<2>--config tx ep max packet
	reg_val = USBC_Readw(USBC_REG_RXMAXP(USBC0_BASE));
	temp    = ep_MaxPkt & ((1 << USBC_BP_RXMAXP_PACKET_COUNT) - 1);
	reg_val |= temp;
	USBC_Writew(reg_val, USBC_REG_RXMAXP(USBC0_BASE));

	//--<3>--config ep transfer type
	//__USBC_Host_Rx_EpType(usbc_base_addr, ep_index, ts_mode, ts_type);
	reg_val=0;
	reg_val |= 0x01 << USBC_BP_RXTYPE_SPEED;
	reg_val |= 0x3 << USBC_BP_RXTYPE_PROROCOL;

	reg_val |= ep_index;	
	USBC_Writeb(reg_val, USBC_REG_RXTYPE(USBC0_BASE));


	//--<4>--config polling interval
	USBC_Writeb(interval, USBC_REG_RXINTERVAL(USBC0_BASE));
}

void USBC_Host_ConfigEp(u32 ep_type, u32 ep_index,u32 ep_MaxPkt, u32 interval)
{
	switch(ep_type) {
	case USBC_EP_TYPE_EP0:
		__USBC_Host_ep0_ConfigEp(interval);
		break;

	case USBC_EP_TYPE_TX:
		__USBC_Host_Tx_ConfigEp(ep_index, ep_MaxPkt, interval);
		break;

	case USBC_EP_TYPE_RX:
		__USBC_Host_Rx_ConfigEp(ep_index,ep_MaxPkt, interval);
		break;

	default:
		break;
	}
}

void USBC_Host_FlushFifo(void)
{
	u16 csr0 = 0;
	//1.ep0
	USBC_Writew(1 << USBC_BP_CSR0_H_FlushFIFO, USBC_REG_CSR0(USBC0_BASE));
	csr0 = USBC_Readw(USBC_REG_CSR0(USBC0_BASE));
	csr0 |= (1 << USBC_BP_CSR0_H_DisPing);
	USBC_Writew(csr0, USBC_REG_CSR0(USBC0_BASE));

	//2.Tx
	USBC_Writew((1 << USBC_BP_TXCSR_H_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_TXCSR_H_FLUSH_FIFO),
	USBC_REG_TXCSR(USBC0_BASE));	

	//3.Rx
	USBC_Writew((1 << USBC_BP_RXCSR_H_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_RXCSR_H_FLUSH_FIFO),
	USBC_REG_RXCSR(USBC0_BASE));
}

void sunxi_hcd_generic_disable(void)
{
	/* disable interrupts */
	USBC_Writeb(0x00, USBC_REG_INTUSBE(USBC0_BASE));
	USBC_Writew(0x00, USBC_REG_INTTxE(USBC0_BASE));
	USBC_Writew(0x00, USBC_REG_INTRxE(USBC0_BASE));

	/* off */
	USBC_Writew(0x00, USBC_REG_DEVCTL(USBC0_BASE));

	/*  flush pending interrupts */
	USBC_Writeb(0xff, USBC_REG_INTUSB(USBC0_BASE));
	USBC_Writew(0x3f, USBC_REG_INTTx(USBC0_BASE));
	USBC_Writew(0x3f, USBC_REG_INTRx(USBC0_BASE));
}



static void __USBC_Dev_ep0_WriteDataHalf(void)
{
	USBC_Writew(1<<USBC_BP_CSR0_D_TX_PKT_READY, USBC_REG_CSR0(USBC0_BASE));
}

static void __USBC_Dev_ep0_WriteDataComplete(void)
{
	USBC_Writew((1<<USBC_BP_CSR0_D_TX_PKT_READY) | (1<<USBC_BP_CSR0_D_DATA_END),
	USBC_REG_CSR0(USBC0_BASE));
}

static void __USBC_Dev_Tx_WriteDataHalf(void)
{
	u16 ep_csr = 0;

	ep_csr = USBC_Readw(USBC_REG_TXCSR(USBC0_BASE));
	ep_csr |= 1 << USBC_BP_TXCSR_D_TX_READY;
	ep_csr &= ~(1 << USBC_BP_TXCSR_D_UNDER_RUN);
	USBC_Writew(ep_csr, USBC_REG_TXCSR(USBC0_BASE));
}

static void __USBC_Dev_Tx_WriteDataComplete(void)
{
	u16 ep_csr = 0;

	ep_csr = USBC_Readw(USBC_REG_TXCSR(USBC0_BASE));
	ep_csr |= 1 << USBC_BP_TXCSR_D_TX_READY;
	ep_csr &= ~(1 << USBC_BP_TXCSR_D_UNDER_RUN);
	USBC_Writew(ep_csr, USBC_REG_TXCSR(USBC0_BASE));
}

static int __USBC_Dev_WriteDataHalf(u32 ep_type)
{
	switch(ep_type)
	{
		case USBC_EP_TYPE_EP0:
			__USBC_Dev_ep0_WriteDataHalf();
			break;

		case USBC_EP_TYPE_TX:
			__USBC_Dev_Tx_WriteDataHalf();
			break;

		case USBC_EP_TYPE_RX:
			//not support
			return -1;
			//break;

		default:
			return -1;
	}

	return 0;
}

static int __USBC_Dev_WriteDataComplete(u32 ep_type)
{
	switch(ep_type)
	{
	case USBC_EP_TYPE_EP0:
		__USBC_Dev_ep0_WriteDataComplete();
		break;

	case USBC_EP_TYPE_TX:
		__USBC_Dev_Tx_WriteDataComplete();
		break;

	case USBC_EP_TYPE_RX:
		//not support
		return -1;
		//break;

	default:
		return -1;
	}

	return 0;
}

static void __USBC_Dev_ep0_ReadDataHalf(void)
{
	USBC_Writew(1<<USBC_BP_CSR0_D_SERVICED_RX_PKT_READY, USBC_REG_CSR0(USBC0_BASE));
}

static void __USBC_Dev_ep0_ReadDataComplete(void)
{
	USBC_Writew((1<<USBC_BP_CSR0_D_SERVICED_RX_PKT_READY) | (1<<USBC_BP_CSR0_D_DATA_END),
	USBC_REG_CSR0(USBC0_BASE));
}

static void __USBC_Dev_Rx_ReadDataHalf(void)
{
	u32 reg_val = 0;

	//overrun, dataerr is used in iso transfer
	reg_val = USBC_Readw(USBC_REG_RXCSR(USBC0_BASE));
	reg_val &= ~(1 << USBC_BP_RXCSR_D_RX_PKT_READY);
	reg_val &= ~(1 << USBC_BP_RXCSR_D_OVERRUN);
	reg_val &= ~(1 << USBC_BP_RXCSR_D_DATA_ERROR);
	USBC_Writew(reg_val, USBC_REG_RXCSR(USBC0_BASE));
}

static void __USBC_Dev_Rx_ReadDataComplete(void)
{
	u32 reg_val = 0;

	//overrun, dataerr is used in iso transfer
	reg_val = USBC_Readw(USBC_REG_RXCSR(USBC0_BASE));
	reg_val &= ~(1 << USBC_BP_RXCSR_D_RX_PKT_READY);
	reg_val &= ~(1 << USBC_BP_RXCSR_D_OVERRUN);
	reg_val &= ~(1 << USBC_BP_RXCSR_D_DATA_ERROR);
	USBC_Writew(reg_val, USBC_REG_RXCSR(USBC0_BASE));
}


static int __USBC_Dev_ReadDataHalf(u32 ep_type)
{
	switch(ep_type)
	{
		case USBC_EP_TYPE_EP0:
			__USBC_Dev_ep0_ReadDataHalf();
			break;

		case USBC_EP_TYPE_TX:
			//not support
			return -1;
			//break;

		case USBC_EP_TYPE_RX:
			__USBC_Dev_Rx_ReadDataHalf();
			break;

		default:
			return -1;
	}

	return 0;
}

static int __USBC_Dev_ReadDataComplete(u32 ep_type)
{
	switch(ep_type)
	{
		case USBC_EP_TYPE_EP0:
			__USBC_Dev_ep0_ReadDataComplete();
			break;

		case USBC_EP_TYPE_TX:
			//not support
			return -1;
			//break;

		case USBC_EP_TYPE_RX:
			__USBC_Dev_Rx_ReadDataComplete();
			break;

		default:
			return -1;
	}

	return 0;
}

/*
 * get the write status, eg write over or not
 * @hUSB:       handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type:    ep type
 * @complete:   if all data has been written over.
 *
 * return: 0 - success, !0 - failed
 */
int USBC_Dev_WriteDataStatus(u32 ep_type, u32 complete)
{
	if (complete)
	{
		return __USBC_Dev_WriteDataComplete(ep_type);
	}
	else
	{
		return __USBC_Dev_WriteDataHalf( ep_type);
	}
}

/*
 * get the read status, eg write over or not
 * @hUSB:       handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type:    ep type
 * @complete:   if all data has been read over.
 *
 * return: 0 - success, !0 - failed
 */
int USBC_Dev_ReadDataStatus(u32 ep_type, u32 complete)
{
	if (complete) {
		return __USBC_Dev_ReadDataComplete(ep_type);
	} else {
		return __USBC_Dev_ReadDataHalf(ep_type);
	}
}
static u32 __USBC_Dev_Tx_IsWriteDataReady(void)
{
	u32 temp = 0;

	temp = USBC_Readw(USBC_REG_TXCSR(USBC0_BASE));
	temp &= (1 << USBC_BP_TXCSR_D_TX_READY);

	return temp;
}

static u32 __USBC_Dev_Tx_IsWriteDataReady_FifoEmpty(void)
{
	u32 temp = 0;

	temp = USBC_Readw(USBC_REG_TXCSR(USBC0_BASE));
	temp &= (1 << USBC_BP_TXCSR_D_TX_READY) | (1 << USBC_BP_TXCSR_D_FIFO_NOT_EMPTY);

	return temp;
}

static u32 __USBC_Dev_Rx_IsReadDataReady(void)
{
	return USBC_REG_test_bit_w(USBC_BP_RXCSR_D_RX_PKT_READY, USBC_REG_RXCSR(USBC0_BASE));
}


static u32 __USBC_Dev_ep0_IsReadDataReady(void)
{
	return USBC_REG_test_bit_w(USBC_BP_CSR0_D_RX_PKT_READY, USBC_REG_CSR0(USBC0_BASE));
}

static u32 __USBC_Dev_ep0_IsWriteDataReady(void)
{
	return USBC_REG_test_bit_w(USBC_BP_CSR0_D_TX_PKT_READY, USBC_REG_CSR0(USBC0_BASE));
}
/*
 * check if the data ready for reading
 * @hUSB:       handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type:    ep type
 *
 */
u32 USBC_Dev_IsReadDataReady(u32 ep_type)
{
	switch(ep_type) {
	case USBC_EP_TYPE_EP0:
		return __USBC_Dev_ep0_IsReadDataReady();

	case USBC_EP_TYPE_TX:
		//not support
		break;

	case USBC_EP_TYPE_RX:
		return __USBC_Dev_Rx_IsReadDataReady();

	default:
		break;
	}

	return 0;
}

/*
 * check if the data ready for writting
 * @hUSB:       handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type:    ep type
 *
 */
u32 USBC_Dev_IsWriteDataReady(u32 ep_type)
{
	switch(ep_type) {
	case USBC_EP_TYPE_EP0:
		return __USBC_Dev_ep0_IsWriteDataReady();

	case USBC_EP_TYPE_TX:
		return __USBC_Dev_Tx_IsWriteDataReady();

	case USBC_EP_TYPE_RX:
		//not support
		break;

	default:
		break;
	}

	return 0;
}

u32 USBC_Dev_IsWriteDataReady_FifoEmpty(u32 ep_type)
{
	switch(ep_type)
	{
		case USBC_EP_TYPE_EP0:
			return __USBC_Dev_ep0_IsWriteDataReady();

		case USBC_EP_TYPE_TX:
			return __USBC_Dev_Tx_IsWriteDataReady_FifoEmpty();

		case USBC_EP_TYPE_RX:
			//not support
			break;

		default:
			break;
	}

	return 0;
}
/*
 * get the length of data that can be read from current FIFO
 * @hUSB:     handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type:  ep type, tx or rx
 *
 * return the data length that can be current read
 */
u32 USBC_ReadLenFromFifo(u32 ep_type)
{

	switch(ep_type)
	{
		case USBC_EP_TYPE_EP0:
			return USBC_Readw(USBC_REG_COUNT0(USBC0_BASE));
			//break;
		case USBC_EP_TYPE_TX:
			return 0;
			//break;
		case USBC_EP_TYPE_RX:
			return USBC_Readw(USBC_REG_RXCOUNT(USBC0_BASE));
			//break;
		default:
			return 0;
	}
}
/*
 * write data packet to fifo
 * @hUSB: handle return by USBC_open_otg, include the key data which USBC need
 * @fifo: fifo address
 * @cnt:  data length
 * @buff: store the data to be written
 *
 * return the lenght that successfully written
 */
u32 USBC_WritePacket(void *fifo, u32 cnt, void *buff)
{
	u32 len = 0;
	u32 i32 = 0;
	u32 i8  = 0;
	u8  *buf8  = 0;
	u32 *buf32 = 0;

	//--<1>-- adjust data
	buf32 = buff;
	len   = cnt;

	i32 = len >> 2;
	i8  = len & 0x03;

	//--<2>-- deal with 4byte part
	while (i32--)
	{

		USBC_Writel(*buf32++, fifo);
	}

	//--<3>-- deal with no 4byte part
	buf8 = (u8 *)buf32;
	while (i8--) {
		USBC_Writeb(*buf8++, fifo);
	}

	return len;
}

/*
 * read data from fifo
 * @hUSB: handle return by USBC_open_otg, include the key data which USBC need
 * @fifo: fifo address
 * @cnt:  data length
 * @buff: store the data that will be read
 *
 * return the lenght that successfully read
 */
u32 USBC_ReadPacket(void  *fifo, u32 cnt, void *buff)
{
	u32 len = 0;
	u32 i32 = 0;
	u32 i8  = 0;
	u8  *buf8  = 0;
	u32 *buf32 = 0;

	//--<1>-- adjust data
	buf32 = buff;
	len   = cnt;

	i32 = len >> 2;
	i8  = len & 0x03;

	//--<2>-- deal with 4byte part
	while (i32--)
	{
		*buf32++ = USBC_Readl(fifo);
	}

	//--<3>-- deal with no 4byte part
	buf8 = (u8 *)buf32;
	while (i8--)
	{
		*buf8++ = USBC_Readb(fifo);
	}

	return len;
}

static void __USBC_Dev_ep0_FlushFifo(void)
{
	USBC_Writew(1 << USBC_BP_CSR0_D_FLUSH_FIFO, USBC_REG_CSR0(USBC0_BASE));
}

static void __USBC_Dev_Tx_FlushFifo(void)
{
	USBC_Writew((1 << USBC_BP_TXCSR_D_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_TXCSR_D_FLUSH_FIFO),
	USBC_REG_TXCSR(USBC0_BASE));
}

static void __USBC_Dev_Rx_FlushFifo(void)
{
	USBC_Writew((1 << USBC_BP_RXCSR_D_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_RXCSR_D_FLUSH_FIFO),
	USBC_REG_RXCSR(USBC0_BASE));
}

void USBC_Dev_FlushFifo(u32 ep_type)
{
	switch(ep_type)
	{
		case USBC_EP_TYPE_EP0:
			__USBC_Dev_ep0_FlushFifo();
		break;

		case USBC_EP_TYPE_TX:
			__USBC_Dev_Tx_FlushFifo();
		break;

		case USBC_EP_TYPE_RX:
			__USBC_Dev_Rx_FlushFifo();
		break;

		default:
		break;
	}
}

static void __USBC_Dev_Tx_EnableIsoEp(void)
{
	USBC_REG_set_bit_w(USBC_BP_TXCSR_D_ISO, USBC_REG_TXCSR(USBC0_BASE));
}

static void __USBC_Dev_Tx_EnableIntEp(void)
{
	USBC_REG_clear_bit_w(USBC_BP_TXCSR_D_ISO, USBC_REG_TXCSR(USBC0_BASE));
}

static void __USBC_Dev_Tx_EnableBulkEp(void)
{
	USBC_REG_clear_bit_w(USBC_BP_TXCSR_D_ISO, USBC_REG_TXCSR(USBC0_BASE));
}

static void __USBC_Dev_Tx_ConfigEp_Default(void)
{
	//--<1>--clear tx csr
	USBC_Writew(0x00, USBC_REG_TXCSR(USBC0_BASE));

	//--<2>--clear tx ep max packet
	USBC_Writew(0x00, USBC_REG_TXMAXP(USBC0_BASE));

	//--<3>--config ep transfer type
}

static void __USBC_Dev_Tx_ConfigEp(u32 ts_type, u32 is_double_fifo, u32 ep_MaxPkt)
{
	u16 reg_val = 0;
	u16 temp = 0;

	//--<1>--config tx csr
	reg_val = (1 << USBC_BP_TXCSR_D_MODE);
	reg_val |= (1 << USBC_BP_TXCSR_D_CLEAR_DATA_TOGGLE);
	reg_val |= (1 << USBC_BP_TXCSR_D_FLUSH_FIFO);
	USBC_Writew(reg_val, USBC_REG_TXCSR(USBC0_BASE));

	if (is_double_fifo) {
		USBC_Writew(reg_val, USBC_REG_TXCSR(USBC0_BASE));
	}

	//--<2>--config tx ep max packet
	reg_val = USBC_Readw(USBC_REG_TXMAXP(USBC0_BASE));
	temp    = ep_MaxPkt & ((1 << USBC_BP_TXMAXP_PACKET_COUNT) - 1);
	reg_val |= temp;
	USBC_Writew(reg_val, USBC_REG_TXMAXP(USBC0_BASE));

	//--<3>--config ep transfer type
	switch(ts_type)
	{
		case USBC_TS_TYPE_ISO:
			__USBC_Dev_Tx_EnableIsoEp();
			break;

		case USBC_TS_TYPE_INT:
			__USBC_Dev_Tx_EnableIntEp();
			break;

		case USBC_TS_TYPE_BULK:
			__USBC_Dev_Tx_EnableBulkEp();
			break;

		default:
			__USBC_Dev_Tx_EnableBulkEp();
			break;
	}
}

static void __USBC_Dev_Tx_ConfigEpDma(void)
{
	u16 ep_csr = 0;

	//auto_set, tx_mode, dma_tx_en, mode1
	ep_csr = USBC_Readb(USBC_REG_TXCSR(USBC0_BASE) + 1);
	ep_csr |= (1 << USBC_BP_TXCSR_D_AUTOSET) >> 8;
	ep_csr |= (1 << USBC_BP_TXCSR_D_MODE) >> 8;
	ep_csr |= (1 << USBC_BP_TXCSR_D_DMA_REQ_EN) >> 8;
	ep_csr |= (1 << USBC_BP_TXCSR_D_DMA_REQ_MODE) >> 8;
	USBC_Writeb(ep_csr, (USBC_REG_TXCSR(USBC0_BASE) + 1));
}

static void __USBC_Dev_Tx_ClearEpDma(void)
{
	u16 ep_csr = 0;

	//auto_set, dma_tx_en, mode1
	ep_csr = USBC_Readb(USBC_REG_TXCSR(USBC0_BASE) + 1);
	ep_csr &= ~((1 << USBC_BP_TXCSR_D_AUTOSET) >> 8);
	ep_csr &= ~((1 << USBC_BP_TXCSR_D_DMA_REQ_EN) >> 8);
	USBC_Writeb(ep_csr, (USBC_REG_TXCSR(USBC0_BASE) + 1));

	//DMA_REQ_EN and DMA_REQ_MODE cannot be cleared in the same cycle
	ep_csr = USBC_Readb(USBC_REG_TXCSR(USBC0_BASE) + 1);
	ep_csr &= ~((1 << USBC_BP_TXCSR_D_DMA_REQ_MODE) >> 8);
	USBC_Writeb(ep_csr, (USBC_REG_TXCSR(USBC0_BASE) + 1));
}

static void __USBC_Dev_Rx_EnableIsoEp(void)
{
	USBC_REG_set_bit_w(USBC_BP_RXCSR_D_ISO, USBC_REG_RXCSR(USBC0_BASE));
}

static void __USBC_Dev_Rx_EnableIntEp(void)
{
	USBC_REG_clear_bit_w(USBC_BP_RXCSR_D_ISO, USBC_REG_RXCSR(USBC0_BASE));
}

static void __USBC_Dev_Rx_EnableBulkEp(void)
{
	USBC_REG_clear_bit_w(USBC_BP_RXCSR_D_ISO, USBC_REG_RXCSR(USBC0_BASE));
}

static void __USBC_Dev_Rx_ConfigEp_Default(void)
{
	//--<1>--clear tx csr
	USBC_Writew(0x00, USBC_REG_RXCSR(USBC0_BASE));

	//--<2>--clear tx ep max packet
	USBC_Writew(0x00, USBC_REG_RXMAXP(USBC0_BASE));

	//--<3>--config ep transfer type
}

static void __USBC_Dev_Rx_ConfigEp(u32 ts_type, u32 is_double_fifo, u32 ep_MaxPkt)
{
	u16 reg_val = 0;
	u16 temp = 0;

	//--<1>--config rx csr
	USBC_Writew((1 << USBC_BP_RXCSR_D_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_RXCSR_D_FLUSH_FIFO),
	USBC_REG_RXCSR(USBC0_BASE));

	if (is_double_fifo) {
		USBC_Writew((1 << USBC_BP_RXCSR_D_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_RXCSR_D_FLUSH_FIFO),
		USBC_REG_RXCSR(USBC0_BASE));
	}

	//--<2>--config rx ep max packet
	reg_val = USBC_Readw(USBC_REG_RXMAXP(USBC0_BASE));
	temp    = ep_MaxPkt & ((1 << USBC_BP_RXMAXP_PACKET_COUNT) - 1);
	reg_val |= temp;
	USBC_Writew(reg_val, USBC_REG_RXMAXP(USBC0_BASE));

	//--<3>--config ep transfer type
	switch(ts_type)
	{
		case USBC_TS_TYPE_ISO:
			__USBC_Dev_Rx_EnableIsoEp();
			break;

		case USBC_TS_TYPE_INT:
			__USBC_Dev_Rx_EnableIntEp();
			break;

		case USBC_TS_TYPE_BULK:
			__USBC_Dev_Rx_EnableBulkEp();
			break;

		default:
			__USBC_Dev_Rx_EnableBulkEp();
			break;
	}
}

static void __USBC_Dev_Rx_ConfigEpDma(void)
{
	u16 ep_csr = 0;

	//auto_clear, dma_rx_en, mode0
	ep_csr = USBC_Readb(USBC_REG_RXCSR(USBC0_BASE) + 1);
	ep_csr |= ((1 << USBC_BP_RXCSR_D_DMA_REQ_MODE) >> 8);
	USBC_Writeb(ep_csr, (USBC_REG_RXCSR(USBC0_BASE) + 1));

	ep_csr = USBC_Readb(USBC_REG_RXCSR(USBC0_BASE) + 1);
	ep_csr |= ((1 << USBC_BP_RXCSR_D_AUTO_CLEAR) >> 8);
	ep_csr |= ((1 << USBC_BP_RXCSR_D_DMA_REQ_EN) >> 8);
	USBC_Writeb(ep_csr, (USBC_REG_RXCSR(USBC0_BASE) + 1));

	ep_csr = USBC_Readb(USBC_REG_RXCSR(USBC0_BASE) + 1);
	ep_csr &= ~((1 << USBC_BP_RXCSR_D_DMA_REQ_MODE) >> 8);
	USBC_Writeb(ep_csr, (USBC_REG_RXCSR(USBC0_BASE) + 1));

	ep_csr = USBC_Readb(USBC_REG_RXCSR(USBC0_BASE) + 1);
	ep_csr |= ((1 << USBC_BP_RXCSR_D_DMA_REQ_MODE) >> 8);
	USBC_Writeb(ep_csr, (USBC_REG_RXCSR(USBC0_BASE) + 1));
}

static void __USBC_Dev_Rx_ClearEpDma(void)
{
	u16 ep_csr = 0;

	//auto_clear, dma_rx_en, mode0
	ep_csr = USBC_Readb(USBC_REG_RXCSR(USBC0_BASE) + 1);

	ep_csr &= ~((1 << USBC_BP_RXCSR_D_AUTO_CLEAR) >> 8);
	ep_csr &= ~((1 << USBC_BP_RXCSR_D_DMA_REQ_MODE) >> 8);
	ep_csr &= ~((1 << USBC_BP_RXCSR_D_DMA_REQ_EN) >> 8);
	USBC_Writeb(ep_csr, (USBC_REG_RXCSR(USBC0_BASE) + 1));
}

static void __USBC_Dev_ep0_ConfigEp0(void)
{
	USBC_Writew(1<<USBC_BP_CSR0_D_FLUSH_FIFO, USBC_REG_CSR0(USBC0_BASE));
}

/*
 * config EP, include double fifo, max packet size, etc.
 * @hUSB:       handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type:    transfer type
 * @is_double_fifo: speed mode
 * @ep_MaxPkt:  max packet size
 *
 */
int USBC_Dev_ConfigEp(u32 ts_type, u32 ep_type, u32 is_double_fifo, u32 ep_MaxPkt)
{
	switch(ep_type)
	{
		case USBC_EP_TYPE_EP0:
			__USBC_Dev_ep0_ConfigEp0();
			break;

		case USBC_EP_TYPE_TX:
			__USBC_Dev_Tx_ConfigEp(ts_type, is_double_fifo,  ep_MaxPkt);
			break;

		case USBC_EP_TYPE_RX:
			__USBC_Dev_Rx_ConfigEp(ts_type, is_double_fifo, ep_MaxPkt);
			break;

		default:
			return -1;
	}

	return 0;
}

void USBC_Clear_Feature_HALT(u32 ep_type)
{
	int reg_val;
	switch(ep_type)
	{
		case USBC_EP_TYPE_EP0:
			//
			break;
		case USBC_EP_TYPE_TX:
			reg_val = (1 << USBC_BP_TXCSR_D_MODE);
			reg_val |= (1 << USBC_BP_TXCSR_D_CLEAR_DATA_TOGGLE);
			reg_val |= (1 << USBC_BP_TXCSR_D_FLUSH_FIFO);
			USBC_Writew(reg_val, USBC_REG_TXCSR(USBC0_BASE));
			break;

		case USBC_EP_TYPE_RX:
			USBC_Writew((1 << USBC_BP_RXCSR_D_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_RXCSR_D_FLUSH_FIFO),
			USBC_REG_RXCSR(USBC0_BASE));
			break;

		default:
			break;
	}
}
void USBC_CoreRegDump(void)
{
	usbprint("\n------------usb deugdump-----------\n");
	usbprint("USBC_REG_ISCR:0x%08x\n",USBC_Readl(USBC_REG_ISCR(USBC0_BASE)));
	usbprint("USBC_REG_PHYCTL:0x%08x\n",USBC_Readl(USBC_REG_PHYCTL(USBC0_BASE)));
	usbprint("USBC_REG_PHYBIST:0x%08x\n",USBC_Readl(USBC_REG_PHYBIST(USBC0_BASE)));
	usbprint("USBC_REG_PHYTUNE:0x%08x\n",USBC_Readl(USBC_REG_PHYTUNE(USBC0_BASE)));
	usbprint("USBC_REG_PMU_IRQ:0x%08x\n",USBC_Readl(USBC_REG_PMU_IRQ(USBC0_BASE)));
	usbprint("USBPHY_CLK_REG:0x%08x\n",USBC_Readl(USBPHY_CLK_REG));
	usbprint("BUS_CLK_GATE0_REG:0x%08x\n",USBC_Readl(BUS_CLK_GATE0_REG));
	usbprint("BUS_CLK_RST_REG:0x%08x\n",USBC_Readl(BUS_CLK_RST_REG));
	usbprint("USBC_REG_CSR:0x%08x\n", USBC_Readl(USBC_REG_CSR(USBC0_BASE)));
	usbprint("USBC_REG_VEND0:0x%02x\n", USBC_Readb(USBC_REG_VEND0(USBC0_BASE)));
	usbprint("USBC_REG_FADDR:0x%02x\n",USBC_Readb(USBC_REG_FADDR(USBC0_BASE)));
	usbprint("USBC_REG_PCTL:0x%02x\n",USBC_Readb(USBC_REG_PCTL(USBC0_BASE)));
	usbprint("USBC_REG_INTUSBE:0x%04x\n",USBC_Readw(USBC_REG_INTUSBE(USBC0_BASE)));
	usbprint("USBC_REG_INTRxE:0x%04x\n",USBC_Readw(USBC_REG_INTRxE(USBC0_BASE)));
	usbprint("USBC_REG_INTTxE:0x%04x\n",USBC_Readw(USBC_REG_INTTxE(USBC0_BASE)));
	usbprint("\n----------------------------------------------\n");
}
