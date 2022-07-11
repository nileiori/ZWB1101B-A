#include <stdio.h>
#include <stdint.h>
//#include "adc.h"
#include "config.h"
#include "systick.h"
#include "usart.h"
#include "scan.h"
//#include "usb.h"
//#include "led.h"
#include "key.h"
#include "timer.h"
//#include "gpio.h"
#include "comm.h"
#include "iqs.h"
#include "system.h"
#if defined ( configUse_fct )
#include "fct.h"
#include "message.h"
#endif
#if defined ( configUse_wdog)
#include "wdog.h"
#endif
#include "taskschedule.h"




