/*
 * usb_host_phy.h
 *
 *  Created on: 2020Äê4ÔÂ3ÈÕ
 *      Author: VeiLiang
 */

#ifndef USB_HOST_PHY_H
#define USB_HOST_PHY_H

#ifdef __cplusplus
extern "C" {
#endif
#include "rtthread.h"
#include <stdint.h>

typedef signed   char                   int8_t;      /**<  8bit integer type */
typedef signed   short                  int16_t;     /**< 16bit integer type */
typedef signed   long                   int32_t;     /**< 32bit integer type */
typedef unsigned char                   uint8_t;     /**<  8bit unsigned integer type */
typedef unsigned short                  uint16_t;    /**< 16bit unsigned integer type */
typedef unsigned long                   uint32_t;    /**< 32bit unsigned integer type */


#define USB_CLEAR_AND_DISABLE_INTERRUPTS do {\
                                                  /* Clear all pending Device Interrupts */ \
                                                  pUSB_Otg->IntrTx1; \
                                                  pUSB_Otg->IntrTx2; \
                                                  \
                                                  pUSB_Otg->IntrRx1; \
                                                  pUSB_Otg->IntrRx2; \
                                                  pUSB_Otg->IntrUSB; \
                                                  \
                                                  /*Disable each individual interrupt for Tx endpoints*/ \
                                                  pUSB_Otg->InTrTx1E &= 0x00; \
                                                  pUSB_Otg->InTrTx2E &= 0x00; \
                                                  \
                                                  /*Disable each individual interrupt for Rx endpoints*/ \
                                                  pUSB_Otg->InTrRx1E &= 0x00; \
                                                  pUSB_Otg->InTrRx2E &= 0x00; \
                                          } while(0)


/**************Bit definition for USB_OTG_VEND0 register *******************/
/*Flags for DEVCTL register*/
#define USB_OTG_DEVCTL_SESSION_Pos               (0U)
#define USB_OTG_DEVCTL_SESSION_Msk               (0x1U << USB_OTG_DEVCTL_SESSION_Pos) /*!< 0x00000001 */
#define USB_OTG_DEVCTL_SESSION                   USB_OTG_DEVCTL_SESSION_Msk
#define USB_OTG_DEVCTL_HOSTMODE_Pos              (2U)
#define USB_OTG_DEVCTL_HOSTMODE_Msk              (0x1U << USB_OTG_DEVCTL_HOSTMODE_Pos) /*!< 0x00000004 */
#define USB_OTG_DEVCTL_HOSTMODE                  USB_OTG_DEVCTL_HOSTMODE_Msk  /*!< USB Mode */
#define USB_OTG_DEVCTL_LSDEV_Pos                 (5U)
#define USB_OTG_DEVCTL_LSDEV_Msk                 (0x1U << USB_OTG_DEVCTL_LSDEV_Pos) /*!< 0x00000020 */
#define USB_OTG_DEVCTL_LSDEV                     USB_OTG_DEVCTL_LSDEV_Msk
#define USB_OTG_DEVCTL_FSDEV_Pos                 (6U)
#define USB_OTG_DEVCTL_FSDEV_Msk                 (0x1U << USB_OTG_DEVCTL_FSDEV_Pos) /*!< 0x00000040 */
#define USB_OTG_DEVCTL_FSDEV                     USB_OTG_DEVCTL_FSDEV_Msk


/*Flags for POWER register*/
#define USB_OTG_POWER_RESET_Pos                  (3U)
#define USB_OTG_POWER_RESET_Msk                  (0x1U << USB_OTG_POWER_RESET_Pos) /*!< 0x00000008 */
#define USB_OTG_POWER_RESET                      USB_OTG_POWER_RESET_Msk  /*!< Reset */

#define USB_OTG_POWER_READHS_Pos                  (4U)
#define USB_OTG_POWER_READHS_Msk                  (0x1U << USB_OTG_POWER_READHS_Pos) /*!< 0x00000008 */
#define USB_OTG_POWER_READHS                      USB_OTG_POWER_READHS_Msk

#define USB_OTG_POWER_SETHS_Pos                  (5U)
#define USB_OTG_POWER_SETHS_Msk                  (0x1U << USB_OTG_POWER_SETHS_Pos) /*!< 0x00000008 */
#define USB_OTG_POWER_SETHS                      USB_OTG_POWER_SETHS_Msk  /*!< Reset *

#define USB_OTG_POWER_ISOUPDATE_Pos              (7U)
#define USB_OTG_POWER_ISOUPDATE_Msk              (0x1U << USB_OTG_POWER_ISOUPDATE_Pos) /*!< 0x00000080 */
#define USB_OTG_POWER_ISOUPDATE                  USB_OTG_POWER_ISOUPDATE_Msk
#define USB_OTG_POWER_RESUME_Pos                 (2U)
#define USB_OTG_POWER_RESUME_Msk                 (0x1U << USB_OTG_POWER_RESUME_Pos) /*!< 0x00000004 */
#define USB_OTG_POWER_RESUME                     USB_OTG_POWER_RESUME_Msk  /*!< Resume */
#define USB_OTG_POWER_SUSPENDMODE_Pos            (1U)
#define USB_OTG_POWER_SUSPENDMODE_Msk            (0x1U << USB_OTG_POWER_SUSPENDMODE_Pos) /*!< 0x00000002 */
#define USB_OTG_POWER_SUSPENDMODE                 USB_OTG_POWER_SUSPENDMODE_Msk  /*!< Suspend Mode */
#define USB_OTG_POWER_SUSPENDEN_Pos              (0U)
#define USB_OTG_POWER_SUSPENDEN_Msk              (0x1U << USB_OTG_POWER_SUSPENDEN_Pos) /*!< 0x00000001 */
#define USB_OTG_POWER_SUSPENDEN                   USB_OTG_POWER_SUSPENDEN_Msk  /*!< Suspend Enable */


/*Flags for TXCSR1 register for BOTH peripheral & host*/
#define USB_OTG_TXCSR1_TXPKTRDY_Pos              (0U)
#define USB_OTG_TXCSR1_TXPKTRDY_Msk              (0x1U << USB_OTG_TXCSR1_TXPKTRDY_Pos) /*!< 0x00000001 */
#define USB_OTG_TXCSR1_TXPKTRDY                  USB_OTG_TXCSR1_TXPKTRDY_Msk  /*!< Tx Packet Ready */
#define USB_OTG_TXCSR1_FIFONOTEMPTY_Pos          (1U)
#define USB_OTG_TXCSR1_FIFONOTEMPTY_Msk          (0x1U << USB_OTG_TXCSR1_FIFONOTEMPTY_Pos) /*!< 0x00000002 */
#define USB_OTG_TXCSR1_FIFONOTEMPTY              USB_OTG_TXCSR1_FIFONOTEMPTY_Msk  /*!< At least 1 packet in Tx FIFO */
#define USB_OTG_TXCSR1_FLUSHFIFO_Pos             (3U)
#define USB_OTG_TXCSR1_FLUSHFIFO_Msk             (0x1U << USB_OTG_TXCSR1_FLUSHFIFO_Pos) /*!< 0x00000008 */
#define USB_OTG_TXCSR1_FLUSHFIFO                 USB_OTG_TXCSR1_FLUSHFIFO_Msk  /*!< Flush Tx Endpoint FIFO */
#define USB_OTG_TXCSR1_CLRDATATOG_Pos            (6U)
#define USB_OTG_TXCSR1_CLRDATATOG_Msk            (0x1U << USB_OTG_TXCSR1_CLRDATATOG_Pos) /*!< 0x00000040 */
#define USB_OTG_TXCSR1_CLRDATATOG                USB_OTG_TXCSR1_CLRDATATOG_Msk  /*!< Reset endpoint data toggle to 0*/

/* Tx ep Control and Status Register for Host only */
#define  USBC_BP_TXCSR_H_AUTOSET			15
#define  USBC_BP_TXCSR_H_RESERVED			14
#define  USBC_BP_TXCSR_H_MODE				13
#define  USBC_BP_TXCSR_H_DMA_REQ_EN			12
#define  USBC_BP_TXCSR_H_FORCE_DATA_TOGGLE		11
#define  USBC_BP_TXCSR_H_DMA_REQ_MODE			10
#define  USBC_BP_TXCSR_H_DATA_TOGGLE_WR_EN	  	9
#define  USBC_BP_TXCSR_H_DATA_TOGGLE			8
#define  USBC_BP_TXCSR_H_NAK_TIMEOUT			7
#define  USBC_BP_TXCSR_H_CLEAR_DATA_TOGGLE		6
#define  USBC_BP_TXCSR_H_TX_STALL			5
#define  USBC_BP_TXCSR_H_FLUSH_FIFO			3
#define  USBC_BP_TXCSR_H_ERROR				2
#define  USBC_BP_TXCSR_H_FIFO_NOT_EMPTY 		1
#define  USBC_BP_TXCSR_H_TX_READY			0

#define  USBC_TXCSR_H_WZC_BITS				((1 << USBC_BP_TXCSR_H_NAK_TIMEOUT) | (1 << USBC_BP_TXCSR_H_TX_STALL) \
							| (1 << USBC_BP_TXCSR_H_ERROR) | (1 << USBC_BP_TXCSR_H_FIFO_NOT_EMPTY))


/*Flags for TXCSR1 register for Peripheral*/
#define USB_OTG_TXCSR1_UNDERRUN_Pos              (2U)
#define USB_OTG_TXCSR1_UNDERRUN_Msk              (0x1U << USB_OTG_TXCSR1_UNDERRUN_Pos) /*!< 0x00000004 */
#define USB_OTG_TXCSR1_UNDERRUN                  USB_OTG_TXCSR1_UNDERRUN_Msk  /*!< In token received while TxPktRdy not set */
#define USB_OTG_TXCSR1_FLUSHFIFO_Pos             (3U)
#define USB_OTG_TXCSR1_FLUSHFIFO_Msk             (0x1U << USB_OTG_TXCSR1_FLUSHFIFO_Pos) /*!< 0x00000008 */
#define USB_OTG_TXCSR1_FLUSHFIFO                 USB_OTG_TXCSR1_FLUSHFIFO_Msk  /*!< Flush Tx Endpoint FIFO */
#define USB_OTG_TXCSR1_SENDSTLL_Pos              (4U)
#define USB_OTG_TXCSR1_SENDSTLL_Msk              (0x1U << USB_OTG_TXCSR1_SENDSTLL_Pos) /*!< 0x00000010 */
#define USB_OTG_TXCSR1_SENDSTLL                  USB_OTG_TXCSR1_SENDSTLL_Msk  /*!< Send STALL handshake to IN token */
#define USB_OTG_TXCSR1_SENTSTLL_Pos              (5U)
#define USB_OTG_TXCSR1_SENTSTLL_Msk              (0x1U << USB_OTG_TXCSR1_SENTSTLL_Pos) /*!< 0x00000020 */
#define USB_OTG_TXCSR1_SENTSTLL                  USB_OTG_TXCSR1_SENTSTLL_Msk  /*!< STALL handshake to IN token has been transmitted*/
#define USB_OTG_TXCSR1_CLRDATATOG_Pos            (6U)
#define USB_OTG_TXCSR1_CLRDATATOG_Msk            (0x1U << USB_OTG_TXCSR1_CLRDATATOG_Pos) /*!< 0x00000040 */
#define USB_OTG_TXCSR1_CLRDATATOG                USB_OTG_TXCSR1_CLRDATATOG_Msk  /*!< Reset endpoint data toggle to 0*/

/*Flags for TXCSR1 register for Host*/
#define USB_OTG_TXCSR1_ERR_Pos                   (2U)
#define USB_OTG_TXCSR1_ERR_Msk                   (0x1U << USB_OTG_TXCSR1_ERR_Pos) /*!< 0x00000004 */
#define USB_OTG_TXCSR1_ERR                       USB_OTG_TXCSR1_ERR_Msk  /*!< In token received while TxPktRdy not set */
#define USB_OTG_TXCSR1_RXSTALL_Pos               (5U)
#define USB_OTG_TXCSR1_RXSTALL_Msk               (0x1U << USB_OTG_TXCSR1_RXSTALL_Pos) /*!< 0x00000004 */
#define USB_OTG_TXCSR1_RXSTALL                   USB_OTG_TXCSR1_RXSTALL_Msk  /*!< In token received while TxPktRdy not set */
#define USB_OTG_TXCSR1_NAKTMOUT_Pos              (7U)
#define USB_OTG_TXCSR1_NAKTMOUT_Msk              (0x1U << USB_OTG_TXCSR1_NAKTMOUT_Pos) /*!< 0x00000004 */
#define USB_OTG_TXCSR1_NAKTMOUT                  USB_OTG_TXCSR1_NAKTMOUT_Msk  /*!< In token received while TxPktRdy not set */


/*Flags for TXCSR2 register*/
#define USB_OTG_TXCSR2_MODE_Pos                  (5U)
#define USB_OTG_TXCSR2_MODE_Msk                  (0x1U << USB_OTG_TXCSR2_MODE_Pos) /*!< 0x00000020 */
#define USB_OTG_TXCSR2_MODE                      USB_OTG_TXCSR2_MODE_Msk /*!< Enable endpoint direction */
#define USB_OTG_TXCSR2_ISO_Pos                   (6U)
#define USB_OTG_TXCSR2_ISO_Msk                   (0x1U << USB_OTG_TXCSR2_ISO_Pos) /*!< 0x00000040 */
#define USB_OTG_TXCSR2_ISO                        USB_OTG_TXCSR2_ISO_Msk  /*!< Set 1 enable Isochronous protocol*/


