#ifndef __drv_pwm_h__
#define __drv_pwm_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "drv_gpio.h"
typedef struct 
{
	unsigned int virt;
	char * clk;
	int32_t duty;
	int32_t period;
	int32_t channel;
	int32_t polarity;
	enum gpio_port pwm_port;
	enum gpio_pin pwm_pin;
	uint8_t pwm_pin_cfg;
}pwm_t;

int32_t pwm_f1c100s_init(pwm_t * pwm);
void pwm_f1c100s_config(pwm_t * pwm, int32_t duty, int32_t period, int32_t polarity);
void pwm_f1c100s_set_duty(pwm_t * pwm, int32_t duty);
inline int32_t pwm_f1c100s_get_duty(pwm_t * pwm) {return pwm->duty;}
void pwm_f1c100s_enable(pwm_t * pwm);
void pwm_f1c100s_disable(pwm_t * pwm);

#ifdef __cplusplus
}
#endif

#endif /* __PWM_H__ */
