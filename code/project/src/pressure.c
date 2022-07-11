#include "digitizer_comm.h"
#include "digitizer_api.h"
#include "pressure.h"
#include "config.h"
#include "led.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FREQ_NUM    105

unsigned int zih_freq, test_z;

const unsigned int freq_result[FREQ_NUM] =
{
#if( configUse_freq == configUse_freq_72M )
    422291, 423536, 424782, 426043, 427303, 428579, 429854, 431145, 432436, 433742,
    435049, 436371, 437694, 439032, 440371, 441726, 443081, 444452, 445824, 447213,
    448602, 450009, 451415, 452839, 454263, 455705, 457147, 458608, 460068, 461548,
    463027, 464526, 466024, 467542, 469060, 470598, 472136, 473694, 475253, 476832,
    478411, 480011, 481611, 483232, 484854, 486497, 488141, 489807, 491473, 493162,
    494851, 496563, 498276, 500012, 501748, 503509, 505269, 507055, 508840, 510651,
    512462, 514298, 516135, 517999, 519862, 521753, 523643, 525561, 527479, 529426,
    531372, 533348, 535323, 537328, 539333, 541368, 505263, 506599, 507936, 509287,
    510638, 512003, 513368, 514748, 516129, 517523, 518918, 520328, 521739, 523164,
    524590, 526031, 527472, 528929, 530386, 531859, 533333, 534822, 536312, 537818,
    539325, 540848, 542372, 543913, 545454
#else
    421052, 421979, 422907, 423842, 424778, 425722, 426666, 427618, 428571, 429532,
    430493, 431462, 432432, 433410, 434389, 435376, 436363, 437359, 438356, 439361,
    440366, 441381, 442396, 443420, 444444, 445477, 446511, 447554, 448598, 449651,
    450704, 451767, 452830, 453903, 454976, 456059, 457142, 458236, 459330, 460434,
    461538, 462653, 463768, 464893, 466019, 467155, 468292, 469440, 470588, 471747,
    472906, 474076, 475247, 476429, 477611, 478805, 480000, 481206, 482412, 483630,
    484848, 486078, 487309, 488552, 489795, 491051, 492307, 493576, 494845, 496127,
    497409, 498704, 500000, 501308, 502617, 503940, 505263, 506599, 507936, 509287,
    510638, 512003, 513368, 514748, 516129, 517523, 518918, 520328, 521739, 523164,
    524590, 526031, 527472, 528929, 530386, 531859, 533333, 534822, 536312, 537818,
    539325, 540848, 542372, 543913, 545454
#endif
};

