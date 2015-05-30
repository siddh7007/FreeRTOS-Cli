/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


/*==================================================================================================
  hello_world.c
  Used hardware: STM32F4 - Discovery evaluation board
  Example of using the FreeRTOS Real-Time Operating System on the STM32F4Discovery board. It is
  used the GNU Cross-toolchain for ARM microprocessors.
  ==================================================================================================
  Author: Henrique Persico Rossi
  e-mail: henriqueprossi@gmail.com
  ================================================================================================*/

/*==================================================================================================
  INCLUDE FILES
==================================================================================================*/
#include "main.h"


/*=================================================================================================
  LOCAL DEFINITIONS
=================================================================================================*/
/* The size of the stack and the priority used by the two echo client tasks. */
#define mainECHO_CLIENT_TASK_STACK_SIZE 	( configMINIMAL_STACK_SIZE * 2 )
#define mainECHO_CLIENT_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

/* The size of the stack and the priority used by the USB CDC command console
task. */
#define mainUART_COMMAND_CONSOLE_STACK_SIZE		( configMINIMAL_STACK_SIZE * 10 )
#define mainUART_COMMAND_CONSOLE_TASK_PRIORITY  	4

#define MOTOR_CONTROL_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define MOTOR_CONTROL_TASK_PRIORITY	0

#define   CNT_FREQ         21000000                           // TIM4 counter clock (prescaled APB1)
#define   IT_PER_SEC       2000                               // Interrupts per second
#define   TIM4_PULSE       ((CNT_FREQ) / (IT_PER_SEC))        // Output compare reg value

/*==================================================================================================
  LOCAL FUNCTIONS
==================================================================================================*/
volatile unsigned long ulHighFrequencyTimerTick = 0UL;

void vConfigureTimerForRunTimeStats( void )
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef       TIM4_OC;

	TIM4_Config();

	 //(uint16_t) PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 28000000) - 1;
	//PrescalerValue = (uint16_t) 0;
	  /* Time base configuration */

	  TIM_TimeBaseStructure.TIM_Period = 65535;
	  TIM_TimeBaseStructure.TIM_Prescaler = 3 ;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	  TIM4_OC.TIM_OCMode      = TIM_OCMode_Toggle;                // Output compare toggling mode
	  TIM4_OC.TIM_OutputState = TIM_OutputState_Enable;           // Enabling the Output Compare state
	  TIM4_OC.TIM_OCPolarity  = TIM_OCPolarity_Low;               // Reverse polarity
	  TIM4_OC.TIM_Pulse       = TIM4_PULSE ;                       // Output Compare 1 reg value
	  TIM_OC1Init(TIM4, &TIM4_OC);                                // Initializing Output Compare 1 structure
	  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);          // Disabling Ch.1 Output Compare preload

	  TIM_Cmd(TIM4, ENABLE);                                      // Ready, Set, Go!
	  TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);                     // Enabling TIM4 Ch.1 interrupts

}

void updatetasktimtick(void)
{
	ulHighFrequencyTimerTick++ ;
	STM_EVAL_LEDToggle(LED3);
}

static void leds_blink_task(void * parameters);

/*==================================================================================================
  LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
  Function    : vApplicationIdleHook
  Description : The idle task can optionally call an application defined hook (or callback)
                function - the idle hook. The idle task runs at the very lowest priority, so such
                an idle hook function will only get executed when there are no tasks of higher
                priority that are able to run. This makes the idle hook function an ideal place to
                put the processor into a low power state - providing an automatic power saving
                whenever there is no processing to be performed.
                The idle hook will only get called if configUSE_IDLE_HOOK is set to 1 within
                FreeRTOSConfig.h.
                The idle hook is called repeatedly as long as the idle task is running. It is
                paramount that the idle hook function does not call any API functions that could
                cause it to block. Also, if the application makes use of the vTaskDelete() API
                function then the idle task hook must be allowed to periodically return (this is
                because the idle task is responsible for cleaning up the resources that were
                allocated by the RTOS kernel to the task that has been deleted).
  Parameters  : None
  Returns     : None
==================================================================================================*/

void vApplicationIdleHook(void)
{
  // TODO: Put the processor into a low power state.
}

/*==================================================================================================
  Function    : vApplicationStackOverflowHook
  Description : Stack overflow is a very common cause of application instability. FreeRTOS therefore
                provides two optional mechanisms that can be used to assist in the detection and
                correction of just such an occurrence. The option used is configured using the
                configCHECK_FOR_STACK_OVERFLOW configuration constant. The application must provide
                a stack overflow hook function if configCHECK_FOR_STACK_OVERFLOW is not set to 0.
                The xTask and pcTaskName parameters pass to the hook function the handle and name of
                the offending task respectively. Note however, depending on the severity of the
                overflow, these parameters could themselves be corrupted, in which case the
                pxCurrentTCB variable can be inspected directly.
                Stack overflow checking introduces a context switch overhead so its use is only
                recommended during the development or testing phases.
  Parameters  : xTask       Handle of the offending task.
                pcTaskName  Name of the offending task.
  Returns     : None
==================================================================================================*/

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
  // TODO: It is a place to add stack overflow processing.
}

