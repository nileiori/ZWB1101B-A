#ifndef __IQS_H__
#define __IQS_H__


#include "config.h"

#pragma anon_unions
// Define Product Numbers

#define IQS624_PRODUCT_NR       67
#define IQS624_SOFTWARE_NR      2

#define I2C_INT1                pin_read(I2C_INT1_PIN_IDX)
#define I2C_INT2                pin_read(I2C_INT2_PIN_IDX)

#define PRODUCT_NUMBER            0x00
#define SOFTWARE_NUMBER           0x01
#define HARDWARE_NUMBER           0x02
#define SYSTEM_FLAGS              0x10
#define PRX_TCH_UI_Flags          0x12
#define Hall_UI_Flags             0x14
#define CHANNEL_DATA              0x20
#define LTA			              0x30
//#define PXS_SETTINGS_0            0x40
#define Proximity_Threshold_CH0   0x50
#define Touch_Threshold_CH0       0x51
#define Proximity_Threshold_CH1   0x52
#define Touch_Threshold_CH1       0x53

#define PXS_UI_SETTINGS           0x60
#define SAR_UI_SETTINGS           0x70
#define HALL_SIN                  0x79
#define HALL_COS                  0x7A

#define HALL_DGE_LSB              0x80
#define HALL_SENS_SETTINGS        0x90
#define HALL_UI_SETTINGS      	  0xA0
#define TEMP_UI_SETTINGS      	  0xC0
#define SYS_SETTINGS 		      0xD0
#define PWR_MODE_SETTINGS 		  0xD2
#define AUTO_MODE_TIME 		      0xD6

#define DIRECT_ADDRESS	          0xF0
#define DIRECT_DATA	              0xF1

/* Bit definitions */
#define ACK_RESET				  0x40
#define REDO_ATI_ALL			  0x02
#define DO_RESEED				  0x01

// RDY Pin for IQS620
#define IQS62x_RDY  			  2
#if 1
/* Change the Prox Sensor Settings */
/* Memory Map Position 0x40 - 0x49 */
#define PXS_SETTINGS_0						0x01
#define PXS_SETTINGS_1						0x02
#define PXS_SETTINGS_2						0x4B
#define PXS_SETTINGS_3						0xD0
#define PXS_SETTINGS_4						0xD0
#define PXS_SETTINGS_5						0x00
#define PXS_SETTINGS_6						0x01
#define PXS_SETTINGS_7						0x01
#define PXS_SETTINGS_8						0x01
#define PXS_SETTINGS_9						0x01

/* Change the Prox UI Settings */
/* Memory Map Position 0x50 - 0x54 */
#define PXS_UI_SETTINGS_0					0x0A
#define PXS_UI_SETTINGS_1					0x10
#define PXS_UI_SETTINGS_2					0x0A
#define PXS_UI_SETTINGS_3					0x10
#define PXS_UI_SETTINGS_4					0x28

/* Change the HALL Sensor Settings */
/* Memory Map Position 0x70 - 0x7A */
#define HALL_SETTINGS_0						0x00
#define HALL_SETTINGS_1						0x03
#define HALL_SETTINGS_2						0xD0
#define HALL_SETTINGS_3						0xD0
#define HALL_SETTINGS_4						0x01
#define HALL_SETTINGS_5						0x01
#define HALL_SETTINGS_6						0x01
#define HALL_SETTINGS_7						0x01
#define HALL_SETTINGS_8						0x63
#define HALL_SETTINGS_9						0x63
#define HALL_SETTINGS_10					0xEB

/* Change the Device & PMU Settings */
/* Memory Map Position 0xD0 - 0xD7 */
#define SYSTEM_SETTINGS						0x00
#define ACTIVE_CHS							0x3F
#define PMU_SETTINGS						0x03
#define REPORT_RATES_TIMINGS_0				0x10
#define REPORT_RATES_TIMINGS_1				0x40
#define REPORT_RATES_TIMINGS_2				0x08
#define REPORT_RATES_TIMINGS_3				0x06
#define REPORT_RATES_TIMINGS_4				0x07

/* Bit definitions */
#define SOFT_RESET			0x80
#define ACK_RESET			0x40
#define EVENT_MODE			0x20
#define FOSC_8MHZ			0x10
#define COMMS_IN_ATI		0x08
#define SMALL_ATI_BAND		0x04
#define REDO_ATI_ALL		0x02
#define DO_RESEED			0x01

#endif

// Enum to indicate if we will do a Capacitive or Inductive sensing setup
typedef enum Setup_Type
{
    Capacitive = 0,
    Inductive
} Setup_Type_e;

typedef enum ATI_Mode
{
    ATI_Disabled = 0,
    Partial_ATI = 1,
    Semi_Partial_ATI = 2,
    Full_ATI = 3,
} ATI_Mode_e;

typedef enum Base_Value
{
    Base_75 = 0,
    Base_100 = 1,
    Base_150 = 2,
    Base_200 = 3,
} Base_Value_e;

