/* Kernel includes. */
#include <stdio.h>
#include <stdint.h>
#include "gd32f3x0.h"
#include "string.h"
#include "config.h"
#include "usbd_std.h"
#include "usart.h"
#include "adc.h"
#include "scan.h"
#include "usb.h"
#include "led.h"
#include "timer.h"
#include "gpio.h"
#include "iqs.h"
#include "systick.h"
#include "key.h"
#include "system.h"
#if defined ( configUse_wdog)
#include "wdog.h"
#endif
#include "app_usart.h"
#include "drv_pin.h"
#include "taskschedule.h"



extern system_config_t g_sys_cfg;

uint32_t  usb_soft_cnt = 0;
system_status_t sys_status;

void bkp_init(void)
{
    /* enable PMU clock */
    rcu_periph_clock_enable(RCU_PMU);
    /* enable the access of the RTC registers */
    pmu_backup_write_enable();

    /* check if RTC has aready been configured */
    if (BKP_VALUE == RTC_BKP0)
    {
        RTC_BKP0 = 0ul;
        g_sys_cfg.channel = REPORT_CHANNEL_USERDEF;
    }
}

#ifdef  configUse_RTC
uint8_t secondCount = 0;
//static uint8_t currSecond = 0;

rtc_parameter_struct rtc_initpara;
rtc_alarm_struct  rtc_alarm;

__IO uint32_t prescaler_a = 0, prescaler_s = 0;


void rtc_pre_config(void)
{
    rcu_osci_on(RCU_IRC40K);
    rcu_osci_stab_wait(RCU_IRC40K);
    rcu_rtc_clock_config(RCU_RTCSRC_IRC40K);

    prescaler_s = 0x18F;
    prescaler_a = 0x63;


    rcu_periph_clock_enable(RCU_RTC);
    rtc_register_sync_wait();
}

void rtc_setup(void)
{
    /* setup RTC time value */
    rtc_initpara.rtc_factor_asyn = prescaler_a;
    rtc_initpara.rtc_factor_syn = prescaler_s;
    rtc_initpara.rtc_year = 0x21;
    rtc_initpara.rtc_day_of_week = RTC_THURSDAY;
    rtc_initpara.rtc_month = RTC_JAN;
    rtc_initpara.rtc_date = 0x12;
    rtc_initpara.rtc_display_format = RTC_24HOUR;
    rtc_initpara.rtc_am_pm = RTC_AM;

    rtc_initpara.rtc_hour   = 0x01;
    rtc_initpara.rtc_minute = 0x00;
    rtc_initpara.rtc_second = 0x00;

    /* RTC current time configuration */
    configASSERT(SUCCESS == rtc_init(&rtc_initpara));

}

void rtc_configuration(void)
{
    rcu_periph_clock_enable(RCU_PMU);
    pmu_backup_write_enable();

    rtc_pre_config();

    rtc_setup();


}

void rtc_alarm_setup(void)
{
    rtc_configuration();
    /* RTC alarm configuration */
    rtc_alarm_disable();
    rtc_alarm.rtc_alarm_mask = RTC_ALARM_DATE_MASK | RTC_ALARM_HOUR_MASK;
    rtc_alarm.rtc_weekday_or_date = RTC_ALARM_DATE_SELECTED;
    rtc_alarm.rtc_alarm_day = 0x24;
    rtc_alarm.rtc_am_pm = RTC_AM;

    rtc_alarm.rtc_alarm_hour    = 0x01;
    rtc_alarm.rtc_alarm_minute  = 0x00;
    rtc_alarm.rtc_alarm_second  = 0x02;

    rtc_alarm_config(&rtc_alarm);

    rtc_interrupt_enable(RTC_INT_ALARM);
    rtc_alarm_enable();

    rtc_flag_clear(RTC_STAT_ALRM0F);
    exti_flag_clear(EXTI_17);
    rcu_all_reset_flag_clear();

    exti_init(EXTI_17, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    nvic_irq_enable(RTC_IRQn, 0, 0);
}

void rtc_update(void)
{
    rtc_current_time_get(&rtc_initpara);
}

#endif

//system config
void prvSetupHardware( void )
{
    SystemInit();

    SystemCoreClockUpdate();

    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);

    systick_config();
}
//peripheral config
void peripheral_init(void)
{
    gd32_hw_usart_init();

    gpio_config();

    usb_config();

    config_init();

    led_init();

    adc_config();

    iqs_init();

    key_init();

    timer_config();

    bkp_init();

#ifdef  configUse_RTC
    rtc_configuration();
#endif
#if defined ( configUse_wdog)
    fwdog_init();
#endif
#ifdef	configUse_SEGGER_RTT
    SEGGER_RTT_Init();

    SEGGER_RTT_printf(0, "\r\nCK_SYS is %d", rcu_clock_freq_get(CK_SYS));
    SEGGER_RTT_printf(0, "\r\nCK_AHB is %d", rcu_clock_freq_get(CK_AHB));
    SEGGER_RTT_printf(0, "\r\nCK_APB1 is %d", rcu_clock_freq_get(CK_APB1));
    SEGGER_RTT_printf(0, "\r\nCK_APB2 is %d", rcu_clock_freq_get(CK_APB2));
#endif

    scan_reset();
    scan_start();
}

