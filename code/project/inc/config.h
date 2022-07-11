#ifndef __CONFIG_H__
#define __CONFIG_H__

#define BOOT_LOADER
#ifdef BOOT_LOADER
#define PRODUCT_CONFIG_ADDRESS              0x08003000
#define SYSTEM_CONFIG_ADDRESS               0x08003400  // first 4 byte 
#else
#define PRODUCT_CONFIG_ADDRESS              0x0800F800
#define SYSTEM_CONFIG_ADDRESS               0x0800FC00  // first 4 byte 
#endif
#include "preferences.h"
#define configASSERT( x ) if( ( x ) == 0 ) { for( ;; ); }	

#define ug_hw_interrupt_disable() __set_PRIMASK(1)//;__set_FAULTMASK(1)
#define ug_hw_interrupt_enable()  __set_PRIMASK(0)//;__set_FAULTMASK(0)

#define BKP_VALUE    0xAAAA

#define UG_TICK_PER_SECOND  200

#define _STR(s)             #s					 
#define STR(s)              _STR(s)

#define UG_FW_VERSION                      1     
#define UG_FW_SUBVERSION                   0   
#define UG_FW_REVISION                     0     

#define UG_PROTOCOL_VERSION                1
#define UG_PROTOCOL_SUBVERSION             0
#define UG_PROTOCOL_REVISION               0

#define UG_FW_DATE_YEAR                    21              
#define UG_FW_DATE_MONTH                   01            
#define UG_FW_DATE_DAY                     29          
#define UG_FW_DATE_HOUR                    15              
#define UG_FW_DATE_MINUTE                  30            
#define UG_FW_DATE_SECOND                  29

/* UG-Device version */
#define UG_DEVICE_VERSION               ((UG_FW_VERSION * 10000) + \
                                        (UG_FW_SUBVERSION * 100) + UG_FW_REVISION)
                                         
#define UG_HAND_PAD_FIRMWARE_VER 		STR(UG_FW_VERSION)"."STR(UG_FW_SUBVERSION)"."STR(UG_FW_REVISION)

#define UG_HAND_PAD_PROTOCOL_VER 		STR(UG_PROTOCOL_VERSION)"."STR(UG_PROTOCOL_SUBVERSION)"."STR(UG_PROTOCOL_REVISION)

#define UG_HAND_PAD_FIRMWARE_DATE 		STR(UG_FW_DATE_YEAR)"-"STR(UG_FW_DATE_MONTH)"-"STR(UG_FW_DATE_DAY)"-"STR(UG_FW_DATE_HOUR)STR(UG_FW_DATE_MINUTE)STR(UG_FW_DATE_SECOND)

#define UG_DEV_NAME 					"Deco Pro MW"

#define UG_DEV_MANUFACTURER             "11 inch PenTab"

#define UG_DEV_SERIAL_NUM 				"0000000000"

//#define     configUse_ESDTEST //做ESD测试时打开

//#define  configUse_wdog

//#define  configUse_RTC

#define	    configUse_74hc138_X

//#define	    configUse_74hc138_Y

#define	    configUse_wireless

//#define		configUse_usart_queue_rev

//#define	    configUse_usart_dma

//#define     configUse_IqsInt

//#define	    configUse_fct
#ifdef      configUse_fct
    #ifndef     configUse_flex_button
    #define	    configUse_flex_button
    #endif
#else
    #ifdef      configUse_flex_button
    #undef	    configUse_flex_button
    #endif
#endif

#define DATA_PACK_LENS  ( 10 )

//#define	configUse_SEGGER_RTT
#ifdef	configUse_SEGGER_RTT
#include "..\..\RTT\RTT\SEGGER_RTT.h"
#include "..\..\RTT\RTT\SEGGER_RTT_Conf.h"
#endif

#define configUse_freq_72M      0
#define configUse_freq_96M      1
#define configUse_freq_168M     2
#define configUse_freq          configUse_freq_72M

#if( configUse_freq == configUse_freq_72M )
#define TICK_MS 72000UL
#define TICK_US 72U
#define TIM15_PRESCALER 71
#define UGEE_DELAY_1US_NEED_CLOCK  71
#elif( configUse_freq == configUse_freq_96M )
#define TICK_MS 96000UL
#define TICK_US 96U
#define UGEE_DELAY_1US_NEED_CLOCK  95
#define TIM15_PRESCALER 95
#elif( configUse_freq == configUse_freq_168M )
#define TICK_MS 168000UL
#define TICK_US 168U
#define UGEE_DELAY_1US_NEED_CLOCK  167
#define TIM15_PRESCALER 167
#endif

#define END_FLAG_VALUE          0x12345678        

#define UG_NAME_MAX 12
#define UG_ALIGN_SIZE 4

typedef struct
{
    uint32_t dfu_flag;

    uint32_t channel;               // 数据通道

    uint32_t android_support;       // 安卓支持
    uint32_t android_direction;     // 安卓板方向 
                                    // 0 - 默认
                                    // 1 - 0°
                                    // 2 - 90°
                                    // 3 - 180°
                                    // 4 - 270°
    
    uint32_t footled_duty;          // 脚灯亮度PWM DUTY  

    uint32_t trackpad_duty;          // 滚轮灯DUTY

    uint32_t trackpad_mode;          // 触控板模式
    
    uint32_t rotate_mode;

    uint32_t android_para;

    uint32_t save;
    
    uint32_t crc;              // 结束标志
    
} system_config_t;
/*
#define SOFT_PWM_FREQ_NUM   1000
typedef struct
{
    uint16_t freq_threshold;
    uint16_t pwm_cnt;
    uint16_t pwm_value;
}soft_pwm_def;
*/
typedef union
{
    struct
    {
        uint8_t swap;
        uint8_t inverse_x;
        uint8_t inverse_y;
    }android_uc;
    uint32_t android_ul;
}android_control;


extern android_control android_handle;
extern const android_control android_procTab[];
//extern system_config_t g_sys_cfg;

extern void config_dfu_flag_set(uint8_t flag);
extern void config_channel_set(uint8_t channel, uint8_t save);
extern uint8_t config_channel_get(void);
extern void config_android_direction_set(uint8_t direction, uint8_t save);
extern uint8_t config_android_direction_get(void);
extern void config_android_support_set(uint8_t support, uint8_t save);
extern uint8_t config_android_support_get(void);
extern void config_footled_duty_set(uint8_t duty);
extern uint32_t config_footled_duty_get(void);
extern uint32_t config_track_pad_duty_get(void);
extern uint32_t config_track_pad_mode_get(void);
extern void config_init(void);
extern void config_led_foot_duty_set(void);
extern void soft_pwm_handler(void);
extern void track_pad_duty_set(uint16_t duty);
extern void track_pad_duty_set(uint16_t duty);
extern void config_track_pad_duty_set(uint16_t duty);
extern void config_track_pad_mode_set(uint8_t mode);
void config_led_track_pad_duty_set(void);
void config_android_handle_set(uint8_t direction, uint8_t save);
void config_android_handle_get(void);
void soft_reset(void);
void config_store(void);
uint8_t config_get_rotate_mode(void);
void config_trackpad_rotate_set(uint8_t usbRev,uint8_t save);

#endif
