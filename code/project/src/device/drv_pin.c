#include "gd32f3x0_gpio.h"
//#include "pin.h"
#include "drv_pin.h"


#define GD32F3XX_PIN_NUMBERS 64 //[28, 32, 48, 64]

#define __GD32_PIN(index, gpio, gpio_index) { 0, RCU_##GPIO##gpio, GPIO##gpio, GPIO_PIN_##gpio_index, EXTI_SOURCE_GPIO##gpio, EXTI_SOURCE_PIN##gpio_index}
#define __GD32_PIN_DEFAULT {-1, (rcu_periph_enum)0, 0, 0, 0}


/* GD32 GPIO driver */
struct pin_index
{
    int index;
    rcu_periph_enum rcc;
    uint32_t gpio;
    uint32_t pin;
    uint8_t port_source;
    uint8_t pin_source;
};

static const struct pin_index pins[] =
{
#if (GD32F3XX_PIN_NUMBERS == 28)
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(2,  F, 0),
    __GD32_PIN(3,  F, 1),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(7,  A, 1),
    __GD32_PIN(8,  A, 2),
    __GD32_PIN(9,  A, 3),
    __GD32_PIN(10, A, 4),
    __GD32_PIN(11, A, 5),
    __GD32_PIN(12, A, 6),
    __GD32_PIN(13, A, 7),
    __GD32_PIN(14, B, 0),
    __GD32_PIN(15, B, 1),
    __GD32_PIN_DEFAULT,
    __GD32_PIN(17, A, 9),
    __GD32_PIN(18, A, 10),
    __GD32_PIN(19, A, 11),
    __GD32_PIN(20, A, 12),
    __GD32_PIN(21, A, 13),
    __GD32_PIN(22, A, 14),
    __GD32_PIN(23, A, 15),
    __GD32_PIN(24, B, 3),
    __GD32_PIN(25, B, 4),
    __GD32_PIN(26, B, 5),
    __GD32_PIN(27, B, 6),
    __GD32_PIN(28, B, 7)
#endif
#if (GD32F3XX_PIN_NUMBERS == 32)
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(2,  F, 0),
    __GD32_PIN(3,  F, 1),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(6,  A, 0),
    __GD32_PIN(7,  A, 1),
    __GD32_PIN(8,  A, 2),
    __GD32_PIN(9,  A, 3),
    __GD32_PIN(10, A, 4),
    __GD32_PIN(11, A, 5),
    __GD32_PIN(12, A, 6),
    __GD32_PIN(13, A, 7),
    __GD32_PIN(14, B, 0),
    __GD32_PIN(15, B, 1),
    __GD32_PIN(16, B, 2),
    __GD32_PIN_DEFAULT,
    __GD32_PIN(18, A, 8),
    __GD32_PIN(19, A, 9),
    __GD32_PIN(20, A, 10),
    __GD32_PIN(21, A, 11),
    __GD32_PIN(22, A, 12),
    __GD32_PIN(23, A, 13),
    __GD32_PIN(24, A, 14),
    __GD32_PIN(25, A, 15),
    __GD32_PIN(26, B, 3),
    __GD32_PIN(27, B, 4),
    __GD32_PIN(28, B, 5),
    __GD32_PIN(29, B, 6),
    __GD32_PIN(30, B, 7),
    __GD32_PIN_DEFAULT,
    __GD32_PIN(32, B, 8)
#endif
#if (GD32F3XX_PIN_NUMBERS == 48)
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(2,  C, 13),
    __GD32_PIN(3,  C, 14),
    __GD32_PIN(4,  C, 15),
    __GD32_PIN(5,  F, 0),
    __GD32_PIN(6,  F, 1),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(10, A, 0),
    __GD32_PIN(11, A, 1),
    __GD32_PIN(12, A, 2),
    __GD32_PIN(13, A, 3),
    __GD32_PIN(14, A, 4),
    __GD32_PIN(15, A, 5),
    __GD32_PIN(16, A, 6),
    __GD32_PIN(17, A, 7),
    __GD32_PIN(18, B, 0),
    __GD32_PIN(19, B, 1),
    __GD32_PIN(20, B, 2),
    __GD32_PIN(21, B, 10),
    __GD32_PIN(22, B, 11),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(25, B, 12),
    __GD32_PIN(26, B, 13),
    __GD32_PIN(27, B, 14),
    __GD32_PIN(28, B, 15),
    __GD32_PIN(29, A, 8),
    __GD32_PIN(30, A, 9),
    __GD32_PIN(31, A, 10),
    __GD32_PIN(32, A, 11),
    __GD32_PIN(33, A, 12),
    __GD32_PIN(34, A, 13),
    __GD32_PIN(35, F, 6),
    __GD32_PIN(36, F, 7),
    __GD32_PIN(37, A, 14),
    __GD32_PIN(38, A, 15),
    __GD32_PIN(39, B, 3),
    __GD32_PIN(40, B, 4),
    __GD32_PIN(41, B, 5),
    __GD32_PIN(42, B, 6),
    __GD32_PIN(43, B, 7),
    __GD32_PIN_DEFAULT,
    __GD32_PIN(45, B, 8),
    __GD32_PIN(46, B, 9),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT
#endif
#if (GD32F3XX_PIN_NUMBERS == 64)
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(2,  C, 13),
    __GD32_PIN(3,  C, 14),
    __GD32_PIN(4,  C, 15),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(8,  C, 0),
    __GD32_PIN(9,  C, 1),
    __GD32_PIN(10, C, 2),
    __GD32_PIN(11, C, 3),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(14, A, 0),
    __GD32_PIN(15, A, 1),
    __GD32_PIN(16, A, 2),
    __GD32_PIN(17, A, 3),
    __GD32_PIN(18, F, 4),
    __GD32_PIN(19, F, 5),
    __GD32_PIN(20, A, 4),
    __GD32_PIN(21, A, 5),
    __GD32_PIN(22, A, 6),
    __GD32_PIN(23, A, 7),
    __GD32_PIN(24, C, 4),
    __GD32_PIN(25, C, 5),
    __GD32_PIN(26, B, 0),
    __GD32_PIN(27, B, 1),
    __GD32_PIN(28, B, 2),
    __GD32_PIN(29, B, 10),
    __GD32_PIN(30, B, 11),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(33, B, 12),
    __GD32_PIN(34, B, 13),
    __GD32_PIN(35, B, 14),
    __GD32_PIN(36, B, 15),
    __GD32_PIN(37, C, 6),
    __GD32_PIN(38, C, 7),
    __GD32_PIN(39, C, 8),
    __GD32_PIN(40, C, 9),
    __GD32_PIN(41, A, 8),
    __GD32_PIN(42, A, 9),
    __GD32_PIN(43, A, 10),
    __GD32_PIN(44, A, 11),
    __GD32_PIN(45, A, 12),
    __GD32_PIN(46, A, 13),
    __GD32_PIN(47, F, 6),
    __GD32_PIN(48, F, 7),
    __GD32_PIN(49, A, 14),
    __GD32_PIN(50, A, 15),
    __GD32_PIN(51, C, 10),
    __GD32_PIN(52, C, 11),
    __GD32_PIN(53, C, 12),
    __GD32_PIN(54, D, 2),
    __GD32_PIN(55, B, 3),
    __GD32_PIN(56, B, 4),
    __GD32_PIN(57, B, 5),
    __GD32_PIN(58, B, 6),
    __GD32_PIN(59, B, 7),
    __GD32_PIN_DEFAULT,
    __GD32_PIN(61, B, 8),
    __GD32_PIN(62, B, 9),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT
#endif
};

