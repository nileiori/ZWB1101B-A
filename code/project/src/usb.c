#include <string.h>
#include "usb_core.h"
#include "hid_core.h"
#include "hid_descriptor.h"
#include "usb_delay.h"
#include "system.h"
#include "usb.h"
#include "serial.h"
#include "app_usart.h"

#include "preferences.h"
#include "led.h"
#include "config.h"
#include "iqs.h"

extern system_status_t sys_status;

uint8_t timer_prescaler = 0;
uint32_t usbfs_prescaler = 0;

uint8_t usb_rx_buffer[USB_RX_BUFF_SIZE];

usb_status_t g_usb_status =
{
    .ep1_tx_complete = 1,
    .ep2_tx_complete = 1,
    .ep3_tx_complete = 1,
    .ep3_rx_complete = 0,
};

usb_core_handle_struct usbhs_core_dev =
{
    .dev =
    {
        .dev_desc = (uint8_t*)&device_descripter,
        .config_desc = (uint8_t*)&configuration_descriptor,
        .strings = usbd_strings,
        .class_init = usbd_hid_init,
        .class_deinit = usbd_hid_deinit,
        .class_req_handler = usbd_hid_classreq_handle,
        .class_data_handler = usbd_hid_data_handler,
    },

    .udelay = usb_delay_us,
    .mdelay = usb_delay_ms
};

uint8_t usb_tx(uint8_t *report, uint16_t len)
{
    static uint8_t buff[USB_TX_BUFF_SIZE];

    if (usbhs_core_dev.dev.status != USB_STATUS_CONFIGURED)
        return USBD_FAIL;

    if (len > USB_TX_BUFF_SIZE)
        return USBD_FAIL;

    memcpy(buff, report, len);

    return usbd_hid_report_send(&usbhs_core_dev, buff, len);

}

static void usb_cmd_channel_set(void)
{
    uint8_t data[12] = {0};
    uint8_t channel;

    channel = usb_rx_buffer[2] & 0x7f;
    if (channel != REPORT_CHANNEL_MOUSE && channel != REPORT_CHANNEL_TABLET && channel != REPORT_CHANNEL_USERDEF)
    {
        channel = Default_Report_ID;
    }

    config_channel_set(channel, usb_rx_buffer[2] & 0x80);

    data[0] = 0x02;
    data[1] = 0xb0;
    data[2] = 0x01;

    usb_tx(data, 12);

}

static void usb_cmd_android_set(void)
{
    uint8_t data[12] = {0};
    uint8_t direction;

    if (usb_rx_buffer[3] & 0x01)
    {
        // 开启安卓支持
        direction = usb_rx_buffer[4];
        config_android_direction_set(direction, 0);
        config_android_support_set(1, 0);
        config_android_handle_set(3,0);
        data[0] = 0x02;
        data[1] = 0xb2;
        data[2] = 0x01;
        data[3] = 0x01;

        usb_tx(data, 12);
    }
    else
    {
        // 关闭安卓支持
        config_android_support_set(0, 0);
        config_android_handle_set(0,0);
        data[0] = 0x02;
        data[1] = 0xb2;
        data[2] = 0x00;
        data[3] = 0x01;

        usb_tx(data, 12);
    }
    config_trackpad_rotate_set(*(usb_rx_buffer + 3),1);
}

