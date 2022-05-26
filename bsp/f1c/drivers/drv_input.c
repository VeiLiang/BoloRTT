#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>

#include "drv_audio.h"
#include "drv_gpio.h"

#define NES_KEY_UP_PORT GPIO_PORT_E
#define NES_KEY_UP_PIN GPIO_PIN_4
#define NES_KEY_DO_PORT GPIO_PORT_E
#define NES_KEY_DO_PIN GPIO_PIN_8
#define NES_KEY_LF_PORT GPIO_PORT_E
#define NES_KEY_LF_PIN GPIO_PIN_7
#define NES_KEY_RG_PORT GPIO_PORT_E
#define NES_KEY_RG_PIN GPIO_PIN_5
#define NES_KEY_SA_PORT GPIO_PORT_E
#define NES_KEY_SA_PIN GPIO_PIN_3
#define NES_KEY_SB_PORT GPIO_PORT_E
#define NES_KEY_SB_PIN GPIO_PIN_2
#define NES_KEY_AA_PORT GPIO_PORT_E
#define NES_KEY_AA_PIN GPIO_PIN_0
#define NES_KEY_BB_PORT GPIO_PORT_E
#define NES_KEY_BB_PIN GPIO_PIN_1
#define NES_KEY_ST_PORT GPIO_PORT_C
#define NES_KEY_ST_PIN GPIO_PIN_3
#define NES_KEY_SE_PORT GPIO_PORT_C
#define NES_KEY_SE_PIN GPIO_PIN_2
void hw_gamepad_init(void)
{
	gpio_set_func(NES_KEY_UP_PORT, NES_KEY_UP_PIN, IO_INPUT);
	gpio_set_func(NES_KEY_DO_PORT, NES_KEY_DO_PIN, IO_INPUT);
	gpio_set_func(NES_KEY_LF_PORT, NES_KEY_LF_PIN, IO_INPUT);
	gpio_set_func(NES_KEY_RG_PORT, NES_KEY_RG_PIN, IO_INPUT);
	gpio_set_func(NES_KEY_SA_PORT, NES_KEY_SA_PIN, IO_INPUT);
	gpio_set_func(NES_KEY_SB_PORT, NES_KEY_SB_PIN, IO_INPUT);
	gpio_set_func(NES_KEY_AA_PORT, NES_KEY_AA_PIN, IO_INPUT);
	gpio_set_func(NES_KEY_BB_PORT, NES_KEY_BB_PIN, IO_INPUT);
	gpio_set_func(NES_KEY_ST_PORT, NES_KEY_ST_PIN, IO_INPUT);
	gpio_set_func(NES_KEY_SE_PORT, NES_KEY_SE_PIN, IO_INPUT);
	gpio_set_pull_mode(NES_KEY_UP_PORT, NES_KEY_UP_PIN,PULL_UP);
	gpio_set_pull_mode(NES_KEY_DO_PORT, NES_KEY_DO_PIN,PULL_UP);
	gpio_set_pull_mode(NES_KEY_LF_PORT, NES_KEY_LF_PIN,PULL_UP);
	gpio_set_pull_mode(NES_KEY_RG_PORT, NES_KEY_RG_PIN,PULL_UP);
	gpio_set_pull_mode(NES_KEY_SA_PORT, NES_KEY_SA_PIN,PULL_UP);
	gpio_set_pull_mode(NES_KEY_SB_PORT, NES_KEY_SB_PIN,PULL_UP);
	gpio_set_pull_mode(NES_KEY_AA_PORT, NES_KEY_AA_PIN,PULL_UP);
	gpio_set_pull_mode(NES_KEY_BB_PORT, NES_KEY_BB_PIN,PULL_UP);
	gpio_set_pull_mode(NES_KEY_ST_PORT, NES_KEY_ST_PIN,PULL_UP);
	gpio_set_pull_mode(NES_KEY_SE_PORT, NES_KEY_SE_PIN,PULL_UP);
}

//   TK_KEY_UP,
//   TK_KEY_DOWN,
//   TK_KEY_LEFT,
//   TK_KEY_RIGHT,
//   TK_KEY_RETURN,
//   TK_KEY_ESCAPE,

int input_key_scan(void)
{
	int ret = 0;
	if(!gpio_get_value(NES_KEY_UP_PORT, NES_KEY_UP_PIN))
	{
		rt_thread_delay(10);
		if(!gpio_get_value(NES_KEY_UP_PORT, NES_KEY_UP_PIN))
			ret  = 1;
	}
    else if(!gpio_get_value(NES_KEY_DO_PORT, NES_KEY_DO_PIN))
	{
		rt_thread_delay(10);
		if(!gpio_get_value(NES_KEY_DO_PORT, NES_KEY_DO_PIN))
			ret  = 2;
	}
    else if(!gpio_get_value(NES_KEY_LF_PORT, NES_KEY_LF_PIN))
	{
		rt_thread_delay(10);
		if(!gpio_get_value(NES_KEY_LF_PORT, NES_KEY_LF_PIN))
			ret  = 3;
	}
    else if(!gpio_get_value(NES_KEY_RG_PORT, NES_KEY_RG_PIN))
	{
		rt_thread_delay(10);
		if(!gpio_get_value(NES_KEY_RG_PORT, NES_KEY_RG_PIN))
			ret  = 4;
	}
    else if(!gpio_get_value(NES_KEY_ST_PORT, NES_KEY_ST_PIN))
	{
		rt_thread_delay(10);
		if(!gpio_get_value(NES_KEY_ST_PORT, NES_KEY_ST_PIN))
			ret  = 5;
	}
    else if(!gpio_get_value(NES_KEY_SE_PORT, NES_KEY_SE_PIN))
	{
		rt_thread_delay(10);
		if(!gpio_get_value(NES_KEY_SE_PORT, NES_KEY_SE_PIN))
			ret  = 6;
	}
    return ret;
}

int input_key_up_press(void)
{
	return !gpio_get_value(NES_KEY_UP_PORT, NES_KEY_UP_PIN);
}

int input_key_do_press(void)
{
	return !gpio_get_value(NES_KEY_DO_PORT, NES_KEY_DO_PIN);
}

int input_key_lf_press(void)
{
	return !gpio_get_value(NES_KEY_LF_PORT, NES_KEY_LF_PIN);
}

int input_key_rg_press(void)
{
	return !gpio_get_value(NES_KEY_RG_PORT, NES_KEY_RG_PIN);
}

int input_key_st_press(void)
{
	return !gpio_get_value(NES_KEY_ST_PORT, NES_KEY_ST_PIN);
}

int input_key_se_press(void)
{
	return !gpio_get_value(NES_KEY_SE_PORT, NES_KEY_SE_PIN);
}


int input_key_sa_press(void)
{
	return !gpio_get_value(NES_KEY_SA_PORT, NES_KEY_SA_PIN);
}

int input_key_sb_press(void)
{
	return !gpio_get_value(NES_KEY_SB_PORT, NES_KEY_SB_PIN);
}

int input_key_aa_press(void)
{
	return !gpio_get_value(NES_KEY_AA_PORT, NES_KEY_AA_PIN);
}

int input_key_bb_press(void)
{
	return !gpio_get_value(NES_KEY_BB_PORT, NES_KEY_BB_PIN);
}
