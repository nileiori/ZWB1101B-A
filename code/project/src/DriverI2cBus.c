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
    uint32_t reload = SysTick->LOAD;		/* �δ�ʱ������װ��ֵ */
    ticks = nus * 120; 						/* ��Ҫ�Ľ����� */
    told = SysTick->VAL;        			/* �ս���ʱ�ļ�����ֵ */

    while(1)
    {
        tnow = SysTick->VAL;

        if(tnow != told)
        {
            if(tnow < told)tcnt += told - tnow;
            else tcnt += reload - tnow + told;
            if(tcnt >= ticks)break;			 /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�. */
			told = tnow;
        }
    }
}
#endif


