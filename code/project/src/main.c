#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include "config.h"
#include "systick.h"
#include "usart.h"
#include "scan.h"
#include "app_usart.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "gpio.h"
#include "comm.h"
#include "iqs.h"
#include "system.h"
#if defined ( configUse_wdog)
#include "wdog.h"
#endif
#include "taskschedule.h"


int main(void)
{

#ifdef BOOT_LOADER
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, APP_LOADED_ADDR);
    __asm("CPSIE  I");   //open ir…q
#endif
    system_init();
    LED_SPARK(e_touch, 1, 20);
	
    while (1)
    {
        //scan_main_calculate();

        hid_report();
		
        key_report();
#ifndef  configUse_IqsInt
        iqs_loop();
#endif
        TimerTaskScheduler();
#if defined ( configUse_wdog)
        fwdog_feed();
#endif
    }
}

