#include "gd32f3x0.h"
#include "gpio.h"
#include "drv_pin.h"
#include "string.h"
#include "queue.h"
#include "led.h"
#include "usb.h"
#include "key.h"
#include "scan.h"
#include "usart.h"
#include "app_usart.h"
#include "system.h"
#include "preferences.h"
#include "system.h"
#include "taskschedule.h"


static queue_t key_q;
static uint8_t keyPressSta = 0;

static const hotkey_t hotkeys[HOTKEY_NUM] =
{
    {KEY1_HK0,   Modify_Key0},
    {KEY1_HK1,   Modify_Key1},
    {KEY1_HK2,   Modify_Key2},
    {KEY1_HK3,   Modify_Key3},
    {KEY1_HK4,   Modify_Key4},
    {KEY1_HK5,   Modify_Key5},
    {KEY1_HK6,   Modify_Key6},
    {KEY1_HK7,   Modify_Key7}
};

uint8_t key_status_read(void)
{
    return keyPressSta;
}

void key_report(void)
{
    static uint32_t key = 0;
    uint8_t  data[20] = {0};
    uint8_t  ctrl = 0;
    uint8_t  i, k;

    if (queue_de(&key_q, (uint8_t*)&key))
    {
        for (i = 0, k = 2; i < HOTKEY_NUM; i++)
        {
            if (key & BIT(i))
            {
                ctrl |= hotkeys[i].ctrl;

                data[k] = hotkeys[i].value;
                k++;
            }
        }

        switch (config_channel_get())
        {
        case REPORT_CHANNEL_MOUSE:
            data[0] = 0x06;
            data[1] = ctrl;

            if(e_work_mode_usb == get_report_mode())
            {
                if(USBD_OK != usb_tx(data, 8))
                    queue_en(&key_q, (uint8_t*)&key);
            }
            else
            {
                REPORT_DATA_FRAME_FORM(data, 8);
            }
            break;

        case REPORT_CHANNEL_TABLET:
            data[0] = 0x06;
            data[1] = ctrl;

            if(e_work_mode_usb == get_report_mode())
            {
                if(USBD_OK != usb_tx(data, 8))
                    queue_en(&key_q, (uint8_t*)&key);
            }
            else
            {
                REPORT_DATA_FRAME_FORM(data, 8);
            }
            break;

        case REPORT_CHANNEL_USERDEF:
            memset(data, 0, sizeof(data));
            data[0] = 0x02;
            data[1] = 0xf0;
            data[2] = key & 0xff;
            data[3] = (key >> 8) & 0xff;

            if(e_work_mode_usb == get_report_mode())
            {
                if(USBD_OK != usb_tx(data, 12))
                    queue_en(&key_q, (uint8_t*)&key);
            }
            else
            {
                REPORT_DATA_FRAME_FORM(data, DATA_PACK_LENS);
            }
            break;
        }
    }
    if(0 == key)
    {
        keyPressSta = 0;
    }
}

EventStatus key_longPressReset(void)
{
    if (pin_read(keys_pin[USER_BUTTON_6]))
    {
        return DISABLE;
    }
    if(sys_status.work_mode == e_work_mode_usb)
    {
        soft_reset();
        return DISABLE;
    }
    
    return ENABLE;
}

EventStatus  key_scan(void)
{
    static uint8_t state[HOTKEY_NUM] = {0};
    static uint32_t key = 0;
    uint8_t i,padStatus;
    uint8_t buf[5];
    static uint16_t longPressCnt = 0;

    padStatus = read_hand_pencil_status();
    for (i = 0; i < HOTKEY_NUM; i++)
    {
        switch (state[i])
        {
        case 0:
            if (!pin_read(keys_pin[i]))
            {
                state[i] = 1;
            }
            break;

        case 1:
            if (!pin_read(keys_pin[i]))
            {
                keyPressSta = 1;
                if(0 == BIT_CHECK(key, i))
                {
                    key |= BIT(i);
                    queue_en(&key_q, (uint8_t*)&key);
                    if(e_pen_status_idle == padStatus)
                    {
                        LED_ON;
                    }
                }
                state[i] = 2;
            }
            else
            {
                state[i] = 0;
            }
            break;

        case 2:
            if (pin_read(keys_pin[i]))
            {
                if(BIT_CHECK(key, i))
                {
                    key &= ~BIT(i);
                    queue_en(&key_q, (uint8_t*)&key);
                    if((0 == key)&&(e_pen_status_idle == padStatus))
                    {
                        LED_OFF;
                    }
                }
                state[i] = 0;
                longPressCnt = 0;
            }
            else
            {
                if((USER_BUTTON_6 == i)&&(sys_status.work_mode == e_work_mode_ble)) 
                {
                    SetTimerTask(TIME_LONGPRESS_RESET, 1);
                    state[i] = 3; 
                }
                else if(longPressCnt++ > 250)
                {
                    if(USER_BUTTON_1 == i)
                    {
                        if(e_work_mode_ble == get_report_mode())
                        {
                            //进入蓝牙配对模式
                            INQUIRY_BLE_STATUS(buf, 0x0f);
                        }
                    }  
                    state[i] = 3; 
                }                
            }
            break;
        case 3:
            if (pin_read(keys_pin[i]))
            {
                if(BIT_CHECK(key, i))
                {
                    key &= ~BIT(i);
                    queue_en(&key_q, (uint8_t*)&key);
                    if((0 == key)&&(e_pen_status_idle == padStatus))
                    {
                        LED_OFF;
                    }
                }
                state[i] = 0;
                longPressCnt = 0;
            }
            break;
        }
    }

    return ENABLE;
}


