/*
 * tvd.h
 *
 *  Created on: 2018Äê12ÔÂ2ÈÕ
 *      Author: lucy
 */

#ifndef DRIVER_INCLUDE_TVD_H_
#define DRIVER_INCLUDE_TVD_H_
#include <rtthread.h>
typedef unsigned int __u32;
typedef unsigned short __u16;
typedef int __s32;
typedef unsigned long long __u64;
typedef long long __s64;
#define TVD_Printf   rt_kprintf

typedef enum
{
        TVD_PL_YUV422,
		TVD_PL_YUV420,
		TVD_MB_YUV420,
}tvd_fmt_t;

typedef enum
{
		TVD_CVBS,
		TVD_YPBPR_I,
		TVD_YPBPR_P,
}tvd_interface_t;

typedef enum
{
		TVD_NTSC,
		TVD_PAL,
		TVD_SECAM,
}tvd_system_t;

typedef enum
{
		TVD_FRAME_DONE,
		TVD_LOCK,
		TVD_UNLOCK,
}tvd_irq_t;


/* */
/* detail information of registers */
/* */
/* pixel format(yuv) */
/* only used in scaler framebuffer */
typedef enum __PIXEL_YUVFMT {
	PIXEL_YUV444 = 0x10,
	PIXEL_YUV422,
	PIXEL_YUV420,
	PIXEL_YUV411,
	PIXEL_CSIRGB,
	PIXEL_OTHERFMT,
} __pixel_yuvfmt_t;

typedef enum { BT601 = 0, BT709, YCC, VXYCC, MONO } __cs_mode_t;

typedef struct {
	__s32 x;
	__s32 y;
	__u32 width;
	__u32 height;
} RECT; /* rect attrib                          */
typedef struct {
	__u32 width;
	__u32 height;
} SIZE; /* rect size                            */

typedef SIZE __rectsz_t;
typedef RECT __rect_t;

#define TVD_MODE_NUM 8
#define TVD_BUFFER_NUM 6

typedef enum tag_BUF_SCRAB_MODE_TVD {
	TVD0_FULL = 0, /*TVD0 on one buffer */
	TVD1_FULL = 1, /*TVD1 on one buffer */

	TVD_01_LR =
	    2, /*TVD0(L) and TVD1(R) with same size and format parallel on one
		  buffer */
	TVD_10_LR =
	    3, /*TVD1(L) and TVD0(R) with same size and format parallel on one
		  buffer */

	TVD_01_UD = 4, /*TVD0(L) and TVD1(R) with same size and format
			  sequential on one buffer */
	TVD_10_UD = 5, /*TVD1(L) and TVD0(R) with same size and format
			  sequential on one buffer */

	TVD0_TDM_2CH_2X1 = 6, /*TVD0 TDM CH0/1 with same size and format */
	TVD0_TDM_4CH_2X2 =
	    7, /*TVD0 TDM CH0/1 + CH2/3 with same size and format */

	TVD_SC_NA = 0xff,
} __buf_scrab_mode_tvd_t;

typedef struct tag_TVD_MODE {

	__pixel_yuvfmt_t color_format; /*__pixel_yuvfmt_t */
	__u32 component_seq;	   /*__yuv_seq_t + __bayer_seq_t */
	__u32 store_mode;	      /*__yuv_mod_t + __bayer_store_mode_t */
	__rectsz_t size;
	__u32 frame_rate;   /*Hz, x1000 */
	__u32 frame_period; /*us */
	__cs_mode_t color_space;
	/*=========add for TVD0+TVD1 =============== */
	__buf_scrab_mode_tvd_t tvd_buf_scrab_mode;
	/*======================== */
} __tvd_mode_t;

typedef struct tag_TVD_MODE_ALL {
	__u32 number;
	__tvd_mode_t tvd_mode[TVD_MODE_NUM];
} __tvd_mode_all_t;

