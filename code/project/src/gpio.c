#include "gd32f3x0_gpio.h"
#include "gpio.h"


#include "..\src\device\drv_pin.h"
//#include "..\src\device\pin.h"

#define PIN_NUM_4051_1    4
#define PIN_NUM_4051_2    6
#define PIN_NUM_4051_3    7
#define PIN_NUM_4051_4    5
#define PIN_NUM_4051_5    2
#define PIN_NUM_4051_6    1
#define PIN_NUM_4051_7    0
#define PIN_NUM_4051_8    3


const ug_base_t keys_pin[] =
{
    KB1_PIN_IDX,
    KB2_PIN_IDX,
    KB3_PIN_IDX,
    KB4_PIN_IDX,
    KB5_PIN_IDX,
    KB6_PIN_IDX,
    KB7_PIN_IDX,
    KB8_PIN_IDX
};

#if ( defined (configUse_74hc138_X))&&( defined (configUse_74hc138_Y))

const CD74HC4051_x_pin_t hc4051_x_pin =
{
    {
        X_SEL00_PIN_IDX,
        X_SEL01_PIN_IDX,
        X_SEL02_PIN_IDX
    },
    X_ADDR00_PIN_IDX,
    X_ADDR01_PIN_IDX,
    X_ADDR02_PIN_IDX

};

const CD74HC4051_y_pin_t hc4051_y_pin =
{
    {
        Y_SEL00_PIN_IDX,
        Y_SEL01_PIN_IDX,
        Y_SEL02_PIN_IDX
    },
    Y_ADDR00_PIN_IDX,
    Y_ADDR01_PIN_IDX,
    Y_ADDR02_PIN_IDX

};

const ant_x_pin_t x_ant_pin[] =
{
    {0,  0},
    {&hc4051_x_pin,  PIN_NUM_4051_1},      // X1
    {&hc4051_x_pin,  PIN_NUM_4051_2},      // X2
    {&hc4051_x_pin,  PIN_NUM_4051_3},      // X3
    {&hc4051_x_pin,  PIN_NUM_4051_4},      // X4
    {&hc4051_x_pin,  PIN_NUM_4051_5},      // X5
    {&hc4051_x_pin,  PIN_NUM_4051_6},      // X6
    {&hc4051_x_pin,  PIN_NUM_4051_7},      // X7
    {&hc4051_x_pin,  PIN_NUM_4051_8},      // X8

};

const ant_y_pin_t y_ant_pin[] =
{
    {0,  0},
    {&hc4051_y_pin,  PIN_NUM_4051_1},      // X1
    {&hc4051_y_pin,  PIN_NUM_4051_2},      // X2
    {&hc4051_y_pin,  PIN_NUM_4051_3},      // X3
    {&hc4051_y_pin,  PIN_NUM_4051_4},      // X4
    {&hc4051_y_pin,  PIN_NUM_4051_5},      // X5
    {&hc4051_y_pin,  PIN_NUM_4051_6},      // X6
    {&hc4051_y_pin,  PIN_NUM_4051_7},      // X7
    {&hc4051_y_pin,  PIN_NUM_4051_8},      // X8

};

#elif  defined (configUse_74hc138_X)

const CD74HC4051_x_pin_t hc4051_x_pin =
{
    {
        X_SEL00_PIN_IDX,
        X_SEL01_PIN_IDX,
        X_SEL02_PIN_IDX
    },
    X_ADDR00_PIN_IDX,
    X_ADDR01_PIN_IDX,
    X_ADDR02_PIN_IDX

};

