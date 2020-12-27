#ifndef __drv_fb_h
#define __drv_fb_h
#include <stdint.h>
#define PHY_LCD_WIDTH       480
#define PHY_LCD_HEIGHT      272
enum VRAM_FORMAT
{
	VRAM_FORMAT_ARGB    =0,
	VRAM_FORMAT_RGB565,
	VRAM_FORMAT_RGB888,
};
enum COLORKEY_MODE
{
	COLOR_KEY_AMATCH    =1,
	COLOR_KEY_INSIDE,
	COLOR_KEY_OUTOFF,
};
enum LAYER_ENABLE
{
	LAYER_DISABLE    =0,
	LAYER_ENABLE,
};
enum HWC_ENABLE
{
	HWC_DISABLE    =0,
	HWC_ENABLE,
};
typedef struct
{
	uint32_t pixel_clock_hz;
	uint32_t width;
	uint32_t height;
	uint32_t h_front_porch;
	uint32_t h_back_porch;
	uint32_t h_sync_len;
	uint32_t v_front_porch;
	uint32_t v_back_porch;
	uint32_t v_sync_len;
	uint32_t h_sync_active;
	uint32_t v_sync_active;
	uint32_t den_active;
	uint32_t clk_active;
	uint32_t line_per_pixel;
	uint32_t rstdefe;
	uint32_t rstdebe;
	uint32_t rsttcon;
	uint32_t backcolor;
} lcd_cfg_Struct;
typedef struct 
{
	uint32_t width;
	uint32_t height;
	uint32_t posx;
	uint32_t posy;
	uint32_t priority;
	uint32_t pipe;
	uint32_t alpha_value;
	uint32_t alpha_enable;
	uint32_t vram_format;
	void * vram;	
}lcd_layer_Struct;
#define FB_RGB_ORDER 1// 0 : RGB 1: BGR
extern lcd_cfg_Struct lcd_cfg;
extern lcd_layer_Struct lcd_layer0;
extern lcd_layer_Struct lcd_layer1;
int board_lcd_init(void);
void lcd_SetBrightness(int32_t brightness);
void Layer_Config(uint8_t LayerNum,lcd_layer_Struct * pdat);
void Layer_SetPos(uint8_t LayerNum,uint32_t px,uint32_t py);
void Layer_SetAlpha(uint8_t LayerNum,uint32_t Alpha);
void Layer_SetEnable(uint8_t LayerNum,enum LAYER_ENABLE Enable);
void Layer_SetFramBuffer(uint8_t LayerNum,uint32_t BufferAddr);
void HWCursor_SetPos(uint32_t px,uint32_t py);
void HWCursor_Init(void);
void HWCursor_Enable(enum HWC_ENABLE Enable);
#endif