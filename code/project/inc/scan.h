#ifndef __SCAN_H__
#define __SCAN_H__

#include "digitizer_comm.h"
#include "preferences.h"


#define SCAN_ANT_NUM            7

enum
{
    e_pen_status_idle = 0,
    e_pen_status_floating,
    e_pen_status_touch
};

typedef enum
{
    PRESCAN_MODE_Y,     //pre scan y
    PRESCAN_MODE_X,     //pre scan x
    PRESS_SCAN_MODE,    //pen pressure scan
    XPOS_SCAN_MODE,     //accurate X scan 
    YPOS_SCAN_MODE,     //accurate Y scan
    MAIN_CALCULATE
} scan_mode_e;

typedef enum
{
    PEN_KEY_K0_UP = 0,  //hang in the air
    //ELEC_PEN_KEY_K1_PRESS,       //press key
    PEN_KEY_K2_UP,      //down key hang in the air
    //ELEC_PEN_KEY_K3_PRESS,       //
    PEN_KEY_K4_UP,      //up key hang in the air
    //ELEC_PEN_KEY_K5_PRESS,       
    PEN_KEY_KK_OTHER = 0xff
    
} PEN_KEYVALUE_ENUM;

typedef struct 
{
    uint8_t x_delay;
    uint8_t y_delay;
    uint8_t press_delay;

} ADC_DELAY_STRUCT;


typedef struct 
{
    uint8_t up_flag;
    uint8_t down_flag;
    uint8_t left_flag;
    uint8_t right_flag;

} BOUNDARYFLAG_TYPE;


typedef struct 
{
    uint32_t x;
    uint32_t y;
    uint16_t press;
    
} POSITION_STATUS_TYPE;

typedef struct
{
    uint32_t     adc_sum;            // ADC累加和
    uint16_t     adc_last;           // 最后一次ADC
    uint8_t      antenna;            // 天线
    uint8_t      freq_index;         // 扫描PWM频率索引
    
} scan_result_t;

typedef struct
{
    uint8_t         chrg_ant;               // 充能天线
    
    uint8_t         pre_freq_idx;           // 充能频率索引
    uint8_t         center_pre_freq_idx;

    uint8_t         freq_idx;
    uint8_t         center_freq_idx;

    uint8_t         center_x_ant;           // 中心x天线
    uint8_t         center_y_ant;           // 中心y天线

    uint8_t         x_delay;
    uint8_t         y_delay;
    uint8_t         z_delay;

    uint8_t         scan_mode;              // 扫描模式
    uint8_t         scan_PWM_Cnt;
    uint8_t         pwmChargeCount;
    uint8_t         handPencilStatus;
    uint8_t         scanAdcConverCount;
    uint8_t         scanAdcDelayCount;
    uint8_t         adcDealEndFlag;
} scan_t;

typedef  struct 
{ 
    uint32_t testPressFreq;
    uint8_t penFirstSatus;
    uint8_t penInOutSatus;
    uint8_t pen_flag;
    uint8_t penKeyValue;
    uint8_t reportBuf[8];
    uint8_t buttonStatus;
    uint8_t reportType;
    BOUNDARYFLAG_TYPE bound;
    POSITION_STATUS_TYPE curPos;
    POSITION_STATUS_TYPE lastPos;

}TABLET_STATUS_TYPE;

typedef struct 
{
    uint8_t  adcMaxAdcIndex;
    uint16_t adcLastValue[7];
    uint32_t adcMaxValue;
    uint32_t adcValue[7];
    uint8_t  freq_index[7];

} SCAN_ADC_VALUE_STRUCT;


typedef struct  {
    unsigned char num;
    unsigned char antana;
    unsigned int value;
    unsigned short int last_value;//180504-A :Dynamic delay
} ANTANA_DATA_STRUCT;

extern scan_t g_scan;

extern SCAN_ADC_VALUE_STRUCT gScanCurZ;
extern ANTANA_DATA_STRUCT gXAntADCVal[XY_ANT_ADC_CACHE_SIZE];
extern ANTANA_DATA_STRUCT gYAntADCVal[XY_ANT_ADC_CACHE_SIZE];


extern def_pos_calc pos_calc; // [CIS-L points][CIS-R points]
extern def_report_pos report_pos;
extern TABLET_STATUS_TYPE gb_hbStatus;
extern ADC_DELAY_STRUCT gADCDelay;


extern scan_result_t* scan_get_max_x(void);
extern void scan_reset(void);
extern void scan_recv(void);
extern void scan_start(void);
extern uint8_t  scan_mode_read(void);
extern uint8_t  read_hand_pencil_status(void);
extern void  set_hand_pencil_status(uint8_t value);
void scan_main_calculate(void);
void scan_chrg(uint8_t x_ant, uint8_t y_ant);

#endif

