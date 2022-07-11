#ifndef __SYSTEM_H__
#define __SYSTEM_H__


#include "config.h"



#ifdef __cplusplus
extern "C" {
#endif

#define NOTICE_BLE_SLEEP                   pin_out_off(IRQ_PIN_IDX)
#define NOTICE_BLE_WAKEUP                  pin_out_on(IRQ_PIN_IDX)

#define NOTICE_BLE_USART_MODE              pin_out_off(READY_PIN_IDX)
#define NOTICE_BLE_USB_MODE                pin_out_on(READY_PIN_IDX)

#define SLEEP_COUNT_1MINUTES    60
#define SLEEP_COUNT_5MINUTES    SLEEP_COUNT_1MINUTES * 5
#define SLEEP_COUNT_30MINUTES   SLEEP_COUNT_1MINUTES * 30

enum sys_work_mode
{
    e_work_mode_usb = 0,
    e_work_mode_ble

};

typedef enum{MAC_INVALID = 0,MAC_VALID = !MAC_INVALID}MAC_Status;

typedef struct
{
    enum sys_work_mode work_mode;    
    uint8_t ble_connect;
    uint8_t chrgSta;
    uint8_t ble_batt;
    uint8_t ble_match_en;    
    struct
    {
        bool        status;
        uint8_t     addr[6];
    }dongle_mac;
    struct
    {
        bool        status;
        uint8_t     addr[6];
    }ble_mac;
    struct
    {
        bool        status;
        uint8_t     ble_fw[20];
    }fw;
    struct
    {
        uint8_t        forbid:1;
        uint8_t        flag:2;
        uint8_t        reset:1;
    }pdCtrl;
    union
    {       
        uint8_t     ucTime[4];
        uint32_t    usTime;
    }sleep;
    uint8_t protocolVer[4];
    uint8_t dev_serialNo[11];
    char    ble_name[14];
    uint8_t led_switch:1;
    uint8_t led_light:7;
    uint8_t led_light_get;
    volatile uint32_t enterIntoPdTimer;
}system_status_t;


extern system_status_t sys_status;
void system_init(void);
EventStatus system_judge_curr_mode(void);
uint8_t get_report_mode(void);
void set_report_mode(enum sys_work_mode mode);
#ifdef  configUse_wireless
void system_halt(void);
void system_clear_sleep_count(void);
EventStatus system_1s_task(void);
uint16_t system_get_sleep_time(void);
uint8_t system_get_ble_batt(void);


#endif

#ifdef __cplusplus
}
#endif



#endif