struct pin_irq_map
{
    ug_uint16_t            pinbit;
    exti_line_enum         irqbit;
    enum IRQn              irqno;
};
static const  struct pin_irq_map gd32_pin_irq_map[] =
{
    {GPIO_PIN_0,  EXTI_0,  EXTI0_1_IRQn    },
    {GPIO_PIN_1,  EXTI_1,  EXTI0_1_IRQn    },
    {GPIO_PIN_2,  EXTI_2,  EXTI2_3_IRQn    },
    {GPIO_PIN_3,  EXTI_3,  EXTI2_3_IRQn    },
    {GPIO_PIN_4,  EXTI_4,  EXTI4_15_IRQn   },
    {GPIO_PIN_5,  EXTI_5,  EXTI4_15_IRQn   },
    {GPIO_PIN_6,  EXTI_6,  EXTI4_15_IRQn   },
    {GPIO_PIN_7,  EXTI_7,  EXTI4_15_IRQn   },
    {GPIO_PIN_8,  EXTI_8,  EXTI4_15_IRQn   },
    {GPIO_PIN_9,  EXTI_9,  EXTI4_15_IRQn   },
    {GPIO_PIN_10, EXTI_10, EXTI4_15_IRQn   },
    {GPIO_PIN_11, EXTI_11, EXTI4_15_IRQn   },
    {GPIO_PIN_12, EXTI_12, EXTI4_15_IRQn   },
    {GPIO_PIN_13, EXTI_13, EXTI4_15_IRQn   },
    {GPIO_PIN_14, EXTI_14, EXTI4_15_IRQn   },
    {GPIO_PIN_15, EXTI_15, EXTI4_15_IRQn   },
};