/*Flags for RXCSR1 register for BOTH peripheral & host*/
#define USB_OTG_RXCSR1_RXPKTRDY_Pos              (0U)
#define USB_OTG_RXCSR1_RXPKTRDY_Msk              (0x1U << USB_OTG_RXCSR1_RXPKTRDY_Pos) /*!< 0x00000001 */
#define USB_OTG_RXCSR1_RXPKTRDY                  USB_OTG_RXCSR1_RXPKTRDY_Msk  /*!< A data packet (setup packet included) recevied */
#define USB_OTG_RXCSR1_FIFOFULL_Pos              (1U)
#define USB_OTG_RXCSR1_FIFOFULL_Msk              (0x1U << USB_OTG_RXCSR1_FIFOFULL_Pos) /*!< 0x00000002 */
#define USB_OTG_RXCSR1_FIFOFULL                  USB_OTG_RXCSR1_FIFOFULL_Msk  /*!< No more packets can loaded into Rx FIFO */
#define USB_OTG_RXCSR1_FLUSHFIFO_Pos             (4U)
#define USB_OTG_RXCSR1_FLUSHFIFO_Msk             (0x1U << USB_OTG_RXCSR1_FLUSHFIFO_Pos) /*!< 0x00000010 */
#define USB_OTG_RXCSR1_FLUSHFIFO                  USB_OTG_RXCSR1_FLUSHFIFO_Msk  /*!< Flush RX FIFO */
#define USB_OTG_RXCSR1_CLRDATATOG_Pos            (7U)
#define USB_OTG_RXCSR1_CLRDATATOG_Msk            (0x1U << USB_OTG_RXCSR1_CLRDATATOG_Pos) /*!< 0x00000080 */
#define USB_OTG_RXCSR1_CLRDATATOG                USB_OTG_RXCSR1_CLRDATATOG_Msk  /*!< Reset endpoint data toggle to 0*/


/*Flags for RXCSR1 register for Peripheral*/
#define USB_OTG_RXCSR1_OVERRUN_Pos               (2U)
#define USB_OTG_RXCSR1_OVERRUN_Msk               (0x1U << USB_OTG_RXCSR1_OVERRUN_Pos) /*!< 0x00000004 */
#define USB_OTG_RXCSR1_OVERRUN                   USB_OTG_RXCSR1_OVERRUN_Msk  /*!< No more packets can loaded into Rx FIFO */
#define USB_OTG_RXCSR1_DATAERR_Pos               (3U)
#define USB_OTG_RXCSR1_DATAERR_Msk               (0x1U << USB_OTG_RXCSR1_DATAERR_Pos) /*!< 0x00000008 */
#define USB_OTG_RXCSR1_DATAERR                   USB_OTG_RXCSR1_DATAERR_Msk  /*!< CRC or bit stuff error */
#define USB_OTG_RXCSR1_SENDSTLL_Pos              (5U)
#define USB_OTG_RXCSR1_SENDSTLL_Msk              (0x1U << USB_OTG_RXCSR1_SENDSTLL_Pos) /*!< 0x00000020 */
#define USB_OTG_RXCSR1_SENDSTLL                  USB_OTG_RXCSR1_SENDSTLL_Msk  /*!< Send STALL handshake */
#define USB_OTG_RXCSR1_SENTSTLL_Pos              (6U)
#define USB_OTG_RXCSR1_SENTSTLL_Msk              (0x1U << USB_OTG_RXCSR1_SENTSTLL_Pos) /*!< 0x00000040 */
#define USB_OTG_RXCSR1_SENTSTLL                  USB_OTG_RXCSR1_SENTSTLL_Msk  /*!< STALL handshake has been transmitted*/


/*Flags for RXCSR1 register for Host*/
#define USB_OTG_RXCSR1_ERR_Pos                   (2U)
#define USB_OTG_RXCSR1_ERR_Msk                   (0x1U << USB_OTG_RXCSR1_ERR_Pos) /*!< 0x00000004 */
#define USB_OTG_RXCSR1_ERR                       USB_OTG_RXCSR1_ERR_Msk  /*!< No more packets can loaded into Rx FIFO */
#define USB_OTG_RXCSR1_ERRTMOUT_Pos              (3U)
#define USB_OTG_RXCSR1_ERRTMOUT_Msk              (0x1U << USB_OTG_RXCSR1_ERRTMOUT_Pos) /*!< 0x00000008 */
#define USB_OTG_RXCSR1_ERRTMOUT                  USB_OTG_RXCSR1_ERRTMOUT_Msk  /*!< CRC or bit stuff error */
#define USB_OTG_RXCSR1_REQPKT_Pos                (5U)
#define USB_OTG_RXCSR1_REQPKT_Msk                (0x1U << USB_OTG_RXCSR1_REQPKT_Pos) /*!< 0x00000008 */
#define USB_OTG_RXCSR1_REQPKT                    USB_OTG_RXCSR1_REQPKT_Msk  /*!< CRC or bit stuff error */
#define USB_OTG_RXCSR1_RXSTALL_Pos               (6U)
#define USB_OTG_RXCSR1_RXSTALL_Msk               (0x1U << USB_OTG_RXCSR1_RXSTALL_Pos) /*!< 0x00000008 */
#define USB_OTG_RXCSR1_RXSTALL                   USB_OTG_RXCSR1_RXSTALL_Msk  /*!< CRC or bit stuff error */


/*Flags for RXCSR2 register*/
#define USB_OTG_RXCSR2_DMAMODE_Pos               (4U)
#define USB_OTG_RXCSR2_DMAMODE_Msk               (0x1U << USB_OTG_RXCSR2_DMAMODE_Pos) /*!< 0x00000010 */
#define USB_OTG_RXCSR2_DMAMODE                   USB_OTG_RXCSR2_DMAMODE_Msk  /*!< DMA Mode */
#define USB_OTG_RXCSR2_DMAEN_Pos                 (5U)
#define USB_OTG_RXCSR2_DMAEN_Msk                 (0x1U << USB_OTG_RXCSR2_DMAMODE_Pos) /*!< 0x00000020 */
#define USB_OTG_RXCSR2_DMAEN                     USB_OTG_RXCSR2_DMAMODE_Msk  /*!< DMA Enable */
#define USB_OTG_RXCSR2_ISO_Pos                   (6U)
#define USB_OTG_RXCSR2_ISO_Msk                   (0x1U << USB_OTG_RXCSR2_ISO_Pos) /*!< 0x00000040 */
#define USB_OTG_RXCSR2_ISO                       USB_OTG_RXCSR2_ISO_Msk  /*!< ISO/AuToReq */
#define USB_OTG_RXCSR2_AUTOREQ_Pos               (6U)
#define USB_OTG_RXCSR2_AUTOREQ_Msk               (0x1U << USB_OTG_RXCSR2_AUTOREQ_Pos) /*!< 0x00000040 */
#define USB_OTG_RXCSR2_AUTOREQ                   USB_OTG_RXCSR2_ISO_Msk  /*!< ISO/AuToReq */
#define USB_OTG_RXCSR2_AUTOCLEAR_Pos             (7U)
#define USB_OTG_RXCSR2_AUTOCLEAR_Msk             (0x1U << USB_OTG_RXCSR2_AUTOCLEAR_Pos) /*!< 0x00000080 */
#define USB_OTG_RXCSR2_AUTOCLEAR                 USB_OTG_RXCSR2_ISO_Msk  /*!< AutoClear */

/*Flags for INTRUSB register*/
#define USB_OTG_INTRUSB_SUSPEND_Pos              (0U)
#define USB_OTG_INTRUSB_SUSPEND_MSK              (0x1U << USB_OTG_INTRUSB_SUSPEND_Pos) /*!< 0x00000001 */
#define USB_OTG_INTRUSB_SUSPEND                  USB_OTG_INTRUSB_SUSPEND_MSK  /*!< Suspend request */
#define USB_OTG_INTRUSB_RESUME_Pos               (1U)
#define USB_OTG_INTRUSB_RESUME_MSK               (0x1U << USB_OTG_INTRUSB_RESUME_Pos) /*!< 0x00000002 */
#define USB_OTG_INTRUSB_RESUME                   USB_OTG_INTRUSB_RESUME_MSK  /*!< Resume request */
#define USB_OTG_INTRUSB_RSTBABL_Pos              (2U)
#define USB_OTG_INTRUSB_RSTBABL_MSK              (0x1U << USB_OTG_INTRUSB_RSTBABL_Pos) /*!< 0x00000004 */
#define USB_OTG_INTRUSB_RSTBABL                  USB_OTG_INTRUSB_RSTBABL_MSK  /*!< Reset or Babble request */
#define USB_OTG_INTRUSB_SOF_Pos                  (3U)
#define USB_OTG_INTRUSB_SOF_MSK                  (0x1U << USB_OTG_INTRUSB_SOF_Pos) /*!< 0x00000008 */
#define USB_OTG_INTRUSB_SOF                      USB_OTG_INTRUSB_SOF_MSK  /*!< SOF request */
#define USB_OTG_INTRUSB_CON_Pos                  (4U)
#define USB_OTG_INTRUSB_CON_MSK                  (0x1U << USB_OTG_INTRUSB_CON_Pos) /*!< 0x00000010 */
#define USB_OTG_INTRUSB_CON                      USB_OTG_INTRUSB_CON_MSK  /*!< Connect request */
#define USB_OTG_INTRUSB_DISCON_Pos               (5U)
#define USB_OTG_INTRUSB_DISCON_MSK               (0x1U << USB_OTG_INTRUSB_DISCON_Pos) /*!< 0x00000020 */
#define USB_OTG_INTRUSB_DISCON                   USB_OTG_INTRUSB_DISCON_MSK  /*!< Connect request */
#define USB_OTG_INTRUSB_SESSION_Pos              (6U)
#define USB_OTG_INTRUSB_SESSION_MSK              (0x1U << USB_OTG_INTRUSB_SESSION_Pos) /*!< 0x00000040 */
#define USB_OTG_INTRUSB_SESSION                  USB_OTG_INTRUSB_SESSION_MSK  /*!< Session request */
#define USB_OTG_INTRUSB_VBUSERR_Pos              (7U)
#define USB_OTG_INTRUSB_VBUSERR_MSK              (0x1U << USB_OTG_INTRUSB_VBUSERR_Pos) /*!< 0x00000080 */
#define USB_OTG_INTRUSB_VBUSERR                  USB_OTG_INTRUSB_VBUSERR_MSK  /*!< VBUS Error request */

/*Flags for INTRUSBE register*/
#define USB_OTG_INTRUSBE_SUSPEND_Pos             (0U)
#define USB_OTG_INTRUSBE_SUSPEND_MSK             (0x1U << USB_OTG_INTRUSBE_SUSPEND_Pos) /*!< 0x00000001 */
#define USB_OTG_INTRUSBE_SUSPEND                 USB_OTG_INTRUSBE_SUSPEND_MSK  /*!< Suspend request */
#define USB_OTG_INTRUSBE_RESUME_Pos              (1U)
#define USB_OTG_INTRUSBE_RESUME_MSK              (0x1U << USB_OTG_INTRUSBE_RESUME_Pos) /*!< 0x00000002 */
#define USB_OTG_INTRUSBE_RESUME                  USB_OTG_INTRUSBE_RESUME_MSK  /*!< Resume request */
#define USB_OTG_INTRUSBE_RSTBABL_Pos             (2U)
#define USB_OTG_INTRUSBE_RSTBABL_MSK             (0x1U << USB_OTG_INTRUSBE_RSTBABL_Pos) /*!< 0x00000004 */
#define USB_OTG_INTRUSBE_RSTBABL                 USB_OTG_INTRUSBE_RSTBABL_MSK  /*!< Reset or Babble request */
#define USB_OTG_INTRUSBE_SOF_Pos                 (3U)
#define USB_OTG_INTRUSBE_SOF_MSK                 (0x1U << USB_OTG_INTRUSBE_SOF_Pos) /*!< 0x00000008 */
#define USB_OTG_INTRUSBE_SOF                     USB_OTG_INTRUSBE_SOF_MSK  /*!< SOF request */
#define USB_OTG_INTRUSBE_CON_Pos                 (4U)
#define USB_OTG_INTRUSBE_CON_MSK                 (0x1U << USB_OTG_INTRUSBE_CON_Pos) /*!< 0x00000010 */
#define USB_OTG_INTRUSBE_CON                     USB_OTG_INTRUSBE_CON_MSK  /*!< Connect request */
#define USB_OTG_INTRUSBE_DISCON_Pos              (5U)
#define USB_OTG_INTRUSBE_DISCON_MSK              (0x1U << USB_OTG_INTRUSBE_DISCON_Pos) /*!< 0x00000020 */
#define USB_OTG_INTRUSBE_DISCON                  USB_OTG_INTRUSBE_DISCON_MSK  /*!< Connect request */
#define USB_OTG_INTRUSBE_SESSION_Pos             (6U)
#define USB_OTG_INTRUSBE_SESSION_MSK             (0x1U << USB_OTG_INTRUSBE_SESSION_Pos) /*!< 0x00000040 */
#define USB_OTG_INTRUSBE_SESSION                 USB_OTG_INTRUSBE_SESSION_MSK  /*!< Session request */
#define USB_OTG_INTRUSBE_VBUSERR_Pos             (7U)
#define USB_OTG_INTRUSBE_VBUSERR_MSK             (0x1U << USB_OTG_INTRUSBE_VBUSERR_Pos) /*!< 0x00000080 */
#define USB_OTG_INTRUSBE_VBUSERR                 USB_OTG_INTRUSBE_VBUSERR_MSK  /*!< VBUS Error request */

