#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__


#define UGEE_USB_RECV_DATA_LEN_SIZE 10  //20
#define XY_ANT_ADC_CACHE_SIZE               10      //x,y ant ADC sample cache size
#define PRESSURE_ADC_CACHE_SIZE         10      //pressure ADC sample cache size

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Product Parameter Use
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Group 1 : Basic Settings
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MY_VID 0x28bd                               //USB Vender ID
#define MY_PID 0x0934                               //USB Product ID
#define OEM_CODE 9                                  //1:UGEE 2:XPPEN 9:Other    =>usb_descriptors.c


#define x_size 10984                                //X Axis Size(mil)  (1000 mil = 1 inch)
#define y_size 6181                                //Y Axis Size(mil)  (1000 mil = 1 inch)

#define ALL_X_ANTANA_NUM 46                             //Total number of X antennas.
#define ALL_Y_ANTANA_NUM 28                             //Total number of Y antennas.
#define Line_PerInch 5080                           //Line Per Inch (2540/5080)
#if(Line_PerInch == 2540)
#define XY_LPI 640                                  //XY Axis Line Per Inch
#define PIXEL_PER2MM  200
#else
#define XY_LPI 640*2                                //5080 XY Axis Line Per Inch 
#define PIXEL_PER2MM  400
#endif

#define Support_Android    1                        //0:No Support 1:Support

//#define FreqType    0                               //0:Up      1:Down
//#define PEN_SETTING    1                            //0:P05(Up) 1:PA1(Up) 2:P08(Down) 3:P08A(Up) 4:PH01(Down) 5:PM1(Down) 6:PE1(Up)
#define PressureLevel 8191 //511                          //P08 P08A must be setting to 511,other setting are 8191
#define Default_Report_ID 2                                 //1:ID09 2:ID07 4:ID02 ,Default Value:ID07

#define DEFAULTFOOTLIGHT 50                                 //0: 10% default light 50:50%  100:90%

#define DEFAULTTRACKPADLIGHT 100

//-----------------------------------------------------------------
// XY inverter Setting
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define X_Inv 0                                     //1:Inverter    0:Off
#define Y_Inv 0                                     //1:Inverter    0:Off


//-----------------------------------------------------------------
// Calibration Setting
//-----------------------------------------------------------------
#define calibration 1                              //0:No Calibration. 1:use xy_calibration.c

//-----------------------------------------------------------------
//  DEBUG SETTING
//-----------------------------------------------------------------
// Coordinate_mode
// 0:Org Mode
// 1:Smooth Mode
// 2:Optimization Mode(Normal Mode)
// 3:Side Mode
// 9:Calibration mode
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Coordinate_mode 2

#if(Coordinate_mode == 9)
#define x_physical_max_value  (uint32_t)(( (x_size + 2000) * Line_PerInch )/1000)
#define y_physical_max_value  (uint32_t)(( (y_size + 2000) * Line_PerInch )/1000)
#define X_offset 0                            //Increase to X:0 
#define Y_offset 0                             //Increase to Y:0 		
#else
#define x_physical_max_value    (int)55798//(( x_size * Line_PerInch )/1000)	
#define y_physical_max_value    (int)31399//(( y_size * Line_PerInch )/1000) 
#define X_offset 3503//1320//457  //1220    1480                            //Increase to X:0 
#define Y_offset 3853//1340//376  //1350   1520                               //Increase to Y:0 
#endif


//-----------------------------------------------------------------
// SideArea Setting
//-----------------------------------------------------------------
/*XH_OutSideArea > XH_OnSideArea > XH_SideBufferArea*/
#define Xh_OutSideArea      60168//26880//27300
#define Xh_OnSideArea      59172//26830
#define Xh_SideBufferArea   58203//x_physical_max_value-5080
/*XL_OutSideArea < XL_OnSideArea < XL_SideBufferArea*/
#define Xl_OutSideArea      2240//1300//950
#define Xl_OnSideArea       3400//1350
#define Xl_SideBufferArea   4225
/*YH_OutSideArea > YH_OnSideArea > YH_SideBufferArea*/
#define Yh_OutSideArea      36182//17300//17800
#define Yh_OnSideArea       35303//17250
#define Yh_SideBufferArea   34225//y_physical_max_value-5080
/*YL_OutSideArea < YL_OnSideArea < YL_SideBufferArea*/
#define Yl_OutSideArea      2830//1250//950
#define Yl_OnSideArea       3890//1300
#define Yl_SideBufferArea   4683



#define PRESCAN_FREQ_TABLE_MAX_NUM  6

#define PRESS_SCAN_ANT_NUM         6
#define PRESS_SCAN_ANT_BOTH_SIDES  2

