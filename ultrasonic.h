#ifndef __ULTRA_SONIC__
#define __ULTRA_SONIC__

#include "pipe.h"

// @TODO timer trigger source에 관해서 좀 더 알아도록 할 것!
typedef struct ULTRA_CONF{
    TIM_TypeDef*  US_TIMER;             // e.g. TIM3
    uint16_t      US_TIMER_TRIG_SOURCE; // e.g. TIM_TS_TI1FP1

    GPIO_TypeDef* US_TRIG_PORT;         // e.g. GPIOB
    uint16_t      US_TRIG_PIN;          // e.g. GPIO_Pin_0

    GPIO_TypeDef* US_ECHO_PORT;         // e.g. GPIOA
    uint16_t      US_ECHO_PIN;          // e.g. GPIO_Pin_6
}Ultra_InitTypeDef;

void                ultra_sensor_init(Ultra_InitTypeDef *Ultra_InitStruct);
// if distance more than 3m then returns 0
uint32_t            ultra_get_distance(TIM_TypeDef* US_TIMER);


#endif