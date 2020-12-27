#ifndef  __USB_H__
#define  __USB_H__

//#define usbprint	rt_kprintf
#define usbprint(...)
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;



int usb_device_init(unsigned char type);

#define USB_TYPE_DISCONNECT 0
#define USB_TYPE_USB_HID 1
#define USB_TYPE_USB_COM 2



/*
 * Standard requests
 */
#define USB_REQ_GET_STATUS		0x00
#define USB_REQ_CLEAR_FEATURE		0x01
/* 0x02 is reserved */
#define USB_REQ_SET_FEATURE		0x03
/* 0x04 is reserved */
#define USB_REQ_SET_ADDRESS		0x05
#define USB_REQ_GET_DESCRIPTOR		0x06
#define USB_REQ_SET_DESCRIPTOR		0x07
#define USB_REQ_GET_CONFIGURATION	0x08
#define USB_REQ_SET_CONFIGURATION	0x09
#define USB_REQ_GET_INTERFACE		0x0A
#define USB_REQ_SET_INTERFACE		0x0B
#define USB_REQ_SYNCH_FRAME		0x0C

#define USB_TYPE_STANDARD		(0x00 << 5)
#define USB_TYPE_CLASS			(0x01 << 5)
#define USB_TYPE_VENDOR			(0x02 << 5)
#define USB_TYPE_RESERVED		(0x03 << 5)

#define USB_RECIP_DEVICE		0x00
#define USB_RECIP_INTERFACE		0x01
#define USB_RECIP_ENDPOINT		0x02
#define USB_RECIP_OTHER			0x03


/* Descriptor types ... USB 2.0 spec table 9.5 */
#define USB_DT_DEVICE			0x01
#define USB_DT_CONFIG			0x02
#define USB_DT_STRING			0x03
#define USB_DT_INTERFACE		0x04
#define USB_DT_ENDPOINT			0x05
#define USB_DT_DEVICE_QUALIFIER		0x06
#define USB_DT_OTHER_SPEED_CONFIG	0x07
#define USB_DT_INTERFACE_POWER		0x08
/* these are from a minor usb 2.0 revision (ECN) */
#define USB_DT_OTG			0x09
#define USB_DT_DEBUG			0x0a
#define USB_DT_INTERFACE_ASSOCIATION	0x0b

#define USB_DT_REPORT	0x21

#define USB_DT_DEVICE_SIZE		18
#define USB_DT_CONFIG_SIZE		9

#define USB_DT_CS_INTERFACE		(USB_TYPE_CLASS | USB_DT_INTERFACE)


/* identification values and masks to identify request types  */
#define USB_REQUEST_CLASS_MASK              (0x60)
#define USB_REQUEST_CLASS_STRD              (0x00)
#define USB_REQUEST_CLASS_CLASS             (0x20)
#define USB_REQUEST_CLASS_VENDOR            (0x40)

#define USB_FUNCTION_VENDOR_ID          (0x0525)                /* Vendor  ID */
#define USB_FUNCTION_PRODUCT_ID         (0xa4a7)                /* DATAPIPE Product ID */
#define USB_FUNCTION_SERIAL_PRODUCT_ID  (0x8206)                /* SERIAL Product ID */
#define USB_FUNCTION_HID_PRODUCT_ID     (0x8203)                /* HID KBW Product ID */
#define USB_FUNCTION_UPOS_PRODUCT_ID    (0x8210)                /* HID POS Product ID */
#define USB_FUNCTION_RELEASE_NO         (0x0310)                /* Release Number */

typedef enum
{
	ENDPOINT_TYPE_CONTROL,
	/* Typically used to configure a device when attached to the host.
	 * It may also be used for other device specific purposes, including
	 * control of other pipes on the device.
	 */
	ENDPOINT_TYPE_ISOCHRONOUS,
	/* Typically used for applications which need guaranteed speed.
	 * Isochronous transfer is fast but with possible data loss. A typical
	 * use is audio data which requires a constant data rate.
	 */
	ENDPOINT_TYPE_BULK,
	/* Typically used by devices that generate or consume data in relatively
	 * large and bursty quantities. Bulk transfer has wide dynamic latitude
	 * in transmission constraints. It can use all remaining available bandwidth,
	 * but with no guarantees on bandwidth or latency. Since the USB bus is
	 * normally not very busy, there is typically 90% or more of the bandwidth
	 * available for USB transfers.
	 */
	ENDPOINT_TYPE_INTERRUPT
	/* Typically used by devices that need guaranteed quick responses
	 * (bounded latency).
	 */
}USB_ENDPOINT_TYPE;


enum USB_STANDARD_REQUEST_CODE {
	GET_STATUS,
	CLEAR_FEATURE,
	SET_FEATURE = 3,
	SET_ADDRESS = 5,
	GET_DESCRIPTOR,
	SET_DESCRIPTOR,
	GET_CONFIGURATION,
	SET_CONFIGURATION,
	GET_INTERFACE,
	SET_INTERFACE,
	SYNCH_FRAME
};


