

#include "gd32f3x0.h"
#include <string.h>
#include "gpio.h"
#include "led.h"
#include "flexible_button.h"
#include "config.h"


#define ENUM_TO_STR(e) (#e)

typedef enum
{
    USER_BUTTON_0 = 0,
    USER_BUTTON_1,
    USER_BUTTON_2,
    USER_BUTTON_3,
    USER_BUTTON_4,
    USER_BUTTON_5,
    USER_BUTTON_6,
    USER_BUTTON_7,
    USER_BUTTON_MAX
} user_button_t;

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
    ENUM_TO_STR(USER_BUTTON_0),
    ENUM_TO_STR(USER_BUTTON_1),
    ENUM_TO_STR(USER_BUTTON_2),
    ENUM_TO_STR(USER_BUTTON_3),
    ENUM_TO_STR(USER_BUTTON_4),
    ENUM_TO_STR(USER_BUTTON_5),
    ENUM_TO_STR(USER_BUTTON_6),
    ENUM_TO_STR(USER_BUTTON_7),
    ENUM_TO_STR(USER_BUTTON_MAX),
};

static flex_button_t user_button[USER_BUTTON_MAX];

static uint8_t common_btn_read(void *arg)
{
    uint8_t value = 0;

    flex_button_t *btn = (flex_button_t *)arg;

    if (btn->id >= USER_BUTTON_MAX)
    {
        configASSERT( 0 );
    }

    value = gpio_input_bit_get(keys_gpio[btn->id].port, keys_gpio[btn->id].pin);


    return value;
}

static void common_btn_evt_cb(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;

    key |= BIT(i);
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

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOF);

    for (i = 0; i < USER_BUTTON_MAX; i ++)
    {
        gpio_mode_set(keys_gpio[i].port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, keys_gpio[i].pin);

        user_button[i].id = i;
        user_button[i].usr_button_read = common_btn_read;
        user_button[i].cb = common_btn_evt_cb;
        user_button[i].pressed_logic_level = 0;
        user_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(1000);
        user_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(2000);
        user_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(3000);


        flex_button_register(&user_button[i]);
    }
}

void button_scan(void)
{
    flex_button_scan();
}


