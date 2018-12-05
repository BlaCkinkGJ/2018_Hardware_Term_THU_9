// flash load "D:\Hardware Experiment 3 2\THU09\Week10\flashclear.axf"
// flash load "D:\Hardware Experiment 3 2\THU09\Week10\Debug\Week10.axf"

#include "pipe.h"

// @TODO 이 부분은 확인해보도록 할 것!!!
#define HIGH 0
#define LOW 1

static void ULTRA_TEST(){
    GPIO_InitTypeDef gpio_init_struct;

    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOD), ENABLE);

    gpio_init_struct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_10MHz;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &gpio_init_struct);
}

static void PIR_TEST(){
    GPIO_InitTypeDef gpio_init_struct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    gpio_init_struct.GPIO_Pin = GPIO_Pin_1;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_10MHz;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOC, &gpio_init_struct);
}

int main(){
    _System_Init();
    ULTRA_TEST();
    PIR_TEST();
    while(true){
        uint32_t     dist[3];
        TIM_TypeDef* timer[3] = {TIM3, TIM4, TIM5} ;
        uint16_t     pin[4]   = {GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_7};
        uint16_t     value    = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
        int          i        = 0;
    
        /// PIR TEST
        if(value == HIGH)
            GPIO_SetBits(GPIOD, pin[4]);
        else
            GPIO_ResetBits(GPIOD, pin[4]);

        /// ULTRA TEST
        for(i = 0; i < 3; i++){ 
            dist[i] = ultra_get_distance(timer[i]);
            if(dist[i] > 5000)
                GPIO_SetBits(GPIOD, pin[i]);
            else
                GPIO_ResetBits(GPIOD, pin[i]);
        }
        


        delay(10000);
    }
}