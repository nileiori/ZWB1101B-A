#include <stdio.h>
#include <string.h>
#include "gd32f3x0.h"
#include "scan.h"
#include "digitizer_comm.h"
#include "xy_calibration.h"
#include "pressure.h"
#include "xy_antana.h"
#include "pwm.h"
#include "adc.h"
#include "usb.h"
#include "config.h"
#include "gpio.h"
#include "timer.h"
#include "system.h"


ADC_DELAY_STRUCT gADCDelay;

TABLET_STATUS_TYPE gb_hbStatus;

SCAN_ADC_VALUE_STRUCT gScanCurZ;

ANTANA_DATA_STRUCT gXAntADCVal[XY_ANT_ADC_CACHE_SIZE];
ANTANA_DATA_STRUCT gYAntADCVal[XY_ANT_ADC_CACHE_SIZE];

def_pos_calc pos_calc; // [CIS-L points][CIS-R points]
def_report_pos report_pos;

scan_t g_scan;

static void scan_delay_adjust(uint16_t adc_value, uint8_t *last_delay)
#if ( configUse_freq == configUse_freq_96M )
{
    if (adc_value > (DELAY_LEVEL + ADC_VOLTAGE_DIFF))
    {
        if (*last_delay < 0xff)
        {
            *last_delay = (*last_delay) + 1;
        }
    }
    else if (adc_value < (DELAY_LEVEL - ADC_VOLTAGE_DIFF))
    {
        if (*last_delay > 0)
        {
            *last_delay = (*last_delay) - 1;
        }
    }
}
#else
{
    uint8_t delayUs = *last_delay;

    if(adc_value > (DELAY_LEVEL + ADC_VOLTAGE_DIFF))
    {
        if(adc_value > ADC_VOLTAGE_HIGH)
        {
            delayUs += 10;
        }
        else
        {
            delayUs += 2;
        }

        if(delayUs > COORDINATE_DELAY)
        {
            delayUs = COORDINATE_DELAY;
        }
    }
    else if(adc_value < (DELAY_LEVEL - ADC_VOLTAGE_DIFF))
    {
        if(delayUs != 0)
        {
            if(adc_value < ADC_VOLTAGE_DIFF)
            {
                if(delayUs > 10)
                {
                    delayUs -= 10;
                }
                else
                {
                    delayUs -= 2;
                }
            }
            else
            {
                delayUs -= 2;
            }
        }
    }

    *last_delay = delayUs;

}
#endif


