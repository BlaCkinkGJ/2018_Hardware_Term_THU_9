#ifndef __ULTRA_H___
#define __ULTRA_H__

#include "universal.h"

typedef struct _HRS04_VAR{
	int cap_rising_edge;
	int cap_falling_edge;
	int pulse_width;
	bool capture;
	bool distance;

	GPIO_TypeDef* trig_port;
	uint16_t      trig_pin;
	uint32_t      trig_rcc;

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

/// HRS04媛�媛����蹂�닔�ㅼ쓣 珥덇린���섎룄濡��⑸땲��
void init_hrsd04_variable(HRS04_VAR* sen);
/// InputSignal Trigger瑜�蹂대궡�꾨줉 �⑸땲��
void Triger_InputSig(HRS04_VAR* sen);
void ultra_setup(HRS04_VAR* sen);
#endif
