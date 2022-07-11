///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright (C) 2020 HanvonUgee Technology Corp. All rights reserved.
//FileName: usb_delay.c
//Brief: USB delay driver
//Platform:GD32F350xxx
//Ver: V1.0.0
//Date:2020.07.15
//Author:
//Modify: Felix_wang
//  1.ÕûÀí´úÂë
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "usb_delay.h"



__IO uint32_t delay_num = 0;

extern uint8_t timer_prescaler;


static void delay_time_set (uint8_t unit);



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      timer nvic initialization
//    \param[in]  none
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void usb_timer_nvic_init(void)
{
    //nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(TIMER13_IRQn, 2U, 0U);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    \brief      timer interrupt handler
//    \param[in]  none
//    \param[out] none
//   \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void timer13_delay_irq (void)
{
    if (RESET != timer_flag_get(TIMER13, TIMER_FLAG_UP))
    {
        timer_flag_clear(TIMER13, TIMER_FLAG_UP);

        if (delay_num > 0x00)
        {
            delay_num--;
        }
        else
        {
            timer_disable(TIMER13);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      delay routine in microseconds
//    \param[in]  time_us: delay Time in microseconds
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void usb_delay_us(uint32_t time_us)
{
    delay_num = time_us;
    delay_time_set(TIM_USEC_DELAY);

    while(0U != delay_num);

    timer_disable(TIMER13);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      delay routine in milliseconds
//    \param[in]  time_ms: delay Time in milliseconds
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void usb_delay_ms(uint32_t time_ms)
{
    delay_num = time_ms;
    delay_time_set(TIM_MSEC_DELAY);

    while(0U != delay_num);

    timer_disable(TIMER13);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      set timer
//    \param[in]  unit: choose milliseconds or microseconds
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void delay_time_set (uint8_t unit)
{
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER13);
    timer_deinit(TIMER13);

    if(TIM_USEC_DELAY == unit)
    {
        timer_initpara.period = 11;
    }
    else if(TIM_MSEC_DELAY == unit)
    {
        timer_initpara.period = 11999;
    }

    timer_initpara.prescaler         = timer_prescaler;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER13, &timer_initpara);

    timer_update_event_enable(TIMER13);
    timer_interrupt_enable(TIMER13,TIMER_INT_UP);
    timer_flag_clear(TIMER13, TIMER_FLAG_UP);
    timer_update_source_config(TIMER13, TIMER_UPDATE_SRC_GLOBAL);

    timer_enable(TIMER13);
	
}

