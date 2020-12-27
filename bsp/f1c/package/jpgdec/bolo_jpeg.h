#ifndef _BOLO_JPEG_H_
#define _BOLO_JPEG_H_

#define __inline inline

#define LCD_TYPE_ORDER_RGB 0
#define LCD_TYPE_ORDER_BGR 1
#define LCD_TYPE_ORDER_GBR 2
#define LCD_TYPE_ORDER_RBG 3
#define LCD_TYPE_ORDER_BRG 4
#define LCD_TYPE_ORDER_GRB 5





void jpeg_decoder_init(void);
int jpeg_decode_main(unsigned char* src, unsigned int inlen ,unsigned char *y_buf, unsigned char* c_buf,
						int *w,int *h);
int jpeg_yuv_mb32_to_rgb888(unsigned char* pYbuf,unsigned char* pCbuf, unsigned char* pData,
            int width,int height,unsigned int type);
            
#endif