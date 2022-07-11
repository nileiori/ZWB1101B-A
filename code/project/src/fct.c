/****************************************************************************************************
 *                                                                                                  *
 *                                                                                                  *
 *                           	    Copyright by                                                    *
 *                                                                                                  *
 *                        Hanvon Ugee Technology Co.,Limited                                        *
 *                     	        Freedom and Democracy                                               *
 *                                                                                                  *
 *                     	        Tel: +27(0)21 863 0033                                              *
 *                    	        E-mail: info@ugee.com.cn                                            *
 *                                                                                                  *
 *==================================================================================================*
 * @file 	  fct.c							 					                                    *
 * @brief 	  Factory Mode			              		                                            *
 * @author 	                                                                                        *
 * @version 	V1.0.0                                                                              *
 * @date 	    9/12/2020                                                                           *
 ****************************************************************************************************/
#include "gd32f3x0.h"
#include "gpio.h"
#include "fct.h"

#ifdef  configUse_fct

#include "iqs.h"
#include "scan.h"
#include "adc.h"
#include "key.h"
#include "led.h"
#include "message.h"
#include "hid_descriptor.h"
#include "usbd_std.h"
#include "stdlib.h"


extern void scan_chrg(uint8_t x_ant, uint8_t y_ant);
extern void pwm_period_set_by_pre_idx(uint8_t idx);
extern uint32_t adc_get_sum(void);
extern void antenna_open(uint8_t x_ant, uint8_t y_ant);

static volatile fct_stu_handle_t   fct_handle = NULL;
static uint32_t last_adc_sum = 0;

uint8_t* fct_start_selftest(void);

/**************************************************************************************************/
/*                                                                                                */
/*                    fct_adc_cap                                                                 */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief   Capture the ADC Value from related channel
 * @param   fct_hd: a handle
 * @retval  the ADC Value
 */
uint32_t fct_adc_cap(struct fct_stu* fct_hd)
{
    uint32_t adc_sum;

    if(fct_hd->ant_select)
    {
        antenna_open(0, fct_hd->ant_index);
        adc_sampling_start(ADC_SAMPLING_COUNT);
    }
    else
    {
        antenna_open(fct_hd->ant_index, 0);
        adc_sampling_start(ADC_SAMPLING_COUNT);
    }
    adc_sum = adc_get_sum();

    if (adc_sum > ADC_SUM_BASE_VALUE)
    {
        if(adc_sum > last_adc_sum)
        {
            last_adc_sum = adc_sum;
            //freq_index = fct_handle->freq_idx;
        }
    }
    fct_handle->freq_idx++;
    fct_handle->freq_idx %= 0x6;
    fct_handle->pwm_open(4);

    return adc_sum;
}


//return a handle
fct_stu_handle_t fct_get_handle(void)
{
    return fct_handle;
}

void fct_reset(void)
{
    fct_handle->rdy         = RESET;
    fct_handle->test_start  = RESET;
    fct_handle->destroy     = RESET;
    fct_handle->test_freq   = RESET;
    
    fct_handle->freq_idx    = 0;
    fct_handle->ant_index   = 1;
    fct_handle->ant_select  = 0;
    fct_handle->adc_val     = 0;

}

static void fct_delay(volatile uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}

void fct_end(void)
{
    if(SET == fct_handle->test_start)
    {
        fct_handle->test_start = RESET;
        fct_delay(2000);
    }
}

void fct_exit(void)
{
    last_adc_sum = 0;
    //release memory space
    free(fct_handle);
    fct_handle = NULL;
    scan_reset();
    scan_start();

}

uint8_t fct_is_destroy(void)
{
    if(fct_handle)
        return fct_handle->destroy;
    return 0;
}

