// flash load "D:\Hardware Experiment 3 2\THU09\Week10\flashclear.axf"
// flash load "D:\Hardware Experiment 3 2\THU09\Week10\Debug\Week10.axf"

#include "universal.h"
#include "ultra.h"
#include "servo.h"
#include "bluetooth.h"

#define FORWARD 1
#define BACKWARD -1

HRS04_VAR ultra1;
// HRS04_VAR ultra2;
// HRS04_VAR ultra3;
PWM pwm;
BlueConfig blue;

/// IRQHandler Setting
void USART1_IRQHandler(){
	unsigned char recvbuf = '\0';

	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		; // Wait the USART1 Tx to SET
	recvbuf = (unsigned char)USART_ReceiveData(USART1);
	USART_SendData(USART1, recvbuf); // ECHO
	USART_SendData(USART2, recvbuf); // Send to bluetooth

	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

void USART2_IRQHandler(){
	unsigned char recvbuf = '\0';

	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		; // Wait the USART2 Tx to SET
	recvbuf = (unsigned char)USART_ReceiveData(USART2); // ECHO
	USART_SendData(USART1, recvbuf); // Send to bluetooth

    blue.isValid = true;

	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}


/// For ultra1
void TIM3_IRQHandler(void){
    if( TIM_GetITStatus(ultra1.timer, ultra1.timer_cc_channel)==SET ){
        TIM_ClearITPendingBit(ultra1.timer, ultra1.timer_cc_channel);
        if(GPIO_ReadInputDataBit(ultra1.timer_port, ultra1.timer_pin)==Bit_SET){ // if timer is high
            ultra1.cap_rising_edge = TIM_GetCapture1(ultra1.timer);    // read capture data
            (ultra1.timer)->CCER |=  ultra1.timer_ccip;    // to falling edge
        }
        else{ // if timer is low
            ultra1.cap_falling_edge = TIM_GetCapture1(ultra1.timer);    // read capture data
            ultra1.pulse_width = (u32) (ultra1.cap_falling_edge - ultra1.cap_rising_edge);
            (ultra1.timer)->CCER &= ~ ultra1.timer_ccip;    // to rising edge
            ultra1.distance = true;
        } // end if 
    } //  end if
}

static void ultra_init(){
    init_hrsd04_variable(&ultra1);
    ultra1.trig_port         = GPIOA;
    ultra1.trig_pin          = GPIO_Pin_7;
    ultra1.timer             = TIM3;
    ultra1.timer_port        = GPIOA;
    ultra1.timer_pin         = GPIO_Pin_6;
    ultra1.timer_inp_channel = TIM_Channel_1;
    ultra1.timer_cc_channel  = TIM_IT_CC1;
    ultra1.timer_ccip        = TIM_CCER_CC1P;
    ultra1.timer_rcc         = RCC_APB1Periph_TIM3;
    ultra1.timer_irq         = TIM3_IRQn;
    ultra1.trig_rcc          = RCC_APB2Periph_GPIOA;
}

static void pwm_setting(){
    pwm.OCMode    = TIM_OCMode_PWM1;
    pwm.rcc_timer = RCC_APB1Periph_TIM4;
    pwm.timer     = TIM4;
    pwm.rcc_gpio  = RCC_APB2Periph_GPIOB;
    pwm.gpio_port = GPIOB;
    pwm.gpio_pin  = GPIO_Pin_8;
    pwm_init(&pwm);
}

int counter = 0;
int now = 0;
bool isCheck = false;
int direction = FORWARD;

static void setup(void){
    SystemInit();
    ultra_init();
    bluetooth_init(&blue);
    pwm_setting();
    pwm_init(&pwm);
    ultra_setup(&ultra1);
    { // initailze the buzzer and PIR sensor
        GPIO_InitTypeDef gpio_init_struct;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
        gpio_init_struct.GPIO_Mode = GPIO_Mode_Out_PP;
        gpio_init_struct.GPIO_Pin = (GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7);
        gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD, &gpio_init_struct);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        gpio_init_struct.GPIO_Mode = GPIO_Mode_IPD;
        gpio_init_struct.GPIO_Pin = (GPIO_Pin_1);
        gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD, &gpio_init_struct);
    }
    ultra1.capture = true;
    Triger_InputSig(&ultra1);
}

static void loop(void)
{
    int isPeople = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
    
    if(blue.isATscan == false){
        bluetooth_send_data("AT+BTSCAN", &blue);
    }
    else{
        bluetooth_send_data("Hello World!!", &blue);
    }

    if(isPeople == 0){
        GPIO_SetBits(GPIOD, GPIO_Pin_4);
    }else{
        GPIO_ResetBits(GPIOD, GPIO_Pin_4);
    }

    GPIO_SetBits(GPIOD, GPIO_Pin_3);
    change_pwm_cycle(&pwm, 180);
    if(ultra1.distance != false){ // distance analysis
        int dist_mm = (ultra1.pulse_width*17/100);
        ultra1.distance = false;
        if(ultra1.capture){ // if capture is enabled
            if(dist_mm < 1050){
                counter = counter + 1 > 5 ? 5 : counter + 1;
                if(counter > 3){
                    GPIO_ResetBits(GPIOD, GPIO_Pin_2);
                    GPIO_SetBits(GPIOD, GPIO_Pin_7);
                    change_pwm_cycle(&pwm, now);
                    isCheck = true;
                }
            }
            else{
                GPIO_SetBits(GPIOD, GPIO_Pin_2);
                GPIO_ResetBits(GPIOD, GPIO_Pin_7);
                counter = 0;
                isCheck = false;
            }

        } // end of if
    } // end of if
    if(!isCheck){
        now = now + 20*direction;
        if(now > 180){
            direction = BACKWARD;
            now = 180;
        }
        if(now < 0){
            direction = FORWARD;
            now = 0;
        }
        change_pwm_cycle(&pwm, now);
    }
    Triger_InputSig(&ultra1);
    GPIO_ResetBits(GPIOD, GPIO_Pin_3);
    delay_ms(250);
}


int main(void){


    setup();
    while(true){
        loop();
    }
}
