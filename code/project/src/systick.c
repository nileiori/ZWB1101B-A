///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright (C) 2020 HanvonUgee Technology Corp. All rights reserved.
//FileName: systick.c
//Brief: the systick configuration file
//Platform:GD32F350xxx
//Ver: V1.0.0
//Date:2020.07.15
//Author:
//Modify: Felix_wang
//	1.????????
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "gd32f3x0.h"
#include "config.h"
#include "systick.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
volatile static uint32_t delay;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      configure systick
//    \param[in]  none
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void systick_config(void)
{
    // setup systick timer for 200Hz interrupts
    if (SysTick_Config(SystemCoreClock / UG_TICK_PER_SECOND))
    {
        // capture error
        while (1) ;

    }
    // configure the systick handler priority
    //NVIC_SetPriority(SysTick_IRQn, 0x0fU);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      delay a time in milliseconds
//    \param[in]  count: count in milliseconds
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void delay_1ms(uint32_t count)
{
    delay = count;

    while(0U != delay)	;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      delay decrement
//    \param[in]  none
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void delay_decrement(void)
{
    if (0U != delay)
    {
        delay--;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////