void fct_init(void)
{
    fct_handle = (fct_stu_handle_t)malloc(sizeof(fct_stu_handle));
    configASSERT( fct_handle );

    fct_handle->rdy             = RESET;
    fct_handle->test_start      = RESET;
    fct_handle->destroy         = RESET;
    fct_handle->test_freq       = RESET;
    
    fct_handle->x_ant           = 0;
    fct_handle->y_ant           = 0;
    fct_handle->period_idx      = 0;
    
    fct_handle->ant_index       = 1;
    fct_handle->ant_select      = 0;
    fct_handle->adc_val         = 0;
    fct_handle->freq_idx        = 0;
    
    fct_handle->pwm_open        = pwm_period_set_by_pre_idx;
    fct_handle->charge          = scan_chrg;
    fct_handle->adc_cap         = fct_adc_cap;
    fct_handle->ant_try_times   = 2000;
    fct_handle->pwm_open(fct_handle->period_idx);
}

void fct_single_ant_test(uint8_t x_ant, uint8_t y_ant)
{
    fct_handle->ant_select      = !x_ant;
    fct_handle->ant_index       = x_ant + y_ant;
    fct_handle->chrg_cnt        = 0;
    fct_handle->test_start      = SET;
    fct_handle->test_freq       = RESET;
    //timer_single_pulse_mode_config(TIMER0, TIMER_SP_MODE_SINGLE);
    do
    {
        if(RESET == fct_handle->test_start)
        {
            break;
        }
        g_scan.scan_PWM_Cnt = 0;
        //antenna charge
        fct_handle->charge(x_ant, y_ant);
        //waitting for data to be ready
        while(RESET == fct_handle->rdy);
        fct_handle->rdy         = RESET;

    }
    while( 1 );

}


//96M
static const uint8_t fct_period_tab[] =
{
    228 , // 0         420k
    216 , // 1         445k
    204 , // 2         470k
    194 , // 3         495k
    185 , // 4         520k
    178 , // 5         540k
};

void fct_pwm_period_set_by_idx(uint8_t idx)
{
    fct_handle->test_freq       = SET;
    timer_disable(TIMER0);
    timer_single_pulse_mode_config(TIMER0, TIMER_SP_MODE_REPETITIVE);

    timer_autoreload_value_config(TIMER0, fct_period_tab[idx]);
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, (fct_period_tab[idx]>>1));
    timer_event_software_generate(TIMER0, TIMER_EVENT_SRC_UPG);
    timer_enable(TIMER0);
    
}
#ifdef configUse_flex_button
void fct_manage_button(flex_button_t *btn)
{
    if(fct_handle)
    { 
        fct_end();
        if(FLEX_BTN_PRESS_LONG_HOLD == btn->event)
        {          
            //timer_single_pulse_mode_config(TIMER0, TIMER_SP_MODE_SINGLE);
            timer_disable(TIMER0);
            fct_handle->destroy = SET;
            LED_B_OFF;
#ifdef configUse_SEGGER_RTT
            SEGGER_RTT_printf(0, "exit selftest mode\r\n");
#endif
        }
        else
        {
            switch(btn->id)
            {
            case USER_BUTTON_1:
                fct_handle->x_ant++;
                if(fct_handle->x_ant > ALL_X_ANTANA_NUM)fct_handle->x_ant = 1;
                message_transmit(e_msg_type_x,fct_handle->x_ant);
                break;
            case USER_BUTTON_2:
                if(fct_handle->x_ant < 2)fct_handle->x_ant = ALL_X_ANTANA_NUM;
                else
                    fct_handle->x_ant--;
                message_transmit(e_msg_type_x,fct_handle->x_ant);
                break;
            case USER_BUTTON_3:
                fct_handle->y_ant++;
                if(fct_handle->y_ant > ALL_Y_ANTANA_NUM)fct_handle->y_ant = 1;
                message_transmit(e_msg_type_y,fct_handle->y_ant);
                break;
            case USER_BUTTON_4:
                if(fct_handle->y_ant < 2)fct_handle->y_ant = ALL_Y_ANTANA_NUM;
                else
                    fct_handle->y_ant--;
                message_transmit(e_msg_type_y,fct_handle->y_ant);
                break;
            case USER_BUTTON_5:
                message_transmit(e_msg_type_freq,fct_handle->period_idx++);
                fct_handle->period_idx %= 6;
                break;
            case USER_BUTTON_6:
                fct_handle->period_idx = 0;
                message_transmit(e_msg_type_freq,fct_handle->period_idx);
                break;
            case USER_BUTTON_7:

                break;
            case USER_BUTTON_8:

                break;
            default:
                break;
            }
        }
    }
    else
    {
        if(FLEX_BTN_PRESS_LONG_HOLD == btn->event)
        {
            fct_init();
            message_init();
            LED_B_ON;
            //enter selftest mode
#ifdef configUse_SEGGER_RTT
            SEGGER_RTT_printf(0, "enter selftest mode\r\n");
#endif
        }
    }

}
#else
void fct_manage_longPress(void)
{
    if(fct_handle)
    { 
        fct_end();   
        
        timer_single_pulse_mode_config(TIMER0, TIMER_SP_MODE_SINGLE);
        timer_disable(TIMER0);
        fct_handle->destroy = SET;
        LED_B_OFF;
#ifdef configUse_SEGGER_RTT
        SEGGER_RTT_printf(0, "exit selftest mode\r\n");
#endif
    }
    else
    {
        fct_init();
        LED_B_ON;
        //enter selftest mode
#ifdef configUse_SEGGER_RTT
        SEGGER_RTT_printf(0, "enter selftest mode\r\n");
#endif
    }
}

