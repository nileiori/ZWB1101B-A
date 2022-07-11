#ifndef __GPIO_H__
#define __GPIO_H__

#include "config.h"
#include "drv_pin.h"



#define PWCL_PIN_IDX         2
#define LED_W_PIN_IDX        30
#define LED_B_PIN_IDX        29
#define IRQ_PIN_IDX          33
#define DISC_ADC_PIN_IDX     36
#define DISC_ADC_Y_PIN_IDX   0
#define DISC_OPA_PIN_IDX     62
#define TX_EN_PIN_IDX        40
#define X_ADDR00_PIN_IDX     28
#define X_ADDR01_PIN_IDX     27
#define X_ADDR02_PIN_IDX     26
#define Y_ADDR00_PIN_IDX     14
#define Y_ADDR01_PIN_IDX     15
#define Y_ADDR02_PIN_IDX     16
#define X_SEL00_PIN_IDX      25
#define X_SEL01_PIN_IDX      24
#define X_SEL02_PIN_IDX      23
#define X_SEL03_PIN_IDX      22
#define Y_SEL00_PIN_IDX      17
#define Y_SEL01_PIN_IDX      18
#define Y_SEL02_PIN_IDX      19
#define Y_SEL03_PIN_IDX      20
#define KB1_PIN_IDX          52
#define KB2_PIN_IDX          53
#define KB3_PIN_IDX          54
#define KB4_PIN_IDX          55
#define KB5_PIN_IDX          57
#define KB6_PIN_IDX          50
#define KB7_PIN_IDX          56
#define KB8_PIN_IDX          51
//#define KB9_PIN_IDX          54
//#define KB10_PIN_IDX         53
//#define KB11_PIN_IDX         52
//#define KB12_PIN_IDX         51
#define AD_IN_X_PIN_IDX      8
#define AD_IN_Y_PIN_IDX      9
#define TX_CLK_PIN_IDX       41
#define USART_TX_PIN_IDX     42
#define USART_RX_PIN_IDX     43
#define I2C_INT1_PIN_IDX     34
#define I2C_INT2_PIN_IDX     35
#define I2C_SCL_PIN_IDX     47
#define I2C_SDA_PIN_IDX     48

#define READY_PIN_IDX        37

#define USB_DM_PIN_IDX       44
#define USB_DP_PIN_IDX       45

#define SWDIO_PIN_IDX       46
#define SWCLK_PIN_IDX       49


//floating pin
#define TX_EN2_PIN_IDX        38
#define TX_EN1_PIN_IDX        39

#define PC14_PIN_IDX       3
#define PC15_PIN_IDX       4
#define PC0_PIN_IDX       8
#define PC2_PIN_IDX       10
#define PC3_PIN_IDX       11
#define PB8_PIN_IDX       61

#define USB_DECT_PIN_IDX  58




#define LED_R_OFF                   pin_out_on(LED_R_PIN_IDX)
#define LED_R_ON                    pin_out_off(LED_R_PIN_IDX)

#define LED_W_OFF                   pin_out_on(LED_W_PIN_IDX)
#define LED_W_ON                    pin_out_off(LED_W_PIN_IDX)

#define LED_B_OFF                   pin_out_on(LED_B_PIN_IDX)
#define LED_B_ON                    pin_out_off(LED_B_PIN_IDX)

#define LED_ON 						LED_W_ON
#define LED_OFF						LED_W_OFF

#define CHARGE_TX_DISABLE           pin_out_off(TX_EN_PIN_IDX)
#define CHARGE_TX_ENABLE            pin_out_on(TX_EN_PIN_IDX)

#define ADC_ENABLE                  pin_out_off(DISC_ADC_PIN_IDX)
#define ADC_DISABLE                 pin_out_on(DISC_ADC_PIN_IDX)

#define ADC_Y_ENABLE                pin_out_off(DISC_ADC_Y_PIN_IDX)
#define ADC_Y_DISABLE               pin_out_on(DISC_ADC_Y_PIN_IDX)

#define OPA_DISABLE                 pin_out_off(DISC_OPA_PIN_IDX)
#define OPA_ENABLE                  pin_out_on(DISC_OPA_PIN_IDX)






