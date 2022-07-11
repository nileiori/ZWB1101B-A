#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef  __TIMER_C__
    #define TIMER_EXT
#else
    #define TIMER_EXT extern
#endif



TIMER_EXT void timer_config(void);
TIMER_EXT void ugee_adc_delay(uint16_t delay);

#endif

