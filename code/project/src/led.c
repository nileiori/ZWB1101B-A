#include "gd32f3x0.h"
#include "Gd32f3x0_timer.h"
#include "config.h"
#include "gpio.h"
#include "stdlib.h"
#include "led.h"



void led_foot_duty_set(uint32_t duty)
{    
    if (duty > 90)
    {
        duty = 90;
    }
    else if (duty < 10)
    {
        duty = 0;
    }
    
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_3, duty);
}

static led_control    led_handle ;

/**************************************************************************************************/
/*                                                                                                */
/*                    led_mode_config                                                             */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief   Config the mode of the led flash
 * @param   mode: mode select
 *          times:the numbers of flashes .especial:led continuous flashing when parameter is set to 0xff
 *          freq:the frequency of flash.especial:led stays on when parameter is set to 0xffff
 * @retval  None
 */
void led_mode_config(e_mode mode,uint8_t times,uint16_t freq)
{
    if(led_handle.mode == mode)return;
    
    led_handle.mode           = mode;
    led_handle.flash_times   = times;
    led_handle.freq          = freq;
    led_handle.shadowfreq    = freq;
}
void led_off(void)
{
	
    led_handle.flash_times   = 0;
    led_handle.mode           = e_idle;
    LED_OFF;
    //LED_B_OFF;
    //LED_R_OFF;
}
void led_set_idle(void)
{

    led_handle.mode           = e_idle;
}
uint8_t led_get_mode(void)
{
    return (uint8_t)led_handle.mode;
}

/**************************************************************************************************/
/*                                                                                                */
/*                    led_manage                                                                  */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief   Manage the methods of the led flash
 * @param   hd: pointer to led handle
 * @retval  None
 */
void led_manage(led_control_t hd)
{
	
    if(hd->flash_times > 0)
    {
        if(0xffff != hd->freq)
        {
            hd->freq--;
            if(0 == hd->freq)
            {
                if(0xff != hd->flash_times)
                {
                    hd->flash_times--;
                }
                if(0 == hd->flash_times)
                {
                    hd->mode = e_idle;
                    LED_OFF;
                }
                else
                {
                    hd->freq = hd->shadowfreq;
                }
                
            }
        }
                
        if(e_idle != hd->mode)
        {
            if(hd->freq > hd->shadowfreq/2)
            {
                if(e_wheel == hd->mode)
                {
                    LED_B_ON;
                }
                else if(e_touch_panel== hd->mode)
                {
                    //LED_R_ON;
                }
                else
                {
                    LED_ON;
                }
            }
            else
            {
                if(e_wheel == hd->mode)
                {
                    LED_B_OFF;
                }
                else if(e_touch_panel== hd->mode)
                {
                    //LED_R_OFF;
                }
                else
                {
                    LED_OFF;
                }
            }
            
        }
    }
}
EventStatus led_flash_monitor(void)
{
    led_manage(&led_handle);

    return ENABLE;
}


void led_init(void)
{
    //led_handle = (led_control_t)malloc(sizeof(led_control));
    //configASSERT( led_handle );

    led_handle.mode           = e_idle;

}

