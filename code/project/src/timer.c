#ifndef __TIMER_C__
#define __TIMER_C__

#include <stdint.h>
#include <string.h>
#include "timer.h"
#include "scan.h"
#include "led.h"
#include "gpio.h"
#include "key.h"
#include "config.h"

#include "pwm.h"



void ugee_adc_delay(uint16_t delay)
{

	TIMER_CNT(TIMER14) = 0;
	timer_flag_clear(TIMER14, TIMER_FLAG_UP);
	while(TIMER_CNT(TIMER14) < delay*UGEE_DELAY_1US_NEED_CLOCK);
     
}

//void TIMER2_IRQHandler(void)
//{

//    if (RESET != timer_flag_get(TIMER2, TIMER_FLAG_UP))
//    {
//        timer_flag_clear(TIMER2, TIMER_FLAG_UP);

//        //led_flash_monitor();

////        key_scan();
//            
//        //button_scan();
//    }

//}


void TIMER0_BRK_UP_TRG_COM_IRQHandler(void)
{    
    if (RESET != timer_flag_get(TIMER0, TIMER_FLAG_UP))
    {
        timer_flag_clear(TIMER0, TIMER_FLAG_UP);

        if( g_scan.scan_PWM_Cnt < g_scan.pwmChargeCount)
        {
            // TIMER_CAR(TIMER0) = gNormalScan.scan_PWM_Freq;
            //TIMER_CH0CV(TIMER_CH_0) = gNormalScan.scan_PWM_Freq>>1;
            //timer_enable(TIMER0);
        }
        else if( g_scan.scan_PWM_Cnt == g_scan.pwmChargeCount)
        {
            //timer_single_pulse_mode_config(TIMER0,TIMER_SP_MODE_SINGLE);
            TIMER_CTL0(TIMER0) |= TIMER_CTL0_SPM;
        }
        else
        {
            //timer_disable(TIMER0);
            CHARGE_TX_DISABLE;
            ugee_adc_delay(8);
            scan_recv(); 
            //scan_start();         
        }
        g_scan.scan_PWM_Cnt ++;
    }
}

void TIMER15_IRQHandler(void)
{
	if (RESET != timer_flag_get(TIMER15, TIMER_FLAG_UP))
	{
		timer_flag_clear(TIMER15, TIMER_FLAG_UP);

		if(g_scan.adcDealEndFlag)
		{
		    g_scan.adcDealEndFlag = 0;	
			TIMER_CAR(TIMER15) = UGEE_CHARGING_AND_ADCSAMPLE_TIME;			

            g_scan.scan_PWM_Cnt = 0;
            switch (g_scan.scan_mode)
            {
            case PRESCAN_MODE_Y:
                pwm_period_set_by_pre_idx(g_scan.pre_freq_idx);
                scan_chrg(0, g_scan.chrg_ant);
                break;

            case PRESCAN_MODE_X:
                pwm_period_set_by_pre_idx(g_scan.center_pre_freq_idx);
                scan_chrg(g_scan.chrg_ant, 0);
                break;

            case PRESS_SCAN_MODE:
                pwm_period_set_by_idx(g_scan.freq_idx);
                scan_chrg(g_scan.center_x_ant, g_scan.center_y_ant);
                break;

            case XPOS_SCAN_MODE:
                pwm_period_set_by_idx(g_scan.center_freq_idx);
                scan_chrg(g_scan.center_x_ant, g_scan.center_y_ant);
                break;

            case YPOS_SCAN_MODE:
                pwm_period_set_by_idx(g_scan.center_freq_idx);
                scan_chrg(g_scan.center_x_ant, g_scan.center_y_ant);
                break;
            }
		}
		else
		{
			TIMER_CAR(TIMER15) = UGEE_CAULURATE_WAITIMG_TIME;
			timer_enable(TIMER15);
		}

	} 
}

void timer0_init(void)
{
    // For charging PWM.
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER0);

    timer_deinit(TIMER0);

    timer_initpara.prescaler         = 0;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 141;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;//COORDINATE_CHRAGING_TIME;
    timer_init(TIMER0, &timer_initpara);

    // CH0 configuration in PWM mode
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_LOW;
    timer_ocintpara.ocnpolarity  = TIMER_OC_POLARITY_LOW;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 71);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    // auto-reload preload enable
    timer_auto_reload_shadow_enable(TIMER0);
    timer_update_source_config(TIMER0, TIMER_UPDATE_SRC_REGULAR);

