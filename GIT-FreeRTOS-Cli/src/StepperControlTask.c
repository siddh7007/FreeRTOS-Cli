#include "main.h"
#include "StepperControlTask.h"

/*-----------------------------------------------------------*/

//GLOBAL
volatile int16_t Codeur_count;
volatile int32_t Codeur_total;
int8_t RotationDirection_CW = 1 ;
int8_t RotationDirection_CCW = 0 ;
int32_t count = 0;
uint32_t cyclecount = 0;
uint32_t n = 0 ;
uint32_t m = 0;
char CW = 1 ;
char CCW = 0;
int pwminitflag = 0 ;

//LOCAL
char buffer[33];
uint32_t TimFreq = 7000000;
uint16_t Period ;
uint16_t PrescalerValue ;

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

while(1)
{
	vTaskDelay(1000);
}

}

int pwm_initconfig(uint32_t OutFreq)

{
		Stepper_Drive_Control_GPIO_Config();
		TIM3_Config();
		Stepper_Control(ENABLE);

		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;

		// Compute the Pre-scaler value
		// TIM3 is 16bit timer so period,s max value can Be (0xffff) 65535
		// that's why the lowest frequency we can achive is around 54 hz, with this config.

		Period  = (TimFreq/OutFreq)- 1;
		PrescalerValue = (uint16_t) ((SystemCoreClock /2) / TimFreq) - 1;

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
		TIM_OCInitStructure.TIM_Pulse = 1750;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

		TIM_OC1Init(TIM3, &TIM_OCInitStructure);

		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);


		TIM_ARRPreloadConfig(TIM3, ENABLE);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);

		// TIM IT enable
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

		// TIM3 enable counter
		//TIM_Cmd(TIM3, ENABLE);
		pwminitflag = 1 ;

		return 0 ;
}


int pwm_steps(uint32_t steps,int Rot)

{
	steps = steps*2 -1 ;
	sprintf(wr, "************* pwminitflag = %d \r\n ", pwminitflag);
    UART_write(USART1, wr);

	if(pwminitflag == 0) pwm_initconfig(2000) ;
	else {

		sprintf(wr, "************* start : Steps = %d \r\n ", steps);
	    UART_write(USART1, wr);

		Stepper_Direction(Rot);
		m = steps;
		Stepper_Control(ENABLE);

		// TIM3 enable counter
		TIM_Cmd(TIM3, ENABLE);
	}

		return 0 ;
}



//Disable and reset TIM3 - PWM output
int pwm_deinitconfig(void)

{

	Stepper_Control(DISABLE);
//	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	sprintf(wr, "************* Stop: finished Steps = %d \r\n ", n/2);
    UART_write(USART1, wr);
    n= 0;
    m =0;
    pwminitflag = 0 ;

	return 0 ;
}

//Stepper-Motor drive control ENABLE / DISABLE
void Stepper_Control(int Control)

{

	if (Control) GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	else GPIO_SetBits(GPIOA,GPIO_Pin_8);
}


//Set Stepper-Motor direction CW = ClockWise / CCW = CounterClockWise
void Stepper_Direction(int Rotation)

{
	if (Rotation == RotationDirection_CW ) GPIO_SetBits(GPIOC,GPIO_Pin_8);
	else GPIO_ResetBits(GPIOC,GPIO_Pin_8);
}

void pulse_counter()

{
	  if ((n == m))
		  {
		  TIM_Cmd(TIM3, DISABLE);
		  Stepper_Control(DISABLE) ;
		  n = 0;
		  m = 0;
		  }
	  else n++ ;
}


//****************************** Clamp Related functions *****************************//


void clamp_home(void)

{



	if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5))

	{
		while (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)){
			vTaskDelay(50);
			pwm_steps(50,CW) ;

			sprintf(wr, "************* Opto Status : %d \n\n ",GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5));
			UART_write(USART1, wr);
		}

	}

}

int cycle_counter(int32_t Frequncy,int32_t Cycle)

{
	sprintf(wr, "************* Requested Clamp Cycle : %d \n\n ",Cycle);
	UART_write(USART1, wr);
	//clamp_home();

uint32_t e ;
	for( e=0 ; e<= Cycle;e++) {



				pwm_steps(2500,CCW) ;

				vTaskDelay(2000);

				pwm_steps(2500,CW) ;

				vTaskDelay(2500);

//				while( GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) )
//				{
//					pwm_steps(50 ,CW) ;
//
//					vTaskDelay(50);
//				}


			sprintf(wr, "************* Clamp Cycle : %d Clamp Cyclecount : %d\r\n ",cyclecount,cyclecount);
			UART_write(USART1, wr);

	}
	Cycle = 0;

	return 0;

}


//
////Encoder Read
//int32_t Encoder_Read(void)
//{
//    Codeur_old = Codeur_actual;
//    Codeur_actual = TIM_GetCounter (TIM2) ;
//    Codeur_count = Codeur_actual - Codeur_old;
//    Codeur_total += Codeur_count;
//    itoa (Codeur_total,buffer,10);
//    UART_write(USART1, buffer);
//    UART_write(USART1, "\r ");
//    return Codeur_total;
//}
//
////Encoder counter reset
//void Encoder_Reset(void)
//
//{
//
//    __disable_irq();
//    Codeur_old = 0;
//    Codeur_total = 0;
//    TIM_SetCounter (TIM2, 0);
//    Encoder_Read();
//    __enable_irq();

//}




