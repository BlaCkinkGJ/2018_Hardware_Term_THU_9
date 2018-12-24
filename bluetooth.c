#include "bluetooth.h"

static void _BT_RCC_Init(){
    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA
                            |RCC_APB2Periph_AFIO
                            |RCC_APB2Periph_USART1)
                            , ENABLE);
    RCC_APB1PeriphClockCmd((RCC_APB1Periph_USART2)
                            , ENABLE);
}

static void _BT_GPIO_Init(){
    GPIO_InitTypeDef gpio_init_struct;

    gpio_init_struct.GPIO_Pin   = GPIO_Pin_9;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin   = GPIO_Pin_10;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Mode  = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin   = GPIO_Pin_2;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin   = GPIO_Pin_3;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Mode  = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &gpio_init_struct);
}

static void _BT_USART_Init(){
    USART_InitTypeDef usart_init_struct;

    // This is a setting for debugging using the PC terminal
    usart_init_struct.USART_BaudRate            = 115200;
    usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_init_struct.USART_Mode                = (USART_Mode_Rx|USART_Mode_Tx);
    usart_init_struct.USART_Parity              = USART_Parity_No;
    usart_init_struct.USART_StopBits            = USART_StopBits_1;
    usart_init_struct.USART_WordLength          = USART_WordLength_8b;
    USART_Init(USART1, &usart_init_struct);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);

    // This is a setting for send and receive data from bluetooth
    usart_init_struct.USART_BaudRate            = 9600;
    usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_init_struct.USART_Mode                = (USART_Mode_Rx|USART_Mode_Tx);
    usart_init_struct.USART_Parity              = USART_Parity_No;
    usart_init_struct.USART_StopBits            = USART_StopBits_1;
    usart_init_struct.USART_WordLength          = USART_WordLength_8b;
    USART_Init(USART2, &usart_init_struct);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}

static void _BT_NVIC_Init(){
    NVIC_InitTypeDef nvic_init_struct;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // PC interrupt setting
    nvic_init_struct.NVIC_IRQChannel                   = USART1_IRQn;
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 1;
    nvic_init_struct.NVIC_IRQChannelSubPriority        = 1;
    nvic_init_struct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic_init_struct);

    // Bluetooth interrupt setting
    nvic_init_struct.NVIC_IRQChannel                   = USART2_IRQn;
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 1;
    nvic_init_struct.NVIC_IRQChannelSubPriority        = 1;
    nvic_init_struct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic_init_struct);
}

void bluetooth_init(BlueConfig *config){
    config->isValid = false;
    config->isATscan = false;
    _BT_RCC_Init();
    _BT_GPIO_Init();
    _BT_USART_Init();
    _BT_NVIC_Init();
}


int bluetooth_send_data(const char *data, BlueConfig *config){
    int idx = 0;
    if(config->isValid == false) return 0; // if send fail
    
    /**
     * Send data to bluetooth module
     * FB755AC Module uses AT command.
     * So if data meet the <CR>.
     * It means end of stream
     */
    for(idx = 0; idx < strlen(data); idx++){
        USART_SendData(USART2, data[idx]);
        delay_ms(10);
    } // end for
    USART_SendData(USART2, '\r');
    delay_ms(10);
    config->isValid = false;
    return 1; // send success
}
