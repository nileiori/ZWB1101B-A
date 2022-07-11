#include "gd32f3x0_timer.h"
#include "config.h"
#include "scan.h"

extern scan_t g_scan;
#define FREQ_OFFSET (0)
#if( configUse_freq == configUse_freq_72M )
//72M
static const uint8_t period_tab[] =
{

    170+FREQ_OFFSET,	//0  421052
    169+FREQ_OFFSET,	//1  423529
    168+FREQ_OFFSET,	//2  426035
    167+FREQ_OFFSET,	//3  428571
    166+FREQ_OFFSET,	//4	431137
    165+FREQ_OFFSET,	//5	433734
    164+FREQ_OFFSET,	//6	436363
    163+FREQ_OFFSET,	//7	439024
    162+FREQ_OFFSET,	//8	441717
    161+FREQ_OFFSET,	//9	444444
    160+FREQ_OFFSET,	//10	447204
    159+FREQ_OFFSET,	//11	450000
    158+FREQ_OFFSET,	//12	452830
    157+FREQ_OFFSET,	//13	455696
    156+FREQ_OFFSET,	//14	458598
    155+FREQ_OFFSET,	//15	461538
    154+FREQ_OFFSET,	//16	464516
    153+FREQ_OFFSET,	//17	467532
    152+FREQ_OFFSET,	//18	470588
    151+FREQ_OFFSET,	//19	473684
    150+FREQ_OFFSET,	//20	476821
    149+FREQ_OFFSET,	//21	480000
    148+FREQ_OFFSET,	//22	483221
    147+FREQ_OFFSET,	//23	486486
    146+FREQ_OFFSET,	//24	489795
    145+FREQ_OFFSET,	//25	493150
    144+FREQ_OFFSET,	//26	496551
    143+FREQ_OFFSET,	//27	500000
    142+FREQ_OFFSET,	//28	503496
    141+FREQ_OFFSET,	//29	507042
    140+FREQ_OFFSET,	//30	510638
    139+FREQ_OFFSET,	//31	514285
    138+FREQ_OFFSET,	//32	517985
    137+FREQ_OFFSET,	//33	521739
    136+FREQ_OFFSET,	//34	525547
    135+FREQ_OFFSET,	//35	529411
    134+FREQ_OFFSET,	//36	533333
    133+FREQ_OFFSET,	//37	537313
    132+FREQ_OFFSET,	//38	541353
    131+FREQ_OFFSET,	//39	545454
    130+FREQ_OFFSET, // 40        510.638306
    129+FREQ_OFFSET, // 41        513.368958
    128+FREQ_OFFSET, // 42        516.129028
    127+FREQ_OFFSET, // 43        518.918945
    126+FREQ_OFFSET, // 44        521.739136
    125+FREQ_OFFSET, // 45        524.590149
    124+FREQ_OFFSET, // 46        527.472534
    123+FREQ_OFFSET, // 47        530.386719
    122+FREQ_OFFSET, // 48        533.333313
    121+FREQ_OFFSET, // 49        536.312866
    120+FREQ_OFFSET, // 50        539.325867
    119+FREQ_OFFSET, // 51        542.372864
    118+FREQ_OFFSET, // 52        545.454529
};
#elif( configUse_freq == configUse_freq_96M )
//96M
static const uint8_t period_tab[] =
{
    227, // 0         421.052643
    226, // 1         422.907501
    225, // 2         424.778748
    224, // 3         426.666656
    223, // 4         428.571442
    222, // 5         430.493286
    221, // 6         432.432434
    220, // 7         434.389130
    219, // 8         436.363647
    218, // 9         438.356171
    217, // 10        440.366974
    216, // 11        442.396301
    215, // 12        444.444458
    214, // 13        446.511627
    213, // 14        448.598145
    212, // 15        450.704224
    211, // 16        452.830200
    210, // 17        454.976318
    209, // 18        457.142853
    208, // 19        459.330139
    207, // 20        461.538452
    206, // 21        463.768127
    205, // 22        466.019409
    204, // 23        468.292694
    203, // 24        470.588226
    202, // 25        472.906403
    201, // 26        475.247528
    200, // 27        477.611938
    199, // 28        480.000000
    198, // 29        482.412048
    197, // 30        484.848480
    196, // 31        487.309631
    195, // 32        489.795929
    194, // 33        492.307678
    193, // 34        494.845367
    192, // 35        497.409332
    191, // 36        500.000000
    190, // 37        502.617798
    189, // 38        505.263153
    188, // 39        507.936493
    187, // 40        510.638306
    186, // 41        513.368958
    185, // 42        516.129028
    184, // 43        518.918945
    183, // 44        521.739136
    182, // 45        524.590149
    181, // 46        527.472534
    180, // 47        530.386719
    179, // 48        533.333313
    178, // 49        536.312866
    177, // 50        539.325867
    176, // 51        542.372864
    175, // 52        545.454529
};
#elif( configUse_freq == configUse_freq_168M )
//168M
static const uint16_t period_tab[] =
{
    397,  // 0         421.052643
    396,  // 1         422.907501
    394,  // 2         424.778748
    392,  // 3         426.666656
    390,  // 4         428.571442
    389,  // 5         430.493286
    387,  // 6         432.432434
    385,  // 7         434.389130
    383,  // 8         436.363647
    382,  // 9         438.356171
    380,  // 10        440.366974
    378,  // 11        442.396301
    376,  // 12        444.444458
    375,  // 13        446.511627
    373,  // 14        448.598145
    371,  // 15        450.704224
    369,  // 16        452.830200
    368,  // 17        454.976318
    366,  // 18        457.142853
    364,  // 19        459.330139
    362, //207, // 20        461.538452
    361,  // 21        463.768127
    359,  // 22        466.019409
    357,  // 23        468.292694
    355,  // 24        470.588226
    354, //202, // 25        472.906403
    352,  // 26        475.247528
    350,  // 27        477.611938
    348, //199*7/4, // 28        480.000000
    347,  // 29        482.412048
    345,  // 30        484.848480
    343,  // 31        487.309631
    341,  // 32        489.795929
    340,  // 33        492.307678
    338,  // 34        494.845367
    336, //192*7/4, // 35        497.409332
    334,  // 36        500.000000
    333,  // 37        502.617798
    331,  // 38        505.263153
    329, //188*7/4, // 39        507.936493
    327,  // 40        510.638306
    326,  // 41        513.368958
    324, //185*7/4, // 42        516.129028
    322,  // 43        518.918945
    320,  // 44        521.739136
    319,  // 45        524.590149
    317,  // 46        527.472534
    315,  // 47        530.386719
    313,  // 48        533.333313
    312,  // 49        536.312866
    310,  // 50        539.325867
    308,  // 51        542.372864
    306,  // 52        545.454529
};
#endif



