/*
 * LEDControlTask.c
 *
 *  Created on: Jun 11, 2015
 *      Author: patels
 */
#include "main.h"
#include "LEDControlTask.h"


/*==================================================================================================
  Function    : leds_blink_task
  Description : This is the task which controls the led's (LED3, LED4, LED5 and LED6) on the board
                by lighting them individually and sequentially every second. Once all of them are
                on, their states are set to off, and the cycle restarts.
  Parameters  : parameters: A parameter which is passed to the task when it started.
  Returns     : None
==================================================================================================*/
/*
 * The task that implements the command console processing.
 */
static void prvLEDControlTask( void *pvParameters );
void vLEDControlStart( uint16_t lcStackSize, UBaseType_t lcPriority );


void vLEDControlStart( uint16_t lcStackSize, UBaseType_t lcPriority )

{
  // The task for blinking the led's is created. It is called just after the scheduler is started.
  xTaskCreate(  prvLEDControlTask          ,
                "LEDs BLINK CTRL"         ,
				lcStackSize               ,
                NULL                      ,
				lcPriority     		      ,
                NULL                      );

  }

static void prvLEDControlTask(void * pvparameters)
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