struct ug_pin_irq_hdr pin_irq_hdr_tab[] =
{
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
    {-1, 0, UG_NULL, UG_NULL, UG_NULL},
};


const struct pin_index *get_pin(uint8_t pin)
{
    const struct pin_index *index;

    if (pin < ITEM_NUM(pins))
    {
        index = &pins[pin];
        if (index->index == -1)
            index = UG_NULL;
    }
    else
    {
        index = UG_NULL;
    }

    return index;
};

void gd32_pin_write(ug_base_t pin, ug_base_t value)
{
    const struct pin_index *index;

    index = get_pin(pin);
    if (index == UG_NULL)
    {
        return;
    }

    if (value == PIN_LOW)
    {
        gpio_bit_reset(index->gpio, index->pin);
    }
    else
    {
        gpio_bit_set(index->gpio, index->pin);
    }
}

int gd32_pin_read(ug_base_t pin)
{
    int value;
    const struct pin_index *index;

    value = PIN_LOW;

    index = get_pin(pin);
    if (index == UG_NULL)
    {
        return value;
    }

    if (gpio_input_bit_get(index->gpio, index->pin) == RESET)
    {
        value = PIN_LOW;
    }
    else
    {
        value = PIN_HIGH;
    }

    return value;
}

void gd32_pin_mode(ug_base_t pin, ug_base_t mode, ug_uint32_t alt_func_num)
{
    const struct pin_index *index;

    index = get_pin(pin);
    if (index == UG_NULL)
    {
        return;
    }

    /* GPIO Periph clock enable */
    rcu_periph_clock_enable(index->rcc);

    if (mode == PIN_MODE_OUTPUT)
    {
        /* output setting */
        gpio_mode_set(index->gpio, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, index->pin);
        gpio_output_options_set(index->gpio, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, index->pin);
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        /* output setting: od. */
        gpio_mode_set(index->gpio, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, index->pin);
        gpio_output_options_set(index->gpio, GPIO_OTYPE_OD, GPIO_OSPEED_MAX, index->pin);
    }
    else if (mode == PIN_MODE_INPUT_ANL)
    {
        /* input setting: analog input . */
        gpio_mode_set(index->gpio, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, index->pin);
    }
    else if (mode == PIN_MODE_INPUT)
    {
        /* input setting: not pull. */
        gpio_mode_set(index->gpio, GPIO_MODE_INPUT, GPIO_PUPD_NONE, index->pin);
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        /* input setting: pull up. */
        gpio_mode_set(index->gpio, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, index->pin);
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
        /* input setting: pull down. */
        gpio_mode_set(index->gpio, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, index->pin);
    }
    else
    {
        /* set GPIO alternate function. */
        gpio_mode_set(index->gpio, GPIO_MODE_AF, GPIO_PUPD_NONE, index->pin);
        gpio_output_options_set(index->gpio, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, index->pin);
        gpio_af_set(index->gpio, alt_func_num, index->pin);
    }

}

ug_inline ug_int32_t bit2bitno(ug_uint32_t bit)
{
    int i;
    for(i = 0; i < 32; i++)
    {
        if((0x01 << i) == bit)
        {
            return i;
        }
    }
    return -1;
}
ug_inline const struct pin_irq_map *get_pin_irq_map(ug_uint32_t pinbit)
{
    ug_int32_t mapindex = bit2bitno(pinbit);
    if(mapindex < 0 || mapindex >= ITEM_NUM(gd32_pin_irq_map))
    {
        return UG_NULL;
    }
    return &gd32_pin_irq_map[mapindex];
};

