// flash load "D:\Hardware Experiment 3 2\THU09\Week10\flashclear.axf"
// flash load "D:\Hardware Experiment 3 2\THU09\Week10\Debug\Week10.axf"

#include "survo.h"
#include <stdbool.h>

static void pwm_setting(){
	PWM pwm;
	pwm.OCMode = TIM_OCMode_PWM1;
	pwm.rcc_timer = RCC_APB1Periph_TIM4;
	pwm.timer = TIM4;
	pwm_init(&pwm);
}

int main(){
	pwm_setting();
	while(true){

	}
}