const CD74HC4051_y_pin_t hc4051_y_pin[] =
{
    {
        Y_SEL00_PIN_IDX,
        Y_ADDR00_PIN_IDX,
        Y_ADDR01_PIN_IDX,
        Y_ADDR02_PIN_IDX
    },
    {
        Y_SEL01_PIN_IDX,
        Y_ADDR00_PIN_IDX,
        Y_ADDR01_PIN_IDX,
        Y_ADDR02_PIN_IDX
    },
    {
        Y_SEL02_PIN_IDX,
        Y_ADDR00_PIN_IDX,
        Y_ADDR01_PIN_IDX,
        Y_ADDR02_PIN_IDX
    },
    {
        Y_SEL03_PIN_IDX,
        Y_ADDR00_PIN_IDX,
        Y_ADDR01_PIN_IDX,
        Y_ADDR02_PIN_IDX
    }
};

const ant_x_pin_t x_ant_pin[] =
{
    {0,  0},
    {&hc4051_x_pin,  PIN_NUM_4051_1},      // X1
    {&hc4051_x_pin,  PIN_NUM_4051_2},      // X2
    {&hc4051_x_pin,  PIN_NUM_4051_3},      // X3
    {&hc4051_x_pin,  PIN_NUM_4051_4},      // X4
    {&hc4051_x_pin,  PIN_NUM_4051_5},      // X5
    {&hc4051_x_pin,  PIN_NUM_4051_6},      // X6
    {&hc4051_x_pin,  PIN_NUM_4051_7},      // X7
    {&hc4051_x_pin,  PIN_NUM_4051_8},      // X8

};

const ant_y_pin_t y_ant_pin[] =
{
    {0,  0},
    {&hc4051_y_pin[0],  PIN_NUM_4051_1},      // Y1
    {&hc4051_y_pin[0],  PIN_NUM_4051_2},      // Y2
    {&hc4051_y_pin[0],  PIN_NUM_4051_3},      // Y3
    {&hc4051_y_pin[0],  PIN_NUM_4051_4},      // Y4
    {&hc4051_y_pin[0],  PIN_NUM_4051_5},      // Y5
    {&hc4051_y_pin[0],  PIN_NUM_4051_6},      // Y6
    {&hc4051_y_pin[0],  PIN_NUM_4051_7},      // Y7
    {&hc4051_y_pin[0],  PIN_NUM_4051_8},      // Y8
    {&hc4051_y_pin[1],  PIN_NUM_4051_1},      // Y9
    {&hc4051_y_pin[1],  PIN_NUM_4051_2},      // Y10
    {&hc4051_y_pin[1],  PIN_NUM_4051_3},      // Y11
    {&hc4051_y_pin[1],  PIN_NUM_4051_4},      // Y12
    {&hc4051_y_pin[1],  PIN_NUM_4051_5},      // Y13
    {&hc4051_y_pin[1],  PIN_NUM_4051_6},      // Y14
    {&hc4051_y_pin[1],  PIN_NUM_4051_7},      // Y15
    {&hc4051_y_pin[1],  PIN_NUM_4051_8},      // Y16
    {&hc4051_y_pin[2],  PIN_NUM_4051_1},      // Y17
    {&hc4051_y_pin[2],  PIN_NUM_4051_2},      // Y18
    {&hc4051_y_pin[2],  PIN_NUM_4051_3},      // Y19
    {&hc4051_y_pin[2],  PIN_NUM_4051_4},      // Y20
    {&hc4051_y_pin[2],  PIN_NUM_4051_5},      // Y21
    {&hc4051_y_pin[2],  PIN_NUM_4051_6},      // Y22
    {&hc4051_y_pin[2],  PIN_NUM_4051_7},      // Y23
    {&hc4051_y_pin[2],  PIN_NUM_4051_8},      // Y24
    {&hc4051_y_pin[3],  PIN_NUM_4051_1},      // Y25
    {&hc4051_y_pin[3],  PIN_NUM_4051_2},      // Y26
    {&hc4051_y_pin[3],  PIN_NUM_4051_3},      // Y27
    {&hc4051_y_pin[3],  PIN_NUM_4051_4},      // Y28
    //{&hc4051_y_pin[3],  3},      // Y29
};
#elif  defined (configUse_74hc138_Y)

