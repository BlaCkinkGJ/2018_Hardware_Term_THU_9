#ifndef __PIPE_H__
#define __PIPE_H__

#include <stdio.h>
#include <stdbool.h>

#include "stm32f10x.h"

#include "core_cm3.h"
#include "misc.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"


typedef struct _HRS04_VAR{
	int cap_rising_edge;
	int cap_falling_edge;
	int pulse_width;
	bool capture;
	bool distance;

	GPIO_TypeDef* trig_port;
	uint16_t      trig_pin;

	// GPIO_TypeDef* echo_port;
	// uint16_t      echo_pin;

	TIM_TypeDef*  timer;
	GPIO_TypeDef* timer_port;
	uint16_t      timer_pin;
	uint16_t      timer_inp_channel;
	uint32_t      timer_rcc;
	uint8_t       timer_irq;
	uint16_t      timer_cc_channel;
	uint16_t      timer_ccip;

}HRS04_VAR;

/// HRS04가 가지는 변수들을 초기화 하도록 합니다.
void init_hrsd04_variable(HRS04_VAR* sen){
	sen->cap_rising_edge = 0;
	sen->cap_falling_edge = 0;
	sen->pulse_width = 0;
	sen->capture = false;
	sen->distance = false;
}

/// InputSignal Trigger를 보내도록 합니다.
void Triger_InputSig(HRS04_VAR* sen){
	GPIO_SetBits(sen->trig_port, sen->trig_pin);
	delay_us(10);
	GPIO_ResetBits(sen->trig_port, sen->trig_pin);
}

void setup(HRS04_VAR* sen)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	// RCC Configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );

	// GPIO Output for Trigger
	GPIO_InitStructure.GPIO_Pin = sen->trig_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(sen->trig_port, &GPIO_InitStructure);

	// TIM3 Ch1 (PA6)
	GPIO_InitStructure.GPIO_Pin = sen->timer_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(sen->timer_port, &GPIO_InitStructure);

	// RCC Configuration
	RCC_APB1PeriphClockCmd(sen->timer_rcc, ENABLE );

	// Enable the TIM3 global Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = sen->timer_irq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	// 1usec for 72MHz clock
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / 1000000) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(sen->timer, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel3 */
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInitStructure.TIM_Channel = sen->timer_inp_channel;
	TIM_ICInit(sen->timer, &TIM_ICInitStructure);

	TIM_Cmd(sen->timer, ENABLE);

	TIM_ITConfig(sen->timer, sen->timer_cc_channel, ENABLE);

	init_hrsd04_variable(sen);
}

#endif