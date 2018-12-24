// flash load "D:\Hardware Experiment 3 2\THU09\Week10\flashclear.axf"
// flash load "D:\Hardware Experiment 3 2\THU09\Week10\Debug\Week10.axf"
#include "servo.h"

void pwm_init(PWM* pwm){
    TIM_TimeBaseInitTypeDef tim_base_init_struct;
    TIM_OCInitTypeDef       tim_oc_init_struct;
    GPIO_InitTypeDef        gpio_init_struct;

    // RCC and GPIO setting
    RCC_APB2PeriphClockCmd(pwm->rcc_gpio, ENABLE);
    gpio_init_struct.GPIO_Pin   = pwm->gpio_pin;
    gpio_init_struct.GPIO_Mode  = GPIO_Mode_AF_PP;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(pwm->gpio_port, &gpio_init_struct);
    
    RCC_APB1PeriphClockCmd(pwm->rcc_timer,ENABLE);

    // Timer setting
    tim_base_init_struct.TIM_Prescaler     = (uint16_t)(SystemCoreClock/1000000)-1;
    tim_base_init_struct.TIM_Period        = 20000-1;
    tim_base_init_struct.TIM_ClockDivision = 0;
    tim_base_init_struct.TIM_CounterMode   = TIM_CounterMode_Down;
    TIM_TimeBaseInit(TIM4,&tim_base_init_struct);

    // initialize the PWM
    tim_oc_init_struct.TIM_OCMode      = pwm->OCMode;
    tim_oc_init_struct.TIM_OCPolarity  = TIM_OCPolarity_High;
    tim_oc_init_struct.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_init_struct.TIM_Pulse       = 1500;
    TIM_OC3Init(pwm->timer,&tim_oc_init_struct);
    TIM_OC3PreloadConfig(pwm->timer,TIM_OCPreload_Disable);

    TIM_ARRPreloadConfig(pwm->timer,ENABLE);
    TIM_Cmd(pwm->timer,ENABLE);
}

void change_pwm_cycle(PWM* pwm, int degree){
    // 0 ~ 180 degree movement
    int pwm_pulse = ((2300 - 700) / 180) * (degree - 15) + 700;
    TIM_OCInitTypeDef tim_oc_init_struct;
    tim_oc_init_struct.TIM_OCMode      = pwm->OCMode;
    tim_oc_init_struct.TIM_OCPolarity  = TIM_OCPolarity_High;
    tim_oc_init_struct.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_init_struct.TIM_Pulse       = pwm_pulse;
    /**
     * This function must use with the delay.
     * If you don't use the delay. Then this cannot operate correctly.
     */
    TIM_OC3Init(pwm->timer, &tim_oc_init_struct);
}
