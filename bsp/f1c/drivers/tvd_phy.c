/*
 * tvd_phy.c
 *
 *  Created on: 2018年12月1日
 *      Author: VeiLiang
 */
#include"tvd_reg.h"
#include "drv_tvd.h"

void tvd_open_clock(void)
{
	//TVD_REG_SET_BIT(TVD_CLK_REG,TVD_CLK_REG_SCLK_BIT);
//	TVD_RegWrite(TVD_CLK_REG,(1<<TVD_CLK_REG_SCLK_BIT)
//														|(1<<TVD_CLK_REG_SLE_BIT)
//														|(1<<TVD_CLK_REG_DIV_BIT));//[32] sclk switch 1:on  [26:24] sclk sel  000:pll(1x) 001:osc24Mhz 010:pll(2x) [3:0]:sclk div ratio
	//TVD_RegWrite(PLL_VEDIO_CTRL_REG,0x83006207);
	TVD_REG_SET_BIT(DRAM_CLK_GATE_REG,DRAM_CLK_TVD_BIT);//open dram  clk for tvd
	TVD_RegWrite(TVD_CLK_REG,0x8000000a);
	TVD_REG_SET_BIT(BUS_CLK_GATE1_REG,BUS_CLK_TVD_BIT);//open ahb bus clk for tvd
	TVD_REG_SET_BIT(BUS_CLK_RST1_REG,BUS_RST_TVD_BIT);//de-assert ahb bus rst for tvd


}
static void msleep(int ms)
{
	volatile int i,j;
	while(ms--)
	{
		for(i = 0 ; i < 1000; i++)
		{
			for(j = 0 ; j < 2400;j++)
			{
				;
			}
		}
	}
}
void tvd_set_hor_start(__u32 id, __u32 h_start)
{
	__u32 reg_val;
	reg_val = TVD_RegRead(TVD_REG_BASE + 0x001c);
	reg_val &= ~(0xfff << 16);
	reg_val |= (h_start & 0xfff) << 16;
	TVD_RegWrite(TVD_REG_BASE + 0x001c, reg_val);
}

void tvd_set_ver_start(__u32 id, __u32 v_start)
{
	__u32 reg_val;
	reg_val = TVD_RegRead(TVD_REG_BASE + 0x001c);
	reg_val &= ~(0x7ff << 0);
	reg_val |= (v_start & 0x7ff) << 0;
	TVD_RegWrite(TVD_REG_BASE + 0x001c, reg_val);
}
__u32 tvd_get_hor_start(__u32 id)
{
	__u32 reg_val;
	reg_val = TVD_RegRead(TVD_REG_BASE + 0x001c);
	reg_val = (reg_val >> 16) & 0xfff;
	return reg_val;
}

__u32 tvd_get_ver_start(__u32 id)
{
	__u32 reg_val;
	reg_val = TVD_RegRead(TVD_REG_BASE + 0x001c);
	reg_val = reg_val & 0x7ff;
	return reg_val;
}

void tvd_set_width(unsigned int id,unsigned int w)
{
	unsigned int reg_val;
	reg_val = TVD_RegRead(TVD_REG_BASE +0x008c+0x100*id);//0x134+0x100*id
	reg_val &= ~(0xfff<<0);
	reg_val |= ((w>720)?720:w)<<0;
        TVD_RegWrite(TVD_REG_BASE+0x008c+0x100*id, reg_val);
}

void tvd_set_width_jump(unsigned int id,unsigned int j)
{
        TVD_RegWrite(TVD_REG_BASE+0x0138+0x100*id, j);
}

void tvd_set_height(unsigned int id,unsigned int h)
{
	unsigned int reg_val;
	reg_val = TVD_RegRead(TVD_REG_BASE + 0x008c+0x100*id);
	reg_val &= ~(0x7ff<<16);
	reg_val |= h<<16;
	//reg_val |= h<<16;
        TVD_RegWrite(TVD_REG_BASE+0x008c+0x100*id, reg_val);
}


