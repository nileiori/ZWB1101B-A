#include <string.h>

#include "gd32f3x0.h"
#include "wdog.h"


#if defined ( configUse_wdog )

void wwdog_init( void )
{

    /* enable WWDGT clock */
    rcu_periph_clock_enable(RCU_WWDGT);

    /* enable WWDGT and set counter value to 127, WWDGT timeout = ~607 us * 64 = 38.8 ms.
       in this case the refresh window is: ~607 * (127-80)= 28.5ms < refresh window < ~607 * 64 =38.8ms.
       set window value to 80; WWDGT counter should be refreshed only when the counter
       is below 80 (and greater than 64) otherwise a reset will be generated.
       WWDGT clock counter = (PCLK1 (54MHz)/4096)/8 = 1647Hz (~607 us) */
    wwdgt_config(127, 80, WWDGT_CFG_PSC_DIV8);
    nvic_irq_enable(WWDGT_IRQn, 7, 0);
    wwdgt_enable();
    wwdgt_flag_clear();
    wwdgt_interrupt_enable();
}
void wwdog_feed( void )
{
    wwdgt_counter_update(127);
}

void fwdog_init( void )
{

    /* enable IRC40K */
    rcu_osci_on(RCU_IRC40K);
    /* wait till IRC40K is ready */
    rcu_osci_stab_wait(RCU_IRC40K);

    /* enable write access to FWDGT_PSC and FWDGT_RLD registers.
       FWDGT counter clock: 40KHz(IRC40K) / 64 = 0.625 KHz */
    fwdgt_config(1250, FWDGT_PSC_DIV64);
    fwdgt_enable();
}
void fwdog_feed( void )
{
    /* reload FWDGT counter */
    fwdgt_counter_reload();
}
void WWDGT_IRQHandler( void )
{
    if(RESET != wwdgt_flag_get())
    {
        //game over
        wwdog_feed();
        wwdgt_flag_clear();
    }
}

#endif

