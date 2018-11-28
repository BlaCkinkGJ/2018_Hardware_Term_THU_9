#ifndef __ULTRA_SONIC__
#define __ULTRA_SONIC__

#include "pipe.h"

#define US_TIMER             TIM3

#define US_TRIG_PORT         GPIOB
#define US_TRIG_PIN          GPIO_Pin_0 // TIM CHANNEL 3

#define US_ECHO_PORT         GPIOA
#define US_ECHO_PIN          GPIO_Pin_6 // TIM CHANNEL 1
#define US_TIMER_TRIG_SOURCE TIM_TS_TI1FP1

void    ultra_RCC_Init();
void    ultra_sensor_Init();
int32_t ultra_get_distance();


#endif