void scan_calculate(void)
{
    uint8_t channel;

    pressure_calculate(g_scan.center_freq_idx);

    get_Xcoordinate(g_scan.center_x_ant);
    get_Ycoordinate(g_scan.center_y_ant);

    //SEGGER_RTT_printf(0, "1. x=%d,y=%d\r\n", gb_hbStatus.lastPos.x, gb_hbStatus.lastPos.y);

#if ((calibration == 1) && (Coordinate_mode != 9))
    calibration_X();
    calibration_Y();
#endif

    channel = config_channel_get();

#if (Coordinate_mode == 0)
    {
        gb_hbStatus.curPos.x = pos_calc.X;
        gb_hbStatus.curPos.y = pos_calc.Y;
        if( channel != REPORT_CHANNEL_USERDEF )
            Mapping();
        else
            Inverter();
        gb_hbStatus.reportBuf[0] = gb_hbStatus.curPos.x;        //X-coord LSB
        gb_hbStatus.reportBuf[1] = (gb_hbStatus.curPos.x >> 8); //X-coord MSB
        gb_hbStatus.reportBuf[2] = gb_hbStatus.curPos.y;        //Y-coord LSB
        gb_hbStatus.reportBuf[3] = (gb_hbStatus.curPos.y >> 8); //Y-coord MSB
    }
#elif (Coordinate_mode == 1)
    {
        Smooth();
        gb_hbStatus.curPos.x = report_pos.X;
        gb_hbStatus.curPos.y = report_pos.Y;
        if( channel != REPORT_CHANNEL_USERDEF )
            Mapping();
        else
            Inverter();
        gb_hbStatus.reportBuf[0] = gb_hbStatus.curPos.x;        //X-coord LSB
        gb_hbStatus.reportBuf[1] = (gb_hbStatus.curPos.x >> 8); //X-coord MSB
        gb_hbStatus.reportBuf[2] = gb_hbStatus.curPos.y;        //Y-coord LSB
        gb_hbStatus.reportBuf[3] = (gb_hbStatus.curPos.y >> 8); //Y-coord MSB
    }
#elif (Coordinate_mode == 3)
    {
        Smooth();
        gb_hbStatus.curPos.x = report_pos.X;
        gb_hbStatus.curPos.y = report_pos.Y;
        gb_hbStatus.reportBuf[0] = gb_hbStatus.curPos.x;        //X-coord LSB
        gb_hbStatus.reportBuf[1] = (gb_hbStatus.curPos.x >> 8); //X-coord MSB
        gb_hbStatus.reportBuf[2] = gb_hbStatus.curPos.y;        //Y-coord LSB
        gb_hbStatus.reportBuf[3] = (gb_hbStatus.curPos.y >> 8); //Y-coord MSB
    }
#elif(Coordinate_mode == 9)
    {
        Smooth();
        gb_hbStatus.curPos.x = report_pos.X;
        gb_hbStatus.curPos.y = report_pos.Y;

        gb_hbStatus.reportBuf[0] = gb_hbStatus.curPos.x;        //X-coord LSB
        gb_hbStatus.reportBuf[1] = (gb_hbStatus.curPos.x >> 8); //X-coord MSB
        gb_hbStatus.reportBuf[2] = gb_hbStatus.curPos.y;        //Y-coord LSB
        gb_hbStatus.reportBuf[3] = (gb_hbStatus.curPos.y >> 8); //Y-coord MSB
    }
#else
    {
        Smooth();
        DeterminePenArea();
        Offset();

        if (channel != REPORT_CHANNEL_USERDEF )
        {
            Mapping();
        }
        else
        {
            Inverter();
        }


        Tilt(g_scan.center_x_ant, g_scan.center_y_ant);

        if(Jitter_Setting == 0)
        {
            Jitter();
        }
        else
        {
            AutoJitter();
        }

    }
#endif
}


void scan_reset(void)
{
    memset(&g_scan, 0, sizeof(g_scan));
    memset(&gADCDelay, 0, sizeof(gADCDelay));
    memset(&report_pos, 0, sizeof(report_pos));

    gb_hbStatus.pen_flag = 0;

    x_deg = 0;
    y_deg = 0;

    gb_hbStatus.penKeyValue =  PEN_KEY_KK_OTHER;

    gb_hbStatus.curPos.press = 0;


    pos_calc.X = 0;
    pos_calc.Y = 0;
    pos_calc.Z = 0;

    g_scan.chrg_ant = 1;
    g_scan.scanAdcConverCount = ADC_SAMPLING_COUNT;

}

static void pre_scan_y_recv(void)
{
    static scan_result_t result = {0};
    uint8_t adc_samp_cnt;
    uint32_t adc_sum;

    //adc_regular_channel_config(0U, ADC_CHANNEL_11, ADC_SAMPLETIME_7POINT5);

    antenna_open(0, g_scan.chrg_ant);
    ugee_adc_delay(g_scan.y_delay);
    g_scan.scanAdcDelayCount  = g_scan.y_delay;
    adc_samp_cnt = g_scan.scanAdcConverCount - g_scan.scanAdcDelayCount;
    adc_sampling_start(adc_samp_cnt);

    adc_sum = adc_get_sum();
    if (adc_sum > ADC_SUM_BASE_VALUE)
    {
        if(adc_sum > result.adc_sum)
        {
            result.adc_sum = adc_sum;
            result.adc_last = adc_get_last_value();
            result.antenna = g_scan.chrg_ant;
            result.freq_index = g_scan.pre_freq_idx;
        }
    }
    //g_scan.chrg_ant = 10;
    if (++g_scan.chrg_ant >= ALL_Y_ANTANA_NUM)
    {
        if (g_scan.pre_freq_idx == (PWM_PRE_PERIOD_TAB_SIZE - 1))
        {
            if (result.adc_last > ADC_LAST_BASE_VALUE)
            {
                g_scan.center_pre_freq_idx = result.freq_index;
                g_scan.center_y_ant = result.antenna;
                g_scan.chrg_ant = 1;

                g_scan.scan_mode = PRESCAN_MODE_X;

                gb_hbStatus.penFirstSatus = 0;
            }
            else
            {
                scan_reset();
            }

            memset(&result, 0, sizeof(result));
        }
        else
        {
            g_scan.pre_freq_idx++;
            g_scan.chrg_ant = 1;
        }
    }
    g_scan.adcDealEndFlag = 1;
    //g_scan.pre_freq_idx = 4;

}