const CD74HC4051_x_pin_t hc4051_x_pin[] =
{
    {
        X_SEL00_PIN_IDX,
        X_ADDR00_PIN_IDX,
        X_ADDR01_PIN_IDX,
        X_ADDR02_PIN_IDX
    },
    {
        X_SEL01_PIN_IDX,
        X_ADDR00_PIN_IDX,
        X_ADDR01_PIN_IDX,
        X_ADDR02_PIN_IDX
    },
    {
        X_SEL02_PIN_IDX,
        X_ADDR00_PIN_IDX,
        X_ADDR01_PIN_IDX,
        X_ADDR02_PIN_IDX
    },
    {
        X_SEL03_PIN_IDX,
        X_ADDR00_PIN_IDX,
        X_ADDR01_PIN_IDX,
        X_ADDR02_PIN_IDX
    }
}

const CD74HC4051_y_pin_t hc4051_y_pin =
{
    {
        Y_SEL00_PIN_IDX,
        Y_SEL01_PIN_IDX,
        Y_SEL02_PIN_IDX
    },
    Y_ADDR00_PIN_IDX,
    Y_ADDR01_PIN_IDX,
    Y_ADDR02_PIN_IDX

};

const ant_x_pin_t x_ant_pin[] =
{
    {0,  0},
    {&hc4051_x_pin[0],  PIN_NUM_4051_1},      // Y1
    {&hc4051_x_pin[0],  PIN_NUM_4051_2},      // Y2
    {&hc4051_x_pin[0],  PIN_NUM_4051_3},      // Y3
    {&hc4051_x_pin[0],  PIN_NUM_4051_4},      // Y4
    {&hc4051_x_pin[0],  PIN_NUM_4051_5},      // Y5
    {&hc4051_x_pin[0],  PIN_NUM_4051_6},      // Y6
    {&hc4051_x_pin[0],  PIN_NUM_4051_7},      // Y7
    {&hc4051_x_pin[0],  PIN_NUM_4051_8},      // Y8
    {&hc4051_x_pin[1],  PIN_NUM_4051_1},      // Y9
    {&hc4051_x_pin[1],  PIN_NUM_4051_2},      // Y10
    {&hc4051_x_pin[1],  PIN_NUM_4051_3},      // Y11
    {&hc4051_x_pin[1],  PIN_NUM_4051_4},      // Y12
    {&hc4051_x_pin[1],  PIN_NUM_4051_5},      // Y13
    {&hc4051_x_pin[1],  PIN_NUM_4051_6},      // Y14
    {&hc4051_x_pin[1],  PIN_NUM_4051_7},      // Y15
    {&hc4051_x_pin[1],  PIN_NUM_4051_8},      // Y16
    {&hc4051_x_pin[2],  PIN_NUM_4051_1},      // Y17
    {&hc4051_x_pin[2],  PIN_NUM_4051_2},      // Y18
    {&hc4051_x_pin[2],  PIN_NUM_4051_3},      // Y19
    {&hc4051_x_pin[2],  PIN_NUM_4051_4},      // Y20
    {&hc4051_x_pin[2],  PIN_NUM_4051_5},      // Y21
    {&hc4051_x_pin[2],  PIN_NUM_4051_6},      // Y22
    {&hc4051_x_pin[2],  PIN_NUM_4051_7},      // Y23
    {&hc4051_x_pin[2],  PIN_NUM_4051_8},      // Y24
    {&hc4051_x_pin[3],  PIN_NUM_4051_1},      // Y25
    {&hc4051_x_pin[3],  PIN_NUM_4051_2},      // Y26
    {&hc4051_x_pin[3],  PIN_NUM_4051_3},      // Y27
    {&hc4051_x_pin[3],  PIN_NUM_4051_4},      // Y28
    {&hc4051_x_pin[3],  PIN_NUM_4051_5},      // Y29
};