/*==================================================================================================
  Function    : vApplicationTickHook
  Description : The tick interrupt can optionally call an application defined hook (or callback)
                function - the tick hook. The tick hook provides a convenient place to implement
                timer functionality.
                The tick hook will only get called if configUSE_TICK_HOOK is set to 1 within
                FreeRTOSConfig.h.
                vApplicationTickHook() executes from within an ISR so must be very short, not use
                much stack, and not call any API functions that don't end in "FromISR" or "FROM_ISR".
  Parameters  : None
  Returns     : None
==================================================================================================*/

void vApplicationTickHook(void)
{
  // TODO: It is a place to add timer processing.
}

/*==================================================================================================
  Function    : vApplicationMallocFailedHook
  Description : The memory allocation schemes implemented by heap_1.c, heap_2.c, heap_3.c and heap_4
                can optionally include a malloc() failure hook (or callback) function that can be
                configured to get called if pvPortMalloc() ever returns NULL.
                Defining the malloc() failure hook will help identify problems caused by lack of
                heap memory - especially when a call to pvPortMalloc() fails within an API function.
                The malloc failed hook will only get called if configUSE_MALLOC_FAILED_HOOK is set
                to 1 within FreeRTOSConfig.h.
  Parameters  : None
  Returns     : None
==================================================================================================*/
xSemaphoreHandle read_byte = 0;

void vApplicationMallocFailedHook(void)
{
  // TODO: It is a place to add processing for error on allocating memory.
}

/*==================================================================================================
  Function    : main
  Description : Entry point of the user application. It only set/clear the LD3, LD4, LD5 and LD6 leds
                on the board.
  Parameters  : None
  Returns     : EXIT_SUCCESS:	Successful termination
                Otherwise:		Error
==================================================================================================*/

int main(void)
{

  /* At this point the microcontroller clock is already set, which is done through SystemInit()
   * function. This is done by means of the C runtime initialization (crt.c), which, afterwards,
   * call the main application.
   */

  /* Update SystemCoreClock variable according to Clock Register Values. The tick frequency is
   * set by the scheduler.
   */
  SystemCoreClockUpdate();

  SystemInit();

  LED_Config();




  /* Most systems default to the wanted configuration, with the noticeable exception of the STM32
   * driver library. If you are using an STM32 with the STM32 driver library then ensure all the
   * priority bits are assigned to be preempt priority bits by calling
   * NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4) before the RTOS is started.
   */

 // vSemaphoreCreateBinary(read_byte);

	/* Set the Vector Table base address at 0x08000000 */
  NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );

  vRegisterCLICommands();

  vUARTCommandConsoleStart( mainUART_COMMAND_CONSOLE_STACK_SIZE, mainUART_COMMAND_CONSOLE_TASK_PRIORITY );

  vMotorControlStart( MOTOR_CONTROL_STACK_SIZE,  MOTOR_CONTROL_TASK_PRIORITY );

  // The task for blinking the led's is created. It is called just after the scheduler is started.
  xTaskCreate(  leds_blink_task           ,
                "leds_blink_task"         ,
                configMINIMAL_STACK_SIZE  ,
                NULL                      ,
                0     				      ,
                NULL                      );

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  vConfigureTimerForRunTimeStats();

  // It is started the scheduler.

  vTaskStartScheduler();

  /* If all is well, the scheduler will now be running, and the following line will never be reached.
   * If the following line does execute, then there was insufficient FreeRTOS heap memory available
   * for the idle and/or timer tasks to be created. See the memory management section on the
   * FreeRTOS web site for more details.
   */
  for (;;);

  return 0;
}

/*==================================================================================================
  Function    : leds_blink_task
  Description : This is the task which controls the led's (LED3, LED4, LED5 and LED6) on the board
                by lighting them individually and sequentially every second. Once all of them are
                on, their states are set to off, and the cycle restarts.
  Parameters  : parameters: A parameter which is passed to the task when it started.
  Returns     : None
==================================================================================================*/

static void leds_blink_task(void * parameters)
{
  for (;;) {

	//UART_write(USART1, " <<<<<<<<<<<<<<<<<<< STM32 USART >>>>>>>>>>>>>>>>>>>>\n \r ");
	GPIO_SetBits(GPIOD, LED4_PIN); // LED4 ON
	vTaskDelay(1000);
    GPIO_SetBits(GPIOD, LED3_PIN); // LED3 ON
    vTaskDelay(1000);
    GPIO_SetBits(GPIOD, LED5_PIN); // LED5 ON
    vTaskDelay(1000);
    GPIO_SetBits(GPIOD, LED6_PIN); // LED6 ON
    vTaskDelay(1000);
    GPIO_ResetBits(GPIOD, LED4_PIN | LED3_PIN | LED5_PIN | LED6_PIN);  // All LEDs are OFF.
    vTaskDelay(1000);
  }
}