static void pre_scan_x_recv(void)
{
    static scan_result_t result = {0};
    uint8_t adc_samp_cnt;
    uint32_t adc_sum;

    //adc_regular_channel_config(0U, ADC_CHANNEL_10, ADC_SAMPLETIME_7POINT5);

    antenna_open(g_scan.chrg_ant, 0);
    ugee_adc_delay(g_scan.y_delay);
    g_scan.scanAdcDelayCount  = g_scan.y_delay;
    adc_samp_cnt = g_scan.scanAdcConverCount - g_scan.scanAdcDelayCount;
    adc_sampling_start(adc_samp_cnt);

    adc_sum = adc_get_sum();
    if (adc_sum > ADC_SUM_BASE_VALUE)
    {
        if(adc_sum > result.adc_sum)
        {
            result.adc_sum = adc_sum;
            result.adc_last = adc_get_last_value();
            result.antenna = g_scan.chrg_ant;
        }
    }

    if (++g_scan.chrg_ant >= ALL_X_ANTANA_NUM)
    {
        if(result.adc_last > ADC_LAST_BASE_VALUE)
        {
            g_scan.center_x_ant = result.antenna;
            g_scan.center_freq_idx = pwm_period_idx_get_by_pre_idx(g_scan.center_pre_freq_idx);
            g_scan.freq_idx = g_scan.center_freq_idx - PRESS_SCAN_ANT_BOTH_SIDES;
            if (g_scan.freq_idx & 0x80)
                g_scan.freq_idx = 0;

            memset(&gScanCurZ, 0, sizeof(SCAN_ADC_VALUE_STRUCT));
            memset(&report_pos, 0, sizeof(def_report_pos));

            g_scan.scan_mode = PRESS_SCAN_MODE;
            gb_hbStatus.pen_flag = 0;

        }
        else
        {
            scan_reset();
        }

        memset(&result, 0, sizeof(result));
    }
    g_scan.adcDealEndFlag = 1;
}

static void scan_pressure_recv(void)
{
    static uint8_t scan_times = 0;
    uint8_t adc_samp_cnt;
    //adc_regular_channel_config(0U, ADC_CHANNEL_11, ADC_SAMPLETIME_7POINT5);

    antenna_open(0, g_scan.center_y_ant);
    ugee_adc_delay(2 * g_scan.z_delay);
    g_scan.scanAdcDelayCount  = g_scan.z_delay;
    adc_samp_cnt = g_scan.scanAdcConverCount - g_scan.scanAdcDelayCount;
    adc_sampling_start(adc_samp_cnt);

    gScanCurZ.adcValue[scan_times] = adc_get_sum();
    gScanCurZ.adcLastValue[scan_times] = adc_get_last_value();
    gScanCurZ.freq_index[scan_times] = g_scan.freq_idx;

    if (++scan_times >= PRESS_SCAN_ANT_NUM)
    {
        uint8_t max_index = 0;
        uint8_t i;

        scan_times = 0;

        for (i = 1; i < PRESS_SCAN_ANT_NUM; i++)
        {
            if (gScanCurZ.adcValue[i] > gScanCurZ.adcValue[max_index])
            {
                max_index = i;
            }
        }

        if (gScanCurZ.adcLastValue[max_index] > NORMALSCAN_ADCVALUE_PENIN_HOLD)
        {
            gb_hbStatus.penInOutSatus = 1;
        }
        else if (gScanCurZ.adcLastValue[max_index]  < NORMALSCAN_ADCVALUE_PENOUT_HOLD)
        {
            gb_hbStatus.penInOutSatus = 0;
        }

        if(max_index == (PRESS_SCAN_ANT_NUM - 1))
        {
            g_scan.freq_idx = 20;
        }

        g_scan.center_freq_idx = gScanCurZ.freq_index[max_index];

        gScanCurZ.adcMaxValue = gScanCurZ.adcValue[max_index];
        gScanCurZ.adcMaxAdcIndex = max_index;

        if (g_scan.center_freq_idx >= PWM_PERIOD_TAB_SIZE)
        {
            scan_reset();
        }
        else
        {
            memset(&gXAntADCVal, 0, sizeof(gXAntADCVal));

            scan_delay_adjust(gScanCurZ.adcLastValue[max_index], &g_scan.z_delay);

            g_scan.scan_mode = XPOS_SCAN_MODE;
        }
    }
    else
    {
        if(scan_times == (PRESS_SCAN_ANT_NUM - 1))
        {
            g_scan.freq_idx = 20;
        }
        else
        {
            if (g_scan.freq_idx < PWM_PERIOD_TAB_SIZE)
                g_scan.freq_idx++;
        }
    }
    g_scan.adcDealEndFlag = 1;
}

