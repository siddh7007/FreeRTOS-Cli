#include "main.h"
#include "StepperControlTask.h"

/*-----------------------------------------------------------*/

//GLOBAL
volatile int16_t Codeur_count;
volatile int32_t Codeur_total;
int8_t RotationDirection_CW = 1 ;
int8_t RotationDirection_CCW = 0 ;
int32_t count = 0;
int32_t cyclecount;
volatile char *wr[32] ;
int n = 0 ;
int m = 0;
char CW = 1 ;
char CCW = 0;

//LOCAL
static volatile int16_t Codeur_old;
static volatile int16_t Codeur_actual;
char buffer[33];

/*
 * The task that implements the command console processing.
 */
static void prvMotorControlTask( void *pvParameters );
void vMotorControlStart( uint16_t mcStackSize, UBaseType_t mcPriority );


void vMotorControlStart( uint16_t mcStackSize, UBaseType_t mcPriority )
{

	/* Create the semaphore used to access the UART Tx.
	xTxMutex = xSemaphoreCreateMutex();
	configASSERT( xTxMutex ); */

	/* Create that task that handles the console itself. */
	xTaskCreate( 	prvMotorControlTask,	    /* The task that implements the Motor Control. */
					"Stepper  MTR CTRL ",		/* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
					mcStackSize,				/* The size of the stack allocated to the task. */
					NULL,						/* The parameter is not used, so NULL is passed. */
					mcPriority,					/* The priority allocated to the task. */
					NULL );						/* A handle is not required, so just pass NULL. */
}



static void prvMotorControlTask( void *pvParameters )
{
	//pwm_initconfig(5000);
while(1)
{
	vTaskDelay(1000);
}

}

int pwm_initconfig(uint32_t OutFreq,uint32_t steps)

{

		Stepper_Control(ENABLE);
		TIM3_Config();

		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;

		uint16_t PrescalerValue ;

		// Compute the Pre-scaler value

		uint32_t TimFreq;
		uint16_t Period ;

		// TIM3 is 16bit timer so period,s max value can Be (0xffff) 65535
		// that's why the lowest frequency we can achive is around 54 hz, with this config.

		TimFreq = 7000000 ;
		Period  = (TimFreq/OutFreq)- 1;

		PrescalerValue = (uint16_t) ((SystemCoreClock /2) / TimFreq) - 1;

		sprintf(wr, "************* Start: Prescal = %d period = %d \r\n ", PrescalerValue, Period);
	    UART_write(USART1, wr);

		//PrescalerValue = (uint16_t) 0;
		//Time base configuration
		TIM_TimeBaseStructure.TIM_Period = Period ;
		TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned3;

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		// PWM1 Mode configuration: Channel1
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = Period / 2;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

		TIM_OC1Init(TIM3, &TIM_OCInitStructure);

		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);


		TIM_ARRPreloadConfig(TIM3, ENABLE);
		m = steps;
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		sprintf(wr, "************* Start: Freq = %d Steps = %d \r\n ", OutFreq, steps);
	    UART_write(USART1, wr);

		// TIM IT enable
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

		// TIM3 enable counter
		TIM_Cmd(TIM3, ENABLE);

		return 0 ;
}

//Disable and reset TIM3 - PWM output
int pwm_deinitconfig(void)

{

	Stepper_Control(DISABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);

	sprintf(wr, "************* Stop: finished Steps = %d \r\n ", n/2);
    UART_write(USART1, wr);
    GPIO_ResetBits(GPIOD, GPIO_Pin_15);
    n= 0;
    m =0;

	return 0 ;
}

//Stepper-Motor drive control ENABLE / DISABLE
void Stepper_Control(int Control)

{
	if (Control) GPIO_ResetBits(GPIOD,GPIO_Pin_3);
	else GPIO_SetBits(GPIOD,GPIO_Pin_3);
}


//Set Stepper-Motor direction CW = ClockWise / CCW = CounterClockWise
void Stepper_Direction(int Rotation)

{
	if (Rotation == RotationDirection_CW ) GPIO_SetBits(GPIOD,GPIO_Pin_3);
	else GPIO_ResetBits(GPIOD,GPIO_Pin_3);
}

//Encoder Read
int32_t Encoder_Read(void)
{
    Codeur_old = Codeur_actual;
    Codeur_actual = TIM_GetCounter (TIM2) ;
    Codeur_count = Codeur_actual - Codeur_old;
    Codeur_total += Codeur_count;
    itoa (Codeur_total,buffer,10);
    UART_write(USART1, buffer);
    UART_write(USART1, "\r ");
    return Codeur_total;
}

//Encoder counter reset
void Encoder_Reset(void)

{

    __disable_irq();
    Codeur_old = 0;
    Codeur_total = 0;
    TIM_SetCounter (TIM2, 0);
    Encoder_Read();
    __enable_irq();

}


void pulse_counter()

{
	  if ((n == m)) {
		  pwm_deinitconfig();
	  }

	  else n++ ;
}


//****************************** Clamp Related functions *****************************//


void clamp_home(void)

{
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) & GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) )

	{
		do {

			Stepper_Direction(CCW) ;
			pwm_initconfig(1000,1000) ;

		} while ( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14)) ;
	}

}

int cycle_counter(int32_t Cycle)

{
	sprintf(wr, "************* Requested Clamp Cycle : %d \n\n ",Cycle);
	UART_write(USART1, wr);
	clamp_home();

	do {
			int a = 0 ;
			do {

				Stepper_Direction(CW) ;
				pwm_initconfig(1000,1000) ;

				} while ( a == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15)) ;

			vTaskDelay(100);

			do {

				Stepper_Direction(CW) ;
				pwm_initconfig(1000,1000) ;

				} while ( a == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14)) ;



			cyclecount++;
			vTaskDelay(100);

			sprintf(wr, "************* Clamp Cycle : %d \r\n ",cyclecount);
			UART_write(USART1, wr);

	} while (Cycle == cyclecount);

	return 0;

}



