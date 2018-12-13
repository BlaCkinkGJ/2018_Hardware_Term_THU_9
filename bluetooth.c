#include "bluetooth.h"

void delay(int num){
	while(num--);
}

void _BT_Init(){
	_BT_RCC_Init();
	_BT_GPIO_Init();
	_BT_USART_Init();
	_BT_NVIC_Init();
}

void _BT_RCC_Init(){
    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA
							|RCC_APB2Periph_AFIO
							|RCC_APB2Periph_USART1)
							, ENABLE);
    RCC_APB1PeriphClockCmd((RCC_APB1Periph_USART2)
							, ENABLE);
}

void _BT_GPIO_Init(){
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

void _BT_USART_Init(){
    USART_InitTypeDef usart_init_struct;

	usart_init_struct.USART_BaudRate            = 115200;
	usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init_struct.USART_Mode                = (USART_Mode_Rx|USART_Mode_Tx);
	usart_init_struct.USART_Parity				= USART_Parity_No;
	usart_init_struct.USART_StopBits			= USART_StopBits_1;
	usart_init_struct.USART_WordLength			= USART_WordLength_8b;
    USART_Init(USART1, &usart_init_struct);
	
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);

	usart_init_struct.USART_BaudRate            = 9600;
	usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init_struct.USART_Mode                = (USART_Mode_Rx|USART_Mode_Tx);
	usart_init_struct.USART_Parity				= USART_Parity_No;
	usart_init_struct.USART_StopBits			= USART_StopBits_1;
	usart_init_struct.USART_WordLength			= USART_WordLength_8b;
	USART_Init(USART2, &usart_init_struct);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
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

// usart1: COM-BOARD		usart2: BT-BOARD
void USART1_IRQHandler(){
	char data[60] = '\0';
	int idx = 0;

	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;
	for(idx = 0; idx < strlen(data); idx++){
        data[idx] = USART_ReceiveData(USART1);
        delay(1000);
    }

	for(idx = 0; idx < strlen(data); idx++){
        USART_SendData(USART1, data[idx]);
        delay(1000);
    }
	USART_SendData(USART1, '\n');
	delay(1000);
	USART_SendData(USART1, '\r');
	delay(1000);

	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

void USART2_IRQHandler(){
	char data[60] = '\0';
	int idx = 0;

	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		;
	for(idx = 0; idx < strlen(data); idx++){
        data[idx] = USART_ReceiveData(USART2);
        delay(1000);
    }
	
	for(idx = 0; idx < strlen(data); idx++){
        USART_SendData(USART2, data[idx]);
        delay(1000);
    }
	USART_SendData(USART2, '\n');
	delay(1000);
	USART_SendData(USART2, '\r');
	delay(1000);

	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

void send_int_toUsart1(int value){
    char data[10]='/0';
    int idx = 0;

    sprintf(data, "%d", value);
	/*
	int count = 0;
	int quot = value;
	int rem = 0;
	char temp;

	while(!quot) {
		rem = quot%10;
		quot = quot/10;
		data[idx++] = '0' + rem;
		count++;
	}
	
	for (idx = 0; idx < count/2; idx++) {		//역순 바로잡기
		temp = data[idx];
		data[idx] = data[(count-1) - idx];
		data[(count - 1) - idx] = temp;
	}
	data[idx] = '\0';
	*/

    for(idx = 0; idx < strlen(data); idx++){
        USART_SendData(USART1, data[idx]);
        delay(1000);
    }
	USART_SendData(USART2, '\n');
	delay(1000);
	USART_SendData(USART2, '\r');
	delay(1000);
}

void send_int_toUsart2(int value){
    char data[10]='/0';
    int idx = 0;

    sprintf(data, "%d", value);

    for(idx = 0; idx < strlen(data); idx++){
        USART_SendData(USART2, data[idx]);
        delay(1000);
    }
	USART_SendData(USART2, '\n');
	delay(1000);
	USART_SendData(USART2, '\r');
	delay(1000);
}

int receive_int_byUsart1(){
	char data[10]='/0';
	int idx = 0;
	int value = 0;

	for(idx = 0; idx < strlen(data); idx++){
        data[idx] = USART_ReceiveData(USART1);
        delay(1000);
    }
	value = atoi(data);	

	return value;
}

int receive_int_byUsart2(){
	char data[10]='/0';
	int idx = 0;
	int value = 0;

	for(idx = 0; idx < strlen(data); idx++){
        data[idx] = USART_ReceiveData(USART2);
        delay(1000);
    }
	value = atoi(data);	

	return value;
}

/*
void send_initial_state(){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char now[20];
	char state[60];
	int idx=0;

	sprintf(now, "%d-%d-%d %d:%d:%d",
			tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
			tm.tm_hour, tm.tm_min, tm.tm_sec);

	for(idx = 0; idx < strlen(now); idx++){
        USART_SendData(USART2, now[idx]);
        delay(1000);
    }
	USART_SendData(USART2, '\n');
	delay(1000);
	USART_SendData(USART2, '\r');
	delay(1000);

	/////센서 초기화상태 표시//////
}
*/

int main() {
	SystemInit();
	_BT_Init();

	//	while(1)
	//	{
	//		if(실행명령어("CONNST") : receive_str_byUsart2 구현?!)
	//		{
	//			서보&센서 Init
	//			초기화 메세지 출력
	//
	//			while(!실행명령어("CONNED"))
	//			{
	//				탐지상태, 추적상태
	//			}
	//		}
	//	}
}
