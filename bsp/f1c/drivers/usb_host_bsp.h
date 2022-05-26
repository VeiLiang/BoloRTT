/*
 * usb_host_bsp.h
 *
 *  Created on: 2020Äê4ÔÂ3ÈÕ
 *      Author: VeiLiang
 */

#ifndef _USB_HOST_BSP_H_
#define _USB_HOST_BSP_H_
#include "usb_host_phy.h"

#define HAL_DelayMs rt_thread_delay

#define usb_echo rt_kprintf
// #define usb_echo(...)
typedef void (*pipe_callback)(void *context);
typedef struct _usb_host_pipe
{
	void * usbHostHandle; 			/*!< This pipe's device's handle*/
	void * pdata; 			/*!< This pipe's device's handle*/
	uint8_t devAddress;               /*!< Device Address*/
	uint8_t pipeType;               /*!< Pipe type, for example USB_ENDPOINT_BULK*/
    uint8_t direction;              /*!< Pipe direction*/
    uint8_t endpointAddress;        /*!< Endpoint address*/
    uint16_t interval;              /*!< FS/LS: frame unit; HS: micro-frame unit*/
    uint16_t maxPacketSize;         /*!< Maximum packet size*/
    uint8_t pipe_index;				/*!< Pipe transform index*/
    uint8_t nextdata01;             /*!< Data toggle*/
    uint16_t nakCount;              /*!< Maximum NAK count*/
//    uint16_t frame_nak;            /*!<  Nak frame count start FS/LS: frame unit; HS: micro-frame unit*/
//    uint16_t frame_trans;            /*!< Last transform frame record FS/LS: frame unit; HS: micro-frame unit*/
    pipe_callback int_ep_callback; /*!< interrupt ep callback for request interrupt transform*/
} usb_host_pipe_t;

typedef enum {
	NONE,
	STATE_OK,
	STATE_STALL,
	STATE_ERR,
	STATE_NAK,
	STATE_WAIT
}usb_ep_state_t;
typedef enum _tr_type
{
    kTr_Ctrl = 0,
    kTr_In,
    kTr_Out,
    kTr_OutHndsk,
    kTr_InHndsk,
    kTr_Unknown
} tr_type_t;
/*! @brief USB error code */
typedef enum _usb_status
{
    kStatus_USB_Success = 0x00U, /*!< Success */
    kStatus_USB_Error,           /*!< Failed */

    kStatus_USB_Busy,                       /*!< Busy */
    kStatus_USB_InvalidHandle,              /*!< Invalid handle */
    kStatus_USB_InvalidParameter,           /*!< Invalid parameter */
    kStatus_USB_InvalidRequest,             /*!< Invalid request */
    kStatus_USB_ControllerNotFound,         /*!< Controller cannot be found */
    kStatus_USB_InvalidControllerInterface, /*!< Invalid controller interface */

    kStatus_USB_NotSupported,   /*!< Configuration is not supported */
    kStatus_USB_Retry,          /*!< Enumeration get configuration retry */
    kStatus_USB_TransferStall,  /*!< Transfer stalled */
    kStatus_USB_TransferFailed, /*!< Transfer failed */
    kStatus_USB_AllocFail,      /*!< Allocation failed */
    kStatus_USB_LackSwapBuffer, /*!< Insufficient swap buffer for KHCI */
    kStatus_USB_TransferCancel, /*!< The transfer cancelled */
    kStatus_USB_BandwidthFail,  /*!< Allocate bandwidth failed */
    kStatus_USB_MSDStatusFail,  /*!< For MSD, the CSW status means fail */
    kStatus_USB_EHCIAttached,
    kStatus_USB_EHCIDetached,
}usb_status_t;
typedef struct{
	usb_musb_reg_t *pUSB_Otg;                        /*!< The base address of the register */
	void *pData;                           /*!< Related handle*/
	usb_ep_state_t ep_state_array[2][16];		/*!< Record ep state host handle*/
	usb_host_pipe_t* current_pipe;		/*!< Record current pipe*/
	uint8_t* pipe_trans_buf;			/*!< Record pipe trans buf pointer */
	uint32_t pipe_trans_len;	/*!< Record pipe trans size */
	uint8_t deviceAttached;	/*!< device attached ? */
	uint8_t errsts;			/*!< error code record */
	uint16_t sof_frame;		/*!< usb sof frame count */
}usb_host_handle_t;

/* USB  standard descriptor endpoint type */
#define USB_ENDPOINT_CONTROL (0x00U)
#define USB_ENDPOINT_ISOCHRONOUS (0x01U)
#define USB_ENDPOINT_BULK (0x02U)
#define USB_ENDPOINT_INTERRUPT (0x03U)

#define USB_TOKEN_TOKENENDPT_MASK                (0xFU)
#define USB_TOKEN_TOKENENDPT_SHIFT               (0U)
#define USB_TOKEN_TOKENENDPT(x)                  (((uint8_t)(((uint8_t)(x)) << USB_TOKEN_TOKENENDPT_SHIFT)) & USB_TOKEN_TOKENENDPT_MASK)


/* USB  standard descriptor transfer direction (cannot change the value because iTD use the value directly) */
#define USB_OUT (0U)
#define USB_IN (1U)

/*!
 * @brief the max NAK count for one transaction.
 * when nak count reach to the value, the transaction fail.
 */
#define USB_HOST_CONFIG_MAX_NAK (3000U)

#endif /* _USB_HOST_BSP_H_ */