const unsigned int count_value[FREQ_NUM] =
{
    0,      2400,   4800,   7200,   9600,   12000,  14400,  16800,  19200,  21600,
    24000,  26400,  28800,  31200,  33600,  36000,  38400,  40800,  43200,  45600,
    48000,  50400,  52800,  55200,  57600,  60000,  62400,  64800,  67200,  69600,
    72000,  74400,  76800,  79200,  81600,  84000,  86400,  88800,  91200,  93600,
    96000,  98400,  100800, 103200, 105600, 108000, 110400, 112800, 115200, 117600,
    120000, 122400, 124800, 127200, 129600, 132000, 134400, 136800, 139200, 141600,
    144000, 146400, 148800, 151200, 153600, 156000, 158400, 160800, 163200, 165600,
    168000, 170400, 172800, 175200, 177600, 180000, 182400, 184800, 187200, 189600,
    192000, 194400, 196800, 199200, 201600, 204000, 206400, 208800, 211200, 213600,
    216000, 218400, 220800, 223200, 225600, 228000, 230400, 232800, 235200, 237600,
    240000, 242400, 244800, 247200, 249600
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int A0sure,A1sure,A2sure,A3sure,A4sure,A5sure,A8sure,A9sure;   //180402-A Nate sol major freq Tip to Key2,4 fast switch issue.
unsigned int pressure_old_temp,smooth_counter;
unsigned char sw1_real_counter, sw1_flag;

void pen_key_status_update(void)
{
    //-----------------------------------------------------------------
    // Pen frequency Range Key0/Key2/Key4/eraser
    //-----------------------------------------------------------------
    if (K0_Start_Lo <= zih_freq && zih_freq < K0_Start_Hi) //Key0 502K~509K
    {
        A0sure++;
        if(A0sure >=1)
        {
            gb_hbStatus.penKeyValue = PEN_KEY_K0_UP;
            A0sure = 1;
            A1sure = 0;
            A2sure = 0;
            A3sure = 0;
            A4sure = 0;
            A5sure = 0;
        }
        else
        {
            // A0sure = 0;
            A1sure = 0;
            A2sure = 0;
            A3sure = 0;
            A4sure = 0;
            A5sure = 0;
        }
    }
    else if(K0_Start_Hi <= zih_freq && zih_freq < K1_Limit) //Key1 509K~540K
    {
        A1sure++;
        if(A1sure >=3)
        {
            gb_hbStatus.penKeyValue = PEN_KEY_K0_UP;
            A0sure = 0;
            A1sure = 3;
            A2sure = 0;
            A3sure = 0;
            A4sure = 0;
            A5sure = 0;
        }
        else
        {
            A0sure = 0;
            //A1sure = 0;
            A2sure = 0;
            A3sure = 0;
            A4sure = 0;
            A5sure = 0;
        }
    }
    else if(K2_Start_Lo <= zih_freq && zih_freq < K2_Start_Hi) //Key2 457K~466K
    {
        A2sure++;
        if(A2sure >=3)
        {
            gb_hbStatus.penKeyValue = PEN_KEY_K2_UP;
            A0sure = 0;
            A1sure = 0;
            A2sure = 3;
            A3sure = 0;
            A4sure = 0;
            A5sure = 0;
        }
        else
        {
            A0sure = 0;
            A1sure = 0;
            //A2sure = 0;
            A3sure = 0;
            A4sure = 0;
            A5sure = 0;
        }
    }
    else if(K2_Start_Hi <= zih_freq && zih_freq < K4_Start_Lo) //Key3 466K~481K
    {
        A3sure++;
        if(A3sure >=3)
        {
            gb_hbStatus.penKeyValue = PEN_KEY_K2_UP;
            A0sure = 0;
            A1sure = 0;
            A2sure = 0;
            A3sure = 3;
            A4sure = 0;
            A5sure = 0;
        }
        else
        {
            A0sure = 0;
            A1sure = 0;
            A2sure = 0;
            //A3sure = 0;
            A4sure = 0;
            A5sure = 0;
        }
    }
    else if(K4_Start_Lo <= zih_freq && zih_freq < K4_Start_Hi) //Key4 481K~487K
    {
        A4sure++;
        if(A4sure >=3)
        {
            gb_hbStatus.penKeyValue = PEN_KEY_K4_UP;
            A0sure = 0;
            A1sure = 0;
            A2sure = 0;
            A3sure = 0;
            A4sure = 3;
            A5sure = 0;
        }
        else
        {
            A0sure = 0;
            A1sure = 0;
            A2sure = 0;
            A3sure = 0;
            //A4sure = 0;
            A5sure = 0;
        }
    }
    else if(K4_Start_Hi <= zih_freq && zih_freq < K0_Start_Lo) //Key4 481K~502K
    {
        A5sure++;
        if(A5sure >=3)
        {
            gb_hbStatus.penKeyValue = PEN_KEY_K4_UP;
            A0sure = 0;
            A1sure = 0;
            A2sure = 0;
            A3sure = 0;
            A4sure = 0;
            A5sure = 3;
        }
        else
        {
            A0sure = 0;
            A1sure = 0;
            A2sure = 0;
            A3sure = 0;
            A4sure = 0;
            //A5sure = 0;
        }
    }
    else
    {
        gb_hbStatus.penKeyValue = PEN_KEY_KK_OTHER;
        A0sure = 0;
        A1sure = 0;
        A2sure = 0;
        A3sure = 0;
        A4sure = 0;
        A5sure = 0;
    }
}

void pressure_smooth(void)
{
    unsigned int temp_ui3 = 0;

    temp_ui3 = PressureSmooth_times;
    if(temp_ui3 == 1) //when it's 1, no point should be added into smooth handling.
    {
        //wihtout smooth
        report_pos.Z_sum = 0;
        report_pos.Counter2 = 0;
    }
    else    //processing real smooth handling.
    {
        while(report_pos.Counter2 >= temp_ui3)
        {
            report_pos.Z_sum = report_pos.Z_sum - report_pos.Z;
            report_pos.Counter2 --;
        }
    }

    // ------ average calculation

    report_pos.Z_sum += zih_freq;
    report_pos.Counter2 ++;

    if(report_pos.Counter2 > 0) // avoid #DIV/0
    {
        report_pos.Z = report_pos.Z_sum / report_pos.Counter2;
    }
    else
    {
        report_pos.Z = zih_freq;
        report_pos.Counter2 = 0;
    }

}

unsigned int pressure_gamma_calibration(unsigned int P)
{
    float modfiy_z;
    unsigned short int temp1;
    unsigned const int Original_pressure[8] = {0, 256, 512, 1024, 2048, 4096, 6144, 8191};
    unsigned const int Modfiy_pressure[8] = {0, 351, 659, 1237, 2323, 4362, 6307, 8191};

    for (temp1 = 0; temp1 < 7; temp1++)
    {
        if ((P > Original_pressure[temp1]) && (P <= Original_pressure[temp1+1]))
        {
            modfiy_z = ((float)((P-Original_pressure[temp1])*(Modfiy_pressure[temp1+1]-Modfiy_pressure[temp1]))
                        /(float)(Original_pressure[temp1+1]-Original_pressure[temp1]))+Modfiy_pressure[temp1];
            temp1 = 7; //equals break;
        }
        else
        {
            modfiy_z = P;
        }
    }

    return (int)modfiy_z;
}

void pressure_freq_calculate(uint8_t center_freq_idx)
{
    uint8_t index=0;
    uint32_t temp;

    index = gScanCurZ.adcMaxAdcIndex;

    pos_calc.Z = PosCalcP_Lib(gScanCurZ.adcValue[index-1],
                              gScanCurZ.adcValue[index],
                              gScanCurZ.adcValue[index+1],
                              center_freq_idx,
                              PenScale);

    pos_calc.Z += (PenScale/2);

    temp = pos_calc.Z/2400;
    zih_freq = (pos_calc.Z-count_value[temp])*(freq_result[temp+1]-freq_result[temp])/(count_value[temp+1]-count_value[temp])+freq_result[temp];

}


void pressure_calculate(uint8_t center_freq_idx)
{

    if (gScanCurZ.adcLastValue[gScanCurZ.adcMaxAdcIndex] != 0 && gb_hbStatus.penInOutSatus == 1)
    {
        pressure_freq_calculate(center_freq_idx);
    }
    else
    {
        pos_calc.Z = 0;
    }

    pen_key_status_update();

    if (gb_hbStatus.penKeyValue == PEN_KEY_K0_UP)
    {
        pressure_smooth();
        if(zih_freq > K0_Start_Hi)
        {
            zih_freq = report_pos.Z;
        }
    }
    else
    {
        report_pos.Counter2 = 0;
        report_pos.Z_sum = 0;
    }

    test_z = zih_freq/10;
    gb_hbStatus.testPressFreq = zih_freq;

    if ((gb_hbStatus.penKeyValue == PEN_KEY_K0_UP) && ((A0sure ==1)||(A1sure ==3)))
    {
        //pen Key0
        if (gScanCurZ.adcMaxValue >= 0x100)
        {
            if ((K0_Start_Lo <= gb_hbStatus.testPressFreq && gb_hbStatus.testPressFreq < K0_Start_Hi))
            {
                if ((sw1_flag == 1) && (FinalPressureCompensationMode == 1))
                {
                    sw1_real_counter++;
                    if(sw1_real_counter >= FinalPressureCompensationPoint + 1)  //consecutive action ( output to floating) will then make the action valid.
                    {
                        sw1_real_counter = FinalPressureCompensationPoint;
                        gb_hbStatus.penKeyValue = PEN_KEY_K0_UP;
                        pos_calc.Z = 0;
                        sw1_flag = 0;
                    }
                    else if ((sw1_real_counter >= FinalPressureCompensationPoint) && (pressure_old_temp > 0x1d0))
                    {
                        pos_calc.Z = 0x1d0;
                    }
                    else
                    {
                        pos_calc.Z = (pressure_old_temp * FinalPressureCompensationValue)/100;
                    }
                }
                else
                {
                    sw1_flag = 0;
                    sw1_real_counter = 0;
                    gb_hbStatus.penKeyValue = PEN_KEY_K0_UP;
                    pos_calc.Z = 0;
                }
            }
            else if(gb_hbStatus.testPressFreq >= K0_Start_Hi && gb_hbStatus.testPressFreq <= K1_Limit)
            {
                pos_calc.Z = ((gb_hbStatus.testPressFreq- K0_Start_Hi) * REAL_PRESSURE_LIMIT) / (K1_PrMax - K0_Start_Hi);
                sw1_flag = 1;  //raise flag to output
                sw1_real_counter = 0;
            }
            else
            {
                pos_calc.Z = 0;
            }
        }
        else
        {
            pos_calc.Z = 0;
        }
    }
    else if ((gb_hbStatus.penKeyValue == PEN_KEY_K2_UP) && ((A2sure ==3) || (A3sure ==3)))
    {
        //pen Key2
        if(K2_Start_Lo < gb_hbStatus.testPressFreq && gb_hbStatus.testPressFreq < K2_Start_Hi)
        {
            pos_calc.Z = 0;
        }
        else if(gb_hbStatus.testPressFreq >= K2_Start_Hi)
        {
            pos_calc.Z = ((gb_hbStatus.testPressFreq - K2_Start_Hi) * REAL_PRESSURE_LIMIT) / (K3_PrMax - K2_Start_Hi);

        }
        else
        {
            pos_calc.Z = 0;
        }
    }
    else if ((gb_hbStatus.penKeyValue == PEN_KEY_K4_UP) && ((A4sure == 3) || (A5sure == 3)))
    {
        //pen Key4
        if(K4_Start_Lo < gb_hbStatus.testPressFreq && gb_hbStatus.testPressFreq < K4_Start_Hi)
        {
            pos_calc.Z = 0;
        }
        else if(gb_hbStatus.testPressFreq >= K4_Start_Hi)
        {
            pos_calc.Z = ((gb_hbStatus.testPressFreq - K4_Start_Hi) * REAL_PRESSURE_LIMIT) / (K5_PrMax - K4_Start_Hi);
        }
        else
        {
            pos_calc.Z = 0;
        }
    }
    else
    {
        pos_calc.Z = 0;
    }

    if(pos_calc.Z >= REAL_PRESSURE_LIMIT)
    {
        pos_calc.Z = REAL_PRESSURE_LIMIT;
    }

    if (gb_hbStatus.penKeyValue == PEN_KEY_K0_UP)
    {
        pressure_old_temp = pos_calc.Z;
    }

    if (gb_hbStatus.penFirstSatus == 0)
    {
        pos_calc.Z = 0;
        gb_hbStatus.penFirstSatus = 3;
    }

    if (Gamma_Setting == 1)
    {
        pos_calc.Z = pressure_gamma_calibration(pos_calc.Z);
    }

    gb_hbStatus.curPos.press = pos_calc.Z;

}



