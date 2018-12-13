// flash load "D:\Hardware Experiment 3 2\THU09\Week10\flashclear.axf"
// flash load "D:\Hardware Experiment 3 2\THU09\Week10\Debug\Week10.axf"

#include "universal.h"
#include "ultra.h"
#include "servo.h"

HRS04_VAR ultra1;
// HRS04_VAR ultra2;
// HRS04_VAR ultra3;


/// For ultra1
void TIM3_IRQHandler(HRS04_VAR* sen){
	if( TIM_GetITStatus(sen->timer, sen->timer_cc_channel)==SET ){
		TIM_ClearITPendingBit(sen->timer, sen->timer_cc_channel);

		if(GPIO_ReadInputDataBit(sen->timer_port, sen->timer_pin)==Bit_SET){ // if timer is high
			sen->cap_rising_edge = TIM_GetCapture1(sen->timer);	// read capture data
			sen->timer->CCER |=  sen->timer_ccip;	// to falling edge
		}
		else{ // if timer is low
			sen->cap_falling_edge = TIM_GetCapture1(sen->timer);	// read capture data
			sen->pulse_width = (u32) (sen->cap_falling_edge - sen->cap_rising_edge);
			sen->timer->CCER &= ~ sen->timer_ccip;	// to rising edge

			sen->distance = true;
		} // end if 
	} //  end if
}

static void ultra_init(){
	init_hrsd04_variable(&ultra1);
	ultra1.trig_port         = GPIOA;
	ultra1.trig_pin          = GPIO_Pin_7;
	ultra1.timer             = TIM3;
	ultra1.timer_port        = GPIOA;
	ultra1.timer_pin         = GPIO_Pin_8;
	ultra1.timer_inp_channel = TIM_Channel_1;
	ultra1.timer_cc_channel  = TIM_IT_CC1;
	ultra1.timer_ccip        = TIM_CCER_CC1P;
	ultra1.timer_rcc         = RCC_APB1Periph_TIM3;
	ultra1.timer_irq         = TIM3_IRQn;
}

static void pwm_setting(){
	PWM pwm;
	pwm.OCMode    = TIM_OCMode_PWM1;
	pwm.rcc_timer = RCC_APB1Periph_TIM4;
	pwm.timer     = TIM4;
	pwm.rcc_gpio  = RCC_APB2Periph_GPIOB;
	pwm.gpio_port = GPIOB;
	pwm.gpio_pin  = GPIO_Pin_8;
	pwm_init(&pwm);
}

static void setup(void){
    ultra_init();
	pwm_setting();
	ultra_setup(&ultra1);

    ultra1.capture = true;
    Triger_InputSig(&ultra1);
}

static void loop(void)
{
	if(ultra1.distance != false){ // distance analysis
        int dist_mm = (ultra1.pulse_width*17/100);
		ultra1.distance = false;
		if(ultra1.capture){ // if capture is enabled
			Triger_InputSig(&ultra1);
		}
	}
	delay_ms(500);
}


int main(void){
    setup();
    while(1){
        loop();
    }
}