const ant_y_pin_t y_ant_pin[] =
{
    {0,  0},
    {&hc4051_y_pin,  PIN_NUM_4051_1},      // X1
    {&hc4051_y_pin,  PIN_NUM_4051_2},      // X2
    {&hc4051_y_pin,  PIN_NUM_4051_3},      // X3
    {&hc4051_y_pin,  PIN_NUM_4051_4},      // X4
    {&hc4051_y_pin,  PIN_NUM_4051_5},      // X5
    {&hc4051_y_pin,  PIN_NUM_4051_6},      // X6
    {&hc4051_y_pin,  PIN_NUM_4051_7},      // X7
    {&hc4051_y_pin,  PIN_NUM_4051_8},      // X8

};

#else

const CD74HC4051_x_pin_t hc4051_x_pin[] =
{
    {
        X_SEL00_PIN_IDX,
        X_ADDR00_PIN_IDX,
        X_ADDR01_PIN_IDX,
        X_ADDR02_PIN_IDX
    },
    {
        X_SEL01_PIN_IDX,
        X_ADDR00_PIN_IDX,
        X_ADDR01_PIN_IDX,
        X_ADDR02_PIN_IDX
    },
    {
        X_SEL02_PIN_IDX,
        X_ADDR00_PIN_IDX,
        X_ADDR01_PIN_IDX,
        X_ADDR02_PIN_IDX
    },
    {
        X_SEL03_PIN_IDX,
        X_ADDR00_PIN_IDX,
        X_ADDR01_PIN_IDX,
        X_ADDR02_PIN_IDX
    }
};

const CD74HC4051_y_pin_t hc4051_y_pin[] =
{
    {
        Y_SEL00_PIN_IDX,
        Y_ADDR00_PIN_IDX,
        Y_ADDR01_PIN_IDX,
        Y_ADDR02_PIN_IDX
    },
    {
        Y_SEL01_PIN_IDX,
        Y_ADDR00_PIN_IDX,
        Y_ADDR01_PIN_IDX,
        Y_ADDR02_PIN_IDX
    },
    {
        Y_SEL02_PIN_IDX,
        Y_ADDR00_PIN_IDX,
        Y_ADDR01_PIN_IDX,
        Y_ADDR02_PIN_IDX
    },
    {
        Y_SEL03_PIN_IDX,
        Y_ADDR00_PIN_IDX,
        Y_ADDR01_PIN_IDX,
        Y_ADDR02_PIN_IDX
    }
};

const ant_x_pin_t x_ant_pin[] =
{
    {0,  0},
    {&hc4051_x_pin[0],  PIN_NUM_4051_1},      // Y1
    {&hc4051_x_pin[0],  PIN_NUM_4051_2},      // Y2
    {&hc4051_x_pin[0],  PIN_NUM_4051_3},      // Y3
    {&hc4051_x_pin[0],  PIN_NUM_4051_4},      // Y4
    {&hc4051_x_pin[0],  PIN_NUM_4051_5},      // Y5
    {&hc4051_x_pin[0],  PIN_NUM_4051_6},      // Y6
    {&hc4051_x_pin[0],  PIN_NUM_4051_7},      // Y7
    {&hc4051_x_pin[0],  PIN_NUM_4051_8},      // Y8
    {&hc4051_x_pin[1],  PIN_NUM_4051_1},      // Y9
    {&hc4051_x_pin[1],  PIN_NUM_4051_2},      // Y10
    {&hc4051_x_pin[1],  PIN_NUM_4051_3},      // Y11
    {&hc4051_x_pin[1],  PIN_NUM_4051_4},      // Y12
    {&hc4051_x_pin[1],  PIN_NUM_4051_5},      // Y13
    {&hc4051_x_pin[1],  PIN_NUM_4051_6},      // Y14
    {&hc4051_x_pin[1],  PIN_NUM_4051_7},      // Y15
    {&hc4051_x_pin[1],  PIN_NUM_4051_8},      // Y16
    {&hc4051_x_pin[2],  PIN_NUM_4051_1},      // Y17
    {&hc4051_x_pin[2],  PIN_NUM_4051_2},      // Y18
    {&hc4051_x_pin[2],  PIN_NUM_4051_3},      // Y19
    {&hc4051_x_pin[2],  PIN_NUM_4051_4},      // Y20
    {&hc4051_x_pin[2],  PIN_NUM_4051_5},      // Y21
    {&hc4051_x_pin[2],  PIN_NUM_4051_6},      // Y22
    {&hc4051_x_pin[2],  PIN_NUM_4051_7},      // Y23
    {&hc4051_x_pin[2],  PIN_NUM_4051_8},      // Y24
    {&hc4051_x_pin[3],  PIN_NUM_4051_1},      // Y25
    {&hc4051_x_pin[3],  PIN_NUM_4051_2},      // Y26
    {&hc4051_x_pin[3],  PIN_NUM_4051_3},      // Y27
    {&hc4051_x_pin[3],  PIN_NUM_4051_4},      // Y28
    {&hc4051_x_pin[3],  PIN_NUM_4051_5},      // Y29
};

