

#ifndef PIN_H__
#define PIN_H__

#include "ugdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* pin device and operations for RT-Thread */
struct ug_device_pin
{
    struct ug_device parent;
    const struct ug_pin_ops *ops;
};

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
struct ug_pin_ops
{
    void (*pin_mode)(struct ug_device *device, ug_base_t pin, ug_base_t mode,ug_uint32_t alt_func_num);
    void (*pin_write)(struct ug_device *device, ug_base_t pin, ug_base_t value);
    int (*pin_read)(struct ug_device *device, ug_base_t pin);

    /* TODO: add GPIO interrupt */
    ug_err_t (*pin_attach_irq)(struct ug_device *device, ug_int32_t pin,
                      ug_uint32_t mode, void (*hdr)(void *args), void *args,void *priority);
    ug_err_t (*pin_detach_irq)(struct ug_device *device, ug_int32_t pin);
    ug_err_t (*pin_irq_enable)(struct ug_device *device, ug_base_t pin, ug_uint32_t enabled);
};

int ug_device_pin_register(const char *name, const struct ug_pin_ops *ops, void *user_data);

void ug_pin_mode(ug_base_t pin, ug_base_t mode,ug_uint32_t alt_func_num);
void ug_pin_write(ug_base_t pin, ug_base_t value);
int  ug_pin_read(ug_base_t pin);
ug_err_t ug_pin_attach_irq(ug_int32_t pin, ug_uint32_t mode,
                           void (*hdr)(void *args), void  *args,void *priority);
ug_err_t ug_pin_detach_irq(ug_int32_t pin);
ug_err_t ug_pin_irq_enable(ug_base_t pin, ug_uint32_t enabled);

#ifdef __cplusplus
}
#endif

#endif