/*Flags for CSR0 register for usb peripheral & host*/
#define USB_OTG_CSR0_RXPKTRDY_Pos                (0U)
#define USB_OTG_CSR0_RXPKTRDY_MSK                (0x1U << USB_OTG_CSR0_RXPKTRDY_Pos) /*!< 0x00000001 */
#define USB_OTG_CSR0_RXPKTRDY                    USB_OTG_CSR0_RXPKTRDY_MSK  /*!< Datapacket received*/
#define USB_OTG_CSR0_TXPKTRDY_Pos                (1U)
#define USB_OTG_CSR0_TXPKTRDY_MSK                (0x1U << USB_OTG_CSR0_TXPKTRDY_Pos) /*!< 0x00000002 */
#define USB_OTG_CSR0_TXPKTRDY                    USB_OTG_CSR0_TXPKTRDY_MSK  /*!< Packet has been loaded into FIFO*/

/*Flags for CSR0 register for usb peripheral*/
#define USB_OTG_CSR0_SENTSTALL_Pos               (2U)
#define USB_OTG_CSR0_SENTSTALL_MSK               (0x1U << USB_OTG_CSR0_SENTSTALL_Pos) /*!< 0x00000004 */
#define USB_OTG_CSR0_SENTSTALL                   USB_OTG_CSR0_SENTSTALL_MSK  /*!< STALL handshake is transmitted*/
#define USB_OTG_CSR0_DATAEND_Pos                 (3U)
#define USB_OTG_CSR0_DATAEND_MSK                 (0x1U << USB_OTG_CSR0_DATAEND_Pos) /*!< 0x00000008 */
#define USB_OTG_CSR0_DATAEND                     USB_OTG_CSR0_DATAEND_MSK  /*!< Data transmission end*/
#define USB_OTG_CSR0_SETUPEND_Pos                (4U)
#define USB_OTG_CSR0_SETUPEND_MSK                (0x1U << USB_OTG_CSR0_SETUPEND_Pos) /*!< 0x00000010 */
#define USB_OTG_CSR0_SETUPEND                    USB_OTG_CSR0_SETUPEND_MSK  /*!< Set up end, an execption*/
#define USB_OTG_CSR0_SENDSTALL_Pos               (5U)
#define USB_OTG_CSR0_SENDSTALL_MSK               (0x1U << USB_OTG_CSR0_SENDSTALL_Pos) /*!< 0x00000020 */
#define USB_OTG_CSR0_SENDSTALL                   USB_OTG_CSR0_SENDSTALL_MSK  /*!< Send Stall*/
#define USB_OTG_CSR0_SERVRXPKTRDY_Pos            (6U)
#define USB_OTG_CSR0_SERVRXPKTRDY_MSK            (0x1U << USB_OTG_CSR0_SERVRXPKTRDY_Pos) /*!< 0x00000040 */
#define USB_OTG_CSR0_SERVRXPKTRDY                USB_OTG_CSR0_SERVRXPKTRDY_MSK  /*!< Server RxPktRdy Flag*/
#define USB_OTG_CSR0_SERVSETUPEND_Pos            (7U)
#define USB_OTG_CSR0_SERVSETUPEND_MSK            (0x1U << USB_OTG_CSR0_SERVSETUPEND_Pos) /*!< 0x00000080 */
#define USB_OTG_CSR0_SERVSETUPEND                USB_OTG_CSR0_SERVSETUPEND_MSK  /*!< Server SetupEnd Flag*/

/*Flags for CSR0 register for usb host*/
#define USB_OTG_CSR0_RXSTALL_Pos                 (2U)
#define USB_OTG_CSR0_RXSTALL_MSK                 (0x1U << USB_OTG_CSR0_RXSTALL_Pos) /*!< 0x00000004 */
#define USB_OTG_CSR0_RXSTALL                     USB_OTG_CSR0_RXSTALL_MSK  /*!< STALL handshake is received*/
#define USB_OTG_CSR0_SETUPPKT_Pos                (3U)
#define USB_OTG_CSR0_SETUPPKT_MSK                (0x1U << USB_OTG_CSR0_SETUPPKT_Pos) /*!< 0x00000008 */
#define USB_OTG_CSR0_SETUPPKT                    USB_OTG_CSR0_SETUPPKT_MSK  /*!< Setup request*/
#define USB_OTG_CSR0_ERR_Pos                     (4U)
#define USB_OTG_CSR0_ERR_MSK                     (0x1U << USB_OTG_CSR0_ERR_Pos) /*!< 0x00000010 */
#define USB_OTG_CSR0_ERR                         USB_OTG_CSR0_ERR_MSK  /*!< Error In transaction*/
#define USB_OTG_CSR0_REQPKT_Pos                  (5U)
#define USB_OTG_CSR0_REQPKT_MSK                  (0x1U << USB_OTG_CSR0_REQPKT_Pos) /*!< 0x00000020 */
#define USB_OTG_CSR0_REQPKT                      USB_OTG_CSR0_REQPKT_MSK  /*!< Resquest an In transaction*/
#define USB_OTG_CSR0_STATUSPKT_Pos               (6U)
#define USB_OTG_CSR0_STATUSPKT_MSK               (0x1U << USB_OTG_CSR0_STATUSPKT_Pos) /*!< 0x00000040 */
#define USB_OTG_CSR0_STATUSPKT                   USB_OTG_CSR0_STATUSPKT_MSK  /*!< Perform a status stage transaction*/

/*Flags for CSR02 register*/
#define USB_OTG_CSR02_FLUSHFIFO_Pos              (0U)
#define USB_OTG_CSR02_FLUSHFIFO_MSK              (0x1U << USB_OTG_CSR02_FLUSHFIFO_Pos) /*!< 0x00000001 */
#define USB_OTG_CSR02_FLUSHFIFO                  USB_OTG_CSR02_FLUSHFIFO_MSK  /*!< FlushFIFO for endpoint 0*/

#define USB_HOST_OTG_ADDR (0x01c13000)




#define __I  volatile
#define __IO  volatile

typedef struct
{
	__IO uint8_t FIFOx[16]; 	  /*!< FIFOs for Endpoints 0 ~ 15. 0x00 ~ 0x0F*/

	__IO uint8_t rfu0[48];		      /* 								  3Fh*/
	__IO uint8_t Power; 			  /*!< Function Power Management	  40h */
	__IO uint8_t DevCtl;              /*!<  USB device control register.  41h */
	__IO uint8_t Index;               /*!< Index register for selecting the endpoint status and control registers.   42h */
	//1
	__IO uint8_t VEND0;				  /*43h----------*/

	__IO uint8_t IntrTx1;			  /*!< Interrupt register for Endpoint 0 plus Tx Endpoints 1 to 7.	 44h */
	__IO uint8_t IntrTx2;			  /*!< Interrupt register for Tx Endpoints 8 to 15.   45h */
	__IO uint8_t IntrRx1;             /*!< Interrupt register for Endpoint 0 plus Tx Endpoints 1 to 7.   46h */
    __IO uint8_t IntrRx2;             /*!< Interrupt register for Tx Endpoints 8 to 15.   47h */
	__IO uint8_t InTrTx1E;			  /*!< Interrupt enable register for IntrTx1.	48h */
	__IO uint8_t InTrTx2E;			  /*!< Interrupt enable register for IntrTx2.	49h */
	__IO uint8_t InTrRx1E;			  /*!< Interrupt enable register for IntrRx1.	4Ah */
	__IO uint8_t InTrRx2E;			  /*!< Interrupt enable register for IntrRx2.	4Bh */
	__IO uint8_t IntrUSB;             /*!< Interrupt register for common USB interrupts.  4Ch */
	__IO uint8_t rfu1[3];		      /* 								  4Fh*/
	__IO uint8_t IntrUSBE;			  /*!< Interrupt enable register for IntrUSB.	50h */
	__IO uint8_t rfu2[3];		      /* 								  53h*/
	__IO uint8_t Frame1; 			 /*!< Frame number bits 0 to 7.   54h */
	__IO uint8_t Frame2;			  /*!< Frame number bits 8 to 10. 55h */
	__IO uint8_t rfu3[42];		      /* 						      7Fh*/
	//2
	__IO uint16_t TxMaxP;			  /*!< Maximum packet size for peripheral IN endpoint. (Index register set to select Endpoints 1 ~ 15 only) 80h */

	union
	{
		__IO uint8_t CSR0;			 /*!< Main Control Status register for Endpoint 0. (Index register set to select Endpoint 82h) */
		__IO uint8_t TxCSR1;		 /*!< Control Status register 1 for peripheral IN endpoint. (Index register set to select Endpoints 1 ~ 15, 82h) */
	}CtrlSts0_Union;

	union
    {
        __IO uint8_t CSR02;          /*!< Main Control Status register for Endpoint 0. (Index register set to select Endpoint 83h) */
        __IO uint8_t TxCSR2;         /*!< Control Status register 1 for peripheral IN endpoint. (Index register set to select Endpoints 1 ~ 15, 83h) */
    }CtrlSts1_Union;


	//3
	__IO uint16_t RxMaxP;		   /*!< Maximum packet size for peripheral OUT endpoint. (Index register set to select Endpoints 1 ~ 15 only) 84h */

	__IO uint8_t RxCSR1; 		  /*!< Control Status register 1 for peripheral OUT endpoint. (Index register set to select Endpoints 1 ~ 15 only) 86h */
	__IO uint8_t RxCSR2; 		  /*!< Control Status register 2 for peripheral OUT endpoint. (Index register set to select Endpoints 1 ~ 15 only) 87h */

	union
	{
		__IO uint8_t Count0;		 /*!< Number of received bytes in Endpoint 0 FIFO. (Index register set to select Endpoint 0), 88h */
		__IO uint8_t RxCount1;		 /*!< Number of bytes in peripheral OUT endpoint FIFO (lower byte). (Index register set to select Endpoints 1 ~ 15) 88h */
	}Count_Union;
	__IO uint8_t RxCount2;         /*!< Number of bytes in peripheral OUT endpoint FIFO (upper byte). (Index register set to select Endpoints 1 ~ 15 only) 89h*/

	__IO uint8_t rfu4[2];			  /*							  8Bh*/

	//4
	__IO uint8_t TxType;           /*!< TxType, only for host mode  8Ch*/

	union
    {
          __IO uint8_t NAKLmt0;      /*!< NAKLimit0, only for host mode 8Dh*/
          __IO uint8_t TxIntv;       /*!< RxIntv, only for host mode 8Dh*/
    }IntvNAKLmt0_Union;

	__IO uint8_t RxType;           /*!< RxType, only for host mode 8Eh*/
    __IO uint8_t RxIntv;           /*!< RxIntv, only for host mode 8Fh*/

	__IO uint8_t DyTxFIFOSize;        /*!< Dynamic FIFO for TxFIFO 1 0x90*/  //size
	__IO uint8_t rfu5[1];			  /*							91h*/
    __IO uint8_t DyTxFIFOAddr;        /*!< Dynamic FIFO for TxFIFO 2 0x92h*/ //addr
	__IO uint8_t rfu6[1];			  /*							93h*/
    __IO uint8_t DyRxFIFOSize;        /*!< Dynamic FIFO for RxFIFO 1 0x94*/  //size
	__IO uint8_t rfu7[1];			  /*							95h*/
    union
    {
        __IO uint8_t DyRxFIFOAddr;      /*!< Dynamic FIFO for RxFIFO 2 0x96*/ //addr
        __IO uint8_t FIFOSize;       /*!< Number of bytes in peripheral OUT endpoint FIFO (upper byte). (Index register set to select Endpoints 1~ 15 only) 96h*/
    }FIFO_Union;
	__IO uint8_t rfu8[1];			  /*							97h*/
    __IO uint8_t FAddr;               /*!< Function Address                        98h */
    __IO uint8_t rfu9[3];
    __IO uint8_t RAddr;
    __IO uint8_t rfu10[3];
} usb_musb_reg_t;