static const uint8_t pre_period_idx_tab[] =
{
#if( configUse_freq == configUse_freq_72M )
    15,       //461538   K2 hang in the air
    19,       //473684  K3 press the pen

    21,       //480000   K4 hang in the air
    26,       //496551   K5  press the pen

    29,       //507042   K0 hang in the air
    32,       //517985  K1 press the pen
#else
    20,  // 461.538452   K2 hang in the air
    25,  // 472.906403   K3 press the pen
    28,  // 480.000000   K4 hang in the air
    35,  // 497.409332   K5  press the pen
    39,  // 507.936493   K0 hang in the air
    42,  // 516.129028   K1 press the pens
#endif
};


void pwm_period_set(uint32_t period)
{
    timer_autoreload_value_config(TIMER0, period);
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, (period>>1));
    timer_event_software_generate(TIMER0, TIMER_EVENT_SRC_UPG);
}

void pwm_period_set_by_idx(uint8_t idx)
{
    timer_autoreload_value_config(TIMER0, period_tab[idx]);
    if(g_scan.scan_mode == PRESS_SCAN_MODE || g_scan.scan_mode == PRESCAN_MODE_Y || g_scan.scan_mode == PRESCAN_MODE_X)
	{
		//TIMER_CREP(TIMER0) = PRESSURE_CHARGING_TIME;
		g_scan.pwmChargeCount = PRESSURE_CHARGING_TIME;
	}
	else
	{
		//TIMER_CREP(TIMER0) = COORDINATE_CHRAGING_TIME;
		g_scan.pwmChargeCount = COORDINATE_CHRAGING_TIME;
	}
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, (period_tab[idx]>>1));
    timer_single_pulse_mode_config(TIMER0,TIMER_SP_MODE_REPETITIVE);
    timer_event_software_generate(TIMER0, TIMER_EVENT_SRC_UPG);
}

void pwm_period_set_by_pre_idx(uint8_t idx)
{
    uint8_t period_idx = pre_period_idx_tab[idx];
    
    timer_autoreload_value_config(TIMER0, period_tab[period_idx]);
    if(g_scan.scan_mode == PRESS_SCAN_MODE || g_scan.scan_mode == PRESCAN_MODE_Y || g_scan.scan_mode == PRESCAN_MODE_X)
	{
		//TIMER_CREP(TIMER0) = PRESSURE_CHARGING_TIME;
		g_scan.pwmChargeCount = PRESSURE_CHARGING_TIME;
	}
	else
	{
		//TIMER_CREP(TIMER0) = COORDINATE_CHRAGING_TIME;
		g_scan.pwmChargeCount = COORDINATE_CHRAGING_TIME;
	}
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, (period_tab[period_idx]>>1));
    timer_single_pulse_mode_config(TIMER0,TIMER_SP_MODE_REPETITIVE);
    timer_event_software_generate(TIMER0, TIMER_EVENT_SRC_UPG);
}


uint8_t pwm_period_idx_get_by_pre_idx(uint8_t idx)
{
    return pre_period_idx_tab[idx];
}