/*
 *  Create Unions of bitfields to make reading and the IQS62x
 *  easier and more convenient
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// System Flags 0x10
typedef union
{
    // Bitfield for System Flags
    struct
    {

        uint8_t NpSegmentActive     : 1;
        uint8_t Event	              : 1;
        uint8_t InAti	              : 1;
        uint8_t PowerMode           : 2;
        uint8_t None	              : 2;
        uint8_t ShowReset           : 1;
    };
    uint8_t SystemFlags;
} SystemFlags_t;


// PXS UI Flags  0x12
typedef union
{
    // Bitfield for PXS UI Flags
    struct
    {
        uint8_t CH0_P_Out		        : 1;
        uint8_t CH1_P_Out	          : 1;
        uint8_t None		            : 2;
        uint8_t CH0_T_Out           : 1;
        uint8_t CH1_T_Out           : 1;
    };
    uint8_t PXSUIFlags;
} PXSUIFlags_t;

// IR Flags
typedef union
{
    // Bitfield for IR UI Flags
    struct
    {
        uint8_t IR_Prox         : 1;
        uint8_t IR_Touch        : 1;
        uint8_t None            : 6;
    };
    uint8_t IRFlags;
} IRFlags_t;

// IR UI Flags
typedef union
{
    // Bitfield for IR UI Flags
    struct
    {
        uint8_t IR_Range       : 4;
    };
    uint8_t IRUIFlags;
} IRUIFlags_t;

// IR Output
typedef union
{
    // Bitfield for IR UI Flags
    struct
    {
        uint8_t IR_High;
        uint8_t IR_Low;
    };
    uint16_t IRValue;
} IRValue_t;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Hall UI Flags
typedef union
{
    // Bitfield for PXS UI Flags
    struct
    {
        uint8_t Hall_N_S			: 1;
        uint8_t Hall_Prox			: 1;
        uint8_t Hall_Touch		: 1;
    };
    uint8_t HallFlags;
} HallFlags_t;

// Hall Value
typedef union
{
    // Bitfield for PXS UI Flags
    struct
    {
        uint8_t Hall_Low;
        uint8_t Hall_High;
    };
    uint16_t HallValue;
} HallValue_t;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Hall UI Flags for IQS624 0x14
typedef union
{
    // Bitfield for PXS UI Flags
    struct
    {
        uint8_t Diff				    : 1;
        uint8_t Count				    : 1;
        uint8_t Post_WW				  : 1;
        uint8_t None				    : 3;
        uint8_t Movement_Dir		: 1;
        uint8_t Wheel_Movement	: 1;
    };
    uint8_t HallFlags;
} HallFlags_IQS624_t;

// Hall UI Flags for IQS624
typedef union
{
    // Bitfield for PXS UI Flags
    struct
    {
        uint8_t ATI_Mode			: 2;
        uint8_t None          : 2;
        uint8_t Charge_Freq   : 2;
        uint8_t None1         : 1;
        uint8_t ACF_enable    : 1;
    };
    uint8_t HallSensorSettings;
} Hall_Sensor_Settings_t;

// Hall UI Flags for IQS624
typedef union
{
    // Bitfield for PXS UI Flags
    struct
    {
        uint8_t ATI_Target				: 6;
        uint8_t ATI_Base				  : 2;
    };
    uint8_t ATISettings;
} Hall_ATI_Settings_t;

// Hall Degrees
typedef union
{
    // Bitfield for Degrees output
    struct
    {
        uint8_t Hall_Degrees_Low;
        uint8_t Hall_Degrees_High;
    };
    uint16_t HallDegrees;
} HallDegrees_t;

// Hall Ratio Settings
typedef union
{
    // Bitfield for PXS UI Flags
    struct
    {
        uint8_t Numerator_Negative		            : 1;
        uint8_t Denominator_Negative	            : 1;
        uint8_t Ratio_Negative			              : 1;
        uint8_t Direction_Invert_Cos_Negative		  : 1;
        uint8_t None					                    : 1;
        uint8_t Quadrature				                : 2;
    };
    uint8_t HallRatioSettings;
} Hall_Ratio_Settings_t;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Temperature Flags Value
typedef union
{
    // Bitfield for Temp Flags
    struct
    {
        uint8_t None				: 7;
        uint8_t Temp_Trip			: 1;
    };
    uint8_t TempFlags;
} TempFlags_t;

// Hall Value
typedef union
{
    // Bitfield for PXS UI Flags
    struct
    {
        uint8_t Temp_Low;
        uint8_t Temp_High;
    };
    uint16_t TempValue;
} TempValue_t;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// PXS CH  Value
typedef union
{
    // Bitfield for PXS UI Flags
    struct
    {
        uint8_t Ch_Low;
        uint8_t Ch_High;
    };
    uint16_t Ch;
} Ch_t;


// "Object" for IQS624
typedef struct IQS624
{

    // System Flags
    SystemFlags_t SystemFlags;

    // Proxsense UI Flags
//    PXSUIFlags_t PXSUIFlags;

    // Placeholder for SAR UI Flags
//    uint8_t SARUIFlags;

    // Hall UI Flags
    HallFlags_IQS624_t HallFlags;

    // Channel 0 data
//    Ch_t Ch[6];

    // LTA data - Ch1
//    Ch_t LTA1;

    // Keep Setup of hall
//    uint8_t HallUISettings;

    // Hall Sensor Settings
//    Hall_Sensor_Settings_t HallSensorSettings;

    // Hall ATI Settings
//    Hall_ATI_Settings_t HallAtiSettings_CH2_CH3;
//    Hall_ATI_Settings_t HallAtiSettings_CH4_CH5;

    // Create space for quadrature
//    Hall_Ratio_Settings_t HallRatioSettings;

    // Hall Degrees Output
    HallDegrees_t HallDegrees;

    // Hall angle difference
//    uint8_t hallAngleDiff;
    // Average (filtered value)
//    Ch_t AvgCh[6];
} IQS624_t;

/********************************************************************************/
/*                                                                              */
/*                    iqs572_config                                             */
/*                                                                              */
/********************************************************************************/
#define IQS572_PRODUCT_NR       58