/** USBHS - Register Layout Typedef */
typedef struct {
		__I  uint16_t CAPLENGTH; // Capability register Length Register 0x000
		__I  uint16_t HCIVERSION; /**< Host Controller Interface Version and Capability Registers Length Register, offset: 0x002 */
		__I  uint32_t HCSPARAMS; /**< Host Controller Structural Parameters Register, offset: 0x004 */
		__I  uint32_t HCCPARAMS; /**< Host Controller Capability Parameters Register, offset: 0x008 */
		__I  uint32_t HCSPPORTROUTE;//Companion Port Route Description 0x00c

		__IO uint32_t USBCMD;                            /**< USB Command Register, offset: 0x010 */
		__IO uint32_t USBSTS;                            /**< USB Status Register, offset: 0x014 */
		__IO uint32_t USBINTR;                           /**< USB Interrupt Enable Register, offset: 0x018 */
		__IO uint32_t FRINDEX;                           /**< Frame Index Register, offset: 0x01C */

		__IO uint32_t CTRLDSSEGMENT;           //4G Segment Selector Register 0x020
		__IO uint32_t PERIODICLISTBASE;                  /**< Periodic Frame List Base Address Register, offset: 0x024 */
		__IO uint32_t ASYNCLISTADDR;                     /**< Current Asynchronous List Address Register, offset: 0x028 */

		__IO uint8_t rfu9[36];
		__I  uint32_t CONFIGFLAG;                        /**< Configure Flag Register, offset: 0x050 */
		__IO uint32_t PORTSC1;                           /**< Port Status and Control Registers, offset: 0x054 */

} usb_echi_reg_t;

//////EHCI
/**
 * @addtogroup USBHS_Register_Masks USBHS Register Masks
 * @{
 */