ug_err_t gd32_pin_attach_irq(ug_int32_t pin,
                             ug_uint32_t mode,
                             void (*hdr)(void *args),
                             void *args,
                             void *priority)
{
    const struct pin_index *index;
    ug_int32_t irqindex = -1;

    index = get_pin(pin);
    if (index == UG_NULL)
    {
        return -UG_ENOSYS;
    }
    irqindex = bit2bitno(index->pin);
    if(irqindex < 0 || irqindex >= ITEM_NUM(gd32_pin_irq_map))
    {
        return -UG_ENOSYS;
    }

    if(pin_irq_hdr_tab[irqindex].pin == pin   &&
            pin_irq_hdr_tab[irqindex].hdr == hdr   &&
            pin_irq_hdr_tab[irqindex].mode == mode &&
            pin_irq_hdr_tab[irqindex].args == args &&
            pin_irq_hdr_tab[irqindex].priority == priority
      )
    {
        return UG_EOK;
    }
    if(pin_irq_hdr_tab[irqindex].pin != -1)
    {
        return -UG_EBUSY;
    }
    pin_irq_hdr_tab[irqindex].pin = pin;
    pin_irq_hdr_tab[irqindex].hdr = hdr;
    pin_irq_hdr_tab[irqindex].mode = mode;
    pin_irq_hdr_tab[irqindex].args = args;
    pin_irq_hdr_tab[irqindex].priority = priority;

    return UG_EOK;
}

ug_err_t gd32_pin_detach_irq(ug_int32_t pin)
{
    const struct pin_index *index;
    ug_int32_t irqindex = -1;

    index = get_pin(pin);
    if (index == UG_NULL)
    {
        return -UG_ENOSYS;
    }
    irqindex = bit2bitno(index->pin);
    if(irqindex < 0 || irqindex >= ITEM_NUM(gd32_pin_irq_map))
    {
        return -UG_ENOSYS;
    }

    if(pin_irq_hdr_tab[irqindex].pin == -1)
    {
        return UG_EOK;
    }
    pin_irq_hdr_tab[irqindex].pin = -1;
    pin_irq_hdr_tab[irqindex].hdr = UG_NULL;
    pin_irq_hdr_tab[irqindex].mode = 0;
    pin_irq_hdr_tab[irqindex].args = UG_NULL;

    return UG_EOK;
}