static void usb_cmd_led_set(void)
{
    uint8_t data[12] = {0};
    uint8_t duty;

    if (usb_rx_buffer[3] & 0x01)
    {
        // 设置灯光亮度
        if(usb_rx_buffer[4] & 0x01)
        {
            duty = usb_rx_buffer[5];

            //track_pad_duty_set(duty);

            config_track_pad_duty_set(duty);

            data[0] = 0x02;
            data[1] = 0xb4;
            data[2] = 0x01;
            data[3] = 0x01;
            data[4] = 0x01;
            data[5] = duty;
//            data[6] = 0x01;

            usb_tx(data, 12);
            //通知蓝牙板点灯
            sys_status.led_switch = 1;
            sys_status.led_light = (duty != 0);
            //BLE_TRANS_CMD_FORM(data,0x19,1);
        }
        else if(usb_rx_buffer[4] & 0x02)
        {
            duty = usb_rx_buffer[5];

//            led_foot_duty_set(duty);
            config_footled_duty_set(duty);

            data[0] = 0x02;
            data[1] = 0xb4;
            data[2] = 0x01;
            data[3] = 0x01;
            data[4] = 0x02;
            data[5] = duty;
            data[6] = 0x01;

            usb_tx(data, 12);
        }
    }
    else
    {
        // 读取灯光亮度
        if(usb_rx_buffer[4] & 0x01)
        {
            duty = config_track_pad_duty_get();

            data[0] = 0x02;
            data[1] = 0xb4;
            data[2] = 0x01;
            data[3] = 0x00;
            data[4] = 0x01;
            data[5] = duty;
//            data[6] = 0x01;
            usb_tx(data, 12);
            //sys_status.led_light_get = 1;
        }
        else if(usb_rx_buffer[4] & 0x02)
        {
            duty = config_footled_duty_get();

            data[0] = 0x02;
            data[1] = 0xb4;
            data[2] = 0x01;
            data[3] = 0x00;
            data[4] = 0x02;
            data[5] = duty;
            data[6] = 0x01;

            usb_tx(data, 12);
        }
    }
}

static void usb_protocolParse(uint8_t *pbuffer)
{
    uint8_t data[20] = {0};
    uint8_t type = *(pbuffer + 3);
    uint16_t tmp;

    memcpy(data,pbuffer,4);
    data[4] = 0x01;
    switch(type)
    {
    case 0x03:
        memcpy(data+5,UG_DEV_SERIAL_NUM,sizeof(UG_DEV_SERIAL_NUM));
        break;
    case 0x04:
        memcpy(data+5,UG_HAND_PAD_FIRMWARE_VER,sizeof(UG_HAND_PAD_FIRMWARE_VER));
        break;
    case 0x05:
        memcpy(data+5,UG_HAND_PAD_FIRMWARE_DATE,sizeof(UG_HAND_PAD_FIRMWARE_DATE));
        break;
    case 0x64:
        tmp         = x_size;
        data[5]     = tmp;
        data[6]     = tmp >> 8;
        tmp         = y_size;
        data[7]     = tmp;
        data[8]     = tmp >> 8;
        tmp         = PressureLevel;
        data[9]     = tmp;
        data[10]    = tmp >> 8;
        break;
    case 0x67:
        memcpy(data+5,sys_status.ble_mac.addr,6);
        break;
    case 0x6e:
        data[5] = 0x10;
        data[6] = 0x00;
        data[7] = 0x0b;
        data[8] = 0x01;
        break;
    case 0x83:
        memcpy(data+5,(uint8_t*)UG_HAND_PAD_PROTOCOL_VER,sizeof(UG_HAND_PAD_PROTOCOL_VER));
        break;
    case 0xf1:
        data[5] = 0x14;
        break;
    default:
        break;
    }

    usb_tx(data, 20);

}