typedef struct
{
    uint32_t port;
    uint32_t pin;
    
} gpio_t;

#if ( defined (configUse_74hc138_X))&&( defined (configUse_74hc138_Y))

typedef struct 
{
    ug_base_t en[3];
	
    ug_base_t addr0;
    ug_base_t addr1;
    ug_base_t addr2;
    
} CD74HC4051_x_pin_t;
typedef struct
{
    const CD74HC4051_x_pin_t *p_hc4051;
    
    uint8_t no;         // ±àºÅ 0 ~ 7
        
} ant_x_pin_t;

typedef struct 
{
    ug_base_t en[3];

    ug_base_t addr0;
    ug_base_t addr1;
    ug_base_t addr2;
    
} CD74HC4051_y_pin_t;

typedef struct
{
    const CD74HC4051_y_pin_t *p_hc4051;
    
    uint8_t no;         // ±àºÅ 0 ~ 7
        
} ant_y_pin_t;

#elif defined  ( configUse_74hc138_X )

typedef struct 
{
    ug_base_t en[3];
	
    ug_base_t addr0;
    ug_base_t addr1;
    ug_base_t addr2;
    
} CD74HC4051_x_pin_t;
typedef struct
{
    const CD74HC4051_x_pin_t *p_hc4051;
    
    uint8_t no;         // ±àºÅ 0 ~ 7
        
} ant_x_pin_t;

typedef struct 
{
    ug_base_t en;

    ug_base_t addr0;
    ug_base_t addr1;
    ug_base_t addr2;
    
} CD74HC4051_y_pin_t;

typedef struct
{
    const CD74HC4051_y_pin_t *p_hc4051;
    
    uint8_t no;         // ±àºÅ 0 ~ 7
        
} ant_y_pin_t;

#elif defined  ( configUse_74hc138_Y)

typedef struct 
{
    ug_base_t en;
	
    ug_base_t addr0;
    ug_base_t addr1;
    ug_base_t addr2;
    
} CD74HC4051_x_pin_t;
typedef struct
{
    const CD74HC4051_x_pin_t *p_hc4051;
    
    uint8_t no;         // ±àºÅ 0 ~ 7
        
} ant_x_pin_t;

typedef struct 
{
    ug_base_t en[3];

    ug_base_t addr0;
    ug_base_t addr1;
    ug_base_t addr2;
    
} CD74HC4051_y_pin_t;

typedef struct
{
    const CD74HC4051_y_pin_t *p_hc4051;
    
    uint8_t no;         // ±àºÅ 0 ~ 7
        
} ant_y_pin_t;

#else

typedef struct 
{
    ug_base_t en;
	
    ug_base_t addr0;
    ug_base_t addr1;
    ug_base_t addr2;
    
} CD74HC4051_x_pin_t,CD74HC4051_y_pin_t;
typedef struct
{
    const CD74HC4051_x_pin_t *p_hc4051;
    
    uint8_t no;         // ±àºÅ 0 ~ 7
        
} ant_x_pin_t;

typedef struct
{
    const CD74HC4051_y_pin_t *p_hc4051;
    
    uint8_t no;         // ±àºÅ 0 ~ 7
        
} ant_y_pin_t;

#endif
extern const ug_base_t keys_pin[];

#if ( defined (configUse_74hc138_X))&&( defined (configUse_74hc138_Y))
extern const CD74HC4051_x_pin_t hc4051_x_pin;
extern const CD74HC4051_y_pin_t hc4051_y_pin;

#elif defined  ( configUse_74hc138_X )
extern const CD74HC4051_x_pin_t hc4051_x_pin;
extern const CD74HC4051_y_pin_t hc4051_y_pin[];

#elif defined  ( configUse_74hc138_Y)
extern const CD74HC4051_x_pin_t hc4051_x_pin[];
extern const CD74HC4051_y_pin_t hc4051_y_pin;

#else
extern const CD74HC4051_x_pin_t hc4051_x_pin[];
extern const CD74HC4051_y_pin_t hc4051_y_pin[];

#endif
extern const ant_x_pin_t x_ant_pin[];
extern const ant_y_pin_t y_ant_pin[];

void gpio_config(void);
void gpio_halt_config(void);


#endif
