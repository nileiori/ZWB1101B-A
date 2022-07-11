#ifndef __ADC_H__
#define __ADC_H__

extern uint16_t adc_get_last_value(void);
extern uint32_t adc_get_sum(void);
extern void adc_sampling_start(uint8_t samp_cnt);
extern void adc_config(void);


#endif

