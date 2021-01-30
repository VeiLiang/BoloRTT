#ifndef _NES_COMMON_H_
#define _NES_COMMON_H_
#include "rtthread.h"
#include "stdlib.h"
#define LCD_WIDTH 480
#define LCD_HEIGHT 272
#define LCD_NES_XOFFSET 100
#define LCD_NES_YOFFSET 0
typedef signed char				s8_t;
typedef unsigned char			u8_t;

typedef signed short			s16_t;
typedef unsigned short			u16_t;

typedef signed int				s32_t;
typedef unsigned int			u32_t;

typedef signed long long		s64_t;
typedef unsigned long long		u64_t;

typedef signed char				s8;
typedef unsigned char			u8;

typedef signed short			s16;
typedef unsigned short			u16;

typedef signed int				s32;
typedef unsigned int			u32;

typedef signed long long		s64;
typedef unsigned long long		u64;

#define SRAMEX 0
#define SRAMIN 1

inline void *mymalloc(int s,int size)
{
	return malloc(size);
}
inline void myfree(int s,void * d)
{
    free(d);
}	 

inline void mymemset(void *s,u8 c,u32 count)  
{  
    u8 *xs = s;  
    while(count--)*xs++=c;  
}
#endif