static void scan_x_recv(void)
{
    static uint8_t scan_times = 0;
    uint8_t recv_ant;
    uint8_t adc_samp_cnt;

    //adc_regular_channel_config(0U, ADC_CHANNEL_10, ADC_SAMPLETIME_7POINT5);

    if (scan_times == 0)
        recv_ant = scan_times + g_scan.center_x_ant - COORDINATE_SCAN_ANT_BOTH_SIDES - 1;
    else if(scan_times == 6)
        recv_ant = scan_times + g_scan.center_x_ant - COORDINATE_SCAN_ANT_BOTH_SIDES + 1;
    else
        recv_ant = scan_times + g_scan.center_x_ant - COORDINATE_SCAN_ANT_BOTH_SIDES;

    antenna_open(recv_ant, 0);
    ugee_adc_delay(2 * g_scan.x_delay);
    g_scan.scanAdcDelayCount  = g_scan.x_delay;
    adc_samp_cnt = g_scan.scanAdcConverCount - g_scan.scanAdcDelayCount;
    adc_sampling_start(adc_samp_cnt);

    gXAntADCVal[scan_times].value = adc_get_sum();
    gXAntADCVal[scan_times].last_value = adc_get_last_value();
    gXAntADCVal[scan_times].antana = recv_ant;

    if (++scan_times >= COORDINATE_SCAN_ANT_NUM)
    {
        uint8_t  max_index = 0;
        uint8_t  i;

        scan_times = 0;

        for (i = 1; i < COORDINATE_SCAN_ANT_NUM; i++)
        {
            if (gXAntADCVal[i].value > gXAntADCVal[max_index].value)
            {
                max_index = i;
            }
        }

        if (gXAntADCVal[max_index].last_value < NORMALSCAN_ADCVALUE_PENOUT_HOLD)
        {
            scan_reset();
            gb_hbStatus.penInOutSatus = 0;
        }
        else
        {
            g_scan.center_x_ant = gXAntADCVal[max_index].antana;

            gXAntADCVal[9].value = gXAntADCVal[max_index].value;
            gXAntADCVal[9].num = max_index;
            gXAntADCVal[9].last_value = gXAntADCVal[max_index].last_value;

            memset(&gYAntADCVal, 0, sizeof(gYAntADCVal));

            scan_delay_adjust(gXAntADCVal[9].last_value, &g_scan.x_delay);

            g_scan.scan_mode = YPOS_SCAN_MODE;
        }
    }
    g_scan.adcDealEndFlag = 1;
}

