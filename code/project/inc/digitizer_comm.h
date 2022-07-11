#ifndef _DIGITIZER_COMM_H
#define _DIGITIZER_COMM_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "gd32f3x0.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Xoffset X_offset
#define Yoffset Y_offset

#define X_PHYSICAL_MAX      x_physical_max_value
#define Y_PHYSICAL_MAX      y_physical_max_value
#define REAL_PRESSURE_LIMIT PressureLevel



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    unsigned int X;
    unsigned int Y;
    unsigned int Z;
    unsigned long X_sum;
    unsigned long Y_sum;
    unsigned long Z_sum;
    char Counter;
    char Counter2;
} def_report_pos;

typedef struct {
    unsigned char flag; //0: empty, 1: pending ,2 enable , 4 disable
    unsigned int X; // result of X [resolution]
    unsigned int Y; // result of Y [resolution]
    unsigned int Z;
    unsigned int L_width; // CIS-L width by filter
    unsigned int R_width; // CIS-R width by filter
    unsigned int L_d; // distance between CIS-L point [mm]
    unsigned int R_d; // distance between CIS-R point [mm]
} def_pos_calc;


typedef struct {
    unsigned int  Tilt_VALUE;
    unsigned int  Tilt_Limit;
    unsigned int  X_TILT_UNDER_0;
    unsigned int  X_TILT_UNDER_10;
    unsigned int  X_TILT_UNDER_20;
    unsigned int  X_TILT_UNDER_30;
    unsigned int  X_TILT_UNDER_40;
    unsigned int  X_TILT_UNDER_50;
    unsigned int  X_TILT_UNDER_60;
    unsigned int  Y_TILT_UNDER_0;
    unsigned int  Y_TILT_UNDER_10;
    unsigned int  Y_TILT_UNDER_20;
    unsigned int  Y_TILT_UNDER_30;
    unsigned int  Y_TILT_UNDER_40;
    unsigned int  Y_TILT_UNDER_50;
    unsigned int  Y_TILT_UNDER_60;
}TILT_PARAMETER;


typedef struct {
    unsigned short NearSmooth_mm;
    unsigned short Middle1Smooth_mm;
    unsigned short Middle2Smooth_mm;
    unsigned short Middle3Smooth_mm;
    unsigned short Middle4Smooth_mm;
    unsigned short Middle5Smooth_mm;
    unsigned short Middle6Smooth_mm;
    unsigned short FarSmooth1_mm;
    unsigned short FarSmooth2_mm;
    unsigned short FarSmooth3_mm;
    unsigned short SlowSmooth_times;
    unsigned short NearSmooth_times;
    unsigned short Middle1Smooth_times;
    unsigned short Middle2Smooth_times;
    unsigned short FarSmooth1_times;
    unsigned short K1_slowSmooth_times;
    unsigned short K1_nearSmooth_times;
    unsigned short K1_middle1Smooth_times;
    unsigned short K1_middle2Smooth_times;
    unsigned short K1_middle3Smooth_times;
    unsigned short K1_middle4Smooth_times;
    unsigned short K1_middle5Smooth_times;
    unsigned short K1_middle6Smooth_times;
    unsigned short K1_farSmooth1_times;
    unsigned short K1_farSmooth2_times;
    unsigned short K1_farSmooth3_times;
    unsigned short Smooth_flag;
}SMOOTH_PARAMETER;

typedef union _BITS_4{
    unsigned int AllMsg;
        struct
            {
        bool bit0:  1; // The structure of bits mapping to union of word is little endian
        bool bit1:  1;
        bool bit2:  1;
        bool bit3:  1;
            };
} BITS_4;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern char x_deg, y_deg;
extern unsigned short RETiltX,RETiltY;
extern unsigned int Smooth_flag;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void get_Xcoordinate(uint8_t center_ant);
void get_Ycoordinate(uint8_t center_ant);
uint8_t Jitter(void);
void AutoJitter(void);
void Offset(void);
void Inverter(void);
void Mapping(void);
void AutoJitterSendToBuffer(unsigned char range);
void DeterminePenArea(void);
void Smooth(void);
void Tilt(uint8_t center_x_ant, uint8_t center_y_ant);


#define PenMidJitter        0
#define PenSiderJitter      1
#define PenCornerJitter     2

#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