void usb_rx_proc(void)
{
    uint8_t data[12] = {0};
//    uint8_t i;
    if (g_usb_status.ep3_rx_complete)
    {
        g_usb_status.ep3_rx_complete = 0;
        //led_mode_config(e_usb_connect, 1, 200);
#ifdef	configUse_SEGGER_RTT
		for(i=0;i<10;i++)
        SEGGER_RTT_printf(0, " 0x%x", usb_rx_buffer[i]);
		SEGGER_RTT_printf(0, "\r\n");
#endif
        if (usb_rx_buffer[0] == 0x02)
        {
            switch (usb_rx_buffer[1])
            {
            case 0xb0:
                // 数据通道设置
                usb_cmd_channel_set();
                //询问电池电量
                //INQUIRY_BLE_STATUS(data,0x09);
                break;

            case 0xb2:
                if (usb_rx_buffer[2] & 0x20)
                {
                    // 安卓支持
                    config_channel_set(Default_Report_ID, 0);
                    usb_cmd_android_set();
                }
                break;

            case 0xb4:
                if (usb_rx_buffer[2] & 0x01)
                {
                    // 灯光控制
                    usb_cmd_led_set();
                }
                else if (usb_rx_buffer[2] & 0x02)
                {
                    if(usb_rx_buffer[3] & 0x01)
                    {
                        if(usb_rx_buffer[5] <= 0x03)
                        {
                            data[0] = 0x02;
                            data[1] = 0xb4;
                            data[2] = 0x02;
                            data[3] = 0x01;
                            data[4] = 0x01;

                            usb_tx(data, 12);
                            config_track_pad_mode_set(usb_rx_buffer[5]);
                            iqs5xx_track_mode_start();
                        }
                        config_trackpad_rotate_set(*(usb_rx_buffer + 6),0);
                    }
                    else
                    {
                        data[0] = 0x02;
                        data[1] = 0xb4;
                        data[2] = 0x02;
                        data[3] = 0x00;
                        data[4] = 0x00;
                        data[5] = config_track_pad_mode_get();
                        data[6] = config_get_rotate_mode();
                        usb_tx(data, 12);
                    }
                }
                else if (usb_rx_buffer[2] & 0x08)//休眠时长
                {
                    data[0] = usb_rx_buffer[0];
                    data[1] = usb_rx_buffer[1];
                    data[2] = usb_rx_buffer[2];
                    data[3] = usb_rx_buffer[3];
                    data[4] = sys_status.sleep.ucTime[0];
                    data[5] = sys_status.sleep.ucTime[1];

                    usb_tx(data, 12);
                }
                else if (usb_rx_buffer[2] == 0x10)//ble dongle match
                {
                    if (usb_rx_buffer[3] == 0x78)
                    {
                        sys_status.dongle_mac.status = TRUE;
                        memcpy(sys_status.dongle_mac.addr,usb_rx_buffer+8,6);
                        //data[0] = usb_rx_buffer[0];
                        //data[1] = usb_rx_buffer[1];
                        //data[2] = usb_rx_buffer[2];
                        //data[3] = 0x1;
                    }

                    //usb_tx(data, 12);
                }
                /*else if (usb_rx_buffer[2] == 0x18)//inquiry batt value
                {
                    data[0] = 0x02;
                    data[1] = 0xf2;
                    data[2] = 0x01;
                    data[3] = sys_status.ble_batt;
                    data[4] = 0x01;
                    usb_tx(data, 12);
                }*/
                else if (usb_rx_buffer[2] == 0x80)
                {
                    //删除蓝牙MAC地址
                    INQUIRY_BLE_STATUS(data,0x16);

                    data[0] = usb_rx_buffer[0];
                    data[1] = usb_rx_buffer[1];
                    data[2] = usb_rx_buffer[2];
                    data[3] = 0x1;

                    usb_tx(data, 12);
                }
                break;
            case 0xb8:
                if (usb_rx_buffer[2] == 0x04)//inquiry batt value
                {
                    //data[0] = 0x02;
                    //data[1] = 0xf2;
                    //data[2] = 0x01;
                    //data[3] = sys_status.ble_batt;
                    //data[4] = sys_status.chrgSta;
                    //usb_tx(data, 12);
                    //询问电池电量
                    INQUIRY_BLE_STATUS(data,0x09);
                }
                break;
            case 0x80:
                if (usb_rx_buffer[2] & 0x06)
                {
                    usb_protocolParse(usb_rx_buffer);
                }
                break;
            }
        }
        memset(usb_rx_buffer,0,sizeof(usb_rx_buffer));
        usbd_ep_rx(&usbhs_core_dev, EP3_OUT, usb_rx_buffer, USB_RX_BUFF_SIZE);

    }
}

void usb_clock_config(void)
{
    uint32_t system_clock;

    system_clock = rcu_clock_freq_get(CK_SYS);

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
    else if (system_clock == 168000000)
    {
        usbfs_prescaler = RCU_USBFS_CKPLL_DIV3_5;
        timer_prescaler = 9;
    }

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

void usb_config(void)
{
    usb_clock_config();

    usb_timer_nvic_init();

    usbd_init(&usbhs_core_dev, USB_FS_CORE_ID);

    usb_interrupt_config();
}

