#ifndef DRV_PIN_H__
#define DRV_PIN_H__

#include "ugdef.h"


#define PIN_LOW                 0x00
#define PIN_HIGH                0x01

#define PIN_MODE_OUTPUT         0x00
#define PIN_MODE_INPUT_ANL		0x01
#define PIN_MODE_INPUT          0x02
#define PIN_MODE_INPUT_PULLUP   0x03
#define PIN_MODE_INPUT_PULLDOWN 0x04
#define PIN_MODE_OUTPUT_OD      0x05
#define PIN_MODE_AF             0x06

#define PIN_IRQ_MODE_RISING             0x00
#define PIN_IRQ_MODE_FALLING            0x01
#define PIN_IRQ_MODE_RISING_FALLING     0x02
#define PIN_IRQ_MODE_HIGH_LEVEL         0x03
#define PIN_IRQ_MODE_LOW_LEVEL          0x04

#define PIN_IRQ_DISABLE                 0x00
#define PIN_IRQ_ENABLE                  0x01

#define PIN_IRQ_PIN_NONE                -1

#define ITEM_NUM(items) sizeof(items)/sizeof(items[0])

struct ug_device_pin_mode
{
    ug_uint16_t pin;
    ug_uint16_t mode;
};
struct ug_device_pin_status
{
    ug_uint16_t pin;
    ug_uint16_t status;
};
struct ug_pin_irq_hdr
{
    ug_int16_t        pin;
    ug_uint16_t       mode;
    void (*hdr)(void *args);
    void             *args;
    void             *priority;
};

typedef struct 
{
    uint8_t nvic_irq_pre_priority; 
    uint8_t nvic_irq_sub_priority;
}irq_priority;

void gd32_pin_mode(ug_base_t pin, ug_base_t mode, ug_uint32_t alt_func_num);
void gd32_pin_write(ug_base_t pin, ug_base_t value);
void pin_out_init(ug_base_t pin, ug_uint8_t mode);
void pin_out_set(ug_base_t pin, bit_status bit_value);
void pin_out_on(ug_base_t pin);
void pin_out_off(ug_base_t pin);
void pin_in_init(ug_base_t pin,ug_uint8_t mode);
int  pin_read(ug_base_t pin);
void pin_af_init(ug_base_t pin,ug_uint32_t alt_func_num);
ug_err_t gd32_pin_irq_enable(ug_base_t pin,ug_uint32_t enabled);
ug_err_t pin_irq_install(ug_base_t pin, ug_base_t mode,
                          ug_uint32_t irqMode,
                          void (*hdr)(void *args),
                          void  *args,
                          void *priority);
//int gd32_hw_pin_init(void);

#endif