enum USB_DESCRIPTOR_TYPE {
	DEVICE_DESCRIPTOR = 1,
	CONFIGURATION_DESCRIPTOR,
	STRING_DESCRIPTOR,
	INTERFACE_DESCRIPTOR,
	ENDPOINT_DESCRIPTOR,
	DEVICE_QUALIFIER_DESCRIPTOR,
	OTHER_SPEED_CONFIGURATION_DESCRIPTOR,
	INTERFACE_POWER1_DESCRIPTOR,
	INTERFACE_ASSOC_DESCRIPTOR = 11,
	HID_DESCRIPTOR_TYPE = 0x21,
	REPORT_DESCRIPTOR =0x22
};


enum USB_FEATURE_SELECTOR {
	ENDPOINT_HALT,
	DEVICE_REMOTE_WAKEUP,
	TEST_MODE
};


enum USB_CLASS_CODE {
	CLASS_DEVICE,
	CLASS_AUDIO,
	CLASS_COMM_AND_CDC_CONTROL,
	CLASS_HID,
	CLASS_PHYSICAL = 0x05,
	CLASS_STILL_IMAGING,
	CLASS_PRINTER,
	CLASS_MASS_STORAGE,
	CLASS_HUB,
	CLASS_CDC_DATA,
	CLASS_SMART_CARD,
	CLASS_CONTENT_SECURITY = 0x0d,
	CLASS_VIDEO,
	CLASS_DIAGNOSTIC_DEVICE = 0xdc,
	CLASS_WIRELESS_CONTROLLER = 0xe0,
	CLASS_MISCELLANEOUS = 0xef,
	CLASS_APP_SPECIFIC = 0xfe,
	CLASS_VENDOR_SPECIFIC = 0xff
};

typedef struct {
	u8 bRequestType;
	u8 bRequest;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
} __attribute__ ((packed)) USB_DeviceRequest;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 bcdUSB;
	u8 bDeviceClass;
	u8 bDeviceSubClass;
	u8 bDeviceProtocol;
	u8 bMaxPacketSize0;
	u16 idVendor;
	u16 idProduct;
	u16 bcdDevice;
	u8 iManufacturer;
	u8 iProduct;
	u8 iSerialNumber;
	u8 bNumConfigurations;
} __attribute__ ((packed)) USB_DeviceDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 bcdUSB;
	u8 bDeviceClass;
	u8 bDeviceSubClass;
	u8 bDeviceProtocol;
	u8 bMaxPacketSize0;
	u8 bNumConfigurations;
	u8 bReserved;
} __attribute__ ((packed)) USB_DeviceQualifierDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 wTotalLength;
	u8 bNumInterfaces;
	u8 bConfigurationValue;
	u8 iConfiguration;
	u8 bmAttributes;
	u8 MaxPower;
} __attribute__ ((packed)) USB_ConfigDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 wTotalLength;
	u8 bNumInterfaces;
	u8 bConfigurationValue;
	u8 iConfiguration;
	u8 bmAttributes;
	u8 bMaxPower;
} __attribute__ ((packed)) USB_OtherSpeedConfigDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u8 bInterfaceNumber;
	u8 bAlternateSetting;
	u8 bNumEndpoints;
	u8 bInterfaceClass;
	u8 bInterfaceSubClass;
	u8 bInterfaceProtocol;
	u8 iInterface;
} __attribute__ ((packed)) USB_InterfaceDescriptor;


typedef struct {
	u8 bLegth;
	u8 bDescriptorType;
	u8 bEndpointAddress;
	u8 bmAttributes;
	u16 wMaxPacketSize;
	u8 bInterval;
} __attribute__ ((packed)) USB_EndPointDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 SomeDesriptor[1];
} __attribute__ ((packed)) USB_StringDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u8 bFirstInterface;
	u8 bInterfaceCount;
	u8 bFunctionClass;
	u8 bFunctionSubClass;
	u8 bFunctionProtocol;
	u8 iFunction;
} __attribute__((packed)) USB_InterfaceAssocDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubType;
	u16 bcdCDC;
} __attribute__((packed)) USB_CDC_HeaderDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubType;
	u8 bmCapabilities;
	u8 bDataInterface;
} __attribute__((packed)) USB_CDC_CallMgmtDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubType;
	u8 bmCapabilities;
} __attribute__ ((packed)) USB_CDC_AcmDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubType;
	u8 bMasterInterface0;
	u8 bSlaveInterface0;
} __attribute__ ((packed)) USB_CDC_UnionDescriptor;


typedef struct {
	u32 dwDTERate;
	u8 bCharFormat;
	u8 bParityType;
	u8 bDataBits;
} __attribute__ ((packed)) USB_CDC_LineCoding;

typedef struct {
    u8  bLength;
    u8  bDescriptorType;
    u16 bcdHID;
    u8  bCountryCode;
    u8  bNumDescriptors;
    u8  next_bDescriptorType;
    u16 wDescriptorLength;
} __attribute__ ((packed)) USB_HID_Descriptor;



#endif   //__BSP_USB_H__


