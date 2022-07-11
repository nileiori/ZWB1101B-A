///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright (C) 2020 HanvonUgee Technology Corp. All rights reserved.
//FileName: main.c
//Brief: main routine for HID boot loader
//Platform:GD32F350xxx
//Ver: V1.0.0
//Date:2020.07.16
//Author:Liming
//Modify: Felix_wang
//	1.ÕûÀí´úÂë
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include  "hid_core.h"
#include <stdio.h>
#include "main.h"
#include "systick.h"
#include  "usb_delay.h"
#include "ugee_comm.h"
#include "dfu.h"
#include "Framework.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t timer_prescaler = 0;
uint32_t usbfs_prescaler = 0;
uint16_t  TimesFlag       = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

usb_core_handle_struct usbhs_core_dev =
{
    .dev =
    {
        .dev_desc = (uint8_t *)&device_descripter,
        .config_desc = (uint8_t *)&configuration_descriptor,
        .strings = usbd_strings,
        .class_init = custom_hid_init,
        .class_deinit = custom_hid_deinit,
        .class_req_handler = custom_hid_req_handler,
        .class_data_handler = custom_hid_data_handler
    },

    .udelay = NULL, //delay_us,
    .mdelay = NULL  //delay_ms
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void usb_clock_config(void);
void usb_interrupt_config(void);


#ifdef USE_IRC48M
void ctc_config(void);
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ugee_disable_usb_timer_irq(void)
{
    //rcu_deinit();
    nvic_irq_disable(TIMER0_BRK_UP_TRG_COM_IRQn);
    nvic_irq_disable((uint8_t)USBFS_IRQn);
    nvic_irq_disable((uint8_t)USBFS_WKUP_IRQn);
    //USB_GLOBAL_INT_DISABLE();
    //timer_interrupt_disable(TIMER0,TIMER_INT_UP);

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 get_usbCurStatus(void)
{
    return usbhs_core_dev.dev.status;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      main routine will construct a USB bootloader
//    \param[in]  none
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    static uint16_t iTimes;
    UINT8  bootRunFlag = 0, uFlag = 0;

    // configure USB clock
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x0);

    usb_clock_config();

    systick_config();

#ifdef configUse_SEGGER_RTT
    SEGGER_RTT_Init();
#endif

    // timer nvic initialization
    //timer_nvic_init();

    gpio_config();
	
	//    if(Flash_Ob_ProtectionCheck(OB_SPEC_ADDR))
//    {
//          Flash_Ob_UserProtection();
//          NVIC_SystemReset();       //system reset    
//    }

    reset_ugUSBStatus();
    getUserConfigInfo(configInfo);

    // USB device stack configure
    usbd_init(&usbhs_core_dev, USB_FS_CORE_ID);

    // USB interrupt configure
    usb_interrupt_config();

    // check if USB device is enumerated successfully
    // while (usbhs_core_dev.dev.status != USB_STATUS_CONFIGURED) {
    //   }

    uFlag = getISPFlag();

    if(gpio_input_bit_get(KEYBOOT_GPIO_PORT, KEYBOOT_PIN) == 0) //boot key
    {
        bootRunFlag = 1;
    }
    else if(uFlag == USER_ISP_FLAG_SW_UPDATE || uFlag == USER_ISP_FLAG_EREASE || checkIfAppInvalid())
    {
        if(rcu_flag_get(RCU_FLAG_SWRST) == RESET &&  (!checkIfAppInvalid()))
        {
            if(getISPFlag() == USER_ISP_FLAG_SW_UPDATE)
            {
                setISPFlag(USER_ISP_FLAG_IDLE);
                NVIC_SystemReset();		//system reset
                return 0;
            }
        }

        bootRunFlag = 1;
    }

    if(bootRunFlag == 1)	//boot loader program
    {

#ifdef	configUse_SEGGER_RTT
        SEGGER_RTT_printf(0, "\r\nfirmware update start...");
#endif
        RunApplication = FALSE;
        while (!FRAMEWORK_ExitFirmwareUpgradeMode())
        {
            usb_TaskFunc();
            FRAMEWORK_FrameWorkTask();

            if (TimesFlag)    //1ms
            {
                TimesFlag = 0;

                if (++iTimes == 100)
                {
                    //gpio_bit_write(GPIO_LED_PORT, LED_R_PIN, (FlagStatus)1);
                    //gpio_bit_write(GPIO_LED_PORT, LED_PIN, (FlagStatus)1);
                }
                else if (iTimes == 200)
                {
                    gpio_bit_write(GPIO_LED_PORT, LED_R_PIN, (FlagStatus)0);
                    gpio_bit_write(GPIO_LED_PORT, LED_PIN, (FlagStatus)0);
                    iTimes = 0;
                }
            }
        }
#ifdef	configUse_SEGGER_RTT
        SEGGER_RTT_printf(0, "\r\nfirmware update complete...");
#endif
    }

    Deinit_AllFunc();
    JmpAppFunc();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      configure USB clock
//    \param[in]  none
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void usb_clock_config(void)
{

#if 0
    uint32_t system_clock = rcu_clock_freq_get(CK_SYS);

    if (system_clock == 48000000)
    {
        usbfs_prescaler = RCU_USBFS_CKPLL_DIV1;
        timer_prescaler = 3;
    }
    else if (system_clock == 72000000)
    {
        usbfs_prescaler = RCU_USBFS_CKPLL_DIV1_5;
        timer_prescaler = 5;
    }
    else if (system_clock == 96000000)
    {
        usbfs_prescaler = RCU_USBFS_CKPLL_DIV2;
        timer_prescaler = 7;
    }
    else
    {
        /*  reserved  */
    }
#else
    usbfs_prescaler = RCU_USBFS_CKPLL_DIV1_5;
    timer_prescaler = 5;
#endif

#ifndef USE_IRC48M
    rcu_ck48m_clock_config(RCU_CK48MSRC_PLL48M);

    rcu_usbfs_clock_config(usbfs_prescaler);
#else
    // enable IRC48M clock
    rcu_osci_on(RCU_IRC48M);

    // wait till IRC48M is ready
    while (SUCCESS != rcu_osci_stab_wait(RCU_IRC48M)) ;

    rcu_ck48m_clock_config(RCU_CK48MSRC_IRC48M);
#endif // USE_IRC48M

    rcu_periph_clock_enable(RCU_USBFS);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      configure USB interrupt
//    \param[in]  none
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void usb_interrupt_config(void)
{
    nvic_irq_enable((uint8_t)USBFS_IRQn, 3U, 0U);

    // enable the power module clock
    rcu_periph_clock_enable(RCU_PMU);

    // USB wakeup EXTI line configuration
    exti_interrupt_flag_clear(EXTI_18);
    exti_init(EXTI_18, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    exti_interrupt_enable(EXTI_18);

    nvic_irq_enable((uint8_t)USBFS_WKUP_IRQn, 0U, 0U);
}




#ifdef USE_IRC48M

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      configure the CTC peripheral
//    \param[in]  none
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ctc_config(void)
{
    // config CTC reference signal source prescaler
    ctc_refsource_prescaler_config(CTC_REFSOURCE_PSC_OFF);
    // select reference signal source
    ctc_refsource_signal_select(CTC_REFSOURCE_USBSOF);
    // select reference signal source polarity
    ctc_refsource_polarity_config(CTC_REFSOURCE_POLARITY_RISING);
    // config hardware automatically trim mode
    ctc_hardware_trim_mode_config(CTC_HARDWARE_TRIM_MODE_ENABLE);

    // config CTC counter reload value, Fclock/Fref-1
    ctc_counter_reload_value_config(0xBB7F);
    // config clock trim base limit value, Fclock/Fref*0.0012/2 */
    ctc_clock_limit_value_config(0x1D);

    // CTC counter enable
    ctc_counter_enable();
}

#endif
