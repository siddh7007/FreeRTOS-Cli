/*
 * peripherals.c

 *
 *  Created on: May 20, 2015
 *      Author: patels
 */

//#include "main.h"
#include "peripherals.h"

/*================ LEDs and Button Configuration ================================  */

void LED_Config(void)

{
	 GPIO_InitTypeDef   GPIO_InitStructure;


	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	  GPIO_InitStructure.GPIO_Pin   = LED4_PIN | LED3_PIN | LED5_PIN | LED6_PIN;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);
	  STM_EVAL_PBInit(BUTTON_USER , BUTTON_MODE_GPIO);

}

/*================ TIM3 Configuration ============================================  */

void TIM3_Config(void)

{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_TIM3;

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* GPIOC and GPIOB clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);

	  NVIC_TIM3.NVIC_IRQChannel                   = TIM3_IRQn;    // Specifying the channel (stm32f4xx.h)
	  NVIC_TIM3.NVIC_IRQChannelPreemptionPriority = 7;            // Only matters for multiple interrupts
	  NVIC_TIM3.NVIC_IRQChannelSubPriority        = 0;            // Only matters for multiple interrupts
	  NVIC_TIM3.NVIC_IRQChannelCmd                = ENABLE;       // Enabling global interrupt
	  NVIC_Init(&NVIC_TIM3);                                      // Initializing NVIC structure

	/* GPIOC Configuration: TIM3 CH1 (PC6) and TIM3 CH2 (PC7) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* GPIOB Configuration:  TIM3 CH3 (PB0) and TIM3 CH4 (PB1) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect TIM3 pins to AF2 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);


}



/*================ TIM4 Configuration ============================================  */

void TIM4_Config(void)

{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_TIM4;

	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* GPIOC and GPIOB clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);

	  NVIC_TIM4.NVIC_IRQChannel                   = TIM4_IRQn;    // Specifying the channel (stm32f4xx.h)
	  NVIC_TIM4.NVIC_IRQChannelPreemptionPriority = 0;            // Only matters for multiple interrupts
	  NVIC_TIM4.NVIC_IRQChannelSubPriority        = 0;            // Only matters for multiple interrupts
	  NVIC_TIM4.NVIC_IRQChannelCmd                = ENABLE;       // Enabling global interrupt
	  NVIC_Init(&NVIC_TIM4);                                      // Initializing NVIC structure

}


/*================ UART Configuration ==============================================  */

void UART_Config(int baudrate)

{
	//structures used configure the hardware
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	//enable the clocks for the GPIOB and the USART
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//Initialize pins GPIOB 6 and GPIOB 7
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; //we are setting the pin to be alternative function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//Connect the TX and RX pins to their alternate function pins
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); //
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	//configure USART
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //enable send and receive (Tx and Rx)
	USART_Init(USART1, &USART_InitStruct);

	//Enable the interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);


	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// finally this enables the complete USART1 peripheral
	USART_Cmd(USART1, ENABLE);
}


/*======== Stepper-Motor Drive related GPIO Configuration ============================  */

void Stepper_Drive_Control_GPIO_Config(void)

{
	//Enable the clocks for the GPIOB, GPIOA and the TIM2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	//Structures used configure the hardware
	GPIO_InitTypeDef GPIO_InitStruct;

    //Configure the GPIOD pin 3
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

    //Configure the GPIOD pin 4
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}
/*================ Encoder TIM Configuration =========================================  */

void ENCR_Config(void)

{

	//Enable the clocks for the GPIOB, GPIOA and the TIM2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);

	//Structures used configure the hardware
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	//Initialize pins GPIOA 15
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//Initialize pins GPIOB 3
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);

	//Configure TIM2
	TIM_EncoderInterfaceConfig (TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_SetAutoreload (TIM2, 0xffff);

	//Enable Encoder
	TIM_Cmd (TIM2, ENABLE);
}

/*================ GPIOs for OPTO Configuration =========================================  */

void OPTO_Config(void)

{
	//Enable the clocks for the GPIOC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	//Structures used configure the hardware
	GPIO_InitTypeDef GPIO_InitStruct;

	//Initialize pins GPIOC 14 and 15
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

}