void tvd_config(unsigned int interface, unsigned int system, unsigned int format)
{
	/* global reg set here */

	if (interface == TVD_SOURCE_CVBS) 
	{ /* composite */
        switch(system)
        {
            case TVD_SOURCE_NTSC:
                TVD_RegWrite(TVD_REG_BASE+0x0008, 0x00010001);
                TVD_RegWrite(TVD_REG_BASE+0x000c, 0x00202068); //adjust luma brightness
                TVD_RegWrite(TVD_REG_BASE+0x0010, 0x00300080); //just statruation and peak gain
                TVD_RegWrite(TVD_REG_BASE+0x0018, 0x21f07c1f); //21f0_7c1f    262E_8BA2 Chroma DTO                
                TVD_RegWrite(TVD_REG_BASE+0x001c, 0x00820022); //hactive and vactive start
                TVD_RegWrite(TVD_REG_BASE+0x0f08, 0x00590100); //notch width 0059_0100
                TVD_RegWrite(TVD_REG_BASE+0x0f0c, 0x00000010); //YC sep
                TVD_RegWrite(TVD_REG_BASE+0x0f10, 0x008A32DD); //sync height
                TVD_RegWrite(TVD_REG_BASE+0x0f14, 0x800000a0); //
                TVD_RegWrite(TVD_REG_BASE+0x0f1c, 0x008A0000); //chroma AGC target
                TVD_RegWrite(TVD_REG_BASE+0x0f2c, 0x0000CB74);
                TVD_RegWrite(TVD_REG_BASE+0x0f44, 0x00004632); //burst gate
                TVD_RegWrite(TVD_REG_BASE+0x0f74, 0x000003c3); //chroma edge enhance
                TVD_RegWrite(TVD_REG_BASE+0x0f80, 0x00500000); //hactive width
                TVD_RegWrite(TVD_REG_BASE+0x0f84, 0x00610000); //vactive height
                TVD_RegWrite(TVD_REG_BASE+0x0000, 0x00000001);//test = 0x80001f3f

                if( TVD_MB_YUV420==format )
                {
                    tvd_set_width(0,704);
                    tvd_set_width_jump(0,704);
                    tvd_set_height(0,224);
                    tvd_set_ver_start(0,0x22+(480-448)/2);
                }
                else
                {
                    tvd_set_width(0,720);
                    tvd_set_width_jump(0,720);
                    tvd_set_height(0,240);              
                }   

                break;
            case TVD_SOURCE_PAL:
                TVD_RegWrite(TVD_REG_BASE+0x0008, 0x01101001);
                TVD_RegWrite(TVD_REG_BASE+0x000c, 0x00202068); //adjust luma brightness
                TVD_RegWrite(TVD_REG_BASE+0x0010, 0x00300050); 
                TVD_RegWrite(TVD_REG_BASE+0x0018, 0x2a098acb); //chroma dto
                TVD_RegWrite(TVD_REG_BASE+0x001c, 0x0087002a); //hactive and vactive start     
                TVD_RegWrite(TVD_REG_BASE+0x0f08, 0x11590902); //disable black level correction for 7.5 blank-to-black setup cagc en
                TVD_RegWrite(TVD_REG_BASE+0x0f0c, 0x00000016); //YC sep
                TVD_RegWrite(TVD_REG_BASE+0x0f10, 0x008a32ec); //
                TVD_RegWrite(TVD_REG_BASE+0x0f14, 0x800000a0); //adjust YC delay               
                TVD_RegWrite(TVD_REG_BASE+0x0f1c, 0x00930000); //chroma AGC target
                TVD_RegWrite(TVD_REG_BASE+0x0f2c, 0x00000d74);             
                TVD_RegWrite(TVD_REG_BASE+0x0f44, 0x0000412d); //burst gate
                TVD_RegWrite(TVD_REG_BASE+0x0f74, 0x00000343); //      
                TVD_RegWrite(TVD_REG_BASE+0x0f80, 0x00500000); //hactive width
                TVD_RegWrite(TVD_REG_BASE+0x0f84, 0x00c10000); //vactive height    
                TVD_RegWrite(TVD_REG_BASE+0x0000, 0x00000001); //

                if( TVD_MB_YUV420==format )
                {
                    tvd_set_width(0,704);
                    tvd_set_width_jump(0,704);
                    tvd_set_height(0,288);              
                }
                else
                {
                    tvd_set_width(0,720);
                    tvd_set_width_jump(0,720);
                    tvd_set_height(0,288);              
                }							
                break;
            case TVD_SOURCE_PAL_M:
                TVD_RegWrite(TVD_REG_BASE+0x0008, 0x00002001);
                TVD_RegWrite(TVD_REG_BASE+0x000c, 0x00002080); 
                TVD_RegWrite(TVD_REG_BASE+0x0010, 0x00300080); //just statruation and peak gain                
                TVD_RegWrite(TVD_REG_BASE+0x0018, 0x21E6EFE3); //21f0_7c1f    262E_8BA2 Chroma DTO
                TVD_RegWrite(TVD_REG_BASE+0x001c, 0x00820022); //hactive and vactive start                                         
                TVD_RegWrite(TVD_REG_BASE+0x0f08, 0x00590100); //notch width 0059_0100
                TVD_RegWrite(TVD_REG_BASE+0x0f0c, 0x00000040);
                TVD_RegWrite(TVD_REG_BASE+0x0f10, 0x008A32DD); //sync height
                TVD_RegWrite(TVD_REG_BASE+0x0f14, 0x800000a0); //
                TVD_RegWrite(TVD_REG_BASE+0x0f1c, 0x008a0000); //chroma AGC target
                TVD_RegWrite(TVD_REG_BASE+0x0f2c, 0x0000CB74);             
                TVD_RegWrite(TVD_REG_BASE+0x0f44, 0x00004632); //burst gate    
                TVD_RegWrite(TVD_REG_BASE+0x0f74, 0x000003c3); //chroma edge enhance                   
                TVD_RegWrite(TVD_REG_BASE+0x0f80, 0x00500000); //hactive width
                TVD_RegWrite(TVD_REG_BASE+0x0f84, 0x00610000); //vactive height
                TVD_RegWrite(TVD_REG_BASE+0x0000, 0x00000001);//test = 0x80001f3f
                if( TVD_MB_YUV420==format )
                {
                    tvd_set_width(0,704);
                    tvd_set_width_jump(0,704);
                    tvd_set_height(0,224);  
                    TVD_RegWrite(TVD_REG_BASE+0x001c, 0x00820032); // vertical start 0x22->(0x22+(480-448)/2)
                }
                else
                {
                    tvd_set_width(0,720);
                    tvd_set_width_jump(0,720);
                    tvd_set_height(0,240);              
                }   
                break;
            case TVD_SOURCE_PAL_N:
                TVD_RegWrite(TVD_REG_BASE+0x0008, 0x01103001);
                TVD_RegWrite(TVD_REG_BASE+0x000c, 0x00002080); //adjust luma brightness    
                TVD_RegWrite(TVD_REG_BASE+0x0010, 0x00300080); //just statruation and peak gain    
                TVD_RegWrite(TVD_REG_BASE+0x0018, 0x21F69446); //chroma dto                
                TVD_RegWrite(TVD_REG_BASE+0x001c, 0x00870026); //hactive and vactive start                                     
                TVD_RegWrite(TVD_REG_BASE+0x0f08, 0x11590902); //disable black level correction for 7.5 blank-to-black setup cagc en
                TVD_RegWrite(TVD_REG_BASE+0x0f0c, 0x00000040);
                TVD_RegWrite(TVD_REG_BASE+0x0f10, 0x008a32ec); //
                TVD_RegWrite(TVD_REG_BASE+0x0f14, 0x800000a0); //adjust YC delay
                TVD_RegWrite(TVD_REG_BASE+0x0f1c, 0x00dc0000); //chroma AGC target
                TVD_RegWrite(TVD_REG_BASE+0x0f2c, 0x00000d74);             
                TVD_RegWrite(TVD_REG_BASE+0x0f44, 0x00004632); //burst gate
                TVD_RegWrite(TVD_REG_BASE+0x0f74, 0x00000343); //chroma peak
                TVD_RegWrite(TVD_REG_BASE+0x0f80, 0x00500000); //hactive width
                TVD_RegWrite(TVD_REG_BASE+0x0f84, 0x00c10000); //vactive height
                TVD_RegWrite(TVD_REG_BASE+0x0000, 0x00000001); //
                if( TVD_MB_YUV420==format )
                {
                    tvd_set_width(0,704);
                    tvd_set_width_jump(0,704);
                    tvd_set_height(0,288);              
                }
                else
                {
                    tvd_set_width(0,720);
                    tvd_set_width_jump(0,720);
                    tvd_set_height(0,288);              
                }   
                break;
            case TVD_SOURCE_SECAM:
                TVD_RegWrite(TVD_REG_BASE+0x0008, 0x01104001);
                TVD_RegWrite(TVD_REG_BASE+0x000c, 0x00002080); //adjust luma brightness                
                TVD_RegWrite(TVD_REG_BASE+0x0010, 0x003100b0);
                TVD_RegWrite(TVD_REG_BASE+0x0018, 0x28A33BB2); //chroma dto    
                TVD_RegWrite(TVD_REG_BASE+0x001c, 0x00870026); //hactive and vactive start                         
                TVD_RegWrite(TVD_REG_BASE+0x0f08, 0x11590902); //disable black level correction for 7.5 blank-to-black setup cagc en
                TVD_RegWrite(TVD_REG_BASE+0x0f0c, 0x00000040);
                TVD_RegWrite(TVD_REG_BASE+0x0f10, 0x008a32ec); //
                TVD_RegWrite(TVD_REG_BASE+0x0f14, 0x800000a0); //adjust YC delay
                TVD_RegWrite(TVD_REG_BASE+0x0f1c, 0x00dc0000); //chroma AGC target
                TVD_RegWrite(TVD_REG_BASE+0x0f2c, 0x00000d74);             
                TVD_RegWrite(TVD_REG_BASE+0x0f44, 0x00005036); //burst gate
                TVD_RegWrite(TVD_REG_BASE+0x0f74, 0x00000343); //chroma peak
                TVD_RegWrite(TVD_REG_BASE+0x0f80, 0x00500000); //hactive width
                TVD_RegWrite(TVD_REG_BASE+0x0f84, 0x00c10000); //vactive height
                TVD_RegWrite(TVD_REG_BASE+0x0000, 0x00000001); //
                if( TVD_MB_YUV420==format )
                {
                    tvd_set_width(0,704);
                    tvd_set_width_jump(0,704);
                    tvd_set_height(0,288);              
                }
                else
                {
                    tvd_set_width(0,720);
                    tvd_set_width_jump(0,720);
                    tvd_set_height(0,288);              
                }   

                break;

        }
	}
#if 0
	{
		__u32 v;
		v = TVD_RegRead(TVD_REG_BASE + 0x0f20);
		/* DebugPrintf("1-----0x%x\n", TVD_RegRead(TVD_REG_BASE+0x0f20));
		 */
		v &= ~((1 << 6) | (1 << 7));
		v |= ((0 << 6) | (1 << 7));
		TVD_RegWrite(TVD_REG_BASE + 0x0f20, v);
		/* DebugPrintf("1-----0x%x\n", TVD_RegRead(TVD_REG_BASE+0x0f20));
		 */
	}
#endif
}
void tvd_3d_comb_fliter(int en,int addr)
{
	unsigned int tmp;
	if(en)
	{
		TVD_RegWrite(TVD_REG_BASE+0x004c,addr);
		TVD_RegWrite(TVD_REG_BASE+0x0050,addr+0x00200000);
		TVD_RegWrite(TVD_REG_BASE+0x0054,0x00200000);
		TVD_RegWrite(TVD_REG_BASE+0x0048,0x04040001);
		tmp = TVD_RegRead(TVD_REG_BASE+0x0008);
		tmp &= 0xfffffffe;
		TVD_RegWrite(TVD_REG_BASE+0x0008,tmp);
		tmp = TVD_RegRead(TVD_REG_BASE+0x0000);
		tmp |= 0x00000010;
		TVD_RegWrite(TVD_REG_BASE+0x0000,tmp);
	}
	else
	{
		tmp = TVD_RegRead(TVD_REG_BASE+0x0008);
		tmp |= 0x00000001;
		TVD_RegWrite(TVD_REG_BASE+0x0008,tmp);
		tmp = TVD_RegRead(TVD_REG_BASE+0x0000);
		tmp &= ~0x00000010;
		TVD_RegWrite(TVD_REG_BASE+0x0000,tmp);
	}
}

