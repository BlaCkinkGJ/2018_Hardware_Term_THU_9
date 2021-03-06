#include "delay.h"

void delay_ms(unsigned int msec){
    uint32_t temp;
    SysTick->LOAD = (uint32_t)msec*(SystemCoreClock/8/1000);
    SysTick->VAL  = 0x00;        // clear Count flag
    SysTick->CTRL = 0x01;
    do{
        temp=SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));    // wait Count flag set
    SysTick->CTRL = 0x00;
    SysTick->VAL  = 0X00;
}

void delay_us(unsigned int usec){
    uint32_t temp;
    SysTick->LOAD = (uint32_t)usec*(SystemCoreClock/8/1000000);
    SysTick->VAL  = 0x00;        // clear Count flag
    SysTick->CTRL = 0x01;
    do{
        temp = SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));    // wait Count flag set
    SysTick->CTRL = 0x00;
    SysTick->VAL  = 0X00;
}