void fct_manage_shortPress(uint8_t keyIndex)
{
    if(fct_handle)
    { 
        fct_end();

        switch(keyIndex)
        {
        case USER_BUTTON_1:
            fct_handle->x_ant++;
            if(fct_handle->x_ant > ALL_X_ANTANA_NUM)fct_handle->x_ant = 1;
            message_transmit(e_msg_type_x,fct_handle->x_ant);
            break;
        case USER_BUTTON_2:
            if(fct_handle->x_ant < 2)fct_handle->x_ant = ALL_X_ANTANA_NUM;
            else
                fct_handle->x_ant--;
            message_transmit(e_msg_type_x,fct_handle->x_ant);
            break;
        case USER_BUTTON_3:
            fct_handle->y_ant++;
            if(fct_handle->y_ant > ALL_Y_ANTANA_NUM)fct_handle->y_ant = 1;
            message_transmit(e_msg_type_y,fct_handle->y_ant);
            break;
        case USER_BUTTON_4:
            if(fct_handle->y_ant < 2)fct_handle->y_ant = ALL_Y_ANTANA_NUM;
            else
                fct_handle->y_ant--;
            message_transmit(e_msg_type_y,fct_handle->y_ant);
            break;
        case USER_BUTTON_5:
            message_transmit(e_msg_type_freq,fct_handle->period_idx++);
            fct_handle->period_idx %= 6;
            break;
        case USER_BUTTON_6:
            fct_handle->period_idx = 0;
            message_transmit(e_msg_type_freq,fct_handle->period_idx);
            break;
        case USER_BUTTON_7:

            break;
        case USER_BUTTON_8:

            break;
        default:
            break;
        }
    }
}
#endif

struct
{
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[13];
} fctIndex =
{
    sizeof(fctIndex), USB_DESCTYPE_STRING,
    {'f', 'c', 't', '_', 't', 'e', 's', 't', '_', ' ', '_', ' ', ' '}
};
struct
{
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[13];
} fctErr =
{
    sizeof(fctErr), USB_DESCTYPE_STRING,
    {'i', 'n', 'd', 'e', 'x', '_', 'i', 'n', 'v', 'a', 'l', 'i', 'd'}
};
//string 222 DEH
const struct
{
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[14];
} sd222 =
{
    sizeof(sd222), USB_DESCTYPE_STRING,
    {'e', 'n', 't', 'e', 'r', ' ', 'f', 'c', 't', '_', 'm', 'o', 'd', 'e'}
};
//string 223 DFH
const struct
{
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[13];

} sd223 =
{
    sizeof(sd223), USB_DESCTYPE_STRING,
    {'e', 'x', 'i', 't', ' ', 'f', 'c', 't', '_', 'm', 'o', 'd', 'e'}
};
//string 224 DFH
const struct
{
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[13];

} sd224 =
{
    sizeof(sd224), USB_DESCTYPE_STRING,
    {'f', 'c', 't', '_', 's', 'u', 'c', 'c', 'e', 's', 's'}
};
//string 225 DFH
const struct
{
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[13];

} sd225 =
{
    sizeof(sd225), USB_DESCTYPE_STRING,
    {'f', 'c', 't', '_', 'f', 'a', 'i', 'l', 'e', 'd'}
};