#define COORDINATE_SCAN_ANT_NUM         7//9
#define COORDINATE_SCAN_ANT_BOTH_SIDES  3//4

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Group 3 : Transmit and receive Setting
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------
// AUTODELAY
//-----------------------------------------------------------------
#define AutoDelayMode 1         //1:On  0:Off
#define DELAY_LEVEL 0xC1F  //0x4d9  1241 1V     //0x800 //1.65V //0x9b2 2V //0xC1F 2.5V  //0xCE5 2.66V //0xeb8 3.0V  //0x1000 3.3V 
#define ADC_VOLTAGE_HIGH  0xeb8   //3.0V
#define ADC_VOLTAGE_DIFF  320//410     // 0.33V 410//0.5V 620

//-----------------------------------------------------------------
// Transmit and receive Setting
//-----------------------------------------------------------------
#define PRESSURE_CHARGING_TIME 36//32 
#define COORDINATE_CHRAGING_TIME 26//42 // need double

#define UGEE_CHARGING_AND_ADCSAMPLE_TIME 150
#define UGEE_CAULURATE_WAITIMG_TIME 50

#define ADC_SAMPLING_COUNT  50//140

#define PRESCAN_DELAY 10
#define PRESSURE_DELAY 20
#define COORDINATE_DELAY 30

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Group 4 : Pressure Setting
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------
// Final Pressure Compensation Setting
//-----------------------------------------------------------------
#define FinalPressureCompensationMode   1           //1:On  0:Off
#define FinalPressureCompensationPoint  3//default 3
#define FinalPressureCompensationValue  66
//-----------------------------------------------------------------
// GAMMA Setting
//-----------------------------------------------------------------
#define Gamma_Setting      1           //1:On    0:Off
//-----------------------------------------------------------------
// Pressure Smooth Setting
//-----------------------------------------------------------------
#define  PressureSmooth_times  5
//-----------------------------------------------------------------
// Pen frequency Setting
//-----------------------------------------------------------------
#define RS8452
#if defined( RS8452 )
#define K2_Start_Lo 457000
#define K2_Start_Hi 469000
#define K3_PrMax    474000
//==============================================
#define K4_Start_Lo 481000
#define K4_Start_Hi 489200
#define K5_PrMax    495000
//==============================================
#define K0_Start_Lo 502000
#define K0_Start_Hi 511150//508500
#define K1_PrMax    515600
#define K1_Limit    540000

#else
#define K2_Start_Lo 457000
#define K2_Start_Hi 466700
#define K3_PrMax    474000
//==============================================
#define K4_Start_Lo 481000
#define K4_Start_Hi 487600
#define K5_PrMax    495000
//==============================================
#define K0_Start_Lo 502000
#define K0_Start_Hi 510500//508500
#define K1_PrMax    521000
#define K1_Limit    540000

#endif


//-----------------------------------------------------------------
// Pen Inrange Setting
//-----------------------------------------------------------------
#define ADC_SUM_BASE_VALUE          1000
#define ADC_LAST_BASE_VALUE         250

//#define PRESCAN_ADCVALUE_PENIN_HOLD 250//60 // 800    //0x320  644 mV
//#define NORMALSCAN_ADCVALUE_PENIN_HOLD  500  //0x26c 500mV
//#define NORMALSCAN_ADCVALUE_PENOUT_HOLD  350  //0x26c 500mV
#define PRESCAN_ADCVALUE_PENIN_HOLD 60//60 // 800    //0x320  644 mV
#define NORMALSCAN_ADCVALUE_PENIN_HOLD 250  //0x26c 500mV
#define NORMALSCAN_ADCVALUE_PENOUT_HOLD 180  //0x26c 500mV

//-----------------------------------------------------------------
// Jitter Setting
//-----------------------------------------------------------------
#define	Jitter_Setting 1 		//0:Jitter  1:AutoJitter	
#define JitterValue 28

#define Auto_Jitter_value 280//250
#define Side_Auto_Jitter_value 320
#define Corner_Auto_Jitter_value 320

//-----------------------------------------------------------------
#define Jitter_0  100
#define Jitter_1  90
#define Jitter_2  80
#define Jitter_3  70
#define Jitter_4  60
#define Jitter_5  60
#define Jitter_6  60
#define Jitter_7  50//20
#define Jitter_8  50//15
#define Jitter_9  50//10
#define Jitter_10  35//10
#define Jitter_11  35
#define Jitter_12  35
#define Jitter_13  30
#define Jitter_14  30
#define Jitter_15  30



//-----------------------------------------------------------------
// Smooth Setting
//-----------------------------------------------------------------

#define    nearSmooth_mm        100*2
#define    middle6Smooth_mm     150*2
#define    middle5Smooth_mm     200*2
#define    middle4Smooth_mm     250*2
#define    middle3Smooth_mm     300*2
#define    middle2Smooth_mm     400*2
#define    middle1Smooth_mm     600*2
#define    farSmooth3_mm       1000*2
#define    farSmooth2_mm       2000*2
#define    farSmooth1_mm       4000*2

