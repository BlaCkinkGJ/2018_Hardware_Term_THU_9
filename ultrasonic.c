#include "ultrasonic.h"

/// this will be changed
void ultra_RCC_Init(){
    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA,  // ECHO
                            RCC_APB2Periph_GPIOB), // TRIGGER
                            ENABLE
    );
    RCC_APB1PeriphClockCmd((RCC_APB1Periph_TIM3),
                            ENABLE);
}

static void ultra_timer_init(){
    RCC_ClocksTypeDef rcc_clocks_status;
    RCC_GetClocksFreq(&rcc_clocks_status);
    // 1 tick = 1 us ( 1 tick = 0.165mm resolution )
    uint16_t prescaler = rcc_clocks_status.SYSCLK_Frequency / 1000000 -1;

    TIM_DeInit(US_TIMER);
    TIM_TimeBaseInitTypeDef timer_init_struct;
    timer_init_struct.TIM_Prescaler     = prescaler;
    timer_init_struct.TIM_CounterMode   = TIM_CounterMode_Up;
    timer_init_struct.TIM_Period        = 0xFFFF;
    timer_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(US_TIMER, &timer_init_struct);

    TIM_OCInitTypeDef timer_oc_init_struct;
    timer_oc_init_struct.TIM_OCMode      = TIM_OCMode_PWM1;
    timer_oc_init_struct.TIM_OutputState = TIM_OutputState_Enable;
    timer_oc_init_struct.TIM_Pulse       = 15; // us
    timer_oc_init_struct.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OC3Init(US_TIMER, &timer_oc_init_struct);

    TIM_ICInitTypeDef timer_ic_init_struct;
    timer_ic_init_struct.TIM_Channel     = TIM_Channel_1;
    timer_ic_init_struct.TIM_ICPolarity  =  TIM_ICPolarity_Rising;
    timer_ic_init_struct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    timer_ic_init_struct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    timer_ic_init_struct.TIM_ICFilter    = 0;
    TIM_PWMIConfig(US_TIMER, &timer_ic_init_struct);

    TIM_SelectInputTrigger(US_TIMER, US_TIMER_TRIG_SOURCE);
    TIM_SelectMasterSlaveMode(US_TIMER, TIM_MasterSlaveMode_Enable);

    TIM_CtrlPWMOutputs(US_TIMER, ENABLE);

    TIM_ClearFlag(US_TIMER, TIM_FLAG_Update);
}

static void ultra_pin_init(){
    GPIO_InitTypeDef gpio_init_struct;

    gpio_init_struct.GPIO_Pin = US_TRIG_PIN;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(US_TRIG_PORT, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = US_ECHO_PIN;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(US_ECHO_PORT, &gpio_init_struct);
}

void ultra_sensor_iniit(){
    ultra_pin_init();
    ultra_timer_init();
}

int32_t ultra_get_distance(){
    (US_TIMER)->CNT = 0;

    TIM_Cmd(US_TIMER, ENABLE);
    while(!TIM_GetFlagStatus(US_TIMER, TIM_FLAG_Update));
    TIM_Cmd(US_TIMER, DISABLE);

    TIM_ClearFlag(US_TIMER, TIM_FLAG_Update);

    return (TIM_GetCapture2(US_TIMER) - TIM_GetCapture1(US_TIMER))*165/1000;
}