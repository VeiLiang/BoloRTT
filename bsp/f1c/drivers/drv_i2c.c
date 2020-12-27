/*
 * driver/i2c-f1c100s.c
 *
 * Copyright(c) 2007-2018 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include "drv_gpio.h"
#include "drv_i2c.h"

enum {
	I2C_ADDR			= 0x000,
	I2C_XADDR			= 0x004,
	I2C_DATA 			= 0x008,
	I2C_CNTR			= 0x00c,
	I2C_STAT			= 0x010,
	I2C_CCR				= 0x014,
	I2C_SRST			= 0x018,
	I2C_EFR				= 0x01c,
	I2C_LCR				= 0x020,
};

enum {
	I2C_STAT_BUS_ERROR	= 0x00,
	I2C_STAT_TX_START	= 0x08,
	I2C_STAT_TX_RSTART	= 0x10,
	I2C_STAT_TX_AW_ACK	= 0x18,
	I2C_STAT_TX_AW_NAK	= 0x20,
	I2C_STAT_TXD_ACK	= 0x28,
	I2C_STAT_TXD_NAK	= 0x30,
	I2C_STAT_LOST_ARB	= 0x38,
	I2C_STAT_TX_AR_ACK	= 0x40,
	I2C_STAT_TX_AR_NAK	= 0x48,
	I2C_STAT_RXD_ACK	= 0x50,
	I2C_STAT_RXD_NAK	= 0x58,
	I2C_STAT_IDLE		= 0xf8,
};

struct i2c_f1c100s_pdata_t {
	unsigned int virt;
};
struct i2c_f1c100s_pdata_t i2c_Data;
static i2c_delay(int us)
{
	volatile int i;
	while(us--)
	{
		for(i = 0;i< 600;i++)
		{
			;
		}
	}
}
static void f1c100s_i2c_set_rate(struct i2c_f1c100s_pdata_t * pdat, unsigned long long rate)
{
//	u64_t pclk = clk_f1c100s_pll_chanel_get_rate(5,24000000);//clk_get_rate(pdat->clk);
//	i2c_printf("i2c_src_clock get %d Hz\n",pclk);
//	s64_t freq, delta, best = 0x7fffffffffffffffLL;
//	int tm = 5, tn = 0;
//	int m, n;
//
//	for(n = 0; n <= 7; n++)
//	{
//		for(m = 0; m <= 15; m++)
//		{
//			freq = pclk / (10 * (m + 1) * (1 << n));
//			delta = rate - freq;
//			if(delta >= 0 && delta < best)
//			{
//				tm = m;
//				tn = n;
//				best = delta;
//			}
//			if(best == 0)
//				break;
//		}
//	}
	int tm = 2, tn = 5;
	//i2c_printf("I2C_CCR bit3:%d ,bit0:%d\n",tm,tn);
	i2c_write32(pdat->virt + I2C_CCR, ((tm & 0xf) << 3) | ((tn & 0x7) << 0));
}
static int f1c100s_i2c_wait_status(struct i2c_f1c100s_pdata_t * pdat)
{
	//ktime_t timeout = ktime_add_ms(ktime_get(), 1);

	unsigned int timeout = 100000;
	do {
		if((i2c_read32(pdat->virt + I2C_CNTR) & (1 << 3)))
			return i2c_read32(pdat->virt + I2C_STAT);
		i2c_delay(10);
	} while(--timeout);
	return I2C_STAT_BUS_ERROR;
}

static int f1c100s_i2c_start(struct i2c_f1c100s_pdata_t * pdat)
{
	i2c_write32(pdat->virt + I2C_CNTR, (1 << 7) | (1 << 6) | (1 << 5) | (1 << 2));

	//ktime_t timeout = ktime_add_ms(ktime_get(), 1);
	unsigned int timeout = 100000;
	do {
		if(!(i2c_read32(pdat->virt + I2C_CNTR) & (1 << 5)))
			break;
		i2c_delay(10);
	} while(--timeout);
	if(!timeout)
	{
		i2c_printf("f1c100s_i2c_start time out \n");
	}
	return f1c100s_i2c_wait_status(pdat);
}

static int f1c100s_i2c_stop(struct i2c_f1c100s_pdata_t * pdat)
{
	i2c_write32(pdat->virt + I2C_CNTR, (i2c_read32(pdat->virt + I2C_CNTR) | (1 << 4)) & ~((1 << 7) | (1 << 3)));
	//ktime_t timeout = ktime_add_ms(ktime_get(), 1);
	unsigned int timeout = 100000;
	do {
		if(!(i2c_read32(pdat->virt + I2C_CNTR) & (1 << 4)))
			return 0;
		i2c_delay(10);
	} while(--timeout);
	return -1;

}

static int f1c100s_i2c_send_data(struct i2c_f1c100s_pdata_t * pdat, unsigned char dat)
{
	i2c_write32(pdat->virt + I2C_DATA, dat);
	i2c_write32(pdat->virt + I2C_CNTR, i2c_read32(pdat->virt + I2C_CNTR) & (~(1 << 3)));
	return f1c100s_i2c_wait_status(pdat);
}

static int f1c100s_i2c_read(struct i2c_f1c100s_pdata_t * pdat, struct i2c_msg_t * msg)
{
	unsigned char * p = msg->buf;
	int len = msg->len;

	if(f1c100s_i2c_send_data(pdat, (unsigned char)(msg->addr << 1 | 1)) != I2C_STAT_TX_AR_ACK)
		return -1;

	while(len > 0)
	{
		switch(f1c100s_i2c_wait_status(pdat))
		{
			case I2C_STAT_TX_AR_ACK:
				if(len == 1)
					i2c_write32(pdat->virt + I2C_CNTR, i2c_read32(pdat->virt + I2C_CNTR) & ~((1 << 3) | (1 << 2)));
				else
					i2c_write32(pdat->virt + I2C_CNTR, i2c_read32(pdat->virt + I2C_CNTR) & ~(1 << 3));
				break;
			case I2C_STAT_RXD_ACK:
				*p++ = i2c_read32(pdat->virt + I2C_DATA);
				len--;
				if(len == 1)
					i2c_write32(pdat->virt + I2C_CNTR, i2c_read32(pdat->virt + I2C_CNTR) & ~((1 << 3) | (1 << 2)));
				else
					i2c_write32(pdat->virt + I2C_CNTR, i2c_read32(pdat->virt + I2C_CNTR) & ~(1 << 3));
				break;
			case I2C_STAT_RXD_NAK:
				*p++ = i2c_read32(pdat->virt + I2C_DATA);
				len--;
				break;
			default:
				return -1;
		}
	}
	return 0;
}

static int f1c100s_i2c_write(struct i2c_f1c100s_pdata_t * pdat, struct i2c_msg_t * msg)
{
	unsigned char * p = msg->buf;
	int len = msg->len;
	if(f1c100s_i2c_send_data(pdat, (unsigned char)(msg->addr << 1)) != I2C_STAT_TX_AW_ACK)
	{
		i2c_printf("f1c100s_i2c_write data failed\n");
		return -1;
	}
	while(len > 0)
	{
		if(f1c100s_i2c_send_data(pdat, *p++) != I2C_STAT_TXD_ACK)
		{
			i2c_printf("!= I2C_STAT_TXD_ACK\n");
			return -1;
		}
		len--;
	}
	return 0;
}

static int i2c_f1c100s_xfer(struct i2c_msg_t * msgs, int num)
{
	struct i2c_f1c100s_pdata_t * pdat = &i2c_Data;
	struct i2c_msg_t * pmsg = msgs;
	int i, res;

	if(!msgs || num <= 0)
		return 0;

	if(f1c100s_i2c_start(pdat) != I2C_STAT_TX_START)
		return 0;

	for(i = 0; i < num; i++, pmsg++)
	{
		if(i != 0)
		{
			if(f1c100s_i2c_start(pdat) != I2C_STAT_TX_RSTART)
				break;
		}
		if(pmsg->flags & I2C_M_RD)
			res = f1c100s_i2c_read(pdat, pmsg);
		else
			res = f1c100s_i2c_write(pdat, pmsg);
		if(res < 0)
			break;
	}
	f1c100s_i2c_stop(pdat);

	return i;
}

unsigned char i2c_reg_read(int ch,unsigned char addr,unsigned char reg )
{
	struct i2c_f1c100s_pdata_t * pdat = &i2c_Data;
	struct i2c_msg_t i2c_msg;
	unsigned char dat;
	int ret = -1;
	dat = reg;
	i2c_msg.addr = addr;
	i2c_msg.buf = &dat;
	i2c_msg.len = 1;
	if(f1c100s_i2c_start(pdat) != I2C_STAT_TX_START)
	{
		i2c_printf("f1c100s_i2c_start(pdat) != I2C_STAT_TX_START \n");
			return 0;
	}
	if(f1c100s_i2c_write(pdat, &i2c_msg))
	{
		f1c100s_i2c_stop(pdat);
		return 0;
	}
	dat = 0;
	if(f1c100s_i2c_start(pdat) != I2C_STAT_TX_RSTART)
	{
		f1c100s_i2c_stop(pdat);
		return 0;
	}
	if(f1c100s_i2c_read(pdat, &i2c_msg))
	{
		f1c100s_i2c_stop(pdat);
		return 0;
	}
	f1c100s_i2c_stop(pdat);
	return dat;
}
int i2c_reg_write(int ch,unsigned char addr,unsigned char reg ,unsigned char wdat)
{
	struct i2c_f1c100s_pdata_t * pdat = &i2c_Data;
	struct i2c_msg_t i2c_msg;
	unsigned char dat[4];
	int ret = -1;
	dat[0] = reg;
	dat[1] = wdat;
	i2c_msg.addr = addr;
	i2c_msg.buf = &dat[0];
	i2c_msg.len = 2;
	if(f1c100s_i2c_start(pdat) != I2C_STAT_TX_START)
	{
		return -3;
	}
	if(f1c100s_i2c_write(pdat, &i2c_msg))
	{
		f1c100s_i2c_stop(pdat);
		return -2;
	}
	f1c100s_i2c_stop(pdat);
	ret = 0;
	return ret;
}

void i2c_open_clock(int ch)
{
	i2c_printf("i2c_open_clock for i2c%d\n",ch);
	switch(ch)
	{
		case 0:
				i2c_set_bit(BUS_CLK_GATING_REG2,1<<TWI0_CLK_GATE_BIT);
				i2c_set_bit(BUS_SOFT_RST_REG2,1<<TWI0_CLK_RST_BIT);
				break;
		case 1:
				i2c_set_bit(BUS_CLK_GATING_REG2,1<<TWI1_CLK_GATE_BIT);
				i2c_set_bit(BUS_SOFT_RST_REG2,1<<TWI1_CLK_RST_BIT);
				break;
		case 2:
				i2c_set_bit(BUS_CLK_GATING_REG2,1<<TWI2_CLK_GATE_BIT);
				i2c_set_bit(BUS_SOFT_RST_REG2,1<<TWI2_CLK_RST_BIT);
				break;
		default:
				break;
	}

}
typedef struct {
	enum gpio_port port_sda;
	int pin_sda;
	enum gpio_port port_sck;
	int pin_sck;
	int func;
	int pull;
}i2c_gpio_stu;
int i2c_f1c100s_init(int ch)
{
	struct i2c_f1c100s_pdata_t * pdat = &i2c_Data;

	i2c_gpio_stu i2c_gpio[3]={
			{GPIO_PORT_E,12,GPIO_PORT_E,11,IO_FUN_2,PULL_DISABLE},
			{GPIO_PORT_D,6,GPIO_PORT_D,5,IO_FUN_2,PULL_DISABLE},
			{GPIO_PORT_E,1,GPIO_PORT_E,0,IO_FUN_3,PULL_DISABLE}
	};

	unsigned int virt = I2C0_BASE_ADDR + ch*0x400 ;

	pdat->virt = virt;

	i2c_printf("init gpio for i2c%d\n",ch);

	// gpio_f1c100s_set_cfg(&i2c_gpio[ch].port_sda, i2c_gpio[ch].pin_sda,i2c_gpio[ch].cfg);
	// gpio_f1c100s_set_pull(&i2c_gpio[ch].port_sda, i2c_gpio[ch].pin_sda, i2c_gpio[ch].pull);
	// gpio_f1c100s_set_cfg(&i2c_gpio[ch].port_sck, i2c_gpio[ch].pin_sck,i2c_gpio[ch].cfg);
	// gpio_f1c100s_set_pull(&i2c_gpio[ch].port_sck, i2c_gpio[ch].pin_sck, i2c_gpio[ch].pull);
 	gpio_set_func(i2c_gpio[ch].port_sda, i2c_gpio[ch].pin_sda, i2c_gpio[ch].func);
  	gpio_set_pull_mode(i2c_gpio[ch].port_sda, i2c_gpio[ch].pin_sda, i2c_gpio[ch].pull);
  	gpio_set_drive_level(i2c_gpio[ch].port_sda, i2c_gpio[ch].pin_sda, DRV_LEVEL_2);

	gpio_set_func(i2c_gpio[ch].port_sck, i2c_gpio[ch].pin_sck, i2c_gpio[ch].func);
  	gpio_set_pull_mode(i2c_gpio[ch].port_sck, i2c_gpio[ch].pin_sck, i2c_gpio[ch].pull);
  	gpio_set_drive_level(i2c_gpio[ch].port_sck, i2c_gpio[ch].pin_sck, DRV_LEVEL_2);

	i2c_open_clock(ch);

	i2c_write32(pdat->virt + I2C_SRST, 1 << 0);
	i2c_write32(pdat->virt + I2C_SRST, 0 << 0);
	f1c100s_i2c_set_rate(pdat, 400000);

	i2c_write32(pdat->virt + I2C_ADDR, 0);
	i2c_write32(pdat->virt + I2C_XADDR, 0);
	i2c_write32(pdat->virt + I2C_CNTR, (1 << 6) | (1 << 4));


	return 0;
}
void i2c_debugdump(void)
{
	struct i2c_f1c100s_pdata_t * pdat = &i2c_Data;
	i2c_printf("I2C_ADDR:0x%08x\n",i2c_read32(pdat->virt + I2C_ADDR));
	i2c_printf("I2C_XADDR:0x%08x\n",i2c_read32(pdat->virt + I2C_XADDR));
	i2c_printf("I2C_DATA:0x%08x\n",i2c_read32(pdat->virt + I2C_DATA));
	i2c_printf("I2C_CNTR:0x%08x\n",i2c_read32(pdat->virt + I2C_CNTR));

	i2c_printf("I2C_STAT:0x%08x\n",i2c_read32(pdat->virt + I2C_STAT));
	i2c_printf("I2C_CCR:0x%08x\n",i2c_read32(pdat->virt + I2C_CCR));
	i2c_printf("I2C_SRST:0x%08x\n",i2c_read32(pdat->virt + I2C_SRST));
	i2c_printf("I2C_EFR:0x%08x\n",i2c_read32(pdat->virt + I2C_EFR));
	i2c_printf("I2C_LCR:0x%08x\n",i2c_read32(pdat->virt + I2C_LCR));
}