const ant_y_pin_t y_ant_pin[] =
{
    {0,  0},
    {&hc4051_y_pin[0],  PIN_NUM_4051_1},      // Y1
    {&hc4051_y_pin[0],  PIN_NUM_4051_2},      // Y2
    {&hc4051_y_pin[0],  PIN_NUM_4051_3},      // Y3
    {&hc4051_y_pin[0],  PIN_NUM_4051_4},      // Y4
    {&hc4051_y_pin[0],  PIN_NUM_4051_5},      // Y5
    {&hc4051_y_pin[0],  PIN_NUM_4051_6},      // Y6
    {&hc4051_y_pin[0],  PIN_NUM_4051_7},      // Y7
    {&hc4051_y_pin[0],  PIN_NUM_4051_8},      // Y8
    {&hc4051_y_pin[1],  PIN_NUM_4051_1},      // Y9
    {&hc4051_y_pin[1],  PIN_NUM_4051_2},      // Y10
    {&hc4051_y_pin[1],  PIN_NUM_4051_3},      // Y11
    {&hc4051_y_pin[1],  PIN_NUM_4051_4},      // Y12
    {&hc4051_y_pin[1],  PIN_NUM_4051_5},      // Y13
    {&hc4051_y_pin[1],  PIN_NUM_4051_6},      // Y14
    {&hc4051_y_pin[1],  PIN_NUM_4051_7},      // Y15
    {&hc4051_y_pin[1],  PIN_NUM_4051_8},      // Y16
    {&hc4051_y_pin[2],  PIN_NUM_4051_1},      // Y17
    {&hc4051_y_pin[2],  PIN_NUM_4051_2},      // Y18
    {&hc4051_y_pin[2],  PIN_NUM_4051_3},      // Y19
    {&hc4051_y_pin[2],  PIN_NUM_4051_4},      // Y20
    {&hc4051_y_pin[2],  PIN_NUM_4051_5},      // Y21
    {&hc4051_y_pin[2],  PIN_NUM_4051_6},      // Y22
    {&hc4051_y_pin[2],  PIN_NUM_4051_7},      // Y23
    {&hc4051_y_pin[2],  PIN_NUM_4051_8},      // Y24
    {&hc4051_y_pin[3],  PIN_NUM_4051_1},      // Y25
    {&hc4051_y_pin[3],  PIN_NUM_4051_2},      // Y26
    {&hc4051_y_pin[3],  PIN_NUM_4051_3},      // Y27
    {&hc4051_y_pin[3],  PIN_NUM_4051_4},      // Y28
    {&hc4051_y_pin[3],  PIN_NUM_4051_5},      // Y29
};

#endif