void tvd_irq_enable(tvd_irq_t irq)
{
	unsigned int reg_val;
	switch(irq){
		case TVD_FRAME_DONE:
			reg_val = TVD_RegRead(TVD_REG_BASE + 0x009c);
			reg_val |= 1<<(24);
			TVD_RegWrite(TVD_REG_BASE + 0x009c, reg_val);
			break;
		case TVD_LOCK:
			reg_val = TVD_RegRead(TVD_REG_BASE + 0x00a4);
			reg_val |= 1 << (0);
			TVD_RegWrite(TVD_REG_BASE + 0x00a4, reg_val);
			break;
		case TVD_UNLOCK:
			reg_val = TVD_RegRead(TVD_REG_BASE + 0x00a4);
			reg_val |= 1 << (1);
			TVD_RegWrite(TVD_REG_BASE + 0x00a4, reg_val);
			break;
		default:
			break;
	}
}

void tvd_irq_disable(tvd_irq_t irq)
{
	unsigned int reg_val;
	switch(irq)
	{
		case TVD_FRAME_DONE:
			reg_val = TVD_RegRead(TVD_REG_BASE + 0x009c);// + 0x148
			reg_val &= ~(1<<(24));
			TVD_RegWrite(TVD_REG_BASE + 0x009c, reg_val);
			break;
		case TVD_LOCK:
			reg_val = TVD_RegRead(TVD_REG_BASE + 0x00a4);
			reg_val &= ~(1 << 0);
			TVD_RegWrite(TVD_REG_BASE + 0x00a4, reg_val);
			break;
		case TVD_UNLOCK:
			reg_val = TVD_RegRead(TVD_REG_BASE + 0x00a4);
			reg_val &= ~(1 << 1);
			TVD_RegWrite(TVD_REG_BASE + 0x00a4, reg_val);
			break;
		default:
				break;
	}
}

