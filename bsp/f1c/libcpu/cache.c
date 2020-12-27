#include "rtthread.h"
#define isb()		__asm__ __volatile__ ("" : : : "memory")
#define dsb()		__asm__ __volatile__ ("mcr p15, 0, %0, c7, c10,  4" : : "r" (0) : "memory")
#define dmb()		__asm__ __volatile__ ("" : : : "memory")
static inline uint32_t get_cache(void)
{
	uint32_t cache;

	__asm__ __volatile__("mrc p15, 0, %0, c0, c0, 1" : "=r" (cache));
	return cache;
}

static inline void __v5_cache_flush_range(uint32_t start, uint32_t stop, uint32_t line)
{
	uint32_t mva;

	start &= ~(line - 1);
	if(stop & (line - 1))
		stop = (stop + line) & ~(line - 1);
	for(mva = start; mva < stop; mva = mva + line)
	{
		__asm__ __volatile__("mcr p15, 0, %0, c7, c14, 1" : : "r" (mva));
	}
}

static inline void __v5_cache_inv_range(uint32_t start, uint32_t stop, uint32_t line)
{
	uint32_t mva;

	start &= ~(line - 1);
	if(stop & (line - 1))
		stop = (stop + line) & ~(line - 1);
	for(mva = start; mva < stop; mva = mva + line)
	{
		__asm__ __volatile__("mcr p15, 0, %0, c7, c6, 1" : : "r" (mva));
	}
}

/*
 * Flush range(clean & invalidate), affects the range [start, stop - 1]
 */
void cache_flush_range(unsigned long start, unsigned long stop)
{
	uint32_t cache;
	uint32_t line;

	cache = get_cache();
	line = 1 << ((cache & 0x3) + 3);
	__v5_cache_flush_range(start, stop, line);
	dsb();
}

/*
 * Invalidate range, affects the range [start, stop - 1]
 */
void cache_inv_range(unsigned long start, unsigned long stop)
{
	uint32_t cache;
	uint32_t line;

	cache = get_cache();
	line = 1 << ((cache & 0x3) + 3);
	__v5_cache_inv_range(start, stop, line);
	dsb();
}