typedef struct
{
    ug_uint8_t  pin_mode;
    ug_base_t   pin;
    ug_uint32_t pin_value;
} gd32_pin_config_t;
#define __GD32_PIN_PARAM_SET(index, pin_mode, pin,pin_value) { pin_mode, pin,pin_value }
static const gd32_pin_config_t  pinParamGroup[] =
{
    __GD32_PIN_PARAM_SET(PC13, PIN_MODE_OUTPUT, PWCL_PIN_IDX,     RESET),
    __GD32_PIN_PARAM_SET(PB11, PIN_MODE_OUTPUT, LED_W_PIN_IDX,    RESET),
//    __GD32_PIN_PARAM_SET(PA14,  PIN_MODE_OUTPUT,  PB8_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PB12, PIN_MODE_OUTPUT, IRQ_PIN_IDX,      SET),
    __GD32_PIN_PARAM_SET(PB15, PIN_MODE_OUTPUT, DISC_ADC_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PB9,  PIN_MODE_OUTPUT, DISC_OPA_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PC9,  PIN_MODE_OUTPUT, TX_EN_PIN_IDX,    RESET),

    __GD32_PIN_PARAM_SET(PB2,  PIN_MODE_OUTPUT, X_ADDR00_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PB1,  PIN_MODE_OUTPUT, X_ADDR01_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PB0,  PIN_MODE_OUTPUT, X_ADDR02_PIN_IDX, SET),

    __GD32_PIN_PARAM_SET(PA0,  PIN_MODE_OUTPUT, Y_ADDR00_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PA1,  PIN_MODE_OUTPUT, Y_ADDR01_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PA2,  PIN_MODE_OUTPUT, Y_ADDR02_PIN_IDX, SET),

    __GD32_PIN_PARAM_SET(PC5,  PIN_MODE_OUTPUT, X_SEL00_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PC4,  PIN_MODE_OUTPUT, X_SEL01_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PA7,  PIN_MODE_OUTPUT, X_SEL02_PIN_IDX,  SET),

    __GD32_PIN_PARAM_SET(PA3,  PIN_MODE_OUTPUT, Y_SEL00_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PF4,  PIN_MODE_OUTPUT, Y_SEL01_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PF5,  PIN_MODE_OUTPUT, Y_SEL02_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PA4,  PIN_MODE_OUTPUT, Y_SEL03_PIN_IDX,  SET),

    __GD32_PIN_PARAM_SET(PC6,  PIN_MODE_OUTPUT, READY_PIN_IDX,    RESET),
    __GD32_PIN_PARAM_SET(PB6,  PIN_MODE_INPUT_PULLDOWN, USB_DECT_PIN_IDX, RESET),
    
    __GD32_PIN_PARAM_SET(PB15, PIN_MODE_INPUT_PULLUP, KB1_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PB14, PIN_MODE_INPUT_PULLUP, KB2_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PB13, PIN_MODE_INPUT_PULLUP, KB3_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PB12, PIN_MODE_INPUT_PULLUP, KB4_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PB5,  PIN_MODE_INPUT_PULLUP, KB5_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PB4,  PIN_MODE_INPUT_PULLUP, KB6_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PA15, PIN_MODE_INPUT_PULLUP, KB7_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PB3,  PIN_MODE_INPUT_PULLUP, KB8_PIN_IDX, SET),
    
    __GD32_PIN_PARAM_SET(PB13, PIN_MODE_INPUT_PULLUP, I2C_INT1_PIN_IDX, SET),
    __GD32_PIN_PARAM_SET(PB14, PIN_MODE_INPUT_PULLUP, I2C_INT2_PIN_IDX, SET),
    
    __GD32_PIN_PARAM_SET(PC0,  PIN_MODE_INPUT_ANL, AD_IN_X_PIN_IDX,SET),
    __GD32_PIN_PARAM_SET(PC1,  PIN_MODE_INPUT_ANL, AD_IN_Y_PIN_IDX,SET),

    __GD32_PIN_PARAM_SET(PA8,  PIN_MODE_AF, TX_CLK_PIN_IDX, GPIO_AF_2),

    __GD32_PIN_PARAM_SET(PA9,  PIN_MODE_AF, USART_TX_PIN_IDX, GPIO_AF_1),
    __GD32_PIN_PARAM_SET(PA10, PIN_MODE_AF, USART_RX_PIN_IDX, GPIO_AF_1)

};

