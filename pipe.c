#include "pipe.h"

void delay(const int num){
    int remain = num;
    while((remain--) > 0);
}

void _System_Init(void){
	SystemInit();
	_RCC_Init();
    _USART_Init();
	_GPIO_Init();
    // interrupt setting
    _EXTI_Init();
    _NVIC_Init();
    // analog digital
	_ADC_Init();
    // DMA setting
	_DMA_Init();

    // analog start
	_ADC_Start();
}

void _RCC_Init(void){
    /* [SAMPLE]
    RCC_APB2PeriphClockCmd(( RCC_APB2Periph_GPIOC
                           | RCC_APB2Periph_GPIOD)
                           , ENABLE);
    */
   ultra_RCC_Init();
}

void _GPIO_Init(void){
    /* [SAMPLE]
    // below usage is initialization GPIO for ADC
    GPIO_InitTypeDef gpio_init_struct;EXTI_ClearITPendingBit
    gpio_init_struct.GPIO_Pin = GPIO_Pin_1 |EXTI_ClearITPendingBit GPIO_Pin_2;
    gpio_init_struct.GPIO_SpeedEXTI_ClearITPendingBit
    gpio_init_struct.GPIO_Mode EXTI_ClearITPendingBit
    GPIO_Init(GPIOC, &gpio_initEXTI_ClearITPendingBit
    */
}

void _USART_Init(){
    /* [SAMPLE]
    // GPIO SEtting PA9(Tx), PAEXTI_ClearITPendingBit
    USART_InitTypeDef usart_iniEXTI_ClearITPendingBit

    usart_init_struct.USART_BauEXTI_ClearITPendingBit
    usart_init_struct.USART_HarEXTI_ClearITPendingBitol_None;
    usart_init_struct.USART_ModEXTI_ClearITPendingBitde_Tx);
    usart_init_struct.USART_ParEXTI_ClearITPendingBit
    usart_init_struct.USART_StoEXTI_ClearITPendingBit
    usart_init_struct.USART_WordEXTI_ClearITPendingBit_8b;
    USART_Init(USART1, &usart_init_struct);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    */
}

void _TIM_Init(void){
    /* [SAMPLE]
    TIM_TimeBaseInitTypeDef time_init_struct;

    // [NOTE]
    // STM32's general clock is 72MHz
    // below setting generate 1ms timer
    // result = 1/f_clk * prescaler * period
    // result = 1/(72*10^6) * 12000 * 60
    time_init_struct.TIM_Prescaler     = 12000 - 1; // -1 is necessary
    time_init_struct.TIM_CounterMode   = TIM_CounterMode_Up;
    time_init_struct.TIM_Period        = 60 - 1; // -1 is necessary
    time_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM2, &time_init_struct);

    TIM_Cmd(TIM2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    */
}

void _NVIC_Init(void){
    /* [SAMPLE]
    // set the NVIC interrupt table for Timer
    NVIC_InitTypeDef nvic_init_struct;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    nvic_init_struct.NVIC_IRQChannel                   = TIM2_IRQn;
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 0x01;
    nvic_init_struct.NVIC_IRQChannelSubPriority        = 0x01;
    nvic_init_struct.NVIC_IRQChannelCmd                = ENABLE;

    NVIC_Init(&nvic_init_struct);
    */
}

void _EXTI_Init(void){
    /* [SAMPLE] 
    // get EXTI IRQ setting
   EXTI_InitTypeDef exti_init_struct;

   exti_init_struct.EXTI_Line    = EXTI_Line3;
   exti_init_struct.EXTI_Mode    = EXTI_Mode_Interrupt;
   exti_init_struct.EXTI_Trigger = EXTI_Trigger_Falling;
   exti_init_struct.EXTI_LineCmd = ENABLE;

   EXTI_Init(&exti_init_struct);
   EXTI_ClearITPendingBit(EXTI_Line3);
   // EXTI_ClearITPendingBit(USART_IT_RXNE); // if you use usart
    */
}

void _ADC_Init(void){
    /* [SAMPLE]
    // open ADC 2 channel
    ADC_InitTypeDef adc_init_struct;
    ADC_DeInit(ADC1);

    adc_init_struct.ADC_Mode               = ADC_Mode_Independent;
    adc_init_struct.ADC_ScanConvMode       = ENABLE;
    adc_init_struct.ADC_ContinuousConvMode = ENABLE;
    adc_init_struct.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
    adc_init_struct.ADC_DataAlign          = ADC_DataAlign_Right;
    adc_init_struct.ADC_NbrOfChannel       = 2;
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_55Cycles5);

    ADC_Init(ADC1, &adc_init_struct);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    */
}

void _ADC_Start(void){
    /* [SAMPLE]
    // start ADC setting
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    */
}

void _DMA_Init(void){
    /* [SAMPLE]
	DMA_InitTypeDef dma_init_struct;
	DMA_DeInit(DMA1_Channel1);
	dma_init_struct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	dma_init_struct.DMA_MemoryBaseAddr     = (uint32_t) lux;
	dma_init_struct.DMA_DIR                = DMA_DIR_PeripheralSRC;
	dma_init_struct.DMA_BufferSize         = 2;
	dma_init_struct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	dma_init_struct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	dma_init_struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	dma_init_struct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;
	dma_init_struct.DMA_Mode               = DMA_Mode_Circular;
	dma_init_struct.DMA_Priority           = DMA_Priority_High;
	dma_init_struct.DMA_M2M                = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel1, &dma_init_struct);
	DMA_Cmd(DMA1_Channel1, ENABLE);
    */
}