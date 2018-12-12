#include "beep.h"

void delay(int num){
	while(num--);
}

void _Beep_Init() {
    GPIO_InitTypeDef gpio_init_struct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB
                            , ENABLE);

    gpio_init_struct.GPIO_Pin = GPIO_Pin_5; //임시적으로 B5핀 배정
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &gpio_init_struct);
}

int main() {
    SystemInit();
    _Beep_init();
    while(1) {
        BEEP_ON;
        delay(10000);
        BEEP_OFF;
    }
}
