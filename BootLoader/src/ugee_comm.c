///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright (C) 2020 HanvonUgee Technology Corp. All rights reserved.
//FileName: ugee_comm.c
//Brief: ugee common functions 
//Platform:GD32F350xxx
//Ver: V1.0.0
//Date:2020.07.16
//Author: liming
//Modify: Felix_wang
//	1.ÕûÀí´úÂë
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "gd32f3x0_it.h"
#include "ugee_comm.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    \brief      configure the gpio peripheral
//    \param[in]  none
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gpio_config(void)
{
//rcu_periph_clock_enable(RCU_AF);    
	rcu_periph_clock_enable(RCC_AHBPeriph_GPIO_LED);

// configure led GPIO port */ 
	gpio_mode_set(GPIO_LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,LED_R_PIN | LED_PIN );
	gpio_output_options_set(GPIO_LED_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_R_PIN | LED_PIN);
	GPIO_BC(GPIO_LED_PORT) = LED_PIN;

// configure led GPIO port */ 
	//gpio_mode_set(LED2_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,LED2_PIN );
	//gpio_output_options_set(LED2_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED2_PIN);
	//GPIO_BOP(LED2_GPIO_PORT) = LED2_PIN;

	rcu_periph_clock_enable(RCC_AHBPeriph_GPIO_KEYBOOT);	
	gpio_mode_set(KEYBOOT_GPIO_PORT,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,KEYBOOT_PIN);

}