/* ID Bit Fields */
#define USBHS_ID_ID_MASK                         0x3Fu
#define USBHS_ID_ID_SHIFT                        0
#define USBHS_ID_ID(x)                           (((uint32_t)(((uint32_t)(x))<<USBHS_ID_ID_SHIFT))&USBHS_ID_ID_MASK)
#define USBHS_ID_NID_MASK                        0x3F00u
#define USBHS_ID_NID_SHIFT                       8
#define USBHS_ID_NID(x)                          (((uint32_t)(((uint32_t)(x))<<USBHS_ID_NID_SHIFT))&USBHS_ID_NID_MASK)
#define USBHS_ID_TAG_MASK                        0x1F0000u
#define USBHS_ID_TAG_SHIFT                       16
#define USBHS_ID_TAG(x)                          (((uint32_t)(((uint32_t)(x))<<USBHS_ID_TAG_SHIFT))&USBHS_ID_TAG_MASK)
#define USBHS_ID_REVISION_MASK                   0x1E00000u
#define USBHS_ID_REVISION_SHIFT                  21
#define USBHS_ID_REVISION(x)                     (((uint32_t)(((uint32_t)(x))<<USBHS_ID_REVISION_SHIFT))&USBHS_ID_REVISION_MASK)
#define USBHS_ID_VERSION_MASK                    0x1E000000u
#define USBHS_ID_VERSION_SHIFT                   25
#define USBHS_ID_VERSION(x)                      (((uint32_t)(((uint32_t)(x))<<USBHS_ID_VERSION_SHIFT))&USBHS_ID_VERSION_MASK)
#define USBHS_ID_VERSIONID_MASK                  0xE0000000u
#define USBHS_ID_VERSIONID_SHIFT                 29
#define USBHS_ID_VERSIONID(x)                    (((uint32_t)(((uint32_t)(x))<<USBHS_ID_VERSIONID_SHIFT))&USBHS_ID_VERSIONID_MASK)
/* HWGENERAL Bit Fields */
#define USBHS_HWGENERAL_PHYM_MASK                0x1C0u
#define USBHS_HWGENERAL_PHYM_SHIFT               6
#define USBHS_HWGENERAL_PHYM(x)                  (((uint32_t)(((uint32_t)(x))<<USBHS_HWGENERAL_PHYM_SHIFT))&USBHS_HWGENERAL_PHYM_MASK)
#define USBHS_HWGENERAL_SM_MASK                  0x600u
#define USBHS_HWGENERAL_SM_SHIFT                 9
#define USBHS_HWGENERAL_SM(x)                    (((uint32_t)(((uint32_t)(x))<<USBHS_HWGENERAL_SM_SHIFT))&USBHS_HWGENERAL_SM_MASK)
/* HWHOST Bit Fields */
#define USBHS_HWHOST_HC_MASK                     0x1u
#define USBHS_HWHOST_HC_SHIFT                    0
#define USBHS_HWHOST_NPORT_MASK                  0xEu
#define USBHS_HWHOST_NPORT_SHIFT                 1
#define USBHS_HWHOST_NPORT(x)                    (((uint32_t)(((uint32_t)(x))<<USBHS_HWHOST_NPORT_SHIFT))&USBHS_HWHOST_NPORT_MASK)
#define USBHS_HWHOST_TTASY_MASK                  0xFF0000u
#define USBHS_HWHOST_TTASY_SHIFT                 16
#define USBHS_HWHOST_TTASY(x)                    (((uint32_t)(((uint32_t)(x))<<USBHS_HWHOST_TTASY_SHIFT))&USBHS_HWHOST_TTASY_MASK)
#define USBHS_HWHOST_TTPER_MASK                  0xFF000000u
#define USBHS_HWHOST_TTPER_SHIFT                 24
#define USBHS_HWHOST_TTPER(x)                    (((uint32_t)(((uint32_t)(x))<<USBHS_HWHOST_TTPER_SHIFT))&USBHS_HWHOST_TTPER_MASK)
/* HWDEVICE Bit Fields */
#define USBHS_HWDEVICE_DC_MASK                   0x1u
#define USBHS_HWDEVICE_DC_SHIFT                  0
#define USBHS_HWDEVICE_DEVEP_MASK                0x3Eu
#define USBHS_HWDEVICE_DEVEP_SHIFT               1
#define USBHS_HWDEVICE_DEVEP(x)                  (((uint32_t)(((uint32_t)(x))<<USBHS_HWDEVICE_DEVEP_SHIFT))&USBHS_HWDEVICE_DEVEP_MASK)
/* HWTXBUF Bit Fields */
#define USBHS_HWTXBUF_TXBURST_MASK               0xFFu
#define USBHS_HWTXBUF_TXBURST_SHIFT              0
#define USBHS_HWTXBUF_TXBURST(x)                 (((uint32_t)(((uint32_t)(x))<<USBHS_HWTXBUF_TXBURST_SHIFT))&USBHS_HWTXBUF_TXBURST_MASK)
#define USBHS_HWTXBUF_TXADD_MASK                 0xFF00u
#define USBHS_HWTXBUF_TXADD_SHIFT                8
#define USBHS_HWTXBUF_TXADD(x)                   (((uint32_t)(((uint32_t)(x))<<USBHS_HWTXBUF_TXADD_SHIFT))&USBHS_HWTXBUF_TXADD_MASK)
#define USBHS_HWTXBUF_TXCHANADD_MASK             0xFF0000u
#define USBHS_HWTXBUF_TXCHANADD_SHIFT            16
#define USBHS_HWTXBUF_TXCHANADD(x)               (((uint32_t)(((uint32_t)(x))<<USBHS_HWTXBUF_TXCHANADD_SHIFT))&USBHS_HWTXBUF_TXCHANADD_MASK)
#define USBHS_HWTXBUF_TXLC_MASK                  0x80000000u
#define USBHS_HWTXBUF_TXLC_SHIFT                 31
/* HWRXBUF Bit Fields */
#define USBHS_HWRXBUF_RXBURST_MASK               0xFFu
#define USBHS_HWRXBUF_RXBURST_SHIFT              0
#define USBHS_HWRXBUF_RXBURST(x)                 (((uint32_t)(((uint32_t)(x))<<USBHS_HWRXBUF_RXBURST_SHIFT))&USBHS_HWRXBUF_RXBURST_MASK)
#define USBHS_HWRXBUF_RXADD_MASK                 0xFF00u
#define USBHS_HWRXBUF_RXADD_SHIFT                8
#define USBHS_HWRXBUF_RXADD(x)                   (((uint32_t)(((uint32_t)(x))<<USBHS_HWRXBUF_RXADD_SHIFT))&USBHS_HWRXBUF_RXADD_MASK)
/* GPTIMER0LD Bit Fields */
#define USBHS_GPTIMER0LD_GPTLD_MASK              0xFFFFFFu
#define USBHS_GPTIMER0LD_GPTLD_SHIFT             0
#define USBHS_GPTIMER0LD_GPTLD(x)                (((uint32_t)(((uint32_t)(x))<<USBHS_GPTIMER0LD_GPTLD_SHIFT))&USBHS_GPTIMER0LD_GPTLD_MASK)
/* GPTIMER0CTL Bit Fields */
#define USBHS_GPTIMER0CTL_GPTCNT_MASK            0xFFFFFFu
#define USBHS_GPTIMER0CTL_GPTCNT_SHIFT           0
#define USBHS_GPTIMER0CTL_GPTCNT(x)              (((uint32_t)(((uint32_t)(x))<<USBHS_GPTIMER0CTL_GPTCNT_SHIFT))&USBHS_GPTIMER0CTL_GPTCNT_MASK)
#define USBHS_GPTIMER0CTL_MODE_MASK              0x1000000u
#define USBHS_GPTIMER0CTL_MODE_SHIFT             24
#define USBHS_GPTIMER0CTL_RST_MASK               0x40000000u
#define USBHS_GPTIMER0CTL_RST_SHIFT              30
#define USBHS_GPTIMER0CTL_RUN_MASK               0x80000000u
#define USBHS_GPTIMER0CTL_RUN_SHIFT              31
/* GPTIMER1LD Bit Fields */
#define USBHS_GPTIMER1LD_GPTLD_MASK              0xFFFFFFu
#define USBHS_GPTIMER1LD_GPTLD_SHIFT             0
#define USBHS_GPTIMER1LD_GPTLD(x)                (((uint32_t)(((uint32_t)(x))<<USBHS_GPTIMER1LD_GPTLD_SHIFT))&USBHS_GPTIMER1LD_GPTLD_MASK)
/* GPTIMER1CTL Bit Fields */
#define USBHS_GPTIMER1CTL_GPTCNT_MASK            0xFFFFFFu
#define USBHS_GPTIMER1CTL_GPTCNT_SHIFT           0
#define USBHS_GPTIMER1CTL_GPTCNT(x)              (((uint32_t)(((uint32_t)(x))<<USBHS_GPTIMER1CTL_GPTCNT_SHIFT))&USBHS_GPTIMER1CTL_GPTCNT_MASK)
#define USBHS_GPTIMER1CTL_MODE_MASK              0x1000000u
#define USBHS_GPTIMER1CTL_MODE_SHIFT             24
#define USBHS_GPTIMER1CTL_RST_MASK               0x40000000u
#define USBHS_GPTIMER1CTL_RST_SHIFT              30
#define USBHS_GPTIMER1CTL_RUN_MASK               0x80000000u
#define USBHS_GPTIMER1CTL_RUN_SHIFT              31
/* USB_SBUSCFG Bit Fields */
#define USBHS_USB_SBUSCFG_BURSTMODE_MASK         0x7u
#define USBHS_USB_SBUSCFG_BURSTMODE_SHIFT        0
#define USBHS_USB_SBUSCFG_BURSTMODE(x)           (((uint32_t)(((uint32_t)(x))<<USBHS_USB_SBUSCFG_BURSTMODE_SHIFT))&USBHS_USB_SBUSCFG_BURSTMODE_MASK)
/* HCIVERSION Bit Fields */
#define USBHS_HCIVERSION_CAPLENGTH_MASK          0xFFu
#define USBHS_HCIVERSION_CAPLENGTH_SHIFT         0
#define USBHS_HCIVERSION_CAPLENGTH(x)            (((uint32_t)(((uint32_t)(x))<<USBHS_HCIVERSION_CAPLENGTH_SHIFT))&USBHS_HCIVERSION_CAPLENGTH_MASK)
#define USBHS_HCIVERSION_HCIVERSION_MASK         0xFFFF0000u
#define USBHS_HCIVERSION_HCIVERSION_SHIFT        16
#define USBHS_HCIVERSION_HCIVERSION(x)           (((uint32_t)(((uint32_t)(x))<<USBHS_HCIVERSION_HCIVERSION_SHIFT))&USBHS_HCIVERSION_HCIVERSION_MASK)
/* HCSPARAMS Bit Fields */
#define USBHS_HCSPARAMS_N_PORTS_MASK             0xFu
#define USBHS_HCSPARAMS_N_PORTS_SHIFT            0
#define USBHS_HCSPARAMS_N_PORTS(x)               (((uint32_t)(((uint32_t)(x))<<USBHS_HCSPARAMS_N_PORTS_SHIFT))&USBHS_HCSPARAMS_N_PORTS_MASK)
#define USBHS_HCSPARAMS_PPC_MASK                 0x10u
#define USBHS_HCSPARAMS_PPC_SHIFT                4
#define USBHS_HCSPARAMS_N_PCC_MASK               0xF00u
#define USBHS_HCSPARAMS_N_PCC_SHIFT              8
#define USBHS_HCSPARAMS_N_PCC(x)                 (((uint32_t)(((uint32_t)(x))<<USBHS_HCSPARAMS_N_PCC_SHIFT))&USBHS_HCSPARAMS_N_PCC_MASK)
#define USBHS_HCSPARAMS_N_CC_MASK                0xF000u
#define USBHS_HCSPARAMS_N_CC_SHIFT               12
#define USBHS_HCSPARAMS_N_CC(x)                  (((uint32_t)(((uint32_t)(x))<<USBHS_HCSPARAMS_N_CC_SHIFT))&USBHS_HCSPARAMS_N_CC_MASK)
#define USBHS_HCSPARAMS_PI_MASK                  0x10000u
#define USBHS_HCSPARAMS_PI_SHIFT                 16
#define USBHS_HCSPARAMS_N_PTT_MASK               0xF00000u
#define USBHS_HCSPARAMS_N_PTT_SHIFT              20
#define USBHS_HCSPARAMS_N_PTT(x)                 (((uint32_t)(((uint32_t)(x))<<USBHS_HCSPARAMS_N_PTT_SHIFT))&USBHS_HCSPARAMS_N_PTT_MASK)
#define USBHS_HCSPARAMS_N_TT_MASK                0xF000000u
#define USBHS_HCSPARAMS_N_TT_SHIFT               24
#define USBHS_HCSPARAMS_N_TT(x)                  (((uint32_t)(((uint32_t)(x))<<USBHS_HCSPARAMS_N_TT_SHIFT))&USBHS_HCSPARAMS_N_TT_MASK)
/* HCCPARAMS Bit Fields */
#define USBHS_HCCPARAMS_ADC_MASK                 0x1u
#define USBHS_HCCPARAMS_ADC_SHIFT                0
#define USBHS_HCCPARAMS_PFL_MASK                 0x2u
#define USBHS_HCCPARAMS_PFL_SHIFT                1
#define USBHS_HCCPARAMS_ASP_MASK                 0x4u
#define USBHS_HCCPARAMS_ASP_SHIFT                2
#define USBHS_HCCPARAMS_IST_MASK                 0xF0u
#define USBHS_HCCPARAMS_IST_SHIFT                4
#define USBHS_HCCPARAMS_IST(x)                   (((uint32_t)(((uint32_t)(x))<<USBHS_HCCPARAMS_IST_SHIFT))&USBHS_HCCPARAMS_IST_MASK)
#define USBHS_HCCPARAMS_EECP_MASK                0xFF00u
#define USBHS_HCCPARAMS_EECP_SHIFT               8
#define USBHS_HCCPARAMS_EECP(x)                  (((uint32_t)(((uint32_t)(x))<<USBHS_HCCPARAMS_EECP_SHIFT))&USBHS_HCCPARAMS_EECP_MASK)
/* DCIVERSION Bit Fields */
#define USBHS_DCIVERSION_DCIVERSION_MASK         0xFFFFu
#define USBHS_DCIVERSION_DCIVERSION_SHIFT        0
#define USBHS_DCIVERSION_DCIVERSION(x)           (((uint16_t)(((uint16_t)(x))<<USBHS_DCIVERSION_DCIVERSION_SHIFT))&USBHS_DCIVERSION_DCIVERSION_MASK)
/* DCCPARAMS Bit Fields */
#define USBHS_DCCPARAMS_DEN_MASK                 0x1Fu
#define USBHS_DCCPARAMS_DEN_SHIFT                0
#define USBHS_DCCPARAMS_DEN(x)                   (((uint32_t)(((uint32_t)(x))<<USBHS_DCCPARAMS_DEN_SHIFT))&USBHS_DCCPARAMS_DEN_MASK)
#define USBHS_DCCPARAMS_DC_MASK                  0x80u
#define USBHS_DCCPARAMS_DC_SHIFT                 7
#define USBHS_DCCPARAMS_HC_MASK                  0x100u
#define USBHS_DCCPARAMS_HC_SHIFT                 8
/* USBCMD Bit Fields */
#define USBHS_USBCMD_RS_MASK                     0x1u
#define USBHS_USBCMD_RS_SHIFT                    0
#define USBHS_USBCMD_RST_MASK                    0x2u
#define USBHS_USBCMD_RST_SHIFT                   1
#define USBHS_USBCMD_FS_MASK                     0xCu
#define USBHS_USBCMD_FS_SHIFT                    2
#define USBHS_USBCMD_FS(x)                       (((uint32_t)(((uint32_t)(x))<<USBHS_USBCMD_FS_SHIFT))&USBHS_USBCMD_FS_MASK)
#define USBHS_USBCMD_PSE_MASK                    0x10u
#define USBHS_USBCMD_PSE_SHIFT                   4
#define USBHS_USBCMD_ASE_MASK                    0x20u
#define USBHS_USBCMD_ASE_SHIFT                   5
#define USBHS_USBCMD_IAA_MASK                    0x40u
#define USBHS_USBCMD_IAA_SHIFT                   6
#define USBHS_USBCMD_ASP_MASK                    0x300u
#define USBHS_USBCMD_ASP_SHIFT                   8
#define USBHS_USBCMD_ASP(x)                      (((uint32_t)(((uint32_t)(x))<<USBHS_USBCMD_ASP_SHIFT))&USBHS_USBCMD_ASP_MASK)
#define USBHS_USBCMD_ASPE_MASK                   0x800u
#define USBHS_USBCMD_ASPE_SHIFT                  11
#define USBHS_USBCMD_SUTW_MASK                   0x2000u
#define USBHS_USBCMD_SUTW_SHIFT                  13
#define USBHS_USBCMD_ATDTW_MASK                  0x4000u
#define USBHS_USBCMD_ATDTW_SHIFT                 14
#define USBHS_USBCMD_FS2_MASK                    0x8000u
#define USBHS_USBCMD_FS2_SHIFT                   15
#define USBHS_USBCMD_ITC_MASK                    0xFF0000u
#define USBHS_USBCMD_ITC_SHIFT                   16
#define USBHS_USBCMD_ITC(x)                      (((uint32_t)(((uint32_t)(x))<<USBHS_USBCMD_ITC_SHIFT))&USBHS_USBCMD_ITC_MASK)
/* USBSTS Bit Fields */
#define USBHS_USBSTS_UI_MASK                     0x1u
#define USBHS_USBSTS_UI_SHIFT                    0
#define USBHS_USBSTS_UEI_MASK                    0x2u
#define USBHS_USBSTS_UEI_SHIFT                   1
#define USBHS_USBSTS_PCI_MASK                    0x4u
#define USBHS_USBSTS_PCI_SHIFT                   2
#define USBHS_USBSTS_FRI_MASK                    0x8u
#define USBHS_USBSTS_FRI_SHIFT                   3
#define USBHS_USBSTS_SEI_MASK                    0x10u
#define USBHS_USBSTS_SEI_SHIFT                   4
#define USBHS_USBSTS_AAI_MASK                    0x20u
#define USBHS_USBSTS_AAI_SHIFT                   5
#define USBHS_USBSTS_URI_MASK                    0x40u
#define USBHS_USBSTS_URI_SHIFT                   6
#define USBHS_USBSTS_SRI_MASK                    0x80u
#define USBHS_USBSTS_SRI_SHIFT                   7
#define USBHS_USBSTS_SLI_MASK                    0x100u
#define USBHS_USBSTS_SLI_SHIFT                   8
#define USBHS_USBSTS_ULPII_MASK                  0x400u
#define USBHS_USBSTS_ULPII_SHIFT                 10
#define USBHS_USBSTS_HCH_MASK                    0x1000u
#define USBHS_USBSTS_HCH_SHIFT                   12
#define USBHS_USBSTS_RCL_MASK                    0x2000u
#define USBHS_USBSTS_RCL_SHIFT                   13
#define USBHS_USBSTS_PS_MASK                     0x4000u
#define USBHS_USBSTS_PS_SHIFT                    14
#define USBHS_USBSTS_AS_MASK                     0x8000u
#define USBHS_USBSTS_AS_SHIFT                    15
#define USBHS_USBSTS_NAKI_MASK                   0x10000u
#define USBHS_USBSTS_NAKI_SHIFT                  16
#define USBHS_USBSTS_UAI_MASK                    0x40000u
#define USBHS_USBSTS_UAI_SHIFT                   18
#define USBHS_USBSTS_UPI_MASK                    0x80000u
#define USBHS_USBSTS_UPI_SHIFT                   19
#define USBHS_USBSTS_TI0_MASK                    0x1000000u
#define USBHS_USBSTS_TI0_SHIFT                   24
#define USBHS_USBSTS_TI1_MASK                    0x2000000u
#define USBHS_USBSTS_TI1_SHIFT                   25
/* USBINTR Bit Fields */
#define USBHS_USBINTR_UE_MASK                    0x1u
#define USBHS_USBINTR_UE_SHIFT                   0
#define USBHS_USBINTR_UEE_MASK                   0x2u
#define USBHS_USBINTR_UEE_SHIFT                  1
#define USBHS_USBINTR_PCE_MASK                   0x4u
#define USBHS_USBINTR_PCE_SHIFT                  2
#define USBHS_USBINTR_FRE_MASK                   0x8u
#define USBHS_USBINTR_FRE_SHIFT                  3
#define USBHS_USBINTR_SEE_MASK                   0x10u
#define USBHS_USBINTR_SEE_SHIFT                  4
#define USBHS_USBINTR_AAE_MASK                   0x20u
#define USBHS_USBINTR_AAE_SHIFT                  5
#define USBHS_USBINTR_URE_MASK                   0x40u
#define USBHS_USBINTR_URE_SHIFT                  6
#define USBHS_USBINTR_SRE_MASK                   0x80u
#define USBHS_USBINTR_SRE_SHIFT                  7
#define USBHS_USBINTR_SLE_MASK                   0x100u
#define USBHS_USBINTR_SLE_SHIFT                  8
#define USBHS_USBINTR_ULPIE_MASK                 0x400u
#define USBHS_USBINTR_ULPIE_SHIFT                10
#define USBHS_USBINTR_NAKE_MASK                  0x10000u
#define USBHS_USBINTR_NAKE_SHIFT                 16
#define USBHS_USBINTR_UAIE_MASK                  0x40000u
#define USBHS_USBINTR_UAIE_SHIFT                 18
#define USBHS_USBINTR_UPIE_MASK                  0x80000u
#define USBHS_USBINTR_UPIE_SHIFT                 19
#define USBHS_USBINTR_TIE0_MASK                  0x1000000u
#define USBHS_USBINTR_TIE0_SHIFT                 24
#define USBHS_USBINTR_TIE1_MASK                  0x2000000u
#define USBHS_USBINTR_TIE1_SHIFT                 25
/* FRINDEX Bit Fields */
#define USBHS_FRINDEX_FRINDEX_MASK               0x3FFFu
#define USBHS_FRINDEX_FRINDEX_SHIFT              0
#define USBHS_FRINDEX_FRINDEX(x)                 (((uint32_t)(((uint32_t)(x))<<USBHS_FRINDEX_FRINDEX_SHIFT))&USBHS_FRINDEX_FRINDEX_MASK)
#define USBHS_FRINDEX_Reerved_MASK               0xFFFFC000u
#define USBHS_FRINDEX_Reerved_SHIFT              14
#define USBHS_FRINDEX_Reerved(x)                 (((uint32_t)(((uint32_t)(x))<<USBHS_FRINDEX_Reerved_SHIFT))&USBHS_FRINDEX_Reerved_MASK)
/* DEVICEADDR Bit Fields */
#define USBHS_DEVICEADDR_USBADRA_MASK            0x1000000u
#define USBHS_DEVICEADDR_USBADRA_SHIFT           24
#define USBHS_DEVICEADDR_USBADR_MASK             0xFE000000u
#define USBHS_DEVICEADDR_USBADR_SHIFT            25
#define USBHS_DEVICEADDR_USBADR(x)               (((uint32_t)(((uint32_t)(x))<<USBHS_DEVICEADDR_USBADR_SHIFT))&USBHS_DEVICEADDR_USBADR_MASK)
/* PERIODICLISTBASE Bit Fields */
#define USBHS_PERIODICLISTBASE_PERBASE_MASK      0xFFFFF000u
#define USBHS_PERIODICLISTBASE_PERBASE_SHIFT     12
#define USBHS_PERIODICLISTBASE_PERBASE(x)        (((uint32_t)(((uint32_t)(x))<<USBHS_PERIODICLISTBASE_PERBASE_SHIFT))&USBHS_PERIODICLISTBASE_PERBASE_MASK)
/* ASYNCLISTADDR Bit Fields */
#define USBHS_ASYNCLISTADDR_ASYBASE_MASK         0xFFFFFFE0u
#define USBHS_ASYNCLISTADDR_ASYBASE_SHIFT        5
#define USBHS_ASYNCLISTADDR_ASYBASE(x)           (((uint32_t)(((uint32_t)(x))<<USBHS_ASYNCLISTADDR_ASYBASE_SHIFT))&USBHS_ASYNCLISTADDR_ASYBASE_MASK)
/* EPLISTADDR Bit Fields */
#define USBHS_EPLISTADDR_EPBASE_MASK             0xFFFFF800u
#define USBHS_EPLISTADDR_EPBASE_SHIFT            11
#define USBHS_EPLISTADDR_EPBASE(x)               (((uint32_t)(((uint32_t)(x))<<USBHS_EPLISTADDR_EPBASE_SHIFT))&USBHS_EPLISTADDR_EPBASE_MASK)
/* TTCTRL Bit Fields */
#define USBHS_TTCTRL_TTHA_MASK                   0x7F000000u
#define USBHS_TTCTRL_TTHA_SHIFT                  24
#define USBHS_TTCTRL_TTHA(x)                     (((uint32_t)(((uint32_t)(x))<<USBHS_TTCTRL_TTHA_SHIFT))&USBHS_TTCTRL_TTHA_MASK)
#define USBHS_TTCTRL_Reerved_MASK                0x80000000u
#define USBHS_TTCTRL_Reerved_SHIFT               31
/* BURSTSIZE Bit Fields */
#define USBHS_BURSTSIZE_RXPBURST_MASK            0xFFu
#define USBHS_BURSTSIZE_RXPBURST_SHIFT           0
#define USBHS_BURSTSIZE_RXPBURST(x)              (((uint32_t)(((uint32_t)(x))<<USBHS_BURSTSIZE_RXPBURST_SHIFT))&USBHS_BURSTSIZE_RXPBURST_MASK)
#define USBHS_BURSTSIZE_TXPBURST_MASK            0xFF00u
#define USBHS_BURSTSIZE_TXPBURST_SHIFT           8
#define USBHS_BURSTSIZE_TXPBURST(x)              (((uint32_t)(((uint32_t)(x))<<USBHS_BURSTSIZE_TXPBURST_SHIFT))&USBHS_BURSTSIZE_TXPBURST_MASK)
/* TXFILLTUNING Bit Fields */
#define USBHS_TXFILLTUNING_TXSCHOH_MASK          0x7Fu
#define USBHS_TXFILLTUNING_TXSCHOH_SHIFT         0
#define USBHS_TXFILLTUNING_TXSCHOH(x)            (((uint32_t)(((uint32_t)(x))<<USBHS_TXFILLTUNING_TXSCHOH_SHIFT))&USBHS_TXFILLTUNING_TXSCHOH_MASK)
#define USBHS_TXFILLTUNING_TXSCHHEALTH_MASK      0x1F00u
#define USBHS_TXFILLTUNING_TXSCHHEALTH_SHIFT     8
#define USBHS_TXFILLTUNING_TXSCHHEALTH(x)        (((uint32_t)(((uint32_t)(x))<<USBHS_TXFILLTUNING_TXSCHHEALTH_SHIFT))&USBHS_TXFILLTUNING_TXSCHHEALTH_MASK)
#define USBHS_TXFILLTUNING_TXFIFOTHRES_MASK      0x3F0000u
#define USBHS_TXFILLTUNING_TXFIFOTHRES_SHIFT     16
#define USBHS_TXFILLTUNING_TXFIFOTHRES(x)        (((uint32_t)(((uint32_t)(x))<<USBHS_TXFILLTUNING_TXFIFOTHRES_SHIFT))&USBHS_TXFILLTUNING_TXFIFOTHRES_MASK)
/* ULPI_VIEWPORT Bit Fields */
#define USBHS_ULPI_VIEWPORT_ULPI_DATWR_MASK      0xFFu
#define USBHS_ULPI_VIEWPORT_ULPI_DATWR_SHIFT     0
#define USBHS_ULPI_VIEWPORT_ULPI_DATWR(x)        (((uint32_t)(((uint32_t)(x))<<USBHS_ULPI_VIEWPORT_ULPI_DATWR_SHIFT))&USBHS_ULPI_VIEWPORT_ULPI_DATWR_MASK)
#define USBHS_ULPI_VIEWPORT_ULPI_DATRD_MASK      0xFF00u
#define USBHS_ULPI_VIEWPORT_ULPI_DATRD_SHIFT     8
#define USBHS_ULPI_VIEWPORT_ULPI_DATRD(x)        (((uint32_t)(((uint32_t)(x))<<USBHS_ULPI_VIEWPORT_ULPI_DATRD_SHIFT))&USBHS_ULPI_VIEWPORT_ULPI_DATRD_MASK)
#define USBHS_ULPI_VIEWPORT_ULPI_ADDR_MASK       0xFF0000u
#define USBHS_ULPI_VIEWPORT_ULPI_ADDR_SHIFT      16
#define USBHS_ULPI_VIEWPORT_ULPI_ADDR(x)         (((uint32_t)(((uint32_t)(x))<<USBHS_ULPI_VIEWPORT_ULPI_ADDR_SHIFT))&USBHS_ULPI_VIEWPORT_ULPI_ADDR_MASK)
#define USBHS_ULPI_VIEWPORT_ULPI_PORT_MASK       0x7000000u
#define USBHS_ULPI_VIEWPORT_ULPI_PORT_SHIFT      24
#define USBHS_ULPI_VIEWPORT_ULPI_PORT(x)         (((uint32_t)(((uint32_t)(x))<<USBHS_ULPI_VIEWPORT_ULPI_PORT_SHIFT))&USBHS_ULPI_VIEWPORT_ULPI_PORT_MASK)
#define USBHS_ULPI_VIEWPORT_ULPI_SS_MASK         0x8000000u
#define USBHS_ULPI_VIEWPORT_ULPI_SS_SHIFT        27
#define USBHS_ULPI_VIEWPORT_ULPI_RW_MASK         0x20000000u
#define USBHS_ULPI_VIEWPORT_ULPI_RW_SHIFT        29
#define USBHS_ULPI_VIEWPORT_ULPI_RUN_MASK        0x40000000u
#define USBHS_ULPI_VIEWPORT_ULPI_RUN_SHIFT       30
#define USBHS_ULPI_VIEWPORT_ULPI_WU_MASK         0x80000000u
#define USBHS_ULPI_VIEWPORT_ULPI_WU_SHIFT        31
/* ENDPTNAK Bit Fields */
#define USBHS_ENDPTNAK_EPRN_MASK                 0xFu
#define USBHS_ENDPTNAK_EPRN_SHIFT                0
#define USBHS_ENDPTNAK_EPRN(x)                   (((uint32_t)(((uint32_t)(x))<<USBHS_ENDPTNAK_EPRN_SHIFT))&USBHS_ENDPTNAK_EPRN_MASK)
#define USBHS_ENDPTNAK_EPTN_MASK                 0xF0000u
#define USBHS_ENDPTNAK_EPTN_SHIFT                16
#define USBHS_ENDPTNAK_EPTN(x)                   (((uint32_t)(((uint32_t)(x))<<USBHS_ENDPTNAK_EPTN_SHIFT))&USBHS_ENDPTNAK_EPTN_MASK)
/* ENDPTNAKEN Bit Fields */
#define USBHS_ENDPTNAKEN_EPRNE_MASK              0xFu
#define USBHS_ENDPTNAKEN_EPRNE_SHIFT             0
#define USBHS_ENDPTNAKEN_EPRNE(x)                (((uint32_t)(((uint32_t)(x))<<USBHS_ENDPTNAKEN_EPRNE_SHIFT))&USBHS_ENDPTNAKEN_EPRNE_MASK)
#define USBHS_ENDPTNAKEN_EPTNE_MASK              0xF0000u
#define USBHS_ENDPTNAKEN_EPTNE_SHIFT             16
#define USBHS_ENDPTNAKEN_EPTNE(x)                (((uint32_t)(((uint32_t)(x))<<USBHS_ENDPTNAKEN_EPTNE_SHIFT))&USBHS_ENDPTNAKEN_EPTNE_MASK)
/* PORTSC1 Bit Fields */
#define USBHS_PORTSC1_CCS_MASK                   0x1u
#define USBHS_PORTSC1_CCS_SHIFT                  0
#define USBHS_PORTSC1_CSC_MASK                   0x2u
#define USBHS_PORTSC1_CSC_SHIFT                  1
#define USBHS_PORTSC1_PE_MASK                    0x4u
#define USBHS_PORTSC1_PE_SHIFT                   2
#define USBHS_PORTSC1_PEC_MASK                   0x8u
#define USBHS_PORTSC1_PEC_SHIFT                  3
#define USBHS_PORTSC1_OCA_MASK                   0x10u
#define USBHS_PORTSC1_OCA_SHIFT                  4
#define USBHS_PORTSC1_OCC_MASK                   0x20u
#define USBHS_PORTSC1_OCC_SHIFT                  5
#define USBHS_PORTSC1_FPR_MASK                   0x40u
#define USBHS_PORTSC1_FPR_SHIFT                  6
#define USBHS_PORTSC1_SUSP_MASK                  0x80u
#define USBHS_PORTSC1_SUSP_SHIFT                 7
#define USBHS_PORTSC1_PR_MASK                    0x100u
#define USBHS_PORTSC1_PR_SHIFT                   8
#define USBHS_PORTSC1_HSP_MASK                   0x200u
#define USBHS_PORTSC1_HSP_SHIFT                  9
#define USBHS_PORTSC1_LS_MASK                    0xC00u
#define USBHS_PORTSC1_LS_SHIFT                   10
#define USBHS_PORTSC1_LS(x)                      (((uint32_t)(((uint32_t)(x))<<USBHS_PORTSC1_LS_SHIFT))&USBHS_PORTSC1_LS_MASK)
#define USBHS_PORTSC1_PP_MASK                    0x1000u
#define USBHS_PORTSC1_PP_SHIFT                   12
#define USBHS_PORTSC1_PO_MASK                    0x2000u
#define USBHS_PORTSC1_PO_SHIFT                   13
#define USBHS_PORTSC1_PIC_MASK                   0xC000u
#define USBHS_PORTSC1_PIC_SHIFT                  14
#define USBHS_PORTSC1_PIC(x)                     (((uint32_t)(((uint32_t)(x))<<USBHS_PORTSC1_PIC_SHIFT))&USBHS_PORTSC1_PIC_MASK)
#define USBHS_PORTSC1_PTC_MASK                   0xF0000u
#define USBHS_PORTSC1_PTC_SHIFT                  16
#define USBHS_PORTSC1_PTC(x)                     (((uint32_t)(((uint32_t)(x))<<USBHS_PORTSC1_PTC_SHIFT))&USBHS_PORTSC1_PTC_MASK)
#define USBHS_PORTSC1_WKCN_MASK                  0x100000u
#define USBHS_PORTSC1_WKCN_SHIFT                 20
#define USBHS_PORTSC1_WKDS_MASK                  0x200000u
#define USBHS_PORTSC1_WKDS_SHIFT                 21
#define USBHS_PORTSC1_WKOC_MASK                  0x400000u
#define USBHS_PORTSC1_WKOC_SHIFT                 22
#define USBHS_PORTSC1_PHCD_MASK                  0x800000u
#define USBHS_PORTSC1_PHCD_SHIFT                 23
#define USBHS_PORTSC1_PFSC_MASK                  0x1000000u
#define USBHS_PORTSC1_PFSC_SHIFT                 24
#define USBHS_PORTSC1_PSPD_MASK                  0xC000000u
#define USBHS_PORTSC1_PSPD_SHIFT                 26
#define USBHS_PORTSC1_PSPD(x)                    (((uint32_t)(((uint32_t)(x))<<USBHS_PORTSC1_PSPD_SHIFT))&USBHS_PORTSC1_PSPD_MASK)
#define USBHS_PORTSC1_PTS_MASK                   0xC0000000u
#define USBHS_PORTSC1_PTS_SHIFT                  30
#define USBHS_PORTSC1_PTS(x)                     (((uint32_t)(((uint32_t)(x))<<USBHS_PORTSC1_PTS_SHIFT))&USBHS_PORTSC1_PTS_MASK)
/* OTGSC Bit Fields */
#define USBHS_OTGSC_VD_MASK                      0x1u
#define USBHS_OTGSC_VD_SHIFT                     0
#define USBHS_OTGSC_VC_MASK                      0x2u
#define USBHS_OTGSC_VC_SHIFT                     1
#define USBHS_OTGSC_HAAR_MASK                    0x4u
#define USBHS_OTGSC_HAAR_SHIFT                   2
#define USBHS_OTGSC_OT_MASK                      0x8u
#define USBHS_OTGSC_OT_SHIFT                     3
#define USBHS_OTGSC_DP_MASK                      0x10u
#define USBHS_OTGSC_DP_SHIFT                     4
#define USBHS_OTGSC_IDPU_MASK                    0x20u
#define USBHS_OTGSC_IDPU_SHIFT                   5
#define USBHS_OTGSC_HABA_MASK                    0x80u
#define USBHS_OTGSC_HABA_SHIFT                   7
#define USBHS_OTGSC_ID_MASK                      0x100u
#define USBHS_OTGSC_ID_SHIFT                     8
#define USBHS_OTGSC_AVV_MASK                     0x200u
#define USBHS_OTGSC_AVV_SHIFT                    9
#define USBHS_OTGSC_ASV_MASK                     0x400u
#define USBHS_OTGSC_ASV_SHIFT                    10
#define USBHS_OTGSC_BSV_MASK                     0x800u
#define USBHS_OTGSC_BSV_SHIFT                    11
#define USBHS_OTGSC_BSE_MASK                     0x1000u
#define USBHS_OTGSC_BSE_SHIFT                    12
#define USBHS_OTGSC_MST_MASK                     0x2000u
#define USBHS_OTGSC_MST_SHIFT                    13
#define USBHS_OTGSC_DPS_MASK                     0x4000u
#define USBHS_OTGSC_DPS_SHIFT                    14
#define USBHS_OTGSC_IDIS_MASK                    0x10000u
#define USBHS_OTGSC_IDIS_SHIFT                   16
#define USBHS_OTGSC_AVVIS_MASK                   0x20000u
#define USBHS_OTGSC_AVVIS_SHIFT                  17
#define USBHS_OTGSC_ASVIS_MASK                   0x40000u
#define USBHS_OTGSC_ASVIS_SHIFT                  18
#define USBHS_OTGSC_BSVIS_MASK                   0x80000u
#define USBHS_OTGSC_BSVIS_SHIFT                  19
#define USBHS_OTGSC_BSEIS_MASK                   0x100000u
#define USBHS_OTGSC_BSEIS_SHIFT                  20
#define USBHS_OTGSC_MSS_MASK                     0x200000u
#define USBHS_OTGSC_MSS_SHIFT                    21
#define USBHS_OTGSC_DPIS_MASK                    0x400000u
#define USBHS_OTGSC_DPIS_SHIFT                   22
#define USBHS_OTGSC_IDIE_MASK                    0x1000000u
#define USBHS_OTGSC_IDIE_SHIFT                   24
#define USBHS_OTGSC_AVVIE_MASK                   0x2000000u
#define USBHS_OTGSC_AVVIE_SHIFT                  25
#define USBHS_OTGSC_ASVIE_MASK                   0x4000000u
#define USBHS_OTGSC_ASVIE_SHIFT                  26
#define USBHS_OTGSC_BSVIE_MASK                   0x8000000u
#define USBHS_OTGSC_BSVIE_SHIFT                  27
#define USBHS_OTGSC_BSEIE_MASK                   0x10000000u
#define USBHS_OTGSC_BSEIE_SHIFT                  28
#define USBHS_OTGSC_MSE_MASK                     0x20000000u
#define USBHS_OTGSC_MSE_SHIFT                    29
#define USBHS_OTGSC_DPIE_MASK                    0x40000000u
#define USBHS_OTGSC_DPIE_SHIFT                   30
/* USBMODE Bit Fields */
#define USBHS_USBMODE_CM_MASK                    0x3u
#define USBHS_USBMODE_CM_SHIFT                   0
#define USBHS_USBMODE_CM(x)                      (((uint32_t)(((uint32_t)(x))<<USBHS_USBMODE_CM_SHIFT))&USBHS_USBMODE_CM_MASK)
#define USBHS_USBMODE_ES_MASK                    0x4u
#define USBHS_USBMODE_ES_SHIFT                   2
#define USBHS_USBMODE_SLOM_MASK                  0x8u
#define USBHS_USBMODE_SLOM_SHIFT                 3
#define USBHS_USBMODE_SDIS_MASK                  0x10u
#define USBHS_USBMODE_SDIS_SHIFT                 4
#define USBHS_USBMODE_TXHSD_MASK                 0x7000u
#define USBHS_USBMODE_TXHSD_SHIFT                12
#define USBHS_USBMODE_TXHSD(x)                   (((uint32_t)(((uint32_t)(x))<<USBHS_USBMODE_TXHSD_SHIFT))&USBHS_USBMODE_TXHSD_MASK)
/* EPSETUPSR Bit Fields */
#define USBHS_EPSETUPSR_EPSETUPSTAT_MASK         0xFu
#define USBHS_EPSETUPSR_EPSETUPSTAT_SHIFT        0
#define USBHS_EPSETUPSR_EPSETUPSTAT(x)           (((uint32_t)(((uint32_t)(x))<<USBHS_EPSETUPSR_EPSETUPSTAT_SHIFT))&USBHS_EPSETUPSR_EPSETUPSTAT_MASK)
/* EPPRIME Bit Fields */
#define USBHS_EPPRIME_PERB_MASK                  0xFu
#define USBHS_EPPRIME_PERB_SHIFT                 0
#define USBHS_EPPRIME_PERB(x)                    (((uint32_t)(((uint32_t)(x))<<USBHS_EPPRIME_PERB_SHIFT))&USBHS_EPPRIME_PERB_MASK)
#define USBHS_EPPRIME_PETB_MASK                  0xF0000u
#define USBHS_EPPRIME_PETB_SHIFT                 16
#define USBHS_EPPRIME_PETB(x)                    (((uint32_t)(((uint32_t)(x))<<USBHS_EPPRIME_PETB_SHIFT))&USBHS_EPPRIME_PETB_MASK)
/* EPFLUSH Bit Fields */
#define USBHS_EPFLUSH_FERB_MASK                  0xFu
#define USBHS_EPFLUSH_FERB_SHIFT                 0
#define USBHS_EPFLUSH_FERB(x)                    (((uint32_t)(((uint32_t)(x))<<USBHS_EPFLUSH_FERB_SHIFT))&USBHS_EPFLUSH_FERB_MASK)
#define USBHS_EPFLUSH_FETB_MASK                  0xF0000u
#define USBHS_EPFLUSH_FETB_SHIFT                 16
#define USBHS_EPFLUSH_FETB(x)                    (((uint32_t)(((uint32_t)(x))<<USBHS_EPFLUSH_FETB_SHIFT))&USBHS_EPFLUSH_FETB_MASK)
/* EPSR Bit Fields */
#define USBHS_EPSR_ERBR_MASK                     0xFu
#define USBHS_EPSR_ERBR_SHIFT                    0
#define USBHS_EPSR_ERBR(x)                       (((uint32_t)(((uint32_t)(x))<<USBHS_EPSR_ERBR_SHIFT))&USBHS_EPSR_ERBR_MASK)
#define USBHS_EPSR_ETBR_MASK                     0xF0000u
#define USBHS_EPSR_ETBR_SHIFT                    16
#define USBHS_EPSR_ETBR(x)                       (((uint32_t)(((uint32_t)(x))<<USBHS_EPSR_ETBR_SHIFT))&USBHS_EPSR_ETBR_MASK)
/* EPCOMPLETE Bit Fields */
#define USBHS_EPCOMPLETE_ERCE_MASK               0xFu
#define USBHS_EPCOMPLETE_ERCE_SHIFT              0
#define USBHS_EPCOMPLETE_ERCE(x)                 (((uint32_t)(((uint32_t)(x))<<USBHS_EPCOMPLETE_ERCE_SHIFT))&USBHS_EPCOMPLETE_ERCE_MASK)
#define USBHS_EPCOMPLETE_ETCE_MASK               0xF0000u
#define USBHS_EPCOMPLETE_ETCE_SHIFT              16
#define USBHS_EPCOMPLETE_ETCE(x)                 (((uint32_t)(((uint32_t)(x))<<USBHS_EPCOMPLETE_ETCE_SHIFT))&USBHS_EPCOMPLETE_ETCE_MASK)
/* EPCR0 Bit Fields */
#define USBHS_EPCR0_RXS_MASK                     0x1u
#define USBHS_EPCR0_RXS_SHIFT                    0
#define USBHS_EPCR0_RXT_MASK                     0xCu
#define USBHS_EPCR0_RXT_SHIFT                    2
#define USBHS_EPCR0_RXT(x)                       (((uint32_t)(((uint32_t)(x))<<USBHS_EPCR0_RXT_SHIFT))&USBHS_EPCR0_RXT_MASK)
#define USBHS_EPCR0_RXE_MASK                     0x80u
#define USBHS_EPCR0_RXE_SHIFT                    7
#define USBHS_EPCR0_TXS_MASK                     0x10000u
#define USBHS_EPCR0_TXS_SHIFT                    16
#define USBHS_EPCR0_TXT_MASK                     0xC0000u
#define USBHS_EPCR0_TXT_SHIFT                    18
#define USBHS_EPCR0_TXT(x)                       (((uint32_t)(((uint32_t)(x))<<USBHS_EPCR0_TXT_SHIFT))&USBHS_EPCR0_TXT_MASK)
#define USBHS_EPCR0_TXE_MASK                     0x800000u
#define USBHS_EPCR0_TXE_SHIFT                    23
/* EPCR Bit Fields */
#define USBHS_EPCR_RXS_MASK                      0x1u
#define USBHS_EPCR_RXS_SHIFT                     0
#define USBHS_EPCR_RXD_MASK                      0x2u
#define USBHS_EPCR_RXD_SHIFT                     1
#define USBHS_EPCR_RXT_MASK                      0xCu
#define USBHS_EPCR_RXT_SHIFT                     2
#define USBHS_EPCR_RXT(x)                        (((uint32_t)(((uint32_t)(x))<<USBHS_EPCR_RXT_SHIFT))&USBHS_EPCR_RXT_MASK)
#define USBHS_EPCR_RXI_MASK                      0x20u
#define USBHS_EPCR_RXI_SHIFT                     5
#define USBHS_EPCR_RXR_MASK                      0x40u
#define USBHS_EPCR_RXR_SHIFT                     6
#define USBHS_EPCR_RXE_MASK                      0x80u
#define USBHS_EPCR_RXE_SHIFT                     7
#define USBHS_EPCR_TXS_MASK                      0x10000u
#define USBHS_EPCR_TXS_SHIFT                     16
#define USBHS_EPCR_TXD_MASK                      0x20000u
#define USBHS_EPCR_TXD_SHIFT                     17
#define USBHS_EPCR_TXT_MASK                      0xC0000u
#define USBHS_EPCR_TXT_SHIFT                     18
#define USBHS_EPCR_TXT(x)                        (((uint32_t)(((uint32_t)(x))<<USBHS_EPCR_TXT_SHIFT))&USBHS_EPCR_TXT_MASK)
#define USBHS_EPCR_TXI_MASK                      0x200000u
#define USBHS_EPCR_TXI_SHIFT                     21
#define USBHS_EPCR_TXR_MASK                      0x400000u
#define USBHS_EPCR_TXR_SHIFT                     22
#define USBHS_EPCR_TXE_MASK                      0x800000u
#define USBHS_EPCR_TXE_SHIFT                     23
/* USBGENCTRL Bit Fields */
#define USBHS_USBGENCTRL_WU_IE_MASK              0x1u
#define USBHS_USBGENCTRL_WU_IE_SHIFT             0
#define USBHS_USBGENCTRL_WU_ULPI_EN_MASK         0x2u
#define USBHS_USBGENCTRL_WU_ULPI_EN_SHIFT        1
#define USBHS_USBGENCTRL_WU_INT_CLR_MASK         0x20u
#define USBHS_USBGENCTRL_WU_INT_CLR_SHIFT        5