int tvd_irq_status_get(tvd_irq_t irq)
{
	unsigned int reg_val, ret = -1;
	switch(irq){
		case TVD_FRAME_DONE:
			reg_val = TVD_RegRead(TVD_REG_BASE+0x0094);//+0x140
			ret = (reg_val>>(24))&1;
			break;
                default:
                	TVD_Printf("tvd get irq status error\n");
                        break;
	}
	return ret;
}

void tvd_irq_status_clear(tvd_irq_t irq)
{
	unsigned int reg_val;
	switch(irq){
		case TVD_FRAME_DONE:
			reg_val = 1<<(24);
			TVD_RegWrite(TVD_REG_BASE+0x0094, reg_val);//+0x140
			break;
		case TVD_LOCK:
			reg_val = 1 << 0;
			TVD_RegWrite(TVD_REG_BASE + 0x00a8, reg_val);
			break;
		case TVD_UNLOCK:
			reg_val = 1 << 1;
			TVD_RegWrite(TVD_REG_BASE + 0x00a8, reg_val);
			break;
		default:
				break;
	}
}

void tvd_capture_on(unsigned int id)
{
	unsigned int reg_val;
	reg_val = TVD_RegRead(TVD_REG_BASE+0x0088+0x100*id);//+0x130
	reg_val |= 1 << 0;
	TVD_RegWrite(TVD_REG_BASE+0x0088+0x100*id, reg_val);
}
void tvd_capture_off(unsigned int id)
{
	unsigned int reg_val;
	reg_val = TVD_RegRead(TVD_REG_BASE+0x0088+0x100*id);
	reg_val &= ~(1<<0);
	TVD_RegWrite(TVD_REG_BASE+0x0088+0x100*id, reg_val);
}

