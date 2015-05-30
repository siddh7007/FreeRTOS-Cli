/*
 * peripherals.h
 *
 *  Created on: May 20, 2015
 *      Author: patels
 */

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_


#include <stm32f4xx.h>
#include <misc.h>
#include <stm32f4xx_usart.h>
#include "stm32f4_discovery.h"
#include <stdio.h>
#include <stdint.h>



/* Private function prototypes -----------------------------------------------*/

void TIM3_Config(void);
void TIM4_Config(void);
void I2C_Config(void);
void LED_Config(void);
void UART_Config(int baudrate);


#endif /* PERIPHERALS_H_ */