static const gd32_pin_config_t  haltPinParamGroup[] =
{
    __GD32_PIN_PARAM_SET(PC13, PIN_MODE_OUTPUT, PWCL_PIN_IDX,     SET),
    __GD32_PIN_PARAM_SET(PB11, PIN_MODE_OUTPUT, LED_W_PIN_IDX,    SET),
    //__GD32_PIN_PARAM_SET(PB11, PIN_MODE_OUTPUT, LED_B_PIN_IDX,    SET),
//    __GD32_PIN_PARAM_SET(PB12, PIN_MODE_INPUT,  IRQ_PIN_IDX,      RESET),
//    __GD32_PIN_PARAM_SET(PB15, PIN_MODE_INPUT,  DISC_ADC_PIN_IDX, SET),
//    __GD32_PIN_PARAM_SET(PB9,  PIN_MODE_INPUT,  DISC_OPA_PIN_IDX, SET),
//    __GD32_PIN_PARAM_SET(PB11, PIN_MODE_INPUT,  LED_B_PIN_IDX,    SET),

//    __GD32_PIN_PARAM_SET(PB2,  PIN_MODE_INPUT,  X_ADDR00_PIN_IDX, SET),
//    __GD32_PIN_PARAM_SET(PB1,  PIN_MODE_INPUT,  X_ADDR01_PIN_IDX, SET),
//    __GD32_PIN_PARAM_SET(PB0,  PIN_MODE_INPUT,  X_ADDR02_PIN_IDX, SET),

//    __GD32_PIN_PARAM_SET(PA0,  PIN_MODE_INPUT,  Y_ADDR00_PIN_IDX, SET),
//    __GD32_PIN_PARAM_SET(PA1,  PIN_MODE_INPUT,  Y_ADDR01_PIN_IDX, SET),
//    __GD32_PIN_PARAM_SET(PA2,  PIN_MODE_INPUT,  Y_ADDR02_PIN_IDX, SET),

//    __GD32_PIN_PARAM_SET(PC5,  PIN_MODE_INPUT,  X_SEL00_PIN_IDX,  SET),
//    __GD32_PIN_PARAM_SET(PC4,  PIN_MODE_INPUT,  X_SEL01_PIN_IDX,  SET),
//    __GD32_PIN_PARAM_SET(PA7,  PIN_MODE_INPUT,  X_SEL02_PIN_IDX,  SET),

//    __GD32_PIN_PARAM_SET(PA3,  PIN_MODE_INPUT,  Y_SEL00_PIN_IDX,  SET),
//    __GD32_PIN_PARAM_SET(PF4,  PIN_MODE_INPUT,  Y_SEL01_PIN_IDX,  SET),
//    __GD32_PIN_PARAM_SET(PF5,  PIN_MODE_INPUT,  Y_SEL02_PIN_IDX,  SET),
//    __GD32_PIN_PARAM_SET(PA4,  PIN_MODE_INPUT,  Y_SEL03_PIN_IDX,  SET),

//    __GD32_PIN_PARAM_SET(PC6,  PIN_MODE_INPUT,  READY_PIN_IDX,    SET),

//    __GD32_PIN_PARAM_SET(PA8,  PIN_MODE_INPUT,  TX_CLK_PIN_IDX,   SET),
//    __GD32_PIN_PARAM_SET(PC9,  PIN_MODE_INPUT,  TX_EN_PIN_IDX,    SET),

//    __GD32_PIN_PARAM_SET(PA9,  PIN_MODE_INPUT, USART_TX_PIN_IDX, SET),
//    __GD32_PIN_PARAM_SET(PA10, PIN_MODE_INPUT, USART_RX_PIN_IDX, SET),
//    
//    __GD32_PIN_PARAM_SET(PA11,  PIN_MODE_INPUT,  USB_DM_PIN_IDX,  SET),
//    __GD32_PIN_PARAM_SET(PA12,  PIN_MODE_INPUT,  USB_DP_PIN_IDX,  SET),

//    //__GD32_PIN_PARAM_SET(PA13,  PIN_MODE_INPUT,  SWDIO_PIN_IDX,  SET),
//    //__GD32_PIN_PARAM_SET(PA14,  PIN_MODE_INPUT,  SWCLK_PIN_IDX,  SET),

//    __GD32_PIN_PARAM_SET(PF6,  PIN_MODE_INPUT,  I2C_SCL_PIN_IDX,  SET),
//    __GD32_PIN_PARAM_SET(PF7,  PIN_MODE_INPUT,  I2C_SDA_PIN_IDX,  SET),
//    __GD32_PIN_PARAM_SET(PB13,  PIN_MODE_OUTPUT_OD, I2C_INT1_PIN_IDX,    SET),
//    __GD32_PIN_PARAM_SET(PB14,  PIN_MODE_OUTPUT_OD, I2C_INT2_PIN_IDX,    SET),

//    __GD32_PIN_PARAM_SET(PC1,  PIN_MODE_OUTPUT_OD, AD_IN_Y_PIN_IDX,SET),
    //floating pin configurate
    __GD32_PIN_PARAM_SET(PA13,  PIN_MODE_INPUT_ANL,  TX_EN2_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PA14,  PIN_MODE_INPUT_ANL,  TX_EN1_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PA13,  PIN_MODE_INPUT_ANL,  PC14_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PA14,  PIN_MODE_INPUT_ANL,  PC15_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PA13,  PIN_MODE_INPUT_ANL,  PC0_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PA14,  PIN_MODE_INPUT_ANL,  PC2_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PA13,  PIN_MODE_INPUT_ANL,  PC3_PIN_IDX,  SET),
    __GD32_PIN_PARAM_SET(PA14,  PIN_MODE_INPUT_ANL,  PB8_PIN_IDX,  SET),
};

