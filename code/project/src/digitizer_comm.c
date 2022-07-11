#include <stdio.h>
#include <stdlib.h>
#include "gd32f3x0.h"
#include "string.h"
#include "digitizer_comm.h"
#include "digitizer_api.h"
#include "scan.h"
#include "math.h"
#include "usb.h"
#include "config.h"
#include "usbd_std.h"
#include "timer.h"

char x_deg, y_deg;
unsigned int AutoJitter_level, AutoJitter_range;

unsigned char  Jitter_Table[16] =
{
    Jitter_0,   Jitter_1,   Jitter_2,   Jitter_3,
    Jitter_4,   Jitter_5,   Jitter_6,   Jitter_7,
    Jitter_8,   Jitter_9,   Jitter_10,  Jitter_11,
    Jitter_12,  Jitter_13,  Jitter_14,  Jitter_15
};
unsigned char  PenJitterRange;
unsigned int Smooth_flag;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void get_Xcoordinate(uint8_t center_ant)
{
    uint8_t index = 0;

    index = gXAntADCVal[9].num;

    if(index == 0)
    {
        index = 1;
    }
    else if(index > 8)
    {
        index = 7;
    }


    pos_calc.X = PosCalcCoordinate_3And4Line_Lib(gXAntADCVal[index - 2].value,
                 gXAntADCVal[index - 1].value,
                 gXAntADCVal[index].value,
                 gXAntADCVal[index + 1].value,
                 gXAntADCVal[index + 2].value,
                 center_ant,
                 XY_LPI);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void get_Ycoordinate(uint8_t center_ant)
{
    uint8_t index = 0;

    index = gYAntADCVal[9].num;

    if(index == 0)
    {
        index = 1;
    }
    else if(index > 8)
    {
        index = 7;
    }

    pos_calc.Y = PosCalcCoordinate_3And4Line_Lib(gYAntADCVal[index - 2].value,
                 gYAntADCVal[index - 1].value,
                 gYAntADCVal[index].value,
                 gYAntADCVal[index + 1].value,
                 gYAntADCVal[index + 2].value,
                 center_ant,
                 XY_LPI);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t Jitter(void)
{
    uint8_t channel = config_channel_get();
    AutoJitter_range = JitterValue;

    if(channel != REPORT_CHANNEL_USERDEF)
    //if(gb_hbStatus.reportType != REPORT_CHANNEL_USERDEF)
    {
        if((abs(gb_hbStatus.curPos.x - gb_hbStatus.lastPos.x) + abs(gb_hbStatus.curPos.y - gb_hbStatus.lastPos.y)) >= AutoJitter_range)
        {
            if((gb_hbStatus.curPos.x > 0) && (gb_hbStatus.curPos.x <= 32767))
            {
                gb_hbStatus.reportBuf[0] = gb_hbStatus.curPos.x;        //X-coord LSB
                gb_hbStatus.reportBuf[1] = (gb_hbStatus.curPos.x >> 8); //X-coord MSB
                gb_hbStatus.lastPos.x = (gb_hbStatus.curPos.x + gb_hbStatus.lastPos.x) / 2;
            }
            if((gb_hbStatus.curPos.y > 0) && (gb_hbStatus.curPos.y <= 32767))
            {
                gb_hbStatus.reportBuf[2] = gb_hbStatus.curPos.y;        //Y-coord LSB
                gb_hbStatus.reportBuf[3] = (gb_hbStatus.curPos.y >> 8); //Y-coord MSB
                gb_hbStatus.lastPos.y = (gb_hbStatus.curPos.y + gb_hbStatus.lastPos.y) / 2;
            }
            return 1;
        }
    }
    else
    {
        if((abs(gb_hbStatus.curPos.x - gb_hbStatus.lastPos.x) + abs(gb_hbStatus.curPos.y - gb_hbStatus.lastPos.y)) >= AutoJitter_range)
        {
            //saving data to USB buffer
            if((gb_hbStatus.curPos.x > 0) && (gb_hbStatus.curPos.x <= X_PHYSICAL_MAX))
            {
                gb_hbStatus.reportBuf[0] = gb_hbStatus.curPos.x;        //X-coord LSB
                gb_hbStatus.reportBuf[1] = (gb_hbStatus.curPos.x >> 8); //X-coord MSB
                gb_hbStatus.lastPos.x = (gb_hbStatus.curPos.x + gb_hbStatus.lastPos.x) / 2;
            }
            if((gb_hbStatus.curPos.y > 0) && (gb_hbStatus.curPos.y <= Y_PHYSICAL_MAX))
            {
                gb_hbStatus.reportBuf[2] = gb_hbStatus.curPos.y;        //Y-coord LSB
                gb_hbStatus.reportBuf[3] = (gb_hbStatus.curPos.y >> 8); //Y-coord MSB
                gb_hbStatus.lastPos.y = (gb_hbStatus.curPos.y + gb_hbStatus.lastPos.y) / 2;
            }


            return 1;
        }
    }

    return 0;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AutoJitter(void)
{
    Jitter_Table[0] = Jitter_0;
    Jitter_Table[1] = Jitter_1;
    Jitter_Table[2] = Jitter_2;
    Jitter_Table[3] = Jitter_3;
    Jitter_Table[4] = Jitter_4;
    Jitter_Table[5] = Jitter_5;
    Jitter_Table[6] = Jitter_6;
    Jitter_Table[7] = Jitter_7;
    Jitter_Table[8] = Jitter_8;
    Jitter_Table[9] = Jitter_9;
    Jitter_Table[10] = Jitter_10;
    Jitter_Table[11] = Jitter_11;
    Jitter_Table[12] = Jitter_12;
    Jitter_Table[13] = Jitter_13;
    Jitter_Table[14] = Jitter_14;
    Jitter_Table[15] = Jitter_15;

    if(((gb_hbStatus.curPos.x > Xh_SideBufferArea) && (gb_hbStatus.curPos.y > Yh_SideBufferArea)) ||
            ((gb_hbStatus.curPos.x < Xl_SideBufferArea) && (gb_hbStatus.curPos.y > Yh_SideBufferArea)) ||
            ((gb_hbStatus.curPos.x > Xh_SideBufferArea) && (gb_hbStatus.curPos.y < Yl_SideBufferArea)) ||
            ((gb_hbStatus.curPos.x < Xl_SideBufferArea) && (gb_hbStatus.curPos.y < Yl_SideBufferArea)))
    {
        PenJitterRange = PenCornerJitter;   //PenCornerJitter = 2
    }
    else if((gb_hbStatus.curPos.x > Xh_SideBufferArea) || (gb_hbStatus.curPos.x < Xl_SideBufferArea) ||
            (gb_hbStatus.curPos.y > Yh_SideBufferArea) || (gb_hbStatus.curPos.y < Yl_SideBufferArea))
    {
        PenJitterRange = PenSiderJitter;    //PenSiderJitter = 1
    }
    else
    {
        PenJitterRange = PenMidJitter;  //PenMidJitter = 0
    }
//////////////////////////DeterminePenJitterRange////////////////////////////////////
    AutoJitter_level = 0;

    if (pos_calc.Z != 0)
    {
        AutoJitter_range = JitterValue;
        AutoJitterSendToBuffer(AutoJitter_range);
    }
    else
    {
        if(PenJitterRange == PenCornerJitter)
        {
            AutoJitter_level = ((gXAntADCVal[9].last_value + gYAntADCVal[9].last_value) / 2) / Corner_Auto_Jitter_value ;
            if (AutoJitter_level >= 15 )
                AutoJitter_range = Jitter_Table[15];
            else
                AutoJitter_range = Jitter_Table[AutoJitter_level];

            AutoJitterSendToBuffer(AutoJitter_range);
        }
        else if(PenJitterRange == PenSiderJitter)
        {
            AutoJitter_level = ((gXAntADCVal[9].last_value + gYAntADCVal[9].last_value) / 2) / Side_Auto_Jitter_value ;
            if ( AutoJitter_level >= 15 )
                AutoJitter_range = Jitter_Table[15];
            else
                AutoJitter_range = Jitter_Table[AutoJitter_level];

            AutoJitterSendToBuffer(AutoJitter_range);
        }
        else if(PenJitterRange == PenMidJitter)
        {
            AutoJitter_level = ((gXAntADCVal[9].last_value + gYAntADCVal[9].last_value) / 2) / Auto_Jitter_value ;
            if ( AutoJitter_level >= 15 )
                AutoJitter_range = Jitter_Table[15];
            else
                AutoJitter_range = Jitter_Table[AutoJitter_level];

            AutoJitterSendToBuffer(AutoJitter_range);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AutoJitterSendToBuffer(unsigned char range)
{
    uint8_t channel;
    
    channel = config_channel_get();
    if (channel != REPORT_CHANNEL_USERDEF)
    {
        range = (range * 32767) / X_PHYSICAL_MAX;
    }

    if (abs(gb_hbStatus.curPos.x - gb_hbStatus.lastPos.x) + abs(gb_hbStatus.curPos.y - gb_hbStatus.lastPos.y) >= range)
    {
        if(channel == REPORT_CHANNEL_USERDEF)
        {

            //saving data to USB buffer
            if((gb_hbStatus.curPos.x > 0) && (gb_hbStatus.curPos.x <= X_PHYSICAL_MAX))
            {
                gb_hbStatus.reportBuf[0] = gb_hbStatus.curPos.x;
                gb_hbStatus.reportBuf[1] = (gb_hbStatus.curPos.x >> 8);
                gb_hbStatus.lastPos.x = (gb_hbStatus.curPos.x + gb_hbStatus.lastPos.x) / 2;
            }
            if((gb_hbStatus.curPos.y > 0) && (gb_hbStatus.curPos.y <= Y_PHYSICAL_MAX))
            {
                gb_hbStatus.reportBuf[2] = gb_hbStatus.curPos.y;
                gb_hbStatus.reportBuf[3] = (gb_hbStatus.curPos.y >> 8);
                gb_hbStatus.lastPos.y = (gb_hbStatus.curPos.y + gb_hbStatus.lastPos.y) / 2;
            }
        }
        else
        {
            if((gb_hbStatus.curPos.x > 0) && (gb_hbStatus.curPos.x <= 0x7fff))
            {
                gb_hbStatus.reportBuf[0] = gb_hbStatus.curPos.x;
                gb_hbStatus.reportBuf[1] = (gb_hbStatus.curPos.x >> 8);
                gb_hbStatus.lastPos.x = (gb_hbStatus.curPos.x + gb_hbStatus.lastPos.x) / 2;
            }
            if((gb_hbStatus.curPos.y > 0) && (gb_hbStatus.curPos.y <= 0x7fff))
            {
                gb_hbStatus.reportBuf[2] = gb_hbStatus.curPos.y;
                gb_hbStatus.reportBuf[3] = (gb_hbStatus.curPos.y >> 8);
                gb_hbStatus.lastPos.y = (gb_hbStatus.curPos.y + gb_hbStatus.lastPos.y) / 2;
            }
        }
    }
    else
    {
        gb_hbStatus.reportBuf[0] = gb_hbStatus.lastPos.x;
        gb_hbStatus.reportBuf[1] = (gb_hbStatus.lastPos.x >> 8);
        gb_hbStatus.reportBuf[2] = gb_hbStatus.lastPos.y;
        gb_hbStatus.reportBuf[3] = (gb_hbStatus.lastPos.y >> 8);
    }

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DeterminePenArea(void)
{
    if((report_pos.X < X_offset) || (report_pos.X > Xh_OutSideArea) ||
            (report_pos.Y < Y_offset) || (report_pos.Y > Yh_OutSideArea))
    {
        gb_hbStatus.curPos.press = 0;
    }
}

void Offset(void)
{
    if((report_pos.X <= Xoffset) || (report_pos.X < Xl_OnSideArea))
    {
        gb_hbStatus.bound.left_flag = 1;
        gb_hbStatus.curPos.x = 0;
    }
    else
    {
        gb_hbStatus.bound.left_flag = 0;
    }

    if((report_pos.X > X_PHYSICAL_MAX + Xoffset) || (report_pos.X > Xh_OnSideArea))
    {
        gb_hbStatus.bound.right_flag = 1;
        gb_hbStatus.curPos.x = X_PHYSICAL_MAX;// + Xoffset;
    }
    else
    {
        gb_hbStatus.bound.right_flag = 0;
    }

    if((report_pos.X >= Xoffset) && (report_pos.X <= X_PHYSICAL_MAX + Xoffset))
    {
        gb_hbStatus.curPos.x = report_pos.X - Xoffset;
    }

    if((report_pos.Y <= Yoffset) || (report_pos.Y < Yl_OnSideArea))
    {
        gb_hbStatus.bound.up_flag = 1;
        gb_hbStatus.curPos.y = 0;
    }
    else
    {
        gb_hbStatus.bound.up_flag = 0;
    }

    if((report_pos.Y > Y_PHYSICAL_MAX + Yoffset) || (report_pos.Y > Yh_OnSideArea))
    {
        gb_hbStatus.bound.down_flag = 1;
        gb_hbStatus.curPos.y  = Y_PHYSICAL_MAX;// + Yoffset;
    }
    else
    {
        gb_hbStatus.bound.down_flag = 0;
    }

    if((report_pos.Y >= Yoffset) && (report_pos.Y <= Y_PHYSICAL_MAX + Yoffset))
    {
        gb_hbStatus.curPos.y  = report_pos.Y - Yoffset;
    }


}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mapping(void)
{
    uint8_t android_support;

    if((gb_hbStatus.curPos.x > 0) && (gb_hbStatus.curPos.x <= X_PHYSICAL_MAX))
    {
        gb_hbStatus.curPos.x = (gb_hbStatus.curPos.x - 0) * 0x7fff / (X_PHYSICAL_MAX - 0);
    }
    else if(gb_hbStatus.curPos.x <= 0)
    {
        gb_hbStatus.curPos.x = 0;
    }
    else if(gb_hbStatus.curPos.x > X_PHYSICAL_MAX)
    {
        gb_hbStatus.curPos.x = 0x7fff;
    }

    if((gb_hbStatus.curPos.y > 0) && (gb_hbStatus.curPos.y <= Y_PHYSICAL_MAX))
    {
        gb_hbStatus.curPos.y = (gb_hbStatus.curPos.y - 0) * 0x7fff / (Y_PHYSICAL_MAX - 0);
    }
    else if(gb_hbStatus.curPos.y <= 0)
    {
        gb_hbStatus.curPos.y = 0;
    }
    else if(gb_hbStatus.curPos.y > Y_PHYSICAL_MAX)
    {
        gb_hbStatus.curPos.y = 0x7fff;
    }

    if(android_handle.android_uc.inverse_x)//X_Inv
    {
        gb_hbStatus.curPos.x = 0x7fff - gb_hbStatus.curPos.x;
    }

    if(android_handle.android_uc.inverse_y)//Y_Inv
    {
        gb_hbStatus.curPos.y = 0x7fff - gb_hbStatus.curPos.y;
    }

    android_support = config_android_support_get();

    if (android_support && (check_sys == 2))
    {
        gb_hbStatus.curPos.y = 0x7fff - gb_hbStatus.curPos.y;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Inverter(void)
{
    if(android_handle.android_uc.inverse_x)//X_Inv
    {
        gb_hbStatus.curPos.x  = X_PHYSICAL_MAX - gb_hbStatus.curPos.x;
    }

    if(android_handle.android_uc.inverse_y)//Y_Inv
    {
        gb_hbStatus.curPos.y  = Y_PHYSICAL_MAX - gb_hbStatus.curPos.y ;
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Smooth(void)
{
    SMOOTH_PARAMETER smooth;

    smooth.NearSmooth_mm = nearSmooth_mm;
    smooth.Middle1Smooth_mm = middle1Smooth_mm;
    smooth.Middle2Smooth_mm = middle2Smooth_mm;
    smooth.Middle3Smooth_mm = middle3Smooth_mm;
    smooth.Middle4Smooth_mm = middle4Smooth_mm;
    smooth.Middle5Smooth_mm = middle5Smooth_mm;
    smooth.Middle6Smooth_mm = middle6Smooth_mm;
    smooth.FarSmooth1_mm = farSmooth1_mm;
    smooth.FarSmooth2_mm = farSmooth2_mm;
    smooth.FarSmooth3_mm = farSmooth3_mm;

    //////////////////////////////////////////////////////////////////////////////
    smooth.SlowSmooth_times = slowSmooth_times;
    smooth.NearSmooth_times = nearSmooth_times;
    smooth.Middle1Smooth_times = middle1Smooth_times;
    smooth.Middle2Smooth_times = middle2Smooth_times;
    smooth.FarSmooth1_times = farSmooth1_times;

    ///////////////////////////////////////////////////////////////////////////////
    smooth.K1_slowSmooth_times = k1_slowSmooth_times;
    smooth.K1_nearSmooth_times = k1_nearSmooth_times;
    smooth.K1_middle1Smooth_times = k1_middle1Smooth_times;
    smooth.K1_middle2Smooth_times = k1_middle2Smooth_times;
    smooth.K1_middle3Smooth_times = k1_middle3Smooth_times;
    smooth.K1_middle4Smooth_times = k1_middle4Smooth_times;
    smooth.K1_middle5Smooth_times = k1_middle5Smooth_times;
    smooth.K1_middle6Smooth_times = k1_middle6Smooth_times;
    smooth.K1_farSmooth1_times = k1_farSmooth1_times;
    smooth.K1_farSmooth2_times = k1_farSmooth2_times;
    smooth.K1_farSmooth3_times = k1_farSmooth3_times;

    if((gb_hbStatus.lastPos.press == 0) && (pos_calc.Z >= 1500))
        smooth.Smooth_flag = 0;
    else
        smooth.Smooth_flag = 1;

    if(smooth.Smooth_flag == 0)
    {
        report_pos.X_sum = 0;
        report_pos.Y_sum = 0;
        report_pos.Counter = 0;
    }

    //  Smooth_flag = report_pos.Counter;

    //  Pos_smooth_Lib(&pos_calc,&report_pos);
    Pos_smooth_new_Lib(&pos_calc, &report_pos, &smooth);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tilt(uint8_t center_x_ant, uint8_t center_y_ant)
{
    TILT_PARAMETER param;

    param.Tilt_VALUE = TILT_VALUE;
    param.Tilt_Limit = TILT_LIMIT;
    param.X_TILT_UNDER_0 = X_TILT_Under_0;
    param.X_TILT_UNDER_10 = X_TILT_Under_10;
    param.X_TILT_UNDER_20 = X_TILT_Under_20;
    param.X_TILT_UNDER_30 = X_TILT_Under_30;
    param.X_TILT_UNDER_40 = X_TILT_Under_40;
    param.X_TILT_UNDER_50 = X_TILT_Under_50;
    param.X_TILT_UNDER_60 = X_TILT_Under_60;
    param.Y_TILT_UNDER_0 = Y_TILT_Under_0;
    param.Y_TILT_UNDER_10 = Y_TILT_Under_10;
    param.Y_TILT_UNDER_20 = Y_TILT_Under_20;
    param.Y_TILT_UNDER_30 = Y_TILT_Under_30;
    param.Y_TILT_UNDER_40 = Y_TILT_Under_40;
    param.Y_TILT_UNDER_50 = Y_TILT_Under_50;
    param.Y_TILT_UNDER_60 = Y_TILT_Under_60;

    Tilt_LIB_B(gXAntADCVal, gYAntADCVal, &pos_calc.Z, \
               center_x_ant, center_y_ant, ALL_X_ANTANA_NUM, ALL_Y_ANTANA_NUM, \
               android_handle.android_uc.inverse_x, android_handle.android_uc.inverse_y, &param, \
               &x_deg, &y_deg, &RETiltX, &RETiltY);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

