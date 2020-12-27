#ifndef __BOLO_IO_H__
#define __BOLO_IO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

static inline uint8_t read8(uint32_t addr)
{
	return( *((volatile uint8_t *)(addr)) );
}

static inline uint16_t read16(uint32_t addr)
{
	return( *((volatile uint16_t *)(addr)) );
}

static inline uint32_t read32(uint32_t addr)
{
	return( *((volatile uint32_t *)(addr)) );
}

static inline uint64_t read64(uint32_t addr)
{
	return( *((volatile uint64_t *)(addr)) );
}

static inline void write8(uint32_t addr, uint8_t value)
{
	*((volatile uint8_t *)(addr)) = value;
}

static inline void write16(uint32_t addr, uint16_t value)
{
	*((volatile uint16_t *)(addr)) = value;
}

static inline void write32(uint32_t addr, uint32_t value)
{
	*((volatile uint32_t *)(addr)) = value;
}

static inline void write64(uint32_t addr, uint64_t value)
{
	*((volatile uint64_t *)(addr)) = value;
}

#define bolo_reg_read(reg)      ( *((volatile uint32_t *)(reg)) )
#define bolo_reg_write(reg,vla) (*((volatile uint32_t *)(reg)) = vla)

#define  bolo_set_bit(reg, bp) 	 		(write32((reg),(read32(reg) | ((bp)))  ))
#define  bolo_clr_bit(reg, bp)	 	 	(write32((reg),(read32(reg) & (~ ((bp))))  ))

#define  bolo_set_shift(reg, bp) 	 		(write32((reg),(read32(reg) | ((1<<bp)))  ))
#define  bolo_clr_shift(reg, bp)	 	 	(write32((reg),(read32(reg) & (~ ((1<<bp))))  ))

#define  bolo_clr_and_set_bit(reg,mask, bp) 	 do {											\
														unsigned int _r = read32(reg);	\
														_r &=  (~(unsigned int)(mask));			\
														_r |=   ((bp) & (mask));	   	   	   	\
														write32((reg),_r); 				\
												}while(0)




uint32_t phys_to_virt(uint32_t phys);
uint32_t virt_to_phys(uint32_t virt);

#ifdef __cplusplus
}
#endif

#endif /* __IO_H__ */
