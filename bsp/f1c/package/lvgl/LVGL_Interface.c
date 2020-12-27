#include "rtthread.h"
#include "lvgl.h"
#include "drv_fb.h"
#include "UartCursor.h"
#include "LVGL_Interface.h"

#define RT_LVGLLOOP_THREAD_STACK_SIZE 2048
#define MY_DISP_HOR_RES  LV_HOR_RES_MAX

static lv_disp_buf_t disp_buf;
static lv_color_t buf_1[MY_DISP_HOR_RES * 10];
static lv_color_t buf_2[MY_DISP_HOR_RES * 10];

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    int32_t x;
    int32_t y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {  
		    ((uint32_t*)lcd_layer0.vram)[y*lcd_layer0.width+x]=color_p->full;
            color_p++;
        }
    }
    lv_disp_flush_ready(disp_drv);
}
static void Lv_LVGL_Init()
{
	lv_disp_drv_t disp_drv;
	lv_init();
	lv_disp_buf_init(&disp_buf, buf_1, buf_2, MY_DISP_HOR_RES*10);
	lv_disp_drv_init(&disp_drv);
	disp_drv.hor_res=LV_HOR_RES_MAX;
	disp_drv.ver_res=LV_VER_RES_MAX;
	disp_drv.buffer=&disp_buf;
	disp_drv.flush_cb=disp_flush;
	lv_disp_drv_register(&disp_drv);
}
/*******************************Touch**************************************/
lv_indev_t * indev_touchpad;
static bool touchpad_is_pressed(void)
{
    if(UartCursorData.State==1) {return true;}
    return false;
}
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    (*x) = UartCursorData.Px;
    (*y) = UartCursorData.Py;
}
static bool touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    /*Save the pressed coordinates and the state*/
    if(touchpad_is_pressed()) {
        touchpad_get_xy(&last_x, &last_y);
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = last_x;
    data->point.y = last_y;

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}
static void Lv_TouchInit()
{
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpad_read;
  indev_touchpad = lv_indev_drv_register(&indev_drv);
}
void thrd_LVGL_Loop(void *parameter)
{
	while(1)
	{
		lv_task_handler();
		rt_thread_delay(5);
	}	
}
static void Creat_LVGL_Thread()
{
	rt_thread_t tid;
	tid = rt_thread_create("GUI_Lvgl", thrd_LVGL_Loop, RT_NULL,
					   RT_LVGLLOOP_THREAD_STACK_SIZE, 15, 20);
    RT_ASSERT(tid != RT_NULL);
    rt_thread_startup(tid);
	rt_kprintf("[Thread]GUIThread Created.\n");
}
void Lvgl_Init()
{
	Lv_LVGL_Init();
	Lv_TouchInit();
	Creat_LVGL_Thread();
}
