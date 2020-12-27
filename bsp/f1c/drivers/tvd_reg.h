/*
 * tvd_reg.h
 *
 *  Created on: 2018Äê12ÔÂ1ÈÕ
 *      Author: VeiLiang
 */

#ifndef DRIVER_INCLUDE_tvd_REG_H_
#define DRIVER_INCLUDE_tvd_REG_H_

#define TVD_RegRead(reg)    (*((volatile unsigned int *)(reg)))
#define TVD_RegWrite(reg, value) (*((volatile unsigned int *)(reg))  = (value))

#define  TVD_REG_SET_BIT( reg,bp) 	 		(TVD_RegWrite((reg),(TVD_RegRead(reg) | (1 << (bp)))  ))
#define  TVD_REG_CLR_BIT(reg,bp)	 	 	(TVD_RegWrite( (reg) ,(TVD_RegRead(reg) & (~ (1 << (bp))))))



#define TVD_REG_BASE	(0x01C0B000)



#define TVD_CLK_REG	(0x01c20124)
#define TVD_CLK_REG_SCLK_BIT 31
#define TVD_CLK_REG_SLE_BIT 24
#define TVD_CLK_REG_DIV_BIT 0

#define PLL_VEDIO_CTRL_REG (0x01c20010)

#define BUS_CLK_RST1_REG	 0x01c202c4 //Bus Clock Reset Register Bit9 : TV in Decoder
#define BUS_RST_TVD_BIT	9

#define BUS_CLK_GATE1_REG	0x01c20064 //Bus Clock Gating Register Bit9 : TV in Decoder
#define BUS_CLK_TVD_BIT	9

#define DRAM_CLK_GATE_REG 0x01c20100
#define DRAM_CLK_TVD_BIT  3  //Dram Clock Gating Register Bit3 : TV in Decoder


//#define
#define TVD_COM_CTRL_REG_o			0x0130
#define TVD_W_H_SET_REG_o			0x0134





//int tvd_set_mode(tvd_mode_t mode);

//void tvd_det_enable();
//void tvd_det_disable();
//unsigned char tvd_det_finish();
//tvd_mode_t tvd_det_mode();




#endif /* DRIVER_INCLUDE_TVD_REG_H_ */