void key_init(void)
{
    static uint32_t buff[10];

    queue_init(&key_q, (uint8_t*)&buff[0], KEY_QUEUE_SIZE, sizeof(uint32_t));
#ifdef configUse_flex_button
    user_button_init();
#endif
}


#ifdef configUse_flex_button

/**************************************************************************************************/
/*                                                                                                */
/*            FlexibleButton is a compact and flexible key processing library based on standard   */
/*                C language. It supports click, double click, short press, long press and        */
/*                     auto shake elimination. You can set the combination keys freely,           */
/*                     which can be used in interrupt and low-power scenarios                     */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief Common key driver
 * @param   None
 * @retval  None
 */

#include <string.h>
#include "..\src\FlexibleButton\flexible_button.h"



#ifdef configUse_SEGGER_RTT

#define ENUM_TO_STR(e) (#e)

static char *enum_event_string[] =
{
    ENUM_TO_STR(FLEX_BTN_PRESS_DOWN),
    ENUM_TO_STR(FLEX_BTN_PRESS_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_DOUBLE_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_REPEAT_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_SHORT_START),
    ENUM_TO_STR(FLEX_BTN_PRESS_SHORT_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_START),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_HOLD),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_HOLD_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_MAX),
    ENUM_TO_STR(FLEX_BTN_PRESS_NONE),
};

static char *enum_btn_id_string[] =
{
    ENUM_TO_STR(USER_BUTTON_1),
    ENUM_TO_STR(USER_BUTTON_2),
    ENUM_TO_STR(USER_BUTTON_3),
    ENUM_TO_STR(USER_BUTTON_4),
    ENUM_TO_STR(USER_BUTTON_5),
    ENUM_TO_STR(USER_BUTTON_6),
    ENUM_TO_STR(USER_BUTTON_7),
    ENUM_TO_STR(USER_BUTTON_8),
    ENUM_TO_STR(USER_BUTTON_MAX),
};
#endif

static flex_button_t user_button[USER_BUTTON_MAX];

static uint8_t common_btn_read(void *arg)
{
    uint8_t value = 0;

    flex_button_t *btn = (flex_button_t *)arg;

    if (btn->id >= USER_BUTTON_MAX)
    {
        configASSERT( 0 );
    }

    value = pin_read(keys_pin[btn->id]);


    return value;
}

static void common_btn_evt_cb(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;
    uint32_t key = 0;

    if(FLEX_BTN_PRESS_NONE == btn->event)
    {
        key &= ~BIT(btn->id);
    }
    else
    {
        keyPressSta = 1;
        key |= BIT(btn->id);
        if(FLEX_BTN_PRESS_LONG_HOLD == btn->event)
        {
            if(USER_BUTTON_6 == btn->id) {
                soft_reset();
            }
        }
    }

    queue_en(&key_q, (uint8_t*)&key);


#ifdef configUse_SEGGER_RTT
    SEGGER_RTT_printf(0, "id: [%d - %s]  event: [%d - %30s]  repeat: %d\r\n",
                      btn->id, enum_btn_id_string[btn->id],
                      btn->event, enum_event_string[btn->event],
                      btn->click_cnt);
#endif

}

void user_button_init(void)
{
    int i;

    memset(&user_button[0], 0x0, sizeof(user_button));

//    rcu_periph_clock_enable(RCU_GPIOA);
//    rcu_periph_clock_enable(RCU_GPIOB);
//    rcu_periph_clock_enable(RCU_GPIOC);
//    rcu_periph_clock_enable(RCU_GPIOF);

    for (i = 0; i < USER_BUTTON_MAX; i ++)
    {
        pin_in_init(keys_pin[i], PIN_MODE_INPUT_PULLUP);

        user_button[i].id = i;
        user_button[i].usr_button_read = common_btn_read;
        user_button[i].cb = common_btn_evt_cb;
        user_button[i].pressed_logic_level = 0;
        //user_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(1000);
        //user_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(2000);
        user_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(150);


        flex_button_register(&user_button[i]);
    }
}

EventStatus button_scan(void)
{
    flex_button_scan();

    return ENABLE;
}

#endif