#define    slowSmooth_times  15
#define    nearSmooth_times 5//12
#define    middle2Smooth_times  3//8
#define    middle1Smooth_times  3//5
#define    farSmooth1_times  1

//------------------------------------------
#define    k1_slowSmooth_times  15//5
#define    k1_nearSmooth_times  5
#define    k1_middle6Smooth_times  5
#define    k1_middle5Smooth_times  5
#define    k1_middle4Smooth_times  5
#define    k1_middle3Smooth_times  5
#define    k1_middle2Smooth_times  5
#define    k1_middle1Smooth_times  4
#define    k1_farSmooth3_times  3
#define    k1_farSmooth2_times  3
#define    k1_farSmooth1_times  3


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Group 6 : TILT Setting
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TILT_VALUE   5000
#define TILT_LIMIT   60
#define TILT_TYPE			0			//0:Board  1:LCD

#define X_TILT_Under_0  700//680//600//400 //0x200
#define X_TILT_Under_10 940//860//700//650 //0x260
#define X_TILT_Under_20 1190//1100//840//780 //0x2b0
#define X_TILT_Under_30 1460//1400//1050 //0x320
#define X_TILT_Under_40 1785//1700//1370  //0x470
#define X_TILT_Under_50 2085//2050//1650 //0x6b0
#define X_TILT_Under_60 2200//2300//1950 //0x720

#define Y_TILT_Under_0  745//400//0x200
#define Y_TILT_Under_10 940//530//0x260
#define Y_TILT_Under_20 1200//680 //0x2b0
#define Y_TILT_Under_30 1440//980//0x320
#define Y_TILT_Under_40 1700//1200//0x470
#define Y_TILT_Under_50 1825
#define Y_TILT_Under_60 2090//1950//0x720



//==============================================
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Group 7 : Hotkey Setting
// Hot Key Define (No Drive)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define HotkeyAmount    1 //1                       //0:NoHotKey    1:6-8-10 HotKey
#define HOTKEY_NUM    8           //(6 or 8)
#define VIRTUAL_HOTKEY_NUM    10
#define WHEEL_NUM    1

//Modify_Key
// L_Ctrl = 0x01
// L_Shft = 0x02
// L_Alt  = 0x04
// L_win  = 0x08
// L_Ctrl = 0x10
// L_Shft = 0x20
// L_Alt  = 0x40
// L_win  = 0x80
//======[HotKey 0]======//
#define KEY1_HK0    0x05        //Keypad B
#define Modify_Key0 0x00
#define Driver_HK0  0x01
//======[HotKey 1]======//
#define KEY1_HK1    0x08        //Keyboard E
#define Modify_Key1 0x00
#define Driver_HK1  0x02
//======[HotKey 2]======//
#define KEY1_HK2    0x00        //Keyboard I
#define Modify_Key2 0x04
#define Driver_HK2  0x04
//======[HotKey 3]======//
#define KEY1_HK3    0x2C        //Keyboard SPACE
#define Modify_Key3 0x00
#define Driver_HK3  0x08
//======[HotKey 4]======//
#define KEY1_HK4    0x19         //Keyboard Ctrl+Alt +"Z"
#define Modify_Key4 0x00
#define Driver_HK4  0x10
//======[HotKey 5]======//
#define KEY1_HK5    0x16     ////Keyboard Ctrl+"S"
#define Modify_Key5 0x01
#define Driver_HK5  0x20
//======[HotKey 6]======//
#define KEY1_HK6    0x1D        //Keyboard Ctrl +"+"
#define Modify_Key6 0x01
#define Driver_HK6  0x40
//======[HotKey 7]======//       //Keyboard Ctrl+"-"
#define KEY1_HK7    0x11
#define Modify_Key7 0x05
#define Driver_HK7  0x80
//======[HotKey 8]======//
#define KEY1_HK8    0x19     //Keyboard V
#define Modify_Key8 0x00
#define Driver_HK8  0x01
//======[HotKey 9]======//
#define KEY1_HK9    0x0F     //Keyboard L
#define Modify_Key9 0x00
#define Driver_HK9  0x02
//======[HotKey 10]======//
#define KEY1_HK10    0x12    // Keyboard Ctrl+"O"
#define Modify_Key10 0x01
#define Driver_HK10  0x04
//======[HotKey 11]======//
#define KEY1_HK11    0x11    // Keyboard Ctrl+"N"
#define Modify_Key11 0x01
#define Driver_HK11  0x08

//======[Wheel Clockwise]======//   //Keyboard Ctrl+"-"
#define Wheel_Left  0x56
#define Modify_Wheel_Left   0x01
#define Driver_Wheel_Left   0x01
//======[Wheel counterclockwise]======//Keyboard Ctrl+"+"
#define Wheel_Right 0x57
#define Modify_Wheel_Right  0x01
#define Driver_Wheel_Right   0x02


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // UGEE_PREFERENCES_H