void system_init(void)
{

    memset((void*)&sys_status.work_mode,0,sizeof(sys_status));
    sys_status.work_mode = e_work_mode_ble;
    strncpy(sys_status.ble_name,UG_DEV_NAME,sizeof(UG_DEV_NAME));
    memcpy(sys_status.dev_serialNo,UG_DEV_SERIAL_NUM,sizeof(UG_DEV_SERIAL_NUM));
    sys_status.pdCtrl.flag = 3;
    sys_status.sleep.usTime = SLEEP_COUNT_5MINUTES;

    prvSetupHardware();

    peripheral_init();

}

uint8_t get_usbCurStatus(void)
{
    return usbhs_core_dev.dev.status;
}

uint8_t check_ugUSBSoftIntCnt(void)
{
    if(usb_soft_cnt > 10)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void clear_ugUSBSoftIntCnt(void)
{
    usb_soft_cnt = 0;
}

uint8_t get_report_mode(void)
{
    return sys_status.work_mode;
}

void set_report_mode(enum sys_work_mode mode)
{
    sys_status.work_mode = mode;
}

EventStatus system_judge_curr_mode(void)
{
#if 0
    if(sys_status.work_mode == e_work_mode_ble)
    {
        if(pin_read(USB_DECT_PIN_IDX))
        {
            //if(check_sys)
            //if(0 == sys_status.ble_connect)
            //{
            //    set_report_mode(e_work_mode_usb);
            //    NOTICE_BLE_WAKEUP;
            //}
            sys_status.chrgSta = 1;
            sys_status.enterIntoPdTimer = 0;
        }

    }
    else
    {
        if(!pin_read(USB_DECT_PIN_IDX))
        {
            clear_ugUSBSoftIntCnt();
            set_report_mode(e_work_mode_ble);
            //config_android_handle_set(0,0);
            NOTICE_BLE_USART_MODE;
            sys_status.chrgSta = 0;
//            check_sys = 0;
        }
    }
#endif
    if(pin_read(USB_DECT_PIN_IDX))
    {
        //if(check_sys)
        //if(0 == sys_status.ble_connect)
        //{
        //    set_report_mode(e_work_mode_usb);
        //    NOTICE_BLE_WAKEUP;
        //}
        sys_status.chrgSta = 1;
        sys_status.enterIntoPdTimer = 0;
    }
    else
    {
        clear_ugUSBSoftIntCnt();
        set_report_mode(e_work_mode_ble);
        //config_android_handle_set(0,0);
        NOTICE_BLE_USART_MODE;
        sys_status.chrgSta = 0;
//            check_sys = 0;
    }
    return ENABLE;
}

#ifdef  configUse_wireless

void system_config_before_sleep(void)
{
    NVIC_DisableIRQ(SysTick_IRQn);
    timer_disable(TIMER0);
    timer_disable(TIMER1);
    //timer_disable(TIMER2);
    timer_disable(TIMER13);
    timer_disable(TIMER14);
    timer_disable(TIMER15);
    //gpio_halt_config();
    adc_disable();
    //USB_GLOBAL_INT_DISABLE();
    //rcu_periph_clock_disable(RCU_USBFS);
}

void system_config_after_wakeup(void)
{

    prvSetupHardware();

    gpio_config();

    adc_config();

    iqs_init();

    timer_config();

    scan_reset();
    scan_start();

    sys_status.enterIntoPdTimer = 0;

}

static void wakeup_hdr(void *args)
{
    args = args;

}
void sleep_int_install(void)
{
    uint8_t i;
    irq_priority priority =
    {
        .nvic_irq_pre_priority = 0,
        .nvic_irq_sub_priority = 0
    };

    for (i = 0; i < USER_BUTTON_MAX; i++)
    {
        pin_irq_install(  keys_pin[i], PIN_MODE_INPUT_PULLUP,
                          PIN_IRQ_MODE_FALLING,
                          wakeup_hdr,
                          UG_NULL,
                          &priority);
    }
    pin_irq_install(  USB_DECT_PIN_IDX, PIN_MODE_INPUT_PULLDOWN,
                      PIN_IRQ_MODE_RISING,
                      wakeup_hdr,
                      UG_NULL,
                      &priority);
}

void sleep_int_uninstall(void)
{
    uint8_t i;

    for (i = 0; i < USER_BUTTON_MAX; i++)
    {
        gd32_pin_irq_enable(keys_pin[i], PIN_IRQ_DISABLE);
    }
    gd32_pin_irq_enable(USB_DECT_PIN_IDX, PIN_IRQ_DISABLE);
}

void system_halt(void)
{
    system_config_before_sleep();
    sleep_int_install();
    pmu_to_deepsleepmode(PMU_LDO_LOWPOWER, WFI_CMD);
    sleep_int_uninstall();
    system_config_after_wakeup();
//    NOTICE_BLE_WAKEUP;
//    LED_SPARK(e_touch,1,20);
}
void system_clear_sleep_count(void)
{
    sys_status.enterIntoPdTimer = 0;
}

uint16_t system_get_sleep_time(void)
{
    //return ((sys_status.ble_connect) ? SLEEP_COUNT_30MINUTES : SLEEP_COUNT_5MINUTES);
    return sys_status.sleep.usTime;
}

uint8_t system_get_ble_batt(void)
{
    return sys_status.ble_batt;
}

EventStatus system_1s_task(void)
{
    uint8_t buf[20] = {0,};
    static  uint8_t  zzz___ = 0;
//    uint16_t sleepThreshold;

    if(FALSE == sys_status.ble_mac.status)
    {
        //????????MAC????
        INQUIRY_BLE_STATUS(buf,0x0d);
    }
    else if(FALSE == sys_status.fw.status)
    {
        //????????????????
        INQUIRY_BLE_STATUS(buf,0x07);
    }
//    else if(0 == sys_status.ble_batt)
//    {
//        //????????????
//        INQUIRY_BLE_STATUS(buf,0x09);
//    }
//    else if(0 == sys_status.chrgSta)
//    {
//        //????????????
//        INQUIRY_BLE_STATUS(buf,0x0b);
//    }
    else if(TRUE == sys_status.dongle_mac.status)
    {
        sys_status.dongle_mac.status = FALSE;
        //????dongle Mac ??ble
        memcpy(buf+4,sys_status.dongle_mac.addr,6);
        BLE_TRANS_CMD_FORM(buf,0x11,6);
    }
    else if(sys_status.led_switch)
    {
        //??????????????
        sys_status.led_switch = 0;
        buf[4] = sys_status.led_light;
        BLE_TRANS_CMD_FORM(buf,0x19,1);
    }
    else if(sys_status.led_light_get)
    {
        sys_status.led_light_get = 0;
        //led??????????
        //INQUIRY_BLE_STATUS(buf,0x21);
//        buf[0] = 0x02;
//        buf[1] = 0xb4;
//        buf[2] = 0x01;
//        buf[3] = 0x00;
//        buf[4] = 0x01;
//        buf[5] = 1;

//        memmove(buf + 4, buf, 6);
//        BLE_TRANS_CMD_FORM(buf, 0xf3, 6);
    }

    //sleepThreshold = (sys_status.ble_connect) ? SLEEP_COUNT_30MINUTES : SLEEP_COUNT_5MINUTES;
    if(sys_status.work_mode == e_work_mode_usb)return ENABLE;
    if(sys_status.pdCtrl.forbid)return ENABLE;

    sys_status.enterIntoPdTimer++;

    if(zzz___)
    {
        zzz___ = 0;
        sys_status.enterIntoPdTimer = 0;
        if(!IS_KEY_PRESSED)//the last line of defense
            system_halt();
    }
    else if(sys_status.enterIntoPdTimer == 10)
    {
        if(0 == sys_status.ble_connect)
        {
            //????????????????????
            INQUIRY_BLE_STATUS(buf,0x13);
        }
    }
//    else if((sys_status.enterIntoPdTimer%5) == 0)
//    {
//        //????????
//        INQUIRY_BLE_STATUS(buf,0x09);
//    }
    else if(sys_status.enterIntoPdTimer > sys_status.sleep.usTime)
    {
        zzz___ = 1;
        //??????????????????
        INQUIRY_BLE_STATUS(buf,0x15);

        NOTICE_BLE_SLEEP;
    }

    return ENABLE;
}

#endif
