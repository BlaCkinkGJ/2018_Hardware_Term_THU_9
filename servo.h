#ifndef __SERVO_H__
#define __SERVO_H__

#include "universal.h"

typedef struct _PWM{
    uint16_t       OCMode;
    uint32_t       rcc_timer;
    TIM_TypeDef*   timer;
    uint32_t       rcc_gpio;
    GPIO_TypeDef*  gpio_port;
    uint16_t       gpio_pin;

}PWM;


void pwm_init(PWM*);
void change_pwm_cycle(PWM*, int);

#endif
