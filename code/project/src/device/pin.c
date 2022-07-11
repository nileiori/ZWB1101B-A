#include "gd32f3x0.h"
#include "device.h"
#include "pin.h"


static struct ug_device_pin _hw_pin;
static ug_size_t _pin_read(ug_device_t dev, ug_off_t pos, void *buffer, ug_size_t size)
{
    struct ug_device_pin_status *status;
    struct ug_device_pin *pin = (struct ug_device_pin *)dev;

    /* check parameters */
    configASSERT(pin != UG_NULL);

    status = (struct ug_device_pin_status *) buffer;
    if (status == UG_NULL || size != sizeof(*status)) return 0;

    status->status = pin->ops->pin_read(dev, status->pin);
    return size;
}

static ug_size_t _pin_write(ug_device_t dev, ug_off_t pos, const void *buffer, ug_size_t size)
{
    struct ug_device_pin_status *status;
    struct ug_device_pin *pin = (struct ug_device_pin *)dev;

    /* check parameters */
    configASSERT(pin != UG_NULL);

    status = (struct ug_device_pin_status *) buffer;
    if (status == UG_NULL || size != sizeof(*status)) return 0;

    pin->ops->pin_write(dev, (ug_base_t)status->pin, (ug_base_t)status->status);

    return size;
}

static ug_err_t  _pin_control(ug_device_t dev, int cmd, void *args)
{
    struct ug_device_pin_mode *mode;
    struct ug_device_pin *pin = (struct ug_device_pin *)dev;

    /* check parameters */
    configASSERT(pin != UG_NULL);

    mode = (struct ug_device_pin_mode *) args;
    if (mode == UG_NULL) return -UG_ERROR;

    pin->ops->pin_mode(dev, (ug_base_t)mode->pin, (ug_base_t)mode->mode, 0);

    return 0;
}

#ifdef UG_USING_DEVICE_OPS
const static struct ug_device_ops pin_ops =
{
    UG_NULL,
    UG_NULL,
    UG_NULL,
    _pin_read,
    _pin_write,
    _pin_control
};
#endif

int ug_device_pin_register(const char *name, const struct ug_pin_ops *ops, void *user_data)
{
    _hw_pin.parent.type         = UG_Device_Class_Miscellaneous;
    _hw_pin.parent.rx_indicate  = UG_NULL;
    _hw_pin.parent.tx_complete  = UG_NULL;

#ifdef UG_USING_DEVICE_OPS
    _hw_pin.parent.ops          = &pin_ops;
#else
    _hw_pin.parent.init         = UG_NULL;
    _hw_pin.parent.open         = UG_NULL;
    _hw_pin.parent.close        = UG_NULL;
    _hw_pin.parent.read         = _pin_read;
    _hw_pin.parent.write        = _pin_write;
    _hw_pin.parent.control      = _pin_control;
#endif

    _hw_pin.ops                 = ops;
    _hw_pin.parent.user_data    = user_data;

    /* register a character device */
    ug_device_register(&_hw_pin.parent, name, UG_DEVICE_FLAG_RDWR);

    return 0;
}

ug_err_t ug_pin_attach_irq(ug_int32_t pin, ug_uint32_t mode,
                           void (*hdr)(void *args), void  *args, void *priority)
{
    configASSERT(_hw_pin.ops != UG_NULL);
    if(_hw_pin.ops->pin_attach_irq)
    {
        return _hw_pin.ops->pin_attach_irq(&_hw_pin.parent, pin, mode, hdr, args, priority);
    }
    return UG_ENOSYS;
}
ug_err_t ug_pin_detach_irq(ug_int32_t pin)
{
    configASSERT(_hw_pin.ops != UG_NULL);
    if(_hw_pin.ops->pin_detach_irq)
    {
        return _hw_pin.ops->pin_detach_irq(&_hw_pin.parent, pin);
    }
    return UG_ENOSYS;
}

ug_err_t ug_pin_irq_enable(ug_base_t pin, ug_uint32_t enabled)
{
    configASSERT(_hw_pin.ops != UG_NULL);
    if(_hw_pin.ops->pin_irq_enable)
    {
        return _hw_pin.ops->pin_irq_enable(&_hw_pin.parent, pin, enabled);
    }
    return UG_ENOSYS;
}

void ug_pin_mode(ug_base_t pin, ug_base_t mode, ug_uint32_t alt_func_num)
{
    configASSERT(_hw_pin.ops != UG_NULL);
    _hw_pin.ops->pin_mode(&_hw_pin.parent, pin, mode, alt_func_num);
}

void ug_pin_write(ug_base_t pin, ug_base_t value)
{
    configASSERT(_hw_pin.ops != UG_NULL);
    _hw_pin.ops->pin_write(&_hw_pin.parent, pin, value);
}

int  ug_pin_read(ug_base_t pin)
{
    configASSERT(_hw_pin.ops != UG_NULL);
    return _hw_pin.ops->pin_read(&_hw_pin.parent, pin);
}



