#include "ultrasonic.h"

static void ultra_timer_init(Ultra_InitTypeDef *Ultra_InitStruct){
    RCC_ClocksTypeDef       rcc_clocks_status;
    uint16_t                prescaler;
    TIM_TimeBaseInitTypeDef timer_init_struct;
    TIM_OCInitTypeDef       timer_oc_init_struct;
    TIM_ICInitTypeDef       timer_ic_init_struct;

    prescaler = rcc_clocks_status.SYSCLK_Frequency / 1000000 -1;

    RCC_GetClocksFreq(&rcc_clocks_status);
    // 1 tick = 1 us ( 1 tick = 0.165mm resolution )

    TIM_DeInit(Ultra_InitStruct->US_TIMER);
    timer_init_struct.TIM_Prescaler     = prescaler;
    timer_init_struct.TIM_CounterMode   = TIM_CounterMode_Up;
    timer_init_struct.TIM_Period        = 0xFFFF;
    timer_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(Ultra_InitStruct->US_TIMER, &timer_init_struct);

    timer_oc_init_struct.TIM_OCMode      = TIM_OCMode_PWM1;
    timer_oc_init_struct.TIM_OutputState = TIM_OutputState_Enable;
    timer_oc_init_struct.TIM_Pulse       = 15; // us
    timer_oc_init_struct.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OC3Init(Ultra_InitStruct->US_TIMER, &timer_oc_init_struct);

    timer_ic_init_struct.TIM_Channel     = TIM_Channel_1;
    timer_ic_init_struct.TIM_ICPolarity  = TIM_ICPolarity_Rising;
    timer_ic_init_struct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    timer_ic_init_struct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    timer_ic_init_struct.TIM_ICFilter    = 0;
    TIM_PWMIConfig(Ultra_InitStruct->US_TIMER, &timer_ic_init_struct);

    TIM_SelectInputTrigger(Ultra_InitStruct->US_TIMER, Ultra_InitStruct->US_TIMER_TRIG_SOURCE);
    TIM_SelectMasterSlaveMode(Ultra_InitStruct->US_TIMER, TIM_MasterSlaveMode_Enable);

    TIM_CtrlPWMOutputs(Ultra_InitStruct->US_TIMER, ENABLE);

    TIM_ClearFlag(Ultra_InitStruct->US_TIMER, TIM_FLAG_Update);
}

static void ultra_pin_init(Ultra_InitTypeDef *Ultra_InitStruct){
    GPIO_InitTypeDef gpio_init_struct;

    gpio_init_struct.GPIO_Pin = Ultra_InitStruct->US_TRIG_PIN;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(Ultra_InitStruct->US_TRIG_PORT, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = Ultra_InitStruct->US_ECHO_PIN;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(Ultra_InitStruct->US_ECHO_PORT, &gpio_init_struct);
}

void ultra_sensor_init(Ultra_InitTypeDef *Ultra_InitStruct){
    ultra_pin_init(Ultra_InitStruct);
    ultra_timer_init(Ultra_InitStruct);
}

uint32_t ultra_get_distance(TIM_TypeDef* US_TIMER){
    (US_TIMER)->CNT = 0;

    TIM_Cmd(US_TIMER, ENABLE);
    while(!TIM_GetFlagStatus(US_TIMER, TIM_FLAG_Update));
    TIM_Cmd(US_TIMER, DISABLE);

    TIM_ClearFlag(US_TIMER, TIM_FLAG_Update);

    // 1 tick = 0.165mm
    return (TIM_GetCapture2(US_TIMER) - TIM_GetCapture1(US_TIMER))*165/1000;
}