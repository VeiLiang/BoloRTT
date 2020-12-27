#ifndef __LVGL_Interface_h
#define __LVGL_Interface_h
/*Touch*/
typedef struct
{
	uint8_t State;//1=Pressed 
	uint16_t Px;
	uint16_t Py;
}TouchDataStruct;
extern TouchDataStruct TouchDat;
void Lvgl_Init(void);
void LVGL_LoopTask(void);
#endif
