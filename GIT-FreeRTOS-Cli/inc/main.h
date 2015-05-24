#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>


// board
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#include <stdint.h>
#include <stdlib.h>
#include <string.h>

extern void vApplicationIdleHook(void);
extern void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName);
extern void vApplicationTickHook(void);
extern void vApplicationMallocFailedHook(void);
extern int main(void);



#define logoElementsDelay() delayMillis(500);

#endif