typedef enum __DRV_TVD_CMD_SET {
	DRV_TVD_CMD_NONE = 0,
	DRV_TVD_CMD_INIT, /*ret = EPDK_OK/EPDK_FAIL */
	DRV_TVD_CMD_EXIT, /*ret = EPDK_OK/EPDK_FAIL */
	/*pbuffer = __csi_mode_t*, ret = EPDK_OK/EPDK_FAIL*/
	DRV_TVD_CMD_SET_MODE,
	/*pbuffer = __csi_mode_t*, ret = EPDK_OK/EPDK_FAIL*/
	DRV_TVD_CMD_GET_MODE,

	DRV_TVD_CMD_GET_MODE_ALL,/*pbuffer = __csi_mode_total_t * */
	DRV_TVD_CMD_CAPTURE_ON,/*ret = EPDK_OK/EPDK_FAIL */
	DRV_TVD_CMD_CAPTURE_OFF,/*ret = EPDK_OK/EPDK_FAIL */
	/*pbuffer= (__csi_frame_t**), ret =EPDK_OK/EPDK_FAIL */
	DRV_TVD_CMD_REQUEST_FRAME,
	DRV_TVD_CMD_RELEASE_FRAME, /*aux = frame_id, ret = EPDK_OK/EPDK_FAIL */
	DRV_TVD_CMD_SET_PTS,       /*aux = pts(__u32) */
	DRV_TVD_CMD_GET_PTS,       /*ret=pts(__u32) */
	/*??PTS??????aux = 0:CSI???PTS;
	 * 1:????????PTS; 2.???PTS */
	DRV_TVD_CMD_SET_PTS_MODE,

	/*??????,??PTS??callback????CB_GetPTS,
	 * ??*arg?__s64*???,??PTS???us.
	 * *pbuffer = callback */
	DRV_TVD_CMD_INSTALL_CALLBACK_GET_PTS,

	/*??frame*/
	/*queue.???capture_on???????capture*/
	/*off???. pbuffer =*/
	/*__csi_frame_queue_t*,?????buffer?frame_id?
	 * ????????csidrv??.*/
	DRV_TVD_CMD_SET_FRAME_QUEUE,
	DRV_TVD_CMD_SET_COLOR_EFFECT, /*absence aux = color effet */
	DRV_TVD_CMD_GET_COLOR_EFFECT, /*absence ret = color effet */
	DRV_TVD_CMD_SET_AWB,	  /*absence aux = awb */
	DRV_TVD_CMD_GET_AWB,	  /*absence ret = awb */
	DRV_TVD_CMD_SET_AE,	   /*absence aux = ae */
	DRV_TVD_CMD_GET_AE,	   /*absence ret = ae */
	DRV_TVD_CMD_SET_BRIGHT,       /*absence aux = bright */
	DRV_TVD_CMD_GET_BRIGHT,       /*absence ret = bright */
	DRV_TVD_CMD_SET_CONTRAST,     /*absence aux = contrast */
	DRV_TVD_CMD_GET_CONTRAST,     /*absence ret = contrast */
	DRV_TVD_CMD_SET_BAND,	 /*absence aux = band */
	DRV_TVD_CMD_GET_BAND, /*absence ret = band//add 2011-8-3 13:13:13 */
	DRV_TVD_CMD_GET_PROGRSSV_INFO, /*absence ret = b_progr */
	DRV_TVD_CMD_SET_MASTER_PORT =
	    DRV_TVD_CMD_GET_PROGRSSV_INFO, /*absence */
	DRV_TVD_CMD_GET_MASTER_PORT,       /*absence */
	DRV_TVD_CMD_SET_TV_SEL = 0x100,
	DRV_TVD_CMD_SEL_CHANNEL, /*aux = __drv_TVD_sel_channel */
	DRV_TVD_CMD_SEL_SOURCE,  /*aux = TVD_SOURCE_CVBS, TVD_SOURCE_YPbPr */
	DRV_TVD_CMD_SEL_FORMAT,  /*aux = tvd_mod_fmt_t */
	DRV_TVD_CMD_PAL_NTSC, /*if TVD_SOURCE_CVBS, aux = __drv_TVD_pal_ntsc */
	/*if TVD_SOURCE_YPbPr, aux =__drv_TVD_YPbPr_size */
	DRV_TVD_CMD_YPbPr_SIZE,
	DRV_TVD_CMD_SET_INPUT_CHANNEL, /*aux = __drv_TVD_INPUT_CHANNEL */
	/* return : bit0: 1->plugin, 0->plugout ;bit4:1->pal,0:ntsc;bit8:*/
	DRV_TVD_CMD_GET_ADC_STATUS,
	/*??32?????0bit?1???????0??????
	 * ?????????4bit(?0???)?1??P???0??N?*/
	DRV_TVD_CMD_GET_STATUS,

	DRV_TVD_CMD_,

} __drv_TVD_cmd_set_t;
typedef struct tag_TVD_FRAME {
	__s32 frame_id;

	__s32 bProgressiveSrc; /* Indicating the source is progressive or not */
	__s32 bTopFieldFirst; /* VPO should check this flag when bProgressiveSrc
			       is FALSE */
	__u16 eAspectRatio; /*the source picture aspect ratio */
	__rect_t src_rect;  /* source valid size, ?????????,
			       ????frame buffer? */
	__rect_t dst_rect; /* source display size,????????? */
	__u32 addr[3];     /* data buffer address */

	__u16 color_format;  /*same with __tvd_mode_t */
	__u32 component_seq; /*same with __tvd_mode_t */
	__u32 store_mode;    /*same with __tvd_mode_t */
	__u16 frame_rate;    /*same with __tvd_mode_t */

	__s64 uPts; /*us */
	__s32 bPtsValid;
} __tvd_frame_t;