void gpio_config(void)
{
    ug_uint16_t i;
    
    for (i = 0; i < ITEM_NUM(pinParamGroup); i++)
    {
        if(pinParamGroup[i].pin_mode == PIN_MODE_OUTPUT)
        {
            pin_out_set(pinParamGroup[i].pin, (bit_status)pinParamGroup[i].pin_value);
        }
        else if(pinParamGroup[i].pin_mode == PIN_MODE_OUTPUT_OD)
        {
            pin_out_init(pinParamGroup[i].pin, pinParamGroup[i].pin_mode);
        }
        else if(pinParamGroup[i].pin_mode == PIN_MODE_AF)
        {
            pin_af_init(pinParamGroup[i].pin, pinParamGroup[i].pin_value);
        }
        else 
        {
            pin_in_init(pinParamGroup[i].pin, pinParamGroup[i].pin_mode);
        }
        
    }

}

void gpio_halt_config(void)
{
    ug_uint16_t i;

    for (i = 0; i < ITEM_NUM(haltPinParamGroup); i++)
    {
        if(haltPinParamGroup[i].pin_mode == PIN_MODE_OUTPUT)
        {
            pin_out_set(haltPinParamGroup[i].pin, (bit_status)haltPinParamGroup[i].pin_value);
        }
        else if(haltPinParamGroup[i].pin_mode == PIN_MODE_OUTPUT_OD)
        {
            pin_out_init(haltPinParamGroup[i].pin, haltPinParamGroup[i].pin_mode);
        }
        else if(haltPinParamGroup[i].pin_mode == PIN_MODE_AF)
        {
            pin_af_init(haltPinParamGroup[i].pin, haltPinParamGroup[i].pin_value);
        }
        else 
        {
            pin_in_init(haltPinParamGroup[i].pin, haltPinParamGroup[i].pin_mode);
        }
        
    }

}