/**
 * @brief  Get USB periphral general interrupt status
 *
 * @param  pUSB_Otg : USB OTG base address
 *
 * @retval None
 */
static inline uint8_t HAL_USB_GetDeviceIntrStatus(usb_musb_reg_t *pUSB_Otg)
{
    uint8_t val = 0;
    val = pUSB_Otg->IntrUSB;
    pUSB_Otg->IntrUSB = val; //clear
    return val;
}

/**
 * @brief  Set USB periphral Address register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  address : USB address assigned by USB host
 *
 * @retval None
 */
static inline void HAL_USB_SetAddress(usb_musb_reg_t *pUSB_Otg, uint8_t address)
{
    pUSB_Otg->FAddr = address;
}

static inline void HAL_USB_SetInReqAddress(usb_musb_reg_t *pUSB_Otg, uint8_t address)
{
    pUSB_Otg->RAddr = address;
}

/**
 * @brief  Clear USB periphral TxCSR1 register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  bitfield : the bitfield for TxCSR1 register
 *
 * @retval None
 */
static inline void HAL_USB_ClearTxCSR1(usb_musb_reg_t *pUSB_Otg, uint8_t bitfield)
{
  	pUSB_Otg->CtrlSts0_Union.TxCSR1 &= ~(bitfield);
}
static inline void HAL_USB_SetAll_TxCSR1(usb_musb_reg_t *pUSB_Otg, uint8_t dat)
{
  	pUSB_Otg->CtrlSts0_Union.TxCSR1 = dat;
}
static inline void HAL_USB_SetAll_TxCSR2(usb_musb_reg_t *pUSB_Otg, uint8_t dat)
{
  	pUSB_Otg->CtrlSts1_Union.TxCSR2 = dat;
}
/**
 * @brief  Set USB periphral TxCSR1 register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  bitfield : the bitfield for TxCSR1 register
 *
 * @retval None
 */
