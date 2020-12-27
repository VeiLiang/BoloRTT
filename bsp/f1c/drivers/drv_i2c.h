/*
 * drv_i2c.h
 *
 *  Created on: 2019Äê7ÔÂ7ÈÕ
 *      Author: VeiLiang
 */

#ifndef _DRV_I2C_H_
#define _DRV_I2C_H_


enum {
	I2C_M_TEN			= 0x0010,
	I2C_M_RD			= 0x0001,
	I2C_M_STOP			= 0x8000,
	I2C_M_NOSTART		= 0x4000,
	I2C_M_REV_DIR_ADDR	= 0x2000,
	I2C_M_IGNORE_NAK	= 0x1000,
	I2C_M_NO_RD_ACK		= 0x0800,
	I2C_M_RECV_LEN		= 0x0400,
};

struct i2c_msg_t {
	int addr;
	int flags;
	int len;
	void * buf;
};

#define I2C0_BASE_ADDR 0x01c27000
#define I2C1_BASE_ADDR 0x01c27400
#define I2C2_BASE_ADDR 0x01c27800

#define i2c_printf rt_kprintf

#define  i2c_read32(reg)	                    (*(volatile unsigned long *)(reg))

#define  i2c_write32(reg, value)	            (*(volatile unsigned long *)(reg) = (value))

#define  i2c_set_bit(reg, bp) 	 		(i2c_write32((reg),(i2c_read32(reg) | ((bp)))  ))
#define  i2c_clr_bit(reg, bp)	 	 	(i2c_write32((reg),(i2c_read32(reg) & (~ ((bp))))  ))

#define F1C100S_CLK_BASE  0x01C20000
#define BUS_CLK_GATING_REG2 (F1C100S_CLK_BASE+0x068)
#define TWI0_CLK_GATE_BIT 16
#define TWI1_CLK_GATE_BIT 17
#define TWI2_CLK_GATE_BIT 18

#define BUS_SOFT_RST_REG2 (F1C100S_CLK_BASE+0x2d0)
#define TWI0_CLK_RST_BIT 16
#define TWI1_CLK_RST_BIT 17
#define TWI2_CLK_RST_BIT 18

int i2c_reg_write(int ch,unsigned char addr,unsigned char reg ,unsigned char wdat);
unsigned char i2c_reg_read(int ch,unsigned char addr,unsigned char reg );


#endif /* _DRV_I2C_H_ */