void tvd_set_addr_y(unsigned int id,unsigned int addr)
{
	unsigned int tmp;
	TVD_RegWrite(TVD_REG_BASE + 0x0080, addr);//+ 0x100
	tmp = TVD_RegRead(TVD_REG_BASE+0x0088);
	tmp |= 0x10000000;
	TVD_RegWrite(TVD_REG_BASE+0x0088,tmp);
}

void tvd_set_addr_c(unsigned int id,unsigned int addr)
{
	unsigned int tmp;
	TVD_RegWrite(TVD_REG_BASE + 0x0084, addr);//+ 0x110
	tmp = TVD_RegRead(TVD_REG_BASE+0x0088);
	tmp |= 0x10000000;
	TVD_RegWrite(TVD_REG_BASE+0x0088,tmp);
}

void tvd_set_fmt(unsigned int id, tvd_fmt_t fmt)
{
	unsigned int reg_val;
	reg_val = TVD_RegRead(TVD_REG_BASE + 0x0088+0x100*id);//+ 0x130
	switch(fmt){
		case TVD_PL_YUV422:
			reg_val &= ~(1<<24);
			reg_val |= 1<<4;
			break;
		case TVD_PL_YUV420:
			reg_val &= ~(1<<24);
			reg_val &= ~(1<<4);
			break;
		case TVD_MB_YUV420:
			reg_val |= 1<<24;
			reg_val &= ~(1<<4);
			break;
	}
	TVD_RegWrite(TVD_REG_BASE + 0x0088+0x100*id, reg_val);
}