void fct_exit_test_mode(void)
{
    fct_end();
    //timer_single_pulse_mode_config(TIMER0, TIMER_SP_MODE_SINGLE);
    timer_disable(TIMER0);
    fct_handle->destroy = SET;
    //LED_SPARK(e_key, 5, 150);

}
uint8_t* fct_enter_test_mode(void)
{
    fct_init();
    message_init();
    //LED_SPARK(e_key, 3, 150);
    return (uint8_t *)&sd222;

}

void fct_data_analyze(uint8_t cmd)
{
    uint8_t type, item;

    if(fct_handle)
    {
        fct_end();
        type = cmd >> 6;
        type &= 0x3;
        item = cmd & 0x3f;
        switch(type)
        {
        case 0:
            message_transmit(e_msg_type_x, item);
            break;
        case 1:
            message_transmit(e_msg_type_y, item);
            break;
        case 2:
            message_transmit(e_msg_type_freq, fct_handle->period_idx++);
            fct_handle->period_idx %= 6;
            break;
        case 3:
            fct_handle->period_idx = 0;
            message_transmit(e_msg_type_freq, fct_handle->period_idx);
            break;
        default:
            break;
        }
    }

}

uint8_t* fct_parse(uint8_t desc_index)
{
    uint8_t *ptr = NULL;
    uint8_t temp;

    if(!fct_handle)
    {
        if(desc_index == 0xDE)             
        {
            ptr = fct_enter_test_mode();
        }
    }
    else
    {
        if(desc_index == 0xDF)            
        {
            ptr = (uint8_t *)&sd223;
            fct_exit_test_mode();
        }
        else if(desc_index == 0xE0)            
        {
            ptr = fct_start_selftest();
        }
        else
        {
            fct_data_analyze(desc_index);
            temp = desc_index >> 6;
            temp &= 0x3;
            if(temp == 0)
            {
                fctIndex.string[9]  = 'x';
                fctIndex.string[10] = '_';
                temp = desc_index & 0x3f;
                if(temp > ALL_X_ANTANA_NUM)
                    return (uint8_t *)&fctErr;
                temp /= 10;
                fctIndex.string[11]  = 0x30 + temp;
                temp = desc_index & 0x3f;
                temp %= 10;
                fctIndex.string[12] = 0x30 + temp;
            }
            else if(temp == 1)
            {
                fctIndex.string[9]  = 'y';
                fctIndex.string[10] = '_';
                temp = desc_index & 0x3f;
                if(temp > ALL_Y_ANTANA_NUM)
                    return (uint8_t *)&fctErr;
                temp /= 10;
                fctIndex.string[11]  = 0x30 + temp;
                temp = desc_index & 0x3f;
                temp %= 10;
                fctIndex.string[12] = 0x30 + temp;
            }
            else
            {
                fctIndex.string[9]  = 'f';
                fctIndex.string[10] = 'r';
                fctIndex.string[11] = 'e';
                fctIndex.string[12] = 'q';
            }
            ptr = (uint8_t *)&fctIndex;
        }
    }
    return ptr;
}

/**************************************************************************************************/
/*                                                                                                */
/*                    fct_start_selftest                                                          */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief   Products need to be self-checked before leaving factory
 * @param   None
 * @retval  None
 */