ug_err_t gd32_pin_irq_enable(ug_base_t pin,ug_uint32_t enabled)
{
    const struct pin_index *index;
    const struct pin_irq_map *irqmap;
    ug_int32_t irqindex = -1;
    irq_priority *priority;

    index = get_pin(pin);
    if (index == UG_NULL)
    {
        return -UG_ENOSYS;
    }
    if(enabled == PIN_IRQ_ENABLE)
    {
        irqindex = bit2bitno(index->pin);
        if(irqindex < 0 || irqindex >= ITEM_NUM(gd32_pin_irq_map))
        {
            return -UG_ENOSYS;
        }

        if(pin_irq_hdr_tab[irqindex].pin == -1)
        {
            return -UG_ENOSYS;
        }
        irqmap = &gd32_pin_irq_map[irqindex];
        /* GPIO Periph clock enable */
        rcu_periph_clock_enable(index->rcc);
        rcu_periph_clock_enable(RCU_CFGCMP);
        /* Configure GPIO_InitStructure */
        /* input setting: not pull. */
        //gpio_mode_set(index->gpio, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, pin);
        /* enable and set key user EXTI interrupt to the lower priority */
        priority = pin_irq_hdr_tab[irqindex].priority;
        nvic_irq_enable(irqmap->irqno, priority->nvic_irq_pre_priority, priority->nvic_irq_sub_priority);
        /* connect key user EXTI line to key GPIO pin */
        syscfg_exti_line_config(index->port_source, index->pin_source);

        switch(pin_irq_hdr_tab[irqindex].mode)
        {
        case PIN_IRQ_MODE_RISING:
            exti_init(irqmap->irqbit, EXTI_INTERRUPT, EXTI_TRIG_RISING);
            break;
        case PIN_IRQ_MODE_FALLING:
            exti_init(irqmap->irqbit, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
            break;
        case PIN_IRQ_MODE_RISING_FALLING:
            exti_init(irqmap->irqbit, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
            break;
        }

        exti_interrupt_flag_clear(irqmap->irqbit);
    }
    else if(enabled == PIN_IRQ_DISABLE)
    {
        irqmap = get_pin_irq_map(index->pin);
        if(irqmap == UG_NULL)
        {
            return -UG_ENOSYS;
        }
        nvic_irq_disable(irqmap->irqno);
    }
    else
    {
        return -UG_ENOSYS;
    }

    return UG_EOK;
}

ug_inline void pin_irq_hdr(int irqno)
{
    exti_interrupt_flag_clear(gd32_pin_irq_map[irqno].irqbit);
    if(pin_irq_hdr_tab[irqno].hdr)
    {
        pin_irq_hdr_tab[irqno].hdr(pin_irq_hdr_tab[irqno].args);
    }
}

void pin_out_init(ug_base_t pin, ug_uint8_t mode)
{
    gd32_pin_mode(pin, mode, UG_NULL);
}

void pin_out_set(ug_base_t pin, bit_status bit_value)
{
    gd32_pin_mode(pin, PIN_MODE_OUTPUT, UG_NULL);
    gd32_pin_write(pin, bit_value);
}

void pin_out_on(ug_base_t pin)
{
    gd32_pin_write(pin, PIN_HIGH);
}

void pin_out_off(ug_base_t pin)
{
    gd32_pin_write(pin, PIN_LOW);
}

void pin_in_init(ug_base_t pin, ug_uint8_t mode)
{
    gd32_pin_mode(pin, mode, UG_NULL);
}

int pin_read(ug_base_t pin)
{
    return gd32_pin_read(pin);
}

void pin_af_init(ug_base_t pin, ug_uint32_t alt_func_num)
{
    gd32_pin_mode(pin, PIN_MODE_AF, alt_func_num);
}

ug_err_t pin_irq_install(ug_base_t pin, ug_base_t mode,
                          ug_uint32_t irqMode,
                          void (*hdr)(void *args),
                          void  *args,
                          void *priority)
{
    gd32_pin_mode(pin, mode, UG_NULL);
    // 绑定中断
    gd32_pin_attach_irq(pin, irqMode, hdr, args, priority);
    // 使能中断
    return gd32_pin_irq_enable(pin, PIN_IRQ_ENABLE);
}

void EXTI2_3_IRQHandler(void)
{
    if (exti_interrupt_flag_get(EXTI_2) != RESET)
    {
        pin_irq_hdr(2);
    }
    if (exti_interrupt_flag_get(EXTI_3) != RESET)
    {
        pin_irq_hdr(3);
    }

}

void EXTI4_15_IRQHandler(void)
{

    if (exti_interrupt_flag_get(EXTI_4) != RESET)
    {
        pin_irq_hdr(4);
    }
    if (exti_interrupt_flag_get(EXTI_5) != RESET)
    {
        pin_irq_hdr(5);
    }
    if (exti_interrupt_flag_get(EXTI_6) != RESET)
    {
        pin_irq_hdr(6);
    }
    if (exti_interrupt_flag_get(EXTI_7) != RESET)
    {
        pin_irq_hdr(7);
    }
    if (exti_interrupt_flag_get(EXTI_8) != RESET)
    {
        pin_irq_hdr(8);
    }
    if (exti_interrupt_flag_get(EXTI_9) != RESET)
    {
        pin_irq_hdr(9);
    }
    if (exti_interrupt_flag_get(EXTI_10) != RESET)
    {
        pin_irq_hdr(10);
    }
    if (exti_interrupt_flag_get(EXTI_11) != RESET)
    {
        pin_irq_hdr(11);
    }
    if (exti_interrupt_flag_get(EXTI_12) != RESET)
    {
        pin_irq_hdr(12);
    }
    if (exti_interrupt_flag_get(EXTI_13) != RESET)
    {
        pin_irq_hdr(13);
    }
    if (exti_interrupt_flag_get(EXTI_14) != RESET)
    {
        pin_irq_hdr(14);
    }
    if (exti_interrupt_flag_get(EXTI_15) != RESET)
    {
        pin_irq_hdr(15);
    }
}