unsigned int tvd_get_status(unsigned int id)
{
	unsigned int reg_val = 0;
	unsigned int det = 0;
	unsigned int system = 0;
	reg_val = TVD_RegRead(TVD_REG_BASE+0x0e40);
	det = ((reg_val & 0x0000000e) == 0x0000000e)?1:0;
	if((reg_val&(1<<18)) && (reg_val & (1 << 16)))
	{
		system = 1;//get system = pal
	}
	else
	{
		system = 0;//get system = ntsc
	}
	return ((det<<0)+(system<<4));//bit0=det bit4=system
}

void TVD_3D_COMB_Filter(__u32 enable, __u32 addr)
{
	__u32 reg_val;

	if (enable) 
	{
		/* 3D config */
		TVD_RegWrite(TVD_REG_BASE + 0x004c, addr);
		TVD_RegWrite(TVD_REG_BASE + 0x0050, addr + 0x200000);
		TVD_RegWrite(TVD_REG_BASE + 0x0054, 0x00200000);
		TVD_RegWrite(TVD_REG_BASE + 0x0048, 0x04040001);
		reg_val = TVD_RegRead(TVD_REG_BASE + 0x0008);
		reg_val &= ~(1 << 0);
		TVD_RegWrite(TVD_REG_BASE + 0x0008, reg_val);
		reg_val = TVD_RegRead(TVD_REG_BASE + 0x0000);
		reg_val |= (1 << 4);
		TVD_RegWrite(TVD_REG_BASE + 0x0000, reg_val);
	} 
	else 
	{
		reg_val = TVD_RegRead(TVD_REG_BASE + 0x0008);
		reg_val |= (1 << 0);
		TVD_RegWrite(TVD_REG_BASE + 0x0008, reg_val);
		reg_val = TVD_RegRead(TVD_REG_BASE + 0x0000);
		reg_val &= ~(1 << 4);
		TVD_RegWrite(TVD_REG_BASE + 0x0000, reg_val);
	}
}

void tvd_y_peak(__u32 enable)
{
	__u32 reg_val;
	if (enable) {
		reg_val = TVD_RegRead(TVD_REG_BASE + 0x000c);
		reg_val |= (1 << 16);
		TVD_RegWrite(TVD_REG_BASE + 0x000c, reg_val);
	} else {
		reg_val = TVD_RegRead(TVD_REG_BASE + 0x000c);
		reg_val &= ~(1 << 16);
		TVD_RegWrite(TVD_REG_BASE + 0x000c, reg_val);
	}
}
void tvd_c_peak(__u32 enable)
{
	__u32 reg_val;
	if (enable) {
		reg_val = TVD_RegRead(TVD_REG_BASE + 0x0010);
		reg_val |= (1 << 16);
		TVD_RegWrite(TVD_REG_BASE + 0x0010, reg_val);
	} else {
		reg_val = TVD_RegRead(TVD_REG_BASE + 0x0010);
		reg_val &= ~(1 << 16);
		TVD_RegWrite(TVD_REG_BASE + 0x0010, reg_val);
	}
}
__s32 tvd_get_lock_state(void)
{
	__u32 reg_val;
	__u32 lock = 0;
	reg_val = TVD_RegRead(TVD_REG_BASE + 0x00a8);

	if (reg_val & (1 << 0))
		lock = 1;/* lock */
	else if (reg_val & (1 << 1))
		lock = 0;/* unlock */
	return lock;
}

__s32 tvd_get_tvin_system()
{
	__u32 reg_val = 0;
	__u32 system = 0;

	reg_val = TVD_RegRead(TVD_REG_BASE + 0x0e40);
	if (reg_val & (1 << 18)) {
		system = TVD_SOURCE_PAL; /* get system = pal */
	} else {
		system = TVD_SOURCE_NTSC; /* get system = ntsc */
	}

	return system;
}