static void scan_y_recv(void)
{
    static uint8_t scan_times = 0;
    uint8_t recv_ant;
    uint8_t adc_samp_cnt;

    //adc_regular_channel_config(0U, ADC_CHANNEL_11, ADC_SAMPLETIME_7POINT5);

    if (scan_times == 0)
        recv_ant = scan_times + g_scan.center_y_ant - COORDINATE_SCAN_ANT_BOTH_SIDES - 1;
    else if (scan_times == 6)
        recv_ant = scan_times + g_scan.center_y_ant - COORDINATE_SCAN_ANT_BOTH_SIDES + 1;
    else
        recv_ant = scan_times + g_scan.center_y_ant - COORDINATE_SCAN_ANT_BOTH_SIDES;

    antenna_open(0, recv_ant);
    ugee_adc_delay(2 * g_scan.y_delay);
    g_scan.scanAdcDelayCount  = g_scan.y_delay;
    adc_samp_cnt = g_scan.scanAdcConverCount - g_scan.scanAdcDelayCount;
    adc_sampling_start(adc_samp_cnt);

    gYAntADCVal[scan_times].value = adc_get_sum();
    gYAntADCVal[scan_times].last_value = adc_get_last_value();
    gYAntADCVal[scan_times].antana = recv_ant;

    if (++scan_times >= COORDINATE_SCAN_ANT_NUM)
    {
        uint8_t  max_index = 0;
        uint8_t  i;

        scan_times = 0;

        for (i = 1; i < COORDINATE_SCAN_ANT_NUM; i++)
        {
            if (gYAntADCVal[i].value > gYAntADCVal[max_index].value)
            {
                max_index = i;
            }
        }

        if (gYAntADCVal[max_index].last_value < NORMALSCAN_ADCVALUE_PENOUT_HOLD)
        {
            scan_reset();
            gb_hbStatus.penInOutSatus = 0;
        }
        else
        {
            g_scan.center_y_ant = gYAntADCVal[max_index].antana;

            gYAntADCVal[9].value = gYAntADCVal[max_index].value;
            gYAntADCVal[9].num = max_index;
            gYAntADCVal[9].last_value = gYAntADCVal[max_index].last_value;

            g_scan.scan_mode = MAIN_CALCULATE;
            scan_main_calculate();
            
            return;
        }
    }
    g_scan.adcDealEndFlag = 1;
}

void scan_main_calculate(void)
{
    if(g_scan.scan_mode == MAIN_CALCULATE)
    {
        scan_calculate();
        gb_hbStatus.pen_flag = 2;

        g_scan.freq_idx = g_scan.center_freq_idx - PRESS_SCAN_ANT_BOTH_SIDES;
        if (g_scan.freq_idx & 0x80)
            g_scan.freq_idx = 0;

        scan_delay_adjust(gYAntADCVal[9].last_value, &g_scan.y_delay);

        g_scan.scan_mode = PRESS_SCAN_MODE;

        g_scan.adcDealEndFlag = 1;
    }
}

void scan_recv(void)
{
    switch (g_scan.scan_mode)
    {
    case PRESCAN_MODE_Y:
        pre_scan_y_recv();
        break;

    case PRESCAN_MODE_X:
        pre_scan_x_recv();
        break;

    case PRESS_SCAN_MODE:
        scan_pressure_recv();
        break;

    case XPOS_SCAN_MODE:
        scan_x_recv();
        break;

    case YPOS_SCAN_MODE:
        scan_y_recv();
        break;
    }
}

void scan_chrg(uint8_t x_ant, uint8_t y_ant)
{
    antenna_open(x_ant, y_ant);

    CHARGE_TX_ENABLE;

    timer_enable(TIMER0);
    timer_enable(TIMER15);
}

void scan_start(void)
{
    g_scan.scan_PWM_Cnt = 0;
    switch (g_scan.scan_mode)
    {
    case PRESCAN_MODE_Y:
        pwm_period_set_by_pre_idx(g_scan.pre_freq_idx);
        scan_chrg(0, g_scan.chrg_ant);
        break;

    case PRESCAN_MODE_X:
        pwm_period_set_by_pre_idx(g_scan.center_pre_freq_idx);
        scan_chrg(g_scan.chrg_ant, 0);
        break;

    case PRESS_SCAN_MODE:
        pwm_period_set_by_idx(g_scan.freq_idx);
        scan_chrg(g_scan.center_x_ant, g_scan.center_y_ant);
        break;

    case XPOS_SCAN_MODE:
        pwm_period_set_by_idx(g_scan.center_freq_idx);
        scan_chrg(g_scan.center_x_ant, g_scan.center_y_ant);
        break;

    case YPOS_SCAN_MODE:
        pwm_period_set_by_idx(g_scan.center_freq_idx);
        scan_chrg(g_scan.center_x_ant, g_scan.center_y_ant);
        break;
    }
}

uint8_t  scan_mode_read(void)
{
    return g_scan.scan_mode;
}

uint8_t  read_hand_pencil_status(void)
{
    return g_scan.handPencilStatus;
}

void  set_hand_pencil_status(uint8_t value)
{
    g_scan.handPencilStatus = value;
}