uint8_t* fct_start_selftest(void)
{
    uint8_t i;//buffer[10];
    uint32_t probe_x[2] = {0,};
    uint32_t probe_y = 0;
    uint32_t tmp;
    uint8_t  flag = 0;
#if 0
#ifdef __IQS_H__
    // Check iqs624
    iqs6xx_buffer_read(buffer, PRODUCT_NUMBER, 1);
    if( IQS624_PRODUCT_NR == buffer[0] )
    {
#ifdef	configUse_SEGGER_RTT
        SEGGER_RTT_printf(0, "\r\nIQS624 OK");
#endif
    }
    // Check iqs572
    while(RESET == I2C_INT2);
    iqs5xx_buffer_read(buffer, PRODUCT_NUMBER, 2);
    if( IQS572_PRODUCT_NR == buffer[1] )
    {
#ifdef	configUse_SEGGER_RTT
        SEGGER_RTT_printf(0, "\r\nIQS572 OK");
#endif
    }
    iqs5xx_comm_end();
#endif
#endif
    // Check antenna
    fct_init();
    while( 1 )
    {
        fct_reset();
        fct_handle->ant_try_times--;
        do
        {
            //antenna charge
            fct_handle->charge(fct_handle->ant_index, 0);
            //waitting for data to be ready
            while(RESET == fct_handle->rdy);
            fct_handle->rdy         = RESET;
            if(fct_handle->adc_val > 1000)
            {
                tmp = probe_x[fct_handle->ant_index / 32] & (0x1 << (fct_handle->ant_index % 32));

                if(0 == tmp)
                {
                    probe_x[fct_handle->ant_index / 32] |= (0x1 << (fct_handle->ant_index % 32));

#ifdef	configUse_SEGGER_RTT
                    SEGGER_RTT_printf(0, "\r\nant_x [%d] OK", fct_handle->ant_index);
#endif
                }
            }

            fct_handle->adc_val     = 0;
            fct_handle->ant_index++;

        }
        while(fct_handle->ant_index <= ALL_X_ANTANA_NUM);


        fct_handle->rdy         = RESET;
        fct_handle->adc_val     = 0;
        fct_handle->ant_select  = 1;
        fct_handle->ant_index   = 1;
        do
        {

            fct_handle->charge(0, fct_handle->ant_index);
            while(RESET == fct_handle->rdy);
            fct_handle->rdy         = RESET;
            if(fct_handle->adc_val > 1000)
            {
                tmp = probe_y & (0x1 << fct_handle->ant_index);
                if(0 == tmp)
                {
                    probe_y |= (0x1 << fct_handle->ant_index);
#ifdef	configUse_SEGGER_RTT
                    SEGGER_RTT_printf(0, "\r\nant_y [%d] OK", fct_handle->ant_index);
#endif
                }
            }

            fct_handle->adc_val     = 0;
            fct_handle->ant_index++;

        }
        while(fct_handle->ant_index <= ALL_Y_ANTANA_NUM);

        if(0 == fct_handle->ant_try_times)
        {
#ifdef	configUse_SEGGER_RTT
            SEGGER_RTT_printf(0, "\r\nUnfortunately...antenna check failed");
#endif
            for(i = 1; i <= ALL_X_ANTANA_NUM; i++)
            {
                tmp = probe_x[i / 32] & (0x1 << (i % 32));
                if(0 == tmp)
                {
#ifdef	configUse_SEGGER_RTT
                    SEGGER_RTT_printf(0, "\r\nant_x [%d] ERR", i);
#endif
                }
            }
            for(i = 1; i <= ALL_Y_ANTANA_NUM; i++)
            {
                tmp = probe_y & (0x1 << i);
                if(0 == tmp)
                {
#ifdef	configUse_SEGGER_RTT
                    SEGGER_RTT_printf(0, "\r\nant_y [%d] ERR", i);
#endif
                }
            }
            flag = 1;
            
            break;
        }
        if((0xfffffffe == probe_x[0]) && (0x7fff == probe_x[1]) && (0x1ffffffe == probe_y))
        {
#ifdef	configUse_SEGGER_RTT
            SEGGER_RTT_printf(0, "\r\nCongratulations...antenna all OK");
#endif
            flag = 0;
                
            break;
        }
    }
    //release memory space
    fct_exit();
    if(flag)
        return (uint8_t *)&sd225;
    return (uint8_t *)&sd224;
}

#endif

