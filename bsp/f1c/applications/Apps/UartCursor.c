 #include "UartCursor.h"
#include "rtthread.h"
#include <rtdevice.h>
#include <rthw.h>
#include<string.h>
#include <drv_fb.h>

#define RT_UART2REC_THREAD_STACK_SIZE   2048
#define RT_VIRCURSOR_THREAD_STACK_SIZE  2048

#define TEMPCACHESIZE  256
#define DATAPACKMAX    512
/*UART Part*/
static rt_device_t Devuart2;


static uint8_t TempBuffer[TEMPCACHESIZE];
static uint8_t rPackData[DATAPACKMAX];
static uint16_t rPackDataLen=0;

static struct rt_semaphore uart2rx_sem;
static struct rt_semaphore packdatarx_sem;

UartCursorDataStruct UartCursorData;

//常规鼠标报告定义
//Byte0:固定一个值
//Byte1:表示按键按下
//Byte2:表示X的低8位
//Byte3:低4位代表X的高4位,高4位代表Y的低4位
//Byte4:表示Y的高8位
//Byte5:表示滚轮
//
#define LKEY_PRESS 0x01
#define RKEY_PRESS 0x02
#define MKEY_PRESS 0x04
#define MOUSE_SCALING 0x02

static rt_bool_t lkey_down=RT_FALSE;
void TureMouseDataCallback(rt_uint8_t *buffer , rt_uint32_t len)
{
    rt_uint16_t xoffset=0;
    rt_uint16_t yoffset=0;

	if((buffer[3]&0x08))
	{
		xoffset=(2048-(buffer[3]&0x07)*256-buffer[2])*MOUSE_SCALING;
		if(UartCursorData.Px>xoffset)
		{
			UartCursorData.Px-=xoffset;
		}
		else
		{
			UartCursorData.Px=0;
		}
	}
	else
	{
		xoffset=((buffer[3]&0x07)*256+buffer[2])*MOUSE_SCALING;
		if((UartCursorData.Px+xoffset)<479)
		{
			UartCursorData.Px+=xoffset;
		}
		else
		{
			UartCursorData.Px=479;
		}
	}
	if(buffer[4]&0x80)
	{
		yoffset=(2048-(buffer[3]>>4)-(buffer[4]&0x7f)*16)*MOUSE_SCALING;
		if(UartCursorData.Py>yoffset)
		{
			UartCursorData.Py-=yoffset;
		}
		else
		{
			UartCursorData.Py=0;
		}
	}
	else
	{
		yoffset=((buffer[3]>>4)+(buffer[4]&0x7f)*16)*MOUSE_SCALING;
		if(UartCursorData.Py+yoffset<271)
		{
			UartCursorData.Py+=yoffset;
		}
		else
		{
			UartCursorData.Py=271;
		}
	}
	if(xoffset!=0||yoffset!=0)
	{
		//cursor_set_position(emouse.x,emouse.y);
		HWCursor_SetPos(UartCursorData.Px,UartCursorData.Py);
	}
	if(buffer[1]&LKEY_PRESS)
	{
		if(lkey_down==RT_FALSE)
		{
			// rt_kprintf("mouse left key press down\n");
			UartCursorData.State = 1;
			lkey_down=RT_TRUE;
		}
	}
	else if(lkey_down==RT_TRUE)
	{
		// rt_kprintf("mouse left key press up\n");
		UartCursorData.State = 0;
		lkey_down=RT_FALSE;
	}

}

static rt_err_t TRuart2_input(rt_device_t dev, rt_size_t size)
{
	rt_sem_release(&uart2rx_sem);
	return RT_EOK;
}
void thrd_UartReceive(void *parameter)
{	
	uint16_t CpL=0;
	rt_err_t er;
	uint16_t TempBLen=0;
	while(1)
	{
		er=rt_sem_take(&uart2rx_sem,10);
		if(er==RT_EOK)
		{
			TempBLen=rt_device_read(Devuart2,0,TempBuffer,TEMPCACHESIZE);
			if(rPackDataLen+TempBLen<=DATAPACKMAX) CpL=TempBLen;
			else
			{
				CpL=DATAPACKMAX-rPackDataLen;
			}
			memcpy(rPackData+rPackDataLen,TempBuffer,CpL);
			rPackDataLen+=CpL;
		}
		else
		{
			if(rPackDataLen>0)
			{
				rt_sem_release(&packdatarx_sem);
			}
		}
	}
}
void thrd_VirCursor(void *parameter)
{
	while(1)
	{
		rt_sem_take(&packdatarx_sem, RT_WAITING_FOREVER);
		if(rPackDataLen>=4)
		{
			UartCursorData.State=rPackData[0];
			UartCursorData.Px=rPackData[1]*256+rPackData[2];
			UartCursorData.Py=rPackData[3]*256+rPackData[4];
			HWCursor_SetPos(rPackData[1]*256+rPackData[2],rPackData[3]*256+rPackData[4]);
		}
		//FreeIt
		rPackDataLen=0;				
	}
}
void UartCursor_Init()
{
	rt_thread_t tid;
	  
	rt_sem_init(&uart2rx_sem, "u2rx_sem", 0, RT_IPC_FLAG_FIFO);	
	rt_sem_init(&packdatarx_sem, "packrx_sem", 0, RT_IPC_FLAG_FIFO);
	
	Devuart2=rt_device_find("uart2");
	rt_device_set_rx_indicate(Devuart2,TRuart2_input);
	rt_device_open(Devuart2, RT_DEVICE_FLAG_INT_RX);
    
	tid = rt_thread_create("Uart2Rec", thrd_UartReceive, RT_NULL,
                           RT_UART2REC_THREAD_STACK_SIZE, 12, 20);
    RT_ASSERT(tid != RT_NULL);
    rt_thread_startup(tid);
	rt_kprintf("[Thread]Uart2Rec Created.\n");
	
    tid = rt_thread_create("VirCurso", thrd_VirCursor, RT_NULL,
                           RT_VIRCURSOR_THREAD_STACK_SIZE, 14, 20);
    RT_ASSERT(tid != RT_NULL);
    rt_thread_startup(tid);
	rt_kprintf("[Thread]VirCursor Created.\n");
	
}