typedef struct tag_TVD_FRAME_QUEUE {
	__s32 num; /*???????num <= TVD_BUFFER_NUM */
	__tvd_frame_t tvd_frame_array[TVD_BUFFER_NUM];
} __tvd_frame_queue_t;

typedef enum e_TVD_SEL_CHANNEL {
	TVD_CHANNEL_0,
	TVD_CHANNEL_1,
	TVD_CHANNEL_2,
	TVD_CHANNEL_3,

	TVD_SEL_CHANNEL_MAX

} __drv_TVD_sel_channel;

typedef enum e_TVD_SEL_SOURCE {
	TVD_SOURCE_CVBS,
	TVD_SOURCE_YPbPr,

	TVD_SEL_SOURCE_MAX

} __drv_TVD_sel_source;

typedef enum e_TVD_PAL_NTSC {
	TVD_SOURCE_NTSC,
	TVD_SOURCE_PAL,
	TVD_SOURCE_PAL_M,
	TVD_SOURCE_PAL_N,
	TVD_SOURCE_SECAM,
	TVD_PAL_NTSC_MAX

} __drv_TVD_pal_ntsc;

typedef enum e_TVD_YPbPr_SIZE {
	TVD_YPbPr_480i,
	TVD_YPbPr_576i,

	TVD_YPbPr_SIZE_MAX

} __drv_TVD_YPbPr_size;

typedef enum e_TVD_INPUT_CHANNEL {
	TVD_INPUT_CHANNEL_0,
	TVD_INPUT_CHANNEL_1,

	TVD_INPUT_CHANNEL_MAX

} __drv_TVD_INPUT_CHANNEL;

typedef enum {
	TVD_UV_NON_MB_COMBINED_YUV422,
	TVD_UV_NON_MB_COMBINED_YUV420,
	TVD_UV_MB_COMBINED_YUV420,
	TVD_MOD_FMT_MAX

} tvd_mod_fmt_t;

void tv_in_decode_init(int id);



#endif /* DRIVER_INCLUDE_TVD_H_ */
