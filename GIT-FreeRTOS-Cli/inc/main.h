#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


// board
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

//UART AND OTHER PERIFERLS
#include "peripherals.h"
#include <misc.h>
#include <stm32f4xx_usart.h>

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "FreeRTOS_CLI.h"



extern void vApplicationIdleHook(void);
extern void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName);
extern void vApplicationTickHook(void);
extern void vApplicationMallocFailedHook(void);
extern int main(void);



#define logoElementsDelay() delayMillis(500);

I2C_TypeDef I2C_1;
GPIO_InitTypeDef GPIO_InitStructureuart;
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

/* Private define ------------------------------------------------------------*/

#define BUFFER_SIZE 32

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


//notice the use of the volatile keyword this is important as without it the compiler may make
//optimisations assuming the value of this variable hasnt changed
volatile char received_buffer[BUFFER_SIZE+1];


//static uint8_t DataReceivedCounter = 0; //tracks the number of characters received so far, reset after each command



/* Private function prototypes -----------------------------------------------*/
void UART_write(USART_TypeDef* USARTx, volatile char *s);
void USARTCommandReceived(char * command);
void ClearCommand();
void Delay(int nCount);
void UART_Config(int baudrate);

void vUARTCommandConsoleStart( uint16_t usStackSize, UBaseType_t uxPriority );
void vMotorControlStart( uint16_t mcStackSize, UBaseType_t mcPriority );
void vLEDControlStart( uint16_t lcStackSize, UBaseType_t lcPriority );
#endif
