#include "pwm_ctr.h"




int pwm_initconfig()

{
	  /* -----------------------------------------------------------------------
	    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles.

	    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1),
	    since APB1 prescaler is different from 1.
	      TIM3CLK = 2 * PCLK1
	      PCLK1 = HCLK / 4
	      => TIM3CLK = HCLK / 2 = SystemCoreClock /2

	    To get TIM3 counter clock at 28 MHz, the prescaler is computed as follows:
	       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
	       Prescaler = ((SystemCoreClock /2) /28 MHz) - 1

	    To get TIM3 output clock at 30 KHz, the period (ARR)) is computed as follows:
	       ARR = (TIM3 counter clock / TIM3 output clock) - 1
	           = 665

	           ARR will be 13 for 2MHz

	    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
	    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
	    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
	    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%

	    Note:
	     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
	     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
	     function to update SystemCoreClock variable value. Otherwise, any configuration
	     based on this variable will be incorrect.
	  ----------------------------------------------------------------------- */

	  /* Compute the prescaler value */
	  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 56000000) - 1;
	  //PrescalerValue = (uint16_t) 0;
	  /* Time base configuration */
	  TIM_TimeBaseStructure.TIM_Period = 2;
	  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	  /* PWM1 Mode configuration: Channel1 */
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	  /* PWM1 Mode configuration: Channel2 */
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

	  TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	  /* PWM1 Mode configuration: Channel3 */
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

	  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

	  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	  /* PWM1 Mode configuration: Channel4 */
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

	  TIM_OC4Init(TIM3, &TIM_OCInitStructure);

	  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	  TIM_ARRPreloadConfig(TIM3, ENABLE);

	  /* TIM3 enable counter */
	  TIM_Cmd(TIM3, ENABLE);

 return 0 ;
}



setpwm_freq_dutycycle(int freq,int dutycycle)

{


}