void tvd_set_color(unsigned int id,unsigned int luma,unsigned int contrast,unsigned int saturation,unsigned int hue)
{
        unsigned int reg_val=0;
        reg_val  = TVD_RegRead(TVD_REG_BASE+0x000c);//0x0018
        reg_val &= 0xffff0000;
        reg_val |= contrast;
        reg_val |= luma<<8;
        TVD_RegWrite(TVD_REG_BASE + 0x000c, reg_val);


        reg_val  = TVD_RegRead(TVD_REG_BASE+0x0010);
        reg_val &= 0xffff0000;
        reg_val |= saturation;
        reg_val |= hue<<8;
        TVD_RegWrite(TVD_REG_BASE + 0x0010, reg_val);//0x001c
}

// value from 00~ff
int tvd_set_chrom_gain(int val)
{
    int v = TVD_RegRead(TVD_REG_BASE+0x0f1c);

    v &= ~(0xff<<16);
    val &= 0xff;
    v |= (val<<16);
    TVD_RegWrite(TVD_REG_BASE+0x0f1c, v);

    return 0;
}

///flag: 0: 取消蓝屏，检测无信号也照样不蓝屏  1:直接输出蓝屏，不检测信号， 2:有信号则正常显示，无信号显示蓝屏
int tvd_set_blue_flag(int flag)
{
//	return;

#if 1
    int v = TVD_RegRead(TVD_REG_BASE+0xf14);//对应R40 0x0004
    v &= ~(0x3<<4);
    v |= ((flag&0x3)<<4);
    TVD_RegWrite(TVD_REG_BASE+0xf14, v);

    return 0;
#endif
}

//void tvd_uv_swap(unsigned char uv_swap)
//{
//        unsigned int reg_val=0;
//        if(uv_swap)
//        {
//                reg_val  = TVD_RegRead(TVD_REG_BASE+0x0014);
//                reg_val &= 0xffffffbf;
//                reg_val |= 1<<6;
//                TVD_RegWrite(TVD_REG_BASE + 0x0014, reg_val);
//        }
//        else
//        {
//                reg_val  = TVD_RegRead(TVD_REG_BASE+0x0014);
//                reg_val &= 0xffffffbf;
//                TVD_RegWrite(TVD_REG_BASE + 0x0014, reg_val);
//        }
//}
#define TVD_NUM 1
void tvd_bsp_init(int id)
{
	tvd_open_clock();
	TVD_RegWrite(TVD_REG_BASE + 0x0088, 0x04000000);
	TVD_RegWrite(TVD_REG_BASE + 0x0070, 0x00000100);
	/*select tvin0 or tvin1*/
	if (id == 0)
		TVD_RegWrite(TVD_REG_BASE + 0x0e04, 0x8002AAA8);
	else
		TVD_RegWrite(TVD_REG_BASE + 0x0e04, 0x8002AAA9);

	TVD_RegWrite(TVD_REG_BASE + 0x0e2c, 0x00110000);//0x00110000  0x000b0000
	TVD_RegWrite(TVD_REG_BASE + 0x0040, 0x04000310); /*? */

	/* reset tvd */
	TVD_RegWrite(TVD_REG_BASE + 0x0000, 0x00000000);
	msleep(1); /* need delay here for tvd reset */

	TVD_RegWrite(TVD_REG_BASE + 0x0000, 0x00001f01);//0x00001f00 0x00001f01

	/* IP config */
	TVD_RegWrite(TVD_REG_BASE + 0x0014,
		 0x20000000); /* 2000_0000    2400_0000 */

	/* TVD_RegWrite(TVD_REG_BASE+0x0f6c, 0x00fffa0a); //YC separation config */
	/*		TVD_RegWrite(TVD_REG_BASE+0x0f44, 0x00004632); //burst gate
	 */
	TVD_RegWrite(TVD_REG_BASE + 0x0f24,
		 0x0682810a); /* DISABLE AGC GATE KILL!!!!!!!!!!!!!!!!! */
	/*TVD_RegWrite(TVD_REG_BASE+0x0f24, 0x0000b90a); //DISABLE AGC
	 */
	/*GATE KILL!!!!!!!!!!!!!!!!! */
	TVD_RegWrite(TVD_REG_BASE + 0x0f28,
		 0x00006440); /* 0000_6440    0000_5838 */
	TVD_RegWrite(TVD_REG_BASE + 0x0f4c,
		 0x0e70106c); /* NO CLAMP DURING VSYNC!!!!!!!!!!!!!!!!! */
	TVD_RegWrite(TVD_REG_BASE + 0x0f54, 0x00000000); /* */
	TVD_RegWrite(TVD_REG_BASE + 0x0f58, 0x00000082); /* */
	TVD_RegWrite(TVD_REG_BASE + 0x0f6c, 0x00fffad0); /* YC separation config */
	TVD_RegWrite(TVD_REG_BASE + 0x0f70, 0x0000a010); /* 0x00002010); */

}

