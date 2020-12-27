/*
 * tvd_dev.c
 *
 *  Created on: 2018Äê12ÔÂ1ÈÕ
 *      Author: VeiLiang
 */
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include "drv_tvd.h"
#include "interrupt.h"

static tvd_id = 0;
// unsigned char tvd_y_buf[720*576]__attribute__ ((aligned (4096)));;
// unsigned char tvd_c_buf[720*576]__attribute__ ((aligned (4096)));;
extern unsigned char jpeg_ydst[1920*1080];
extern unsigned char jpeg_cdst[1920*1080];
unsigned char tvd_y_buf_disp[720*576];
unsigned char tvd_c_buf_disp[720*576];
//int tcountt = 0;
//int time_get_tick = 0;
void tvd_irq_handle(int arg)
{
	//TVD_Printf("tvd irq\n");

//	TVD_Printf("%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\n",
//			tvd_y_buf[0],tvd_y_buf[1],tvd_y_buf[2],tvd_y_buf[3],
//			tvd_y_buf[4],tvd_y_buf[5],tvd_y_buf[6],tvd_y_buf[7]);
//	TVD_Printf("%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\n",
//				tvd_y_buf[1000],tvd_y_buf[1001],tvd_y_buf[1002],tvd_y_buf[1003],
//				tvd_y_buf[1004],tvd_y_buf[1005],tvd_y_buf[1006],tvd_y_buf[1007]);
	//tcountt ++;
	tvd_irq_status_clear(tvd_id,TVD_FRAME_DONE);
	//rt_kprintf("%dms",rt_tick_get() - time_get_tick);
	//time_get_tick = rt_tick_get() ;
	memcpy(tvd_y_buf_disp,jpeg_ydst,720*576);
	memcpy(tvd_c_buf_disp,jpeg_cdst,720*576);
	

	// dma_cache_sync(tvd_y_buf,720*576,3);
	// dma_cache_sync(tvd_c_buf,720*576,3);
	//if((tcountt & 0xff) == 0xff)
	{
		RefleshLcdWithTVD(tvd_y_buf_disp,tvd_c_buf_disp,720,576);
	}
	

}

static unsigned char cvbs_3d_comb_buf[0x400000]__attribute__ ((aligned (4096)));
void tv_in_decode_init(int id)
{
	int		   	retval  = 0;
	tvd_id = id;
	rt_hw_interrupt_install(TVD_INTERRUPT,tvd_irq_handle,(void*)0,"tvd_isr");
	rt_hw_interrupt_umask(TVD_INTERRUPT);
	//retval = request_irq(TVD_INTERRUPT, tvd_irq_handle,0);//first requet irq
	printf("request_irq \n");

	tvd_bsp_init(id);//bsp init like open clk ,reset ...
	TVD_Printf("tvd_bsp_init \n");
	tvd_config(TVD_SOURCE_CVBS,TVD_SOURCE_PAL,TVD_PL_YUV420);
	TVD_Printf("tvd_config \n");

	tvd_set_fmt(id,TVD_PL_YUV420);
	TVD_Printf("tvd_set_fmt \n");
	tvd_set_addr_y(id,jpeg_ydst);
	tvd_set_addr_c(id,jpeg_cdst);
	//Enable irq
	tvd_irq_status_clear(id,TVD_FRAME_DONE);
	TVD_Printf("tvd_irq_status_clear \n");

	tvd_irq_enable(id,TVD_FRAME_DONE);
	TVD_Printf("tvd_irq_enable \n");
	tvd_3d_comb_fliter(1,cvbs_3d_comb_buf);
	tvd_set_blue_flag(2);

	tvd_capture_on(id);
	TVD_Printf("tvd_capture_on \n");



}