static inline void HAL_USB_SetTxCSR1(usb_musb_reg_t *pUSB_Otg, uint8_t bitfield)
{
	pUSB_Otg->CtrlSts0_Union.TxCSR1 |= (bitfield);
}

/**
 * @brief  Clear USB periphral TxCSR2 register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  bitfield : the bitfield for TxCSR2 register
 *
 * @retval None
 */
static inline void HAL_USB_ClearTxCSR2(usb_musb_reg_t *pUSB_Otg, uint8_t bitfield)
{
	pUSB_Otg->CtrlSts1_Union.TxCSR2 &= ~(bitfield);
}

/**
 * @brief  Set USB periphral TxCSR2 register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  bitfield : the bitfield for TxCSR2 register
 *
 * @retval None
 */
static inline void HAL_USB_SetTxCSR2(usb_musb_reg_t *pUSB_Otg, uint8_t bitfield)
{
	pUSB_Otg->CtrlSts1_Union.TxCSR2 |= (bitfield);
}

/**
 * @brief  Clear USB periphral Power register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  bitfield : the bitfield for Power register
 *
 * @retval None
 */
static inline void HAL_USB_ClearPower(usb_musb_reg_t *pUSB_Otg, uint8_t bitfield)
{
	pUSB_Otg->Power &= ~(bitfield);
}