void tvd_debugdump(int id)
{
	// int i;
//	 for( i = 0 ; i < 0xfff; i+=4 )
//		{
//			//TVD_Printf("Base+%03x : 0x%08x\n",i,TVD_RegRead(TVD_REG_BASE+0x0008));
//			TVD_RegWrite(TVD_REG_BASE + i, 0xffffffff);
//			 msleep(1);
//			 printf("%d\n",i);
//		}



	TVD_Printf("TVD_CLK_REG: 0x%08x\n",TVD_RegRead(TVD_CLK_REG));
	TVD_Printf("BUS_CLK_RST1_REG: 0x%08x\n",TVD_RegRead(BUS_CLK_RST1_REG));
	TVD_Printf("BUS_CLK_GATE1_REG: 0x%08x\n",TVD_RegRead(BUS_CLK_GATE1_REG));
	TVD_Printf("DRAM_CLK_GATE_REG: 0x%08x\n",TVD_RegRead(DRAM_CLK_GATE_REG));
	TVD_Printf("PLL_VEDIO_CTRL_REG: 0x%08x\n",TVD_RegRead(PLL_VEDIO_CTRL_REG));

	volatile unsigned int	addr ;
	volatile unsigned int reg_vule ;
	for(addr=0x01c0b000 ; addr <=(0x01c0b000+0xfc) ; addr = addr+4 )
	{
		reg_vule = *(volatile unsigned int*)addr ;
		TVD_Printf("tvd reg[%x]=%8x\n",addr,reg_vule);
	}
	for(addr=0x01c0be00 ; addr <=(0x01c0be00+0xfc) ; addr = addr+4 )
	{
		reg_vule = *(volatile unsigned int*)addr ;
		TVD_Printf("tvd reg[%x]=%8x\n",addr,reg_vule);
	}
//	for( i = 0 ; i < 0xfff;i+=4 )
//	{
//		TVD_Printf("Base+%03x : 0x%08x\n",i,TVD_RegRead(TVD_REG_BASE+i));
//	}

//	TVD_Printf("Base+0x0000 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0000));
//	TVD_Printf("Base+0x0F08 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F08));
//	TVD_Printf("Base+0x0F0c : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F0c));
//	TVD_Printf("Base+0x0F10 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F10));
//	TVD_Printf("Base+0x0F14 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F14));
//	TVD_Printf("Base+0x0F18 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F18));
//	TVD_Printf("Base+0x0F1c : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F1c));
//	TVD_Printf("Base+0x0F24 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F24));
//	TVD_Printf("Base+0x0F28 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F28));
//	TVD_Printf("Base+0x0F2c : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F2c));
//	TVD_Printf("Base+0x0F44 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F44));
//	TVD_Printf("Base+0x0F4c : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F4c));
//	TVD_Printf("Base+0x0F54 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F54));
//	TVD_Printf("Base+0x0F6c : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F6c));
//	TVD_Printf("Base+0x0F74 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F74));
//	TVD_Printf("Base+0x0F80 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F80));
//	TVD_Printf("Base+0x0F84 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0F84));
//
//	TVD_Printf("Base+0x000c : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x000c));
//	TVD_Printf("Base+0x0014 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0014));
//	TVD_Printf("Base+0x008c : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x008c));
//	TVD_Printf("Base+0x0E2c : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0E2c));
//
//	TVD_Printf("Base+0x0094 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0094));
//	TVD_Printf("Base+0x009c : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x009c));
//
//	TVD_Printf("Base+0x0080 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0080));
//	TVD_Printf("Base+0x0084 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0084));
//
//	TVD_Printf("Base+0x0088 : 0x%08x\n",TVD_RegRead(TVD_REG_BASE+0x0088));

}
