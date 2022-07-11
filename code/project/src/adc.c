#include <string.h>
#include "gd32f3x0.h"
#include "gpio.h"
#include "preferences.h"
#include "adc.h"
#include "timer.h"


static uint16_t adc_value[ADC_SAMPLING_COUNT + 1];

uint16_t adc_get_last_value(void)
{
    return adc_value[ADC_SAMPLING_COUNT];
}

uint32_t adc_get_sum(void)
{
    uint32_t sum = 0;
    uint8_t  i;

    for (i = 0; i < ADC_SAMPLING_COUNT; i++)
    {
        sum  += adc_value[i];
    }

    return sum;
}

void adc_sampling_start(uint8_t samp_cnt)
{
    uint8_t i;

    static uint32_t temp = 0;

    OPA_DISABLE;
    ugee_adc_delay(6);
    ADC_ENABLE;

    for (i = 0; i <= ADC_SAMPLING_COUNT; i++)
    {
        if (i < samp_cnt)
        {
            adc_flag_clear(ADC_FLAG_EOC);
            adc_software_trigger_enable(ADC_REGULAR_CHANNEL);

            while (SET != adc_flag_get(ADC_FLAG_EOC))
            {
            }

            adc_value[i] = ADC_RDATA;

            if (adc_value[i] > temp)
            {
                temp = adc_value[i];
                //printf("max ad : %d\r\n", temp);
            }
        }
        else
        {
            adc_value[i] = 0;
        }
    }

    if (samp_cnt > 0)
    {
        adc_value[ADC_SAMPLING_COUNT] = adc_value[samp_cnt - 1];
    }

    OPA_ENABLE;
    ADC_DISABLE;
}

void adc_config(void)
{
    rcu_periph_clock_enable(RCU_ADC);
    rcu_adc_clock_config(RCU_ADCCK_AHB_DIV3);

    // ADC contineous function enable
    adc_special_function_config(ADC_CONTINUOUS_MODE, DISABLE);
    // ADC trigger config
    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
    // ADC data alignment config
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);

    // ADC channel length config
    adc_channel_length_config(ADC_REGULAR_CHANNEL, 1U);

    // ADC regular channel config
    adc_regular_channel_config(0U, ADC_CHANNEL_11, ADC_SAMPLETIME_7POINT5);
    adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);

    // enable ADC interface
    adc_enable();
    // ADC calibration and reset calibration
    adc_calibration_enable();
}


