#include "bluetooth.h"

void _Bluetooth_Execute(){
	_BT_RCC_Init();
	_BT_USART1_Init();
	_BT_USART2_Init();
	_BT_NVIC_Init();
}

//string: receive & send
void usart1_recv_string(unsigned char *str) {
	int i=0;
	
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET)
		;
	while (USART_ReceiveData(USART1) != '\0') {
		str[i]=(unsigned char)USART_ReceiveData(USART1);
		i++;
	}
	str[i] = '\0';
}

void usart1_send_string(unsigned char *str) {
	int i=0;
	while(str[i] != '\0') {
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET )
			;
		USART_SendData(USART2, str[i]);
		str++;
	}
	USART_SendData(USART2, '\0');
	
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

void usart2_recv_string(unsigned char *str) {
	int i=0;
	
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET)
		;
	while (USART_ReceiveData(USART2) != '\0') {
		str[i]=(unsigned char)USART_ReceiveData(USART2);
		i++;
	}
	str[i] = '\0';
}

void usart2_send_string(unsigned char *str) {
	int i=0;
	while(str[i] != '\0') {
		while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET )
			;
		USART_SendData(USART1, str[i]);
		str++;
	}
	USART_SendData(USART1, '\0');
	
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

//USART interrupt
void _BT_USART1_IRQHandler(){
	unsigned char *inputstring='\0';

	usart1_recv_string(inputstring);
	usart1_send_string(inputstring);
}

void _BT_USART2_IRQHandler(){
	unsigned char *inputstring='\0';

	usart2_recv_string(inputstring);
	usart2_send_string(inputstring);
}

//Init
void _BT_RCC_Init(){
    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA
							|RCC_APB2Periph_AFIO
							|RCC_APB2Periph_USART1)
							, ENABLE);
    RCC_APB1PeriphClockCmd((RCC_APB1Periph_USART2)
							, ENABLE);
}
void _BT_USART1_Init(){
    GPIO_InitTypeDef gpio_init_struct;
    USART_InitTypeDef usart_init_struct;

    gpio_init_struct.GPIO_Pin   = GPIO_Pin_9;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_struct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio_init_struct);

	gpio_init_struct.GPIO_Pin   = GPIO_Pin_10;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_struct.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &gpio_init_struct);

	usart_init_struct.USART_BaudRate            = 115200;
	usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init_struct.USART_Mode                = (USART_Mode_Rx|USART_Mode_Tx);
	usart_init_struct.USART_Parity				= USART_Parity_No;
	usart_init_struct.USART_StopBits			= USART_StopBits_1;
	usart_init_struct.USART_WordLength			= USART_WordLength_8b;
    USART_Init(USART1, &usart_init_struct);
	
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	EXTI_ClearITPendingBit(USART_IT_RXNE);
	USART_Cmd(USART1, ENABLE);
}

void _BT_USART2_Init() {
	GPIO_InitTypeDef gpio_init_struct;
    USART_InitTypeDef usart_init_struct;

	gpio_init_struct.GPIO_Pin   = GPIO_Pin_2;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_struct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio_init_struct);

	gpio_init_struct.GPIO_Pin   = GPIO_Pin_3;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_struct.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &gpio_init_struct);

	usart_init_struct.USART_BaudRate            = 9600;
	usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init_struct.USART_Mode                = (USART_Mode_Rx|USART_Mode_Tx);
	usart_init_struct.USART_Parity				= USART_Parity_No;
	usart_init_struct.USART_StopBits			= USART_StopBits_1;
	usart_init_struct.USART_WordLength			= USART_WordLength_8b;
	USART_Init(USART2, &usart_init_struct);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	EXTI_ClearITPendingBit(USART_IT_RXNE);
	USART_Cmd(USART2, ENABLE);
}

void _BT_NVIC_Init(){
	NVIC_InitTypeDef nvic_init_struct;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	nvic_init_struct.NVIC_IRQChannel					= USART1_IRQn;
	nvic_init_struct.NVIC_IRQChannelPreemptionPriority	= 1;
	nvic_init_struct.NVIC_IRQChannelSubPriority			= 1;
	nvic_init_struct.NVIC_IRQChannelCmd					= ENABLE;
	NVIC_Init(&nvic_init_struct);

	nvic_init_struct.NVIC_IRQChannel					= USART2_IRQn;
	nvic_init_struct.NVIC_IRQChannelPreemptionPriority	= 1;
	nvic_init_struct.NVIC_IRQChannelSubPriority			= 1;
	nvic_init_struct.NVIC_IRQChannelCmd					= ENABLE;
	NVIC_Init(&nvic_init_struct);
}