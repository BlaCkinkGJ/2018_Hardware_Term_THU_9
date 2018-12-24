#ifndef __ULTRA_H___
#define __ULTRA_H__

#include "universal.h"

typedef struct _HRS04_VAR{
    int cap_rising_edge;
    int cap_falling_edge;
    uint32_t pulse_width;
    bool capture;
    bool distance;

    GPIO_TypeDef* trig_port;
    uint16_t      trig_pin;
    uint32_t      trig_rcc;


    TIM_TypeDef*  timer;
    GPIO_TypeDef* timer_port;
    uint16_t      timer_pin; // (= echo_pin)
    uint16_t      timer_inp_channel;
    uint32_t      timer_rcc;
    uint8_t       timer_irq;
    uint16_t      timer_cc_channel;
    uint16_t      timer_ccip;

}HRS04_VAR;


void ultra_setup          (HRS04_VAR* sen);
void Triger_InputSig      (HRS04_VAR* sen);
void init_hrsd04_variable (HRS04_VAR* sen);
#endif
