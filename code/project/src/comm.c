#include "gd32f3x0.h"
#include "usb.h"
#include "led.h"
#include "scan.h"
#include "config.h"
#include "system.h"
#include "usbd_std.h"
#include "pressure.h"
#include <string.h>
#include "gpio.h"
#include "key.h"
#include "timer.h"
#include "usart.h"
#include "app_usart.h"
#include "system.h"
#include "taskschedule.h"

extern unsigned int test_mode;
extern unsigned int zih_freq, test_z;
extern unsigned int AutoJitter_level, AutoJitter_range;
extern system_status_t sys_status;
//static uint8_t taskPending = 0;
static uint8_t f_data[20];

typedef enum
{
    e_pen_model_other = 0,
    e_pen_model_PE1,
    e_pen_model_PE2,
    e_pen_model_PA5 = 15,
    e_pen_model_PA6 = 16,
    e_pen_model_PH1 = 21,
    e_pen_model_PH2 = 22,
    e_pen_model_P01 = 31,
    e_pen_model_P02 = 32,
} e_pen_type;

void hid_report(void)
{
    static uint8_t pressZeroCnt = 0;
    uint8_t channel;
    uint8_t android_support;
    uint8_t data[20] = {0};

    usb_rx_proc();

    channel = config_channel_get();
    android_support = config_android_support_get();

    if ((gb_hbStatus.penFirstSatus == 3) && (gb_hbStatus.penInOutSatus == 0))
    {
//        taskPending = 0;
//        ResumeTimerTask(MAX_TIMETASK);
        led_off();
        if(IS_KEY_PRESSED)
            LED_ON;
        //LED_OFF;
        pressZeroCnt = 0;
        set_hand_pencil_status(e_pen_status_idle);
        switch(channel)
        {
        case REPORT_CHANNEL_MOUSE:
            data[0] = 0x09;
            gb_hbStatus.buttonStatus = 0x00;  //out range
            data[1] = gb_hbStatus.buttonStatus;
            data[2] = gb_hbStatus.lastPos.x;
            data[3] = gb_hbStatus.lastPos.x >> 8;
            data[4] = gb_hbStatus.lastPos.y;
            data[5] = gb_hbStatus.lastPos.y >> 8;
            
            if(e_work_mode_usb == get_report_mode()) 
            {      
                if(USBD_OK != usb_tx(data, 8))  
                {
                    memcpy(f_data,data,20);
                    SetTimerTask(TIME_COMM_CLR_TILT, 1);
                }
            }                                         
            else                                      
            {                                         
                REPORT_DATA_FRAME_FORM(data, 8);    
            } 
            gb_hbStatus.penFirstSatus = 0;

            break;

        case REPORT_CHANNEL_TABLET:
            data[0] = 0x07;
            gb_hbStatus.buttonStatus = 0xC0;
            data[1] = gb_hbStatus.buttonStatus;

            if (android_support && (check_sys == 2))
            {
                if(android_handle.android_uc.swap)
                {
                    data[2] = gb_hbStatus.lastPos.y;
                    data[3] = gb_hbStatus.lastPos.y >> 8;
                    data[4] = gb_hbStatus.lastPos.x;
                    data[5] = gb_hbStatus.lastPos.x >> 8;
                }
                else
                {
                    data[2] = gb_hbStatus.lastPos.x;
                    data[3] = gb_hbStatus.lastPos.x >> 8;
                    data[4] = gb_hbStatus.lastPos.y;
                    data[5] = gb_hbStatus.lastPos.y >> 8;
                }
            }
            else
            {
                data[2] = gb_hbStatus.lastPos.x;
                data[3] = gb_hbStatus.lastPos.x >> 8;
                data[4] = gb_hbStatus.lastPos.y;
                data[5] = gb_hbStatus.lastPos.y >> 8;
            }

            if(e_work_mode_usb == get_report_mode()) 
            {                                         
                if(USBD_OK != usb_tx(data, 10))  
                {
                    memcpy(f_data,data,20);
                    SetTimerTask(TIME_COMM_CLR_TILT, 1);
                }                    
            }                                         
            else                                      
            {                                         
                REPORT_DATA_FRAME_FORM(data, 10);    
            } 
            gb_hbStatus.penFirstSatus = 0;

            break;

        case REPORT_CHANNEL_USERDEF:
            data[0] = 0x02;
            gb_hbStatus.buttonStatus = 0xC0;
            data[1] = gb_hbStatus.buttonStatus;
            data[2] = gb_hbStatus.lastPos.x;
            data[3] = gb_hbStatus.lastPos.x >> 8;
            data[4] = gb_hbStatus.lastPos.y;
            data[5] = gb_hbStatus.lastPos.y >> 8;

            if(e_work_mode_usb == get_report_mode()) 
            {                                         
                if(USBD_OK != usb_tx(data, 12))  
                {
                    memcpy(f_data,data,20);
                    SetTimerTask(TIME_COMM_CLR_TILT, 1);
                }                   
            }                                         
            else                                      
            {                                         
                REPORT_DATA_FRAME_FORM(data, DATA_PACK_LENS);    
            } 
            gb_hbStatus.penFirstSatus = 0;
            //SetTimerTask(TIME_COMM_CLR_TILT, 1);
            break;

        }

        gb_hbStatus.lastPos.x = 0;
        gb_hbStatus.lastPos.y = 0;
        gb_hbStatus.curPos.press = 0;
        pos_calc.Z = 0;
        report_pos.Z = 0;
        pos_calc.X = 0;
        report_pos.X = 0;
        pos_calc.Y = 0;
        report_pos.Y = 0;
        report_pos.X_sum = 0;
        report_pos.Y_sum = 0;
        report_pos.Z_sum = 0;
        report_pos.Counter = 0;
        report_pos.Counter2 = 0;
        sw1_flag = 0;
    }
    else if ((gb_hbStatus.penKeyValue  != 0xff)  &&  gb_hbStatus.pen_flag >= 2 && gb_hbStatus.penInOutSatus)
    {
//        if(0 == taskPending)
//        {
//            taskPending = 1;
//            SuspendTimerTask(MAX_TIMETASK);
//            ResumeTimerTask(TIME_KEY);
//        }
        gb_hbStatus.pen_flag = 0;

        switch(channel)
        {
        case REPORT_CHANNEL_MOUSE:
            if(gb_hbStatus.curPos.press == 0)
            {
                set_hand_pencil_status(e_pen_status_floating);
                gb_hbStatus.buttonStatus = 0x00 + (gb_hbStatus.penKeyValue << 1);  //pen tip suspend and button data combined.
                if(gb_hbStatus.buttonStatus == 0x00)
                {
                    LED_SPARK_CONTINUOUS(e_floating, 30); //continuous flashing
                }
                else
                {
                    LED_LIGHT_CONTINUOUS(e_touch);
                }
            }
            else
            {

                LED_LIGHT_CONTINUOUS(e_touch); //Lighting Form
                set_hand_pencil_status(e_pen_status_touch);
                if (gb_hbStatus.penFirstSatus == 0) //the first inrange record must have NO pressure. The following record can have inrange value and pressure.
                {
                    gb_hbStatus.buttonStatus = 0x00 + (gb_hbStatus.penKeyValue << 1);
                    gb_hbStatus.curPos.press = 0;
                }
                else
                {
                    gb_hbStatus.buttonStatus = 0x01 + (gb_hbStatus.penKeyValue << 1);  //pen tip touching the tablet and button data
                }
            }

            data[0] = 0x09;
            data[1] = gb_hbStatus.buttonStatus;
            data[2] = gb_hbStatus.reportBuf[0];
            data[3] = gb_hbStatus.reportBuf[1];
            data[4] = gb_hbStatus.reportBuf[2];
            data[5] = gb_hbStatus.reportBuf[3];
            data[6] = gb_hbStatus.curPos.press;
            data[7] = gb_hbStatus.curPos.press >> 8;

            if(e_work_mode_usb == get_report_mode()) 
            {                                         
                usb_tx(data, 8);                    
            }                                         
            else                                      
            {                                         
                REPORT_DATA_FRAME_FORM(data, 8);    
            } 
            break;

        case REPORT_CHANNEL_TABLET:
            if(gb_hbStatus.curPos.press == 0)
            {
                set_hand_pencil_status(e_pen_status_floating);
                gb_hbStatus.buttonStatus = 0xA0 + (gb_hbStatus.penKeyValue << 1);  //pen tip suspend and button data combined.
                pressZeroCnt++;
                if(pressZeroCnt > 5)pressZeroCnt = 5;
                if(gb_hbStatus.buttonStatus == 0xa0)
                {
                    LED_SPARK_CONTINUOUS(e_floating, 30); //continuous flashing
                }
                else
                {
                    LED_LIGHT_CONTINUOUS(e_touch);
                }
            }
            else
            {
                set_hand_pencil_status(e_pen_status_touch);
                LED_LIGHT_CONTINUOUS(e_touch); //Lighting Form
                if(gb_hbStatus.penFirstSatus == 0) //the first inrange record must have NO pressure. The following record can have inrange value and pressure.
                {
                    gb_hbStatus.buttonStatus = 0xA0 + (gb_hbStatus.penKeyValue << 1);
                    gb_hbStatus.curPos.press = 0;
                    pressZeroCnt++;
                    if(pressZeroCnt > 10)pressZeroCnt = 10;
                }
                else
                {
                    if(pressZeroCnt)
                    {
                        pressZeroCnt = 0;
                        gb_hbStatus.curPos.press = 0;
                        gb_hbStatus.buttonStatus = 0xA0 + (gb_hbStatus.penKeyValue << 1);
                    }
                    else
                        gb_hbStatus.buttonStatus = 0xA1 + (gb_hbStatus.penKeyValue << 1);  //pen tip touching the tablet and button data
                }
            }

            if (gb_hbStatus.buttonStatus == 0xA9)
            {
                gb_hbStatus.buttonStatus = 0xA4;
            }

            data[0] = 0x07;
            data[1] = gb_hbStatus.buttonStatus;

            if (android_support && (check_sys == 2))
            {
                if(android_handle.android_uc.swap)
                {
                    data[2] = gb_hbStatus.reportBuf[2];
                    data[3] = gb_hbStatus.reportBuf[3];
                    data[4] = gb_hbStatus.reportBuf[0];
                    data[5] = gb_hbStatus.reportBuf[1];
                }
                else
                {
                    data[2] = gb_hbStatus.reportBuf[0];
                    data[3] = gb_hbStatus.reportBuf[1];
                    data[4] = gb_hbStatus.reportBuf[2];
                    data[5] = gb_hbStatus.reportBuf[3];
                }

            }
            else
            {
                data[2] = gb_hbStatus.reportBuf[0];
                data[3] = gb_hbStatus.reportBuf[1];
                data[4] = gb_hbStatus.reportBuf[2];
                data[5] = gb_hbStatus.reportBuf[3];
            }

            data[6] = gb_hbStatus.curPos.press;
            data[7] = gb_hbStatus.curPos.press >> 8;

            data[8] = x_deg;
            data[9] = y_deg;

            if((data[2] == 0)&&(data[3] == 0)&&(data[4] == 0)&&(data[5] == 0))return;
            if(e_work_mode_usb == get_report_mode()) 
            {                                         
                usb_tx(data, 10);                    
            }                                         
            else                                      
            {                                         
                REPORT_DATA_FRAME_FORM(data, 10);    
            } 
            break;


        case REPORT_CHANNEL_USERDEF:
            if(gb_hbStatus.curPos.press == 0)
            {
                set_hand_pencil_status(e_pen_status_floating);
                gb_hbStatus.buttonStatus = 0xa0 + (gb_hbStatus.penKeyValue << 1);  //pen tip suspend and button data combined.
                if(gb_hbStatus.buttonStatus == 0xa0)
                {
                    LED_SPARK_CONTINUOUS(e_floating, 30); //continuous flashing
                }
                else
                {
                    LED_LIGHT_CONTINUOUS(e_touch);
                }
            }
            else
            {
                set_hand_pencil_status(e_pen_status_touch);
                LED_LIGHT_CONTINUOUS(e_touch); //Lighting Form
                if(gb_hbStatus.penFirstSatus == 0) //the first inrange record must have NO pressure. The following record can have inrange value and pressure.
                {
                    gb_hbStatus.buttonStatus = 0xa0 + (gb_hbStatus.penKeyValue << 1);
                    gb_hbStatus.curPos.press = 0;
                }
                else
                {
                    gb_hbStatus.buttonStatus = 0xa1 + (gb_hbStatus.penKeyValue << 1);  //pen tip touching the tablet and button data
                }
            }
            /*if(gb_hbStatus.curPos.press == 0)
            {
                set_hand_pencil_status(e_pen_status_floating);
                gb_hbStatus.buttonStatus = 0xA0 + (gb_hbStatus.penKeyValue << 1);  //pen tip suspend and button data combined.
                pressZeroCnt++;
                if(pressZeroCnt > 5)pressZeroCnt = 5;
                if(gb_hbStatus.buttonStatus == 0xa0)
                {
                    LED_SPARK_CONTINUOUS(e_floating, 30); //continuous flashing
                }
                else
                {
                    LED_LIGHT_CONTINUOUS(e_touch);
                }
            }
            else
            {
                set_hand_pencil_status(e_pen_status_touch);
                LED_LIGHT_CONTINUOUS(e_touch); //Lighting Form
                if(gb_hbStatus.penFirstSatus == 0) //the first inrange record must have NO pressure. The following record can have inrange value and pressure.
                {
                    gb_hbStatus.buttonStatus = 0xA0 + (gb_hbStatus.penKeyValue << 1);
                    gb_hbStatus.curPos.press = 0;
                    pressZeroCnt++;
                    if(pressZeroCnt > 10)pressZeroCnt = 10;
                }
                else
                {
                    if(pressZeroCnt)
                    {
                        pressZeroCnt = 0;
                        gb_hbStatus.curPos.press = 0;
                        gb_hbStatus.buttonStatus = 0xA0 + (gb_hbStatus.penKeyValue << 1);
                    }
                    else
                        gb_hbStatus.buttonStatus = 0xA1 + (gb_hbStatus.penKeyValue << 1);  //pen tip touching the tablet and button data
                }
            }*/
            data[0] = 0x02;
            data[1] = gb_hbStatus.buttonStatus;
            data[2] = gb_hbStatus.reportBuf[0];
            data[3] = gb_hbStatus.reportBuf[1];
            data[4] = gb_hbStatus.reportBuf[2];
            data[5] = gb_hbStatus.reportBuf[3];
            data[6] = gb_hbStatus.curPos.press;
            data[7] = gb_hbStatus.curPos.press >> 8;
            data[8] = x_deg;
            data[9] = y_deg;

            if(test_mode == 1)	//X_Energy
            {
                data[6] = gXAntADCVal[9].last_value;
                data[7] = gXAntADCVal[9].last_value >> 8;
                data[8] = g_scan.center_x_ant;
                data[9] = g_scan.center_y_ant;
            }
            else if(test_mode == 2)//Y_Energy
            {
                data[6] = gYAntADCVal[9].last_value;
                data[7] = gYAntADCVal[9].last_value >> 8;
                data[8] = g_scan.center_x_ant;
                data[9] = g_scan.center_y_ant;
            }
            else if(test_mode == 3)//P_Energy
            {
                data[6] = gScanCurZ.adcMaxValue;
                data[7] = gScanCurZ.adcMaxValue >> 8;
                data[8] = g_scan.center_x_ant;
                data[9] = g_scan.center_y_ant;
            }
            else if(test_mode == 4)//Ferq_mode
            {
                data[6] = test_z;
                data[7] = test_z >> 8;
                data[8] = g_scan.center_x_ant;
                data[9] = g_scan.center_y_ant;
            }
            else if(test_mode == 5)//Jitter_Energy
            {
                data[6] = (gXAntADCVal[9].last_value + gYAntADCVal[9].last_value) / 2;
                data[7] = (gXAntADCVal[9].last_value + gYAntADCVal[9].last_value) / 2 >> 8;
                data[8] = g_scan.center_x_ant;
                data[9] = g_scan.center_y_ant;
            }
            else if(test_mode == 6)//Jitter_mode
            {
                data[6] = AutoJitter_range;
                data[7] = AutoJitter_range >> 8;
                data[8] = g_scan.center_x_ant;
                data[9] = g_scan.center_y_ant;
            }
            else if(test_mode == 7)//Smooth_mode
            {
                data[6] = 0;
                data[7] = 0;
                data[8] = g_scan.center_x_ant;
                data[9] = g_scan.center_y_ant;
            }
            else if(test_mode == 8)//XTilt_mode
            {
                data[6] = RETiltX;
                data[7] = RETiltX >> 8;
                data[8] = x_deg;
                data[9] = y_deg;
            }
            else if(test_mode == 9)//YTilt_mode
            {
                data[6] = RETiltY;
                data[7] = RETiltY >> 8;
                data[8] = x_deg;
                data[9] = y_deg;
            }
            else if(test_mode == 10)
            {
                //memcpy(data+6,sys_status.ble_macAddr,6);
            }
//            else if(test_mode == 11)
//            {
//                data[6] = HOTKEY_NUM;
//                data[7] = 0;
//                data[8] = e_pen_model_PE1;
//                data[9] = 0;
//            }
//            else if(test_mode == 12)
//            {
//                data[6] = config_android_direction_get();
//            }
//            else if(test_mode == 13)
//            {
//                data[6] = sys_status.ble_batt;
//                data[7] = sys_status.ble_batt >> 8;
//                data[8] = x_deg;
//                data[9] = y_deg;
//            }
            else
            {
                data[6] = gb_hbStatus.curPos.press;
                data[7] = gb_hbStatus.curPos.press >> 8;
                data[8] = x_deg;
                data[9] = y_deg;
            }
            if((data[2] == 0)&&(data[3] == 0)&&(data[4] == 0)&&(data[5] == 0))return;
            if(e_work_mode_usb == get_report_mode()) 
            {                                         
                usb_tx(data, 12);                    
            }                                         
            else                                      
            {                                         
                REPORT_DATA_FRAME_FORM(data, DATA_PACK_LENS);    
            } 
            break;

        }

        gb_hbStatus.lastPos.x = (gb_hbStatus.reportBuf[1] << 8) | gb_hbStatus.reportBuf[0];
        gb_hbStatus.lastPos.y = (gb_hbStatus.reportBuf[3] << 8) | gb_hbStatus.reportBuf[2];
        gb_hbStatus.lastPos.press = gb_hbStatus.curPos.press;

        gb_hbStatus.penFirstSatus  = 3;

        gb_hbStatus.curPos.x = 0;
        gb_hbStatus.curPos.y = 0;
        gb_hbStatus.curPos.press = 0;

        memset(gb_hbStatus.reportBuf, 0x0, sizeof(gb_hbStatus.reportBuf));
    }
}

EventStatus comm_repeat_report(void)
{
    uint8_t channel = config_channel_get();

    switch(channel)
    {
    case REPORT_CHANNEL_MOUSE:
        if(USBD_OK == usb_tx(f_data, 8))
            return DISABLE;
        break;
    case REPORT_CHANNEL_TABLET:
        if(USBD_OK == usb_tx(f_data, 10))
            return DISABLE;
        break;
    case REPORT_CHANNEL_USERDEF:
        if(USBD_OK == usb_tx(f_data, 12))
            return DISABLE;
        break;
    }
    return ENABLE;
}



