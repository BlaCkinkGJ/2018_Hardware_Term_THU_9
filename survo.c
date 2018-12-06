// flash load "D:\Hardware Experiment 3 2\THU09\Week10\flashclear.axf"
// flash load "D:\Hardware Experiment 3 2\THU09\Week10\Debug\Week10.axf"

#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "lcd.h"
#include "Touch.h"

int timer = 0;

void delay(int num){
	num = num * 100000;
	while(num--);
}

void Change_PWM_Cycle(int degree)
{
    int pwm_pulse;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    pwm_pulse = ((2300 - 700) / 180) * (degree - 15) + 700;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = pwm_pulse;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    delay(100);
}

void PWM_EXIT(){
	TIM_OCInitTypeDef TIM_OCInitStructure;
	int pwm_pulse = 75 * 2000 / 100;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = pwm_pulse;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
}

int main()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	int result = 0;

	SystemInit();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(SystemCoreClock/1000000)-1;
	TIM_TimeBaseStructure.TIM_Period = 20000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

	/// initialization
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1500;
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Disable);

	TIM_ARRPreloadConfig(TIM4,ENABLE);
	TIM_Cmd(TIM4,ENABLE);

	while(1){
		int isUp = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
		int isDown = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
		int isLeft = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
		int isRight = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4);
		if(isUp == 0){
			result = 1;
		}
		if(isDown == 0){
			result = 2;
		}
		if(isLeft == 0){
			result = 3;
		}
		if(isRight == 0){
			result = 4;
		}
		switch(result){
		case 1:
			Change_PWM_Cycle(30);
			break;
		case 2:
			Change_PWM_Cycle(45);
			break;
		case 3:
			Change_PWM_Cycle(90);
			break;
		case 4:
			Change_PWM_Cycle(180);
			break;
		default:
			Change_PWM_Cycle(60);
			break;
		}
	}
}