//Registers Memory Map
#define GESTURE_EVENTS_0        0x000D
#define GESTURE_EVENTS_1        0x000E

#define SYSTEM_INFO_0           0x000F

#define REL_COORDS_X            0x0012
#define ABS_COORDS_X            0x0016
#define TOUCH_STRENGTH          0x001A
#define TOUCH_AREA              0x001C
#define COMM_END_CODE           0xEEEE
#define SYSTEM_CONTROL_0        0x0431
#define SYSTEM_CONTROL_1        0x0432
#define SYSTEM_CONFIG_0         0x058E
#define SYSTEM_CONFIG_1         0x058F
#define SINGLE_FINGER_GESTURES  0x06B7
#define MULTI_FINGER_GESTURES   0x06B8

// Gesture Events 0
typedef union
{
    // Bitfield for Temp Flags
    struct
    {
        uint8_t Single_tap		: 1;
        uint8_t Tap_hold		: 1;
        uint8_t Swipe_X_nega	: 1;
        uint8_t Swipe_X_posi    : 1;
        uint8_t Swipe_Y_posi	: 1;
        uint8_t Swipe_Y_nega    : 1;
        uint8_t None            : 2;
    };
    uint8_t SingleFinger;
} Single_Finger_Gesture_t;

// Gesture Events 1
typedef union
{
    // Bitfield for Temp Flags
    struct
    {
        uint8_t Double_tap		: 1;
        uint8_t Scroll			: 1;
        uint8_t Zoom			: 1;
        uint8_t None            : 5;
    };
    uint8_t MultiFinger;
} Multi_Finger_Gesture_t;

// "Object" for IQS572
typedef struct IQS572
{
    uint8_t SingleGes;
    uint8_t MultiGes;
    uint8_t SysInfo_0;
    uint8_t SysInfo_1;
    uint8_t Finger_num;
    uint8_t Relative_XY[4];
    uint8_t Absolute_XY[4];
    uint8_t iqsAct;
    uint8_t iqsStep;
    uint8_t iqsClick;
    uint8_t iqsClickCnt;
    uint8_t dragStatus;
} IQS572_t;

enum
{
    e_TrackPad_rotate_degree_0,
    e_TrackPad_rotate_degree_90,
    e_TrackPad_rotate_degree_180,
    e_TrackPad_rotate_degree_270
};

enum Gesture_Events_0
{
    SINGLE_TAP,
    PRESS_AND_HOLD,
    SWIPE_X_NEG,
    SWIPE_X_POS,
    SWIPE_Y_POS,
    SWIPE_Y_NEG
};

enum Gesture_Events_1
{
    TWO_FINGER_TAP,
    SCROLL,
    ZOOM
};

enum touchMode
{
    e_touch_only,
    e_touch_move
};

#define TRACK_TYPE_MOUSE    1
#define TRACK_TYPE_WHEEL    2
#define TRACK_TYPE_CUSTOM   3


#ifdef __cplusplus
extern "C" {
#endif

void iqs_interrupt_config(void);
void iqs6xx_buffer_read(uint8_t* p_buffer,uint8_t read_address,uint16_t number_of_byte);
void iqs6xx_buffer_write(uint8_t* p_buffer,uint8_t write_address,uint16_t number_of_byte);
void iqs5xx_buffer_read(uint8_t* p_buffer,uint16_t read_address,uint16_t number_of_byte);
void iqs5xx_buffer_write(uint8_t* p_buffer,uint16_t write_address,uint16_t number_of_byte);
void iqs5xx_comm_end(void);
void iqs5xx_track_mode_start(void);

EventStatus iqs5xx_clear_status(void);
#if defined( configUse_ESDTEST )
EventStatus iqs6xx_err_scan(void);
EventStatus iqs5xx_err_scan(void);
#endif
void iqs_init(void);
#ifndef  configUse_IqsInt
void iqs_loop( void );
#endif
#ifdef __cplusplus
}
#endif

#endif