/**
 * @brief  Set USB periphral Power register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  bitfield : the bitfield for Power register
 *
 * @retval None
 */
static inline void HAL_USB_SetPower(usb_musb_reg_t *pUSB_Otg, uint8_t bitfield)
{
	pUSB_Otg->Power |= (bitfield);

}

static inline int HAL_USB_PowerIsHs(usb_musb_reg_t *pUSB_Otg)
{
	return (pUSB_Otg->Power & USB_OTG_POWER_READHS);

}

/**
 * @brief  Clear USB periphral CSR0 register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  bitfield : the bitfield for CSR0 register
 *
 * @retval None
 */
static inline void HAL_USB_ClearCSR0(usb_musb_reg_t *pUSB_Otg, uint8_t bitfield)
{
	pUSB_Otg->CtrlSts0_Union.CSR0 &= ~(bitfield);
}

/**
 * @brief  Set USB periphral CSR0 register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  bitfield : the bitfield for CSR0 register
 *
 * @retval None
 */
static inline void HAL_USB_SetCSR0(usb_musb_reg_t *pUSB_Otg, uint8_t bitfield)
{
	pUSB_Otg->CtrlSts0_Union.CSR0 |= (bitfield);
}

/**
 * @brief  Set USB periphral RxType register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  rxIntv : RxType value
 *
 * @retval None
 */
static inline void HAL_USB_SetRxType(usb_musb_reg_t *pUSB_Otg, uint8_t rxType)
{
	pUSB_Otg->RxType = rxType;
}

static inline void HAL_USB_SetRxMaxP(usb_musb_reg_t *pUSB_Otg, uint16_t max_pakt)
{
	pUSB_Otg->RxMaxP = max_pakt;
}

/**
 * @brief  Set USB periphral TxType register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  rxIntv : TxType value
 *
 * @retval None
 */
static inline void HAL_USB_SetTxType(usb_musb_reg_t *pUSB_Otg, uint8_t txType)
{
	pUSB_Otg->TxType = txType;
}

static inline void HAL_USB_SetTxMaxP(usb_musb_reg_t *pUSB_Otg, uint16_t max_pakt)
{
	pUSB_Otg->TxMaxP = max_pakt;
}


/**
 * @brief  Set USB periphral RxIntv register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  rxIntv : RxIntv value
 *
 * @retval None
 */
static inline void HAL_USB_SetRxIntv(usb_musb_reg_t *pUSB_Otg, uint8_t rxIntv)
{
	pUSB_Otg->RxIntv = rxIntv;
}

/**
 * @brief  Set USB periphral TxIntv register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  txIntv : TxIntv value
 *
 * @retval None
 */
static inline void HAL_USB_SetTxIntv(usb_musb_reg_t *pUSB_Otg, uint8_t txIntv)
{
	pUSB_Otg->IntvNAKLmt0_Union.TxIntv = txIntv;
}

/**
 * @brief  Get the data count in USB periphral's endpoint FIFO,
 *         usually HAL_USB_SetIndex should be invoked previously
 *
 * @param  pUSB_Otg : USB OTG base address
 *
 * @retval The data count in USB periphral's endpoint FIFO
 */
static inline uint16_t HAL_USB_GetOutEpFifoCnt(usb_musb_reg_t *pUSB_Otg)
{
  uint16_t intr;
  intr = (pUSB_Otg->RxCount2) << 8 | pUSB_Otg->Count_Union.RxCount1;
	return intr;
}

/**
 * @brief  Get USB periphral's frame count
 *
 * @param  pUSB_Otg : USB OTG base address
 *
 * @retval USB periphral's frame count
 */
static inline uint16_t HAL_USB_GetFrameCnt(usb_musb_reg_t *pUSB_Otg)
{
  uint16_t intr;
  intr = (uint16_t)((pUSB_Otg->Frame2 << 8U) | pUSB_Otg->Frame1);
	return intr;
}

/**
 * @brief  Clear USB periphral NAKLmt0 register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  limit : the value for NAKLmt0 register
 *
 * @retval None
 */
static inline void HAL_USB_SetNakLmt0(usb_musb_reg_t *pUSB_Otg, uint8_t limit)
{
	pUSB_Otg->IntvNAKLmt0_Union.NAKLmt0	= limit;
}

/**
 * @brief  Clear USB periphral RxCSR1 register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  bitfield : the bitfield for RxCSR1 register
 *
 * @retval None
 */
static inline void HAL_USB_ClearRxCSR1(usb_musb_reg_t *pUSB_Otg, uint8_t bitfield)
{
	pUSB_Otg->RxCSR1 &= ~(bitfield);
}

/**
 * @brief  Set USB periphral RxCSR1 register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  bitfield : the bitfield for RxCSR1 register
 *
 * @retval None
 */
static inline void HAL_USB_SetRxCSR1(usb_musb_reg_t *pUSB_Otg, uint8_t bitfield)
{
	pUSB_Otg->RxCSR1 |= (bitfield);
}

/**
 * @brief  Clear USB periphral RxCSR2 register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  bitfield : the bitfield for RxCSR2 register
 *
 * @retval None
 */
static inline void HAL_USB_ClearRxCSR2(usb_musb_reg_t *pUSB_Otg, uint8_t bitfield)
{
	pUSB_Otg->RxCSR2 &= ~(bitfield);
}

/**
 * @brief  Set USB periphral RxCSR2 register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  bitfield : the bitfield for RxCSR2 register
 *
 * @retval None
 */
static inline void HAL_USB_SetRxCSR2(usb_musb_reg_t *pUSB_Otg, uint8_t bitfield)
{
	pUSB_Otg->RxCSR2 |= (bitfield);
  rt_kprintf("CRxCSR2:0x%02x\n",pUSB_Otg->RxCSR2);
}

/**
 * @brief  Set USB periphral Index register
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  index : index value
 *
 * @retval None
 */
static inline void HAL_USB_SetIndex(usb_musb_reg_t *pUSB_Otg, uint8_t index)
{
	pUSB_Otg->Index = index;
}

/**
 * @brief  Return the value of USB periphral Inded register
 *
 * @param  pUSB_Otg : USB OTG base address
 *
 * @retval The value of USB periphral Inded register
 */
static inline uint8_t HAL_USB_GetIndex(usb_musb_reg_t *pUSB_Otg)
{
  uint8_t intr;

  intr = pUSB_Otg->Index;
	return intr;
}

/**
 * @brief  Return the value of USB periphral DevCtrl register
 *
 * @param  pUSB_Otg : USB OTG base address
 *
 * @retval The value of USB periphral DevCtrl register
 */
static inline uint8_t HAL_USB_GetDevCtrl(usb_musb_reg_t *pUSB_Otg)
{
  uint8_t intr;
  intr = pUSB_Otg->DevCtl;
  return intr;
}

static inline void HAL_USB_SetDevCtrlBit(usb_musb_reg_t *pUSB_Otg,int bit)
{
  pUSB_Otg->DevCtl |= bit ;
}

static inline void HAL_USB_ClrDevCtrlBit(usb_musb_reg_t *pUSB_Otg,int bit)
{
  pUSB_Otg->DevCtl &= bit ;
}
/**
 * @brief  Return the USB peripheral OUT endpoints interrupt status
 *
 * @param  pUSB_Otg : USB OTG base address
 *
 * @retval USB all Out Enpoints interrupt status
 */
static inline uint16_t USB_ReadDevAllRxEpInterrupt (usb_musb_reg_t *pUSB_Otg)
{
    uint16_t intr;

    intr = pUSB_Otg->IntrRx1 | (pUSB_Otg->IntrRx2 << 8);
    pUSB_Otg->IntrRx1 = (uint8_t)intr;
    pUSB_Otg->IntrRx2 = (uint8_t)intr>>8;
    return intr;
}

/**
  * @brief  Return the USB peripheral IN endpoints interrupt status
  *
  * @param  pUSB_Otg : USB OTG base address
  *
  * @retval USB all In Enpoints interrupt status
  */
static inline uint16_t USB_ReadDevAllTxEpInterrupt (usb_musb_reg_t *pUSB_Otg)
{
  uint16_t intr;

  intr = pUSB_Otg->IntrTx1 | (pUSB_Otg->IntrTx2 << 8);
  /*Clear Intr flag*/
  pUSB_Otg->IntrTx1 = (uint8_t)intr;
  pUSB_Otg->IntrTx2 = (uint8_t)intr>>8;
  return intr;
}

/**
 * @brief  Allocate USB internal sram as dynamic mode
 *
 * @param  pUSB_Otg : USB OTG base address
 *
 * @retval None
 */
void HAL_USB_SetDynamicSize(usb_musb_reg_t *pUSB_Otg);

/**
 * @brief  Flush a USB endpoint or all endpoints TxFIFO
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  num : endpoint number
 *         This parameter can be a value from 0 to 3, or 16
 *
 * @retval None
 */
void HAL_USB_FlushTxFifo(usb_musb_reg_t *pUSB_Otg, uint32_t num);

/**
 * @brief  Flush a USB endpoint or all endpoints RxFIFO
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  num : endpoint number
 *         This parameter can be a value from 0 to 3, or 16
 *
 * @retval None
 */
void HAL_USB_FlushRxFifo(usb_musb_reg_t *pUSB_Otg, uint32_t num);

/**
 * @brief  Clear RxPkt Ready for USB endpoint
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  ep : endpoint number
 *         This parameter can be a value from 0 to 3
 *
 * @retval None
 */
void HAL_USB_ClearEndpointRxPktRdy(usb_musb_reg_t *pUSB_Otg, uint8_t ep);

/**
 * @brief  Set TxPkt Ready for USB endpoint
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  ep : endpoint number
 *         This parameter can be a value from 0 to 3
 * @param  dataend : indication of the last data written to endpoint's FIFO
 *         and it's valid only for USB Control Enpoint
 *
 * @retval None
 */
void HAL_USB_SetEndpointxTxPktRdy(usb_musb_reg_t *pUSB_Otg, uint8_t ep, uint8_t dataend);

/**
 * @brief  Wirte a length of data to USB endpoint's FIFO
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  src : the buffer pointer to data
 * @param  ep : endpoint number
 *         This parameter can be a value from 0 to 3
 * @param  len : the length of data to be written
 *
 * @retval None
 */
void HAL_USB_WritePacket(usb_musb_reg_t *pUSB_Otg, uint8_t *src, uint8_t ep, uint16_t len);

/**
 * @brief  Read a length of data from USB endpoint's FIFO
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  dest : the buffer pointer to data
 * @param  ep : endpoint number
 *         This parameter can be a value from 0 to 3
 * @param  len : the length of data to be read
 *
 * @retval None
 */
uint8_t HAL_USB_ReadPacket(usb_musb_reg_t *pUSB_Otg, uint8_t *dest, uint8_t ep, uint16_t len);

/**
 * @brief  Initialize USB OTG peripheral
 *
 * @param  pUSB_Otg : USB OTG base address
 *
 * @retval None
 */
void HAL_USB_InitializeDevice(usb_musb_reg_t *pUSB_Otg);

/**
 * @brief  Enable a USB endpoint's general interrupt
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  ep : endpoint number
 *         This parameter can be a value from 0 to 3
 *
 * @retval None
 */
void HAL_USB_EnableEndpoints(usb_musb_reg_t *pUSB_Otg, uint8_t ep);

/**
 * @brief  Disable a USB endpoint's general interrupt
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  ep : endpoint number
 *         This parameter can be a value from 0 to 3
 *
 * @retval None
 */
void HAL_USB_DisableEndpoints(usb_musb_reg_t *pUSB_Otg, uint8_t ep);

/**
 * @brief  De-initialize USB OTG peripheral
 *
 * @param  pUSB_Otg : USB OTG base address
 *
 * @retval None
 */
void HAL_USB_DeInitializeDevice(usb_musb_reg_t *pUSB_Otg);

/**
 * @brief  Returns a USB In-Endpoint Interrupt
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  ep : endpoint number
 *         This parameter can be a value from 0 to 3
 *
 * @retval USB In-Endpoint interrupt status
 */
uint32_t HAL_USB_ReadDevInEPInterrupt (usb_musb_reg_t *pUSB_Otg, uint8_t ep);

/**
 * @brief  Returns a USB Out-Endpoint Interrupt
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  ep : endpoint number
 *         This parameter can be a value from 0 to 3
 *
 * @retval USB Out-Endpoint interrupt status
 */
uint32_t HAL_USB_ReadDevOutEPInterrupt (usb_musb_reg_t *pUSB_Otg, uint8_t ep);

/**
 * @brief  Stall an Endpoint data-transaction
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  ep : endpoint number
 *         This parameter can be a value from 0 to 3
 *
 * @retval None
 */
void HAL_USB_EndpointsStall(usb_musb_reg_t *pUSB_Otg, uint8_t ep);

/**
 * @brief  Unstall an Endpoint data-transaction
 *
 * @param  pUSB_Otg : USB OTG base address
 * @param  ep : endpoint number
 *         This parameter can be a value from 0 to 3
 *
 * @retval None
 */
void HAL_USB_EndpointsUnstall(usb_musb_reg_t *pUSB_Otg, uint8_t ep);

void HAL_USB_IRQHandler(void);



#ifdef __cplusplus
}
#endif

#endif