//    timer_single_pulse_mode_config(TIMER0, TIMER_SP_MODE_SINGLE);

    timer_primary_output_config(TIMER0, ENABLE);

    timer_flag_clear(TIMER0, TIMER_FLAG_UP);
    timer_interrupt_enable(TIMER0, TIMER_INT_UP);
    nvic_irq_enable(TIMER0_BRK_UP_TRG_COM_IRQn, 1, 0);

}

void timer1_init(void)
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
    
    rcu_periph_clock_enable(RCU_TIMER1);

    timer_deinit(TIMER1);

    rcu_periph_clock_enable(RCU_GPIOB);

    pin_af_init(LED_W_PIN_IDX, GPIO_AF_2);
    //pin_af_init(LED_B_PIN_IDX, GPIO_AF_2);

    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 99;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);

    // CH1 configuration in PWM mode 
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_LOW;
    timer_ocintpara.ocnpolarity  = TIMER_OC_POLARITY_LOW;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    //timer_channel_output_config(TIMER1,TIMER_CH_2,&timer_ocintpara);

    timer_channel_output_config(TIMER1,TIMER_CH_3,&timer_ocintpara);
    
    config_led_foot_duty_set();
    //config_led_track_pad_duty_set();
    
    //timer_channel_output_mode_config(TIMER1,TIMER_CH_2,TIMER_OC_MODE_PWM0);
    //timer_channel_output_shadow_config(TIMER1,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);

    timer_channel_output_mode_config(TIMER1,TIMER_CH_3,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);
    
    timer_single_pulse_mode_config(TIMER1,TIMER_SP_MODE_REPETITIVE);

    // auto-reload preload enable 
    timer_auto_reload_shadow_enable(TIMER1);
    timer_primary_output_config(TIMER1,ENABLE); 
    
    // auto-reload preload enable 
    timer_enable(TIMER1);
    
}

void timer2_init(void)
{
    // Used for LED control and key scanning.
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER2);

    timer_deinit(TIMER2);

    timer_initpara.prescaler         = 7199;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 49;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;

    timer_init(TIMER2, &timer_initpara);

    timer_auto_reload_shadow_disable(TIMER2);

    timer_single_pulse_mode_config(TIMER2, TIMER_SP_MODE_REPETITIVE);

    timer_interrupt_enable(TIMER2, TIMER_INT_UP);
    nvic_irq_enable(TIMER2_IRQn, 6, 0);
    timer_enable(TIMER2);

}

void timer14_init(void)
{
    // For delay.
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER14);

    timer_deinit(TIMER14);

    timer_initpara.prescaler         = 0;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 50000; //
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;

    timer_init(TIMER14, &timer_initpara);

    // auto-reload preload enable
    timer_auto_reload_shadow_enable(TIMER14);

    timer_single_pulse_mode_config(TIMER14, TIMER_SP_MODE_REPETITIVE);

    timer_interrupt_enable(TIMER14, TIMER_INT_UP);

    timer_enable(TIMER14);
}

void timer15_init(void)
{
	timer_parameter_struct timer15_initpara;

	rcu_periph_clock_enable(RCU_TIMER15);

	timer_deinit(TIMER15);

	timer15_initpara.prescaler         = TIM15_PRESCALER;
	timer15_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer15_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer15_initpara.period            = 180;
	timer15_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer15_initpara.repetitioncounter = 0;

	timer_init(TIMER15,&timer15_initpara);

	// auto-reload preload enable
	timer_auto_reload_shadow_disable(TIMER15);

	timer_single_pulse_mode_config(TIMER15,TIMER_SP_MODE_SINGLE);
	// auto-reload preload enable
	//timer_enable(TIMER2);

	timer_interrupt_enable(TIMER15, TIMER_INT_UP);
	nvic_irq_enable(TIMER15_IRQn, 1, 0);   
	//timer_enable(TIMER15);
}
void timer_config(void)
{
    timer0_init();
//    timer1_init();
//    timer2_init();
    timer14_init();
    timer15_init();
}

#endif
