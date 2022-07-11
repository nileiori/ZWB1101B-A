#ifndef __LED_H__
#define __LED_H__



typedef enum
{
    e_floating,
    e_touch,
    e_key,
    e_usb_connect,
    e_wheel,
    e_touch_panel,
    e_idle = 0xff
} e_mode;
typedef struct
{
    e_mode      mode;
    uint8_t     flash_times;
    uint16_t    freq;
    uint16_t    shadowfreq;
} led_control;
typedef led_control* led_control_t;

#define LED_SPARK(e_type,num,freq_tick)          led_mode_config(e_type,num,freq_tick)
#define LED_SPARK_CONTINUOUS(e_type,freq_tick)   led_mode_config(e_type,0xff,freq_tick)
#define LED_LIGHT_CONTINUOUS(e_type)             led_mode_config(e_type,1,0xffff)

void led_foot_duty_set(uint32_t duty);
EventStatus led_flash_monitor(void);
void led_init(void);
void led_mode_config(e_mode mode,uint8_t times,uint16_t freq);
void led_off(void);
void led_set_idle(void);
uint8_t led_get_mode(void);
#endif

