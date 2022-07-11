//#include "gd32f3x0.h"
#include "gd32f3x0_gpio.h"
#include "gpio.h"
#include "driveri2cbus.h"


//#define Delay( n ) 
#if 1
void Delay(volatile uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}
#else
void Delay(vu32 nus)
{
	uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;		/* 滴答定时器的重装载值 */
    ticks = nus * 120; 						/* 需要的节拍数 */
    told = SysTick->VAL;        			/* 刚进入时的计数器值 */

    while(1)
    {
        tnow = SysTick->VAL;

        if(tnow != told)
        {
            if(tnow < told)tcnt += told - tnow;
            else tcnt += reload - tnow + told;
            if(tcnt >= ticks)break;			 /* 时间超过/等于要延迟的时间,则退出. */
			told = tnow;
        }
    }
}
#endif


