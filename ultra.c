#include "ultra.h"

/// HRS04媛�媛����蹂�닔�ㅼ쓣 珥덇린���섎룄濡��⑸땲��
void init_hrsd04_variable(HRS04_VAR* sen){
	sen->cap_rising_edge = 0;
	sen->cap_falling_edge = 0;
	sen->pulse_width = 0;
	sen->capture = false;
	sen->distance = false;
}

/// InputSignal Trigger瑜�蹂대궡�꾨줉 �⑸땲��
void Triger_InputSig(HRS04_VAR* sen){
	GPIO_SetBits(sen->trig_port, sen->trig_pin);
	delay_us(10);
	GPIO_ResetBits(sen->trig_port, sen->trig_pin);
}

void ultra_setup(HRS04_VAR* sen)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	// RCC Configuration
	RCC_APB2PeriphClockCmd(sen->trig_rcc, ENABLE);

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
