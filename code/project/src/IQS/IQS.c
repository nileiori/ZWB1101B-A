/******************************************************************************
 *                                                                            *
 *                                                                            *
 *                           	Copyright by                                  *
 *                                                                            *
 *                         	  Azoteq (Pty) Ltd                                *
 *                     	  Republic of South Africa                            *
 *                                                                            *
 *                     	   Tel: +27(0)21 863 0033                             *
 *                    	  E-mail: info@azoteq.com                             *
 *                                                                            *
 *============================================================================*
 * @file 	    IQS62x.c							 					      *
 * @brief 	  Arduino example code for IQS62x			              		  *
 * @author 	                                                                  *
 * @version 	V1.0.0                                                        *
 * @date 	    8/25/2020                                                     *
 ******************************************************************************/
#include "gd32f3x0.h"
#include "gpio.h"
#include "drv_pin.h"
#include "ugee_i2c.h"
#include "iqs.h"
#include "usb.h"
#include "scan.h"
#include "led.h"
#include "timer.h"
#include "system.h"
#include "string.h"
#include "taskschedule.h"
#include "usart.h"
#include "app_usart.h"
#include "system.h"
#include "key.h"


//uint8_t iqs_ble_connect = 0;
EventStatus iqs5xx_ble_status(void)
{
//    iqs_ble_connect = 1;

    return DISABLE;
}

void iqs_interrupt_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    //rcu_periph_clock_enable(RCU_CFGCMP);

    pin_in_init(I2C_INT1_PIN_IDX, PIN_MODE_INPUT);
    pin_in_init(I2C_INT2_PIN_IDX, PIN_MODE_INPUT);
//    syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN13);

//    exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);

//    exti_interrupt_flag_clear(EXTI_13);

//    nvic_irq_enable(EXTI4_15_IRQn, 2U, 1U);

//    syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN14);

//    exti_init(EXTI_14, EXTI_INTERRUPT, EXTI_TRIG_RISING);

//    exti_interrupt_flag_clear(EXTI_14);

//    nvic_irq_enable(EXTI4_15_IRQn, 0U, 0U);
}

/********************************iqs572_config start***********************************************/
//ProxFusion IC's
IQS624_t iqs624;	// Create variable for iqs624

/**************************************************************************************************/
/*                                                                                                */
/*                    iqs6xx_buffer_read                                                          */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief   Read Data we need from the IQS6xx
 * @param   p_buffer: pointer to read date
 *          read_address: The address of the data being read
 *          number_of_byte: The number of the data being read
 * @retval  None
 */
void iqs6xx_buffer_read(uint8_t* p_buffer, uint8_t read_address, uint16_t number_of_byte)
{
    ugee_i2c_buffer_read(p_buffer, read_address, IQS624I2C_ADDRESS, number_of_byte);

}
/**************************************************************************************************/
/*                                                                                                */
/*                    iqs6xx_buffer_write                                                         */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief   Write Data we need to the IQS6xx
 * @param   p_buffer: pointer to read date
 *          write_address: The address of the data being write
 *          number_of_byte: The number of the data being read
 * @retval  None
 */
void iqs6xx_buffer_write(uint8_t* p_buffer, uint8_t write_address, uint16_t number_of_byte)
{
    ugee_i2c_buffer_write(p_buffer, write_address, IQS624I2C_ADDRESS, number_of_byte);
}


uint16_t absolute(int16_t value)
{
    return ((value < 0) ? (-value) : value);
}

void wheel_data_report(uint8_t data)
{
    uint8_t channel;
    uint8_t hid_report_report[20] = {0};

    channel = config_channel_get();

    switch(channel)
    {
    case REPORT_CHANNEL_MOUSE:
        if(data == 0xc0)
        {
            hid_report_report[0] = 0x01;
            hid_report_report[1] = 0;
            hid_report_report[2] = 0;
            hid_report_report[3] = 0;
            hid_report_report[4] = 0;
            hid_report_report[5] = 0;
            hid_report_report[6] = 0xff;
            hid_report_report[7] = 0;

            if(e_work_mode_usb == get_report_mode())
            {
                usb_tx(hid_report_report, 8);
            }
            else
            {
                REPORT_DATA_FRAME_FORM(hid_report_report, 8);
            }
        }
        else if(data == 0x80)
        {
            hid_report_report[0] = 0x01;
            hid_report_report[1] = 0;
            hid_report_report[2] = 0;
            hid_report_report[3] = 0;
            hid_report_report[4] = 0;
            hid_report_report[5] = 0;
            hid_report_report[6] = 0x01;
            hid_report_report[7] = 0;

            if(e_work_mode_usb == get_report_mode())
            {
                usb_tx(hid_report_report, 8);
            }
            else
            {
                REPORT_DATA_FRAME_FORM(hid_report_report, 8);
            }
        }

        break;

    case REPORT_CHANNEL_TABLET:
        if(data == 0xc0)
        {
            hid_report_report[0] = 0x01;
            hid_report_report[1] = 0;
            hid_report_report[2] = 0;
            hid_report_report[3] = 0;
            hid_report_report[4] = 0;
            hid_report_report[5] = 0;
            hid_report_report[6] = 0xff;
            hid_report_report[7] = 0;

            if(e_work_mode_usb == get_report_mode())
            {
                usb_tx(hid_report_report, 8);
            }
            else
            {
                REPORT_DATA_FRAME_FORM(hid_report_report, 8);
            }
        }
        else if(data == 0x80)
        {
            hid_report_report[0] = 0x01;
            hid_report_report[1] = 0;
            hid_report_report[2] = 0;
            hid_report_report[3] = 0;
            hid_report_report[4] = 0;
            hid_report_report[5] = 0;
            hid_report_report[6] = 0x01;
            hid_report_report[7] = 0;

            if(e_work_mode_usb == get_report_mode())
            {
                usb_tx(hid_report_report, 8);
            }
            else
            {
                REPORT_DATA_FRAME_FORM(hid_report_report, 8);
            }
        }

        break;


    case REPORT_CHANNEL_USERDEF:
        if(data == 0xc0)
        {
            hid_report_report[0] = 0x02;
            hid_report_report[1] = 0xf0;
            hid_report_report[2] = 0;
            hid_report_report[3] = 0;
            hid_report_report[4] = 0;
            hid_report_report[5] = 0;
            hid_report_report[6] = 0;
            hid_report_report[7] = 0x02;
            hid_report_report[8] = 0;
            hid_report_report[9] = 0;

            if(e_work_mode_usb == get_report_mode())
            {
                usb_tx(hid_report_report, 12);
            }
            else
            {
                REPORT_DATA_FRAME_FORM(hid_report_report, DATA_PACK_LENS);
            }
        }
        else if(data == 0x80)
        {
            hid_report_report[0] = 0x02;
            hid_report_report[1] = 0xf0;
            hid_report_report[2] = 0;
            hid_report_report[3] = 0;
            hid_report_report[4] = 0;
            hid_report_report[5] = 0;
            hid_report_report[6] = 0;
            hid_report_report[7] = 0x01;
            hid_report_report[8] = 0;
            hid_report_report[9] = 0;

            if(e_work_mode_usb == get_report_mode())
            {
                usb_tx(hid_report_report, 12);
            }
            else
            {
                REPORT_DATA_FRAME_FORM(hid_report_report, DATA_PACK_LENS);
            }
        }
        break;

    default:
        break;
    }


}
/**************************************************************************************************/
/*                                                                                                */
/*                    Setup IQS624                                                                */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief Check which setup we need to run for the IQS624 - capacitive & hall rotation
 * @param
 * @retval  None
 */
void iqs624_setup( void )
#if 0
{
    // Buffer to read data into
    uint8_t buffer[10];
    //iqs6xx_buffer_read(buffer, PRODUCT_NUMBER, 1);
    //configASSERT( IQS624_PRODUCT_NR == buffer[0] );

    // Fill buffer with first settings
    buffer[0] = 0x68 | REDO_ATI_ALL;
    iqs6xx_buffer_write(buffer, SYS_SETTINGS, 1);

    buffer[0] = 0x03;
    buffer[1] = 0x0E;
    buffer[2] = 0xC8;
    buffer[3] = 0xFA;
    iqs6xx_buffer_write(buffer, SYS_SETTINGS + 2, 4);

    buffer[0] = 0x14;
    iqs6xx_buffer_write(buffer, AUTO_MODE_TIME, 1);
    buffer[0] = 140;
    buffer[1] = 213;
    iqs6xx_buffer_write(buffer, HALL_SIN, 2);
    buffer[0] = 40;
    iqs6xx_buffer_write(buffer, Touch_Threshold_CH1, 1);
    buffer[0] = 0;
    iqs6xx_buffer_write(buffer, SAR_UI_SETTINGS, 1);

    //buffer[0] = PXS_CH1_TCH_THRES;
    //iqs6xx_buffer_write(buffer,PXS_CH1_THRES, 1);

    // Wait for Redo Ati to complete
    do
    {
        iqs6xx_buffer_read(&iqs624.SystemFlags.SystemFlags, SYSTEM_FLAGS, 1);
    }
    while (iqs624.SystemFlags.InAti);

    // Force a write of the read-only auto compensation values
    // Ratio Scale factor - 2 bytes
    //buffer[0] = 0xD4;
    //res |= iqs6xx_buffer_write(buffer,0xF0, 1);

    //buffer[0] = 0xFF;
    //res |= iqs6xx_buffer_write(buffer,0xF1, 1);

//    buffer[0] = 0xD5;
//    iqs6xx_buffer_write(buffer, 0xF0, 1);

//    buffer[0] = 0x01;
//    iqs6xx_buffer_write(buffer, 0xF0, 1);
  
}
#else
{
	uint8_t status_flag;

	uint8_t DevSetup[] = {
		0xD0,
		(SYSTEM_SETTINGS|ACK_RESET), ACTIVE_CHS, PMU_SETTINGS,
	};

	uint8_t PXSSetup[] = {
		0x40,
		PXS_SETTINGS_0, PXS_SETTINGS_1, PXS_SETTINGS_2,
 		PXS_SETTINGS_3, PXS_SETTINGS_4, PXS_SETTINGS_5,
		PXS_SETTINGS_6, PXS_SETTINGS_7, PXS_SETTINGS_8,
		PXS_SETTINGS_9
	};

	uint8_t PXSUi[] = {
		0x50,
		PXS_UI_SETTINGS_0, PXS_UI_SETTINGS_1, PXS_UI_SETTINGS_2,
		PXS_UI_SETTINGS_3, PXS_UI_SETTINGS_4
	 };

	uint8_t Hall[] = {
		0x70,
		HALL_SETTINGS_0, HALL_SETTINGS_1, HALL_SETTINGS_2,
		HALL_SETTINGS_3, HALL_SETTINGS_4, HALL_SETTINGS_5,
		HALL_SETTINGS_6, HALL_SETTINGS_7, HALL_SETTINGS_8,
		HALL_SETTINGS_9, HALL_SETTINGS_10
	};

	uint8_t RRT[] = {
		0xD3,
		REPORT_RATES_TIMINGS_0, REPORT_RATES_TIMINGS_1, REPORT_RATES_TIMINGS_2,
		REPORT_RATES_TIMINGS_3, REPORT_RATES_TIMINGS_4
	};

	uint8_t ReDoATI[] = {
		0xD0,
		(SYSTEM_SETTINGS|REDO_ATI_ALL)
	};

	uint8_t StreamMode[] = {
		0xD0,
		SYSTEM_SETTINGS
	};

	uint8_t EventMode[] = {
		0xD0,
		(SYSTEM_SETTINGS|EVENT_MODE)
	};

	// Force to Stream Mode (No wait RDY)
	iqs6xx_buffer_write((uint8_t *)StreamMode + 1, StreamMode[0], sizeof(StreamMode) - 1);

	// Device and Power Mode Settings
	iqs6xx_buffer_write(DevSetup + 1, DevSetup[0], sizeof(DevSetup) - 1);

	// Touch-Proximity sensor settings
	iqs6xx_buffer_write(PXSSetup + 1, PXSSetup[0], sizeof(PXSSetup) - 1);

	// Touch-Proximity UI settings
	iqs6xx_buffer_write(PXSUi + 1, PXSUi[0], sizeof(PXSUi) - 1);

	// HALL Sensor Settings
	iqs6xx_buffer_write(Hall + 1, Hall[0], sizeof(Hall) - 1);

	// Device Report Rate Settings
	iqs6xx_buffer_write(RRT + 1, RRT[0], sizeof(RRT) - 1);

	// Redo ATI
	iqs6xx_buffer_write(ReDoATI + 1, ReDoATI[0], sizeof(ReDoATI) - 1);

	// Wait for Redo ATI to complete
	do
	{
		iqs6xx_buffer_read(&status_flag, 0x10, 0x01);

		status_flag &= 0x04;
	} while(status_flag);
	
	// Event Mode
	iqs6xx_buffer_write(EventMode + 1, EventMode[0], sizeof(EventMode) - 1);
	
	// Auto ATI Disable
	iqs6xx_buffer_write(&status_flag,0x71,  0x01);
	status_flag &= 0xfc;
	iqs6xx_buffer_write(&status_flag,0x71,0x01);
}
#endif
#define DEG_THOD    60

/**
 * @brief	Process the Events that the IQS624 reported to us. Here we will run a state machine
 * 			to handle the different modes
 * @param	None
 * @retval	None
 */
void iqs6xx_events_process( void )
{

    uint16_t deg = 0;
    static uint16_t last_deg = 0;
    static uint8_t sp = 0;
    // System flags, Global Events, PXS UI Flags and Hall UI Flags
    //iqs6xx_buffer_read(&iqs624.SystemFlags.SystemFlags, SYSTEM_FLAGS, 1);
    // Read Hall UI Flags
    //iqs6xx_buffer_read(&iqs624.PXSUIFlags.PXSUIFlags, PRX_TCH_UI_Flags, 1);
    // Read Hall UI Flags
#ifndef  configUse_IqsInt
    iqs6xx_buffer_read(&iqs624.HallFlags.HallFlags, Hall_UI_Flags, 1);
    // Read Hall Degrees output
    iqs6xx_buffer_read(&iqs624.HallDegrees.Hall_Degrees_Low, HALL_DGE_LSB, 2);
#endif
    deg = iqs624.HallDegrees.HallDegrees;

    if (absolute(deg - last_deg) >= DEG_THOD)
    {
        // Save the degrees for later use
        last_deg = deg;
        sp++;
        if(sp > 1)
        {
            sp = 0;
            wheel_data_report(iqs624.HallFlags.HallFlags & 0xc0);
        }
    }

}

/********************************iqs624_config end*************************************************/



/********************************iqs572_config start***********************************************/
IQS572_t iqs572;


/**************************************************************************************************/
/*                                                                                                */
/*                    iqs5xx_buffer_read                                                          */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief   Read Data to the IQS5xx
 * @param   p_buffer: pointer to read date
 *          read_address: The address of the data being read
 *          number_of_byte: The number of the data being read
 * @retval  None
 */
void iqs5xx_buffer_read(uint8_t* p_buffer, uint16_t read_address, uint16_t number_of_byte)
{
    ugee_i2c_buffer_read_iqs572(p_buffer, read_address, IQS572I2C_ADDRESS, number_of_byte);
}
/**************************************************************************************************/
/*                                                                                                */
/*                    iqs5xx_buffer_write                                                         */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief   Write Data we need to the IQS5xx
 * @param   p_buffer: pointer to read date
 *          write_address: The address of the data being write
 *          number_of_byte: The number of the data being read
 * @retval  None
 */
void iqs5xx_buffer_write(uint8_t* p_buffer, uint16_t write_address, uint16_t number_of_byte)
{
    ugee_i2c_buffer_write_iqs572(p_buffer, write_address, IQS572I2C_ADDRESS, number_of_byte);

}
/**************************************************************************************************/
/*                                                                                                */
/*                    iqs5xx_comm_end                                                             */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief   End CommunicationWindow command, by writing a single byte (anydata) to the address 0xEEEE
 * @param   None
 * @retval  None
 */
void iqs5xx_comm_end(void)
{
    uint8_t buf;
    iqs5xx_buffer_read(&buf, COMM_END_CODE, 1);
}

void trackpad_data_report( uint8_t data[], enum touchMode mode)
{
    uint8_t hid_report_report[20] = {0};
    uint8_t rotate_mode = config_get_rotate_mode();

    hid_report_report[0] = 0x01;
    if(e_touch_move == mode)
    {
        hid_report_report[1] = 0x01;
    }

    switch(rotate_mode)
    {
    case e_TrackPad_rotate_degree_0:
        hid_report_report[2] = data[1];
        hid_report_report[3] = data[0];
        hid_report_report[4] = data[3];
        hid_report_report[5] = data[2];

        break;
    case e_TrackPad_rotate_degree_90:
        hid_report_report[2] = 0xff - data[3];
        hid_report_report[3] = 0xff - data[2];
        hid_report_report[4] = data[1];
        hid_report_report[5] = data[0];

        break;
    case e_TrackPad_rotate_degree_180:
        hid_report_report[2] = 0xff - data[1];
        hid_report_report[3] = 0xff - data[0];
        hid_report_report[4] = 0xff - data[3];
        hid_report_report[5] = 0xff - data[2];

        break;
    case e_TrackPad_rotate_degree_270:
        hid_report_report[2] = data[3];
        hid_report_report[3] = data[2];
        hid_report_report[4] = 0xff - data[1];
        hid_report_report[5] = 0xff - data[0];

        break;
    default:
        break;
    }

    if(e_work_mode_usb == get_report_mode())
    {
        usb_tx(hid_report_report, 8);
    }
    else
    {
        REPORT_DATA_FRAME_FORM(hid_report_report, 8);
    }

}

/**************************************************************************************************/
/*                                                                                                */
/*                    iqs5xx_gesture_config                                                       */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief       enable or disable Gesture function
 * @param[in]   newvalue: ENABLE or DISABLE
 * @retval      None
 */
void iqs5xx_gesture_config(ControlStatus newvalue)
{
    uint8_t buffer[2];

    if(newvalue)
    {
        buffer[0] = 0x3f;
        buffer[1] = 0x07;
    }
    else
    {
        buffer[0] = 0x00;
        buffer[1] = 0x00;
    }
    iqs5xx_buffer_write(buffer, SINGLE_FINGER_GESTURES, 2);

}

void gesture_report_singleTap( void )
{
    uint8_t hid_report_report[24] = {0};

    hid_report_report[0] = 0x01;
    hid_report_report[1] = 0x01;

    hid_report_report[8] = 0x01;


    if(e_work_mode_usb == get_report_mode())
    {
        usb_tx(hid_report_report, 16);
    }
    else
    {
        hid_report_report[0] = 0x01;
        hid_report_report[1] = 0x01;
        hid_report_report[2] = 0x00;
        hid_report_report[3] = 0x00;
        hid_report_report[4] = 0x00;
        hid_report_report[5] = 0x00;
        hid_report_report[6] = 0x00;
        hid_report_report[7] = 0x00;
        REPORT_DATA_FRAME_FORM(hid_report_report, 8);
        hid_report_report[0] = 0x01;
        hid_report_report[1] = 0x00;
        hid_report_report[2] = 0x00;
        hid_report_report[3] = 0x00;
        hid_report_report[4] = 0x00;
        hid_report_report[5] = 0x00;
        hid_report_report[6] = 0x00;
        hid_report_report[7] = 0x00;
        REPORT_DATA_FRAME_FORM(hid_report_report, 8);
    }

}
//void gesture_report_drag( void )
//{
//
//   trackpad_data_report(iqs572.Relative_XY, e_touch_move);
//}

void gesture_report_click_end( void )
{
    uint8_t hid_report_report[20] = {0};

    hid_report_report[0] = 1;

    if(e_work_mode_usb == get_report_mode())
    {
        usb_tx(hid_report_report, 8);
    }
    else
    {
        REPORT_DATA_FRAME_FORM(hid_report_report, 8);
    }
}

EventStatus iqs5xx_clear_status(void)
{
    iqs572.iqsAct = 0;
    iqs572.iqsClick = 0;
    iqs572.iqsClickCnt = 0;

    return DISABLE;
}

#if defined( configUse_ESDTEST )
uint8_t iqs624errFlg = 0;
uint8_t iqs572errFlg = 0;
static uint8_t iqs624errCnt = 0;
static uint8_t iqs572errCnt = 0;

EventStatus iqs6xx_err_scan(void)
{
    uint8_t buffer[8] = {1,};

    iqs6xx_buffer_read(buffer+7, PRODUCT_NUMBER, 1);
    if( IQS624_PRODUCT_NR == buffer[7] )
    {
        iqs624errCnt = 0;
        iqs624errFlg = 0;
    }
    else
    {
        iqs624errCnt++;
        if(iqs624errCnt > 50)
        {
            iqs624errCnt = 0;
            iqs624errFlg = 1;
        }
    }
#ifdef	configUse_SEGGER_RTT
    //SEGGER_RTT_printf(0, "\r\nIQS624_PRODUCT_NR = 0x%x", buffer[0]);
    usb_tx(buffer, 8);
#endif

    if(iqs624errCnt > 5)
    {
        soft_reset();
    }

    return ENABLE;
}

EventStatus iqs5xx_err_scan(void)
{
    uint8_t buffer[2];

    iqs5xx_buffer_read(buffer, PRODUCT_NUMBER, 2);
    if( IQS572_PRODUCT_NR == buffer[1] )
    {
        iqs572errCnt = 0;
        iqs572errFlg = 0;
    }
    else
    {
        iqs572errCnt++;
        if(iqs572errCnt > 50)
        {
            iqs572errCnt = 0;
            iqs572errFlg = 1;
        }
    }

    if(iqs572errCnt > 5)
    {
        soft_reset();
    }

    return ENABLE;
}

#endif

void iqs572_setup( void )
{
    // Buffer to read data into
    //uint8_t buffer[10];
    //while(RESET == I2C_INT2);
    //iqs5xx_buffer_read(buffer, PRODUCT_NUMBER, 2);
    //configASSERT( IQS572_PRODUCT_NR == buffer[1] );
    iqs5xx_gesture_config(ENABLE);
    iqs5xx_comm_end();

    memset((void*)&iqs572.SingleGes, 0, sizeof(iqs572));
}

void iqs5xx_track_mode_start(void)
{
    iqs572.iqsStep = 0;
}

uint16_t iqs572_track_mode_data[6];
uint8_t  track_status = 0;
uint16_t track_posi_count = 0;
uint16_t track_minus_count = 0;

void iqs5xx_track_mode_pro(uint16_t Absolute_X, uint16_t Absolute_Y, uint8_t track_mode)
{
    uint8_t hid_report_data[20] = {0};
    int div;

    switch(iqs572.iqsStep)
    {
    case 0:
        iqs572_track_mode_data[0] = Absolute_X;
        iqs572_track_mode_data[1] = Absolute_Y;
        iqs572.iqsStep++;
        break;
    case 1:
        iqs572_track_mode_data[2] = Absolute_X;
        iqs572_track_mode_data[3] = Absolute_Y;
        iqs572.iqsStep++;
        break;
    case 2:
        iqs572_track_mode_data[4] = Absolute_X;
        iqs572_track_mode_data[5] = Absolute_Y;
        iqs572.iqsStep++;
        break;
    case 3:
        iqs572.iqsStep = 0;
        div = (((iqs572_track_mode_data[2]-iqs572_track_mode_data[0])*(iqs572_track_mode_data[5]-iqs572_track_mode_data[1])) - \
               ((iqs572_track_mode_data[3]-iqs572_track_mode_data[1])*(iqs572_track_mode_data[4]-iqs572_track_mode_data[0])));
        if(div > 15)
        {
            if(track_posi_count++ > 1)
            {
                track_status = 0;
                track_minus_count = 0;
            }
            else if(track_status == 1)
            {
                if(track_minus_count > 2)return;
            }
            if(TRACK_TYPE_WHEEL == track_mode)
            {
                hid_report_data[0] = 0x01;
                hid_report_data[6] = 0x01;
                if(e_work_mode_usb == get_report_mode())
                {
                    usb_tx(hid_report_data, 8);
                }
                else
                {
                    REPORT_DATA_FRAME_FORM(hid_report_data, 8);
                }
            }
            else if(TRACK_TYPE_CUSTOM == track_mode)
            {
                hid_report_data[0] = 0x02;
                hid_report_data[1] = 0xf0;
                hid_report_data[7] = 0x04;
                if(e_work_mode_usb == get_report_mode())
                {
                    usb_tx(hid_report_data, 12);
                }
                else
                {
                    REPORT_DATA_FRAME_FORM(hid_report_data, DATA_PACK_LENS);
                }
            }
        }
        else if(div < -15)
        {
            if(track_minus_count++ > 1)
            {
                track_status = 1;
                track_posi_count = 0;
            }
            else if(track_status == 0)
            {
                if(track_posi_count > 2)return;
            }
            if(TRACK_TYPE_WHEEL == track_mode)
            {
                hid_report_data[0] = 0x01;
                hid_report_data[6] = 0xff;
                if(e_work_mode_usb == get_report_mode())
                {
                    usb_tx(hid_report_data, 8);
                }
                else
                {
                    REPORT_DATA_FRAME_FORM(hid_report_data, 8);
                }
            }
            else if(TRACK_TYPE_CUSTOM == track_mode)
            {
                hid_report_data[0] = 0x02;
                hid_report_data[1] = 0xf0;
                hid_report_data[7] = 0x08;
                if(e_work_mode_usb == get_report_mode())
                {
                    usb_tx(hid_report_data, 12);
                }
                else
                {
                    REPORT_DATA_FRAME_FORM(hid_report_data, DATA_PACK_LENS);
                }
            }
        }
        break;
    default:
        iqs572.iqsStep = 0;
        break;
    }
}

void iqs5xx_events_process( uint8_t track_mode, uint8_t scan_mode)
{
    uint16_t Relative_X, Relative_Y,Absolute_X,Absolute_Y;

#ifndef  configUse_IqsInt
    if(TRACK_TYPE_MOUSE == track_mode)
    {
        if(e_pen_status_idle != scan_mode)return;

        //NVIC_DisableIRQ(TIMER0_BRK_UP_TRG_COM_IRQn);
        ug_hw_interrupt_disable();
        iqs5xx_buffer_read(&iqs572.SingleGes, GESTURE_EVENTS_0, 13);
        iqs5xx_comm_end();
        //NVIC_EnableIRQ(TIMER0_BRK_UP_TRG_COM_IRQn);
        ug_hw_interrupt_enable();

    }
    else
    {
        if(scan_mode >= e_pen_status_touch)return;
        if(scan_mode == e_pen_status_idle)
            ug_hw_interrupt_disable();
        iqs5xx_buffer_read(&iqs572.SingleGes, GESTURE_EVENTS_0, 13);
        iqs5xx_comm_end();
        if(scan_mode == e_pen_status_idle)
            ug_hw_interrupt_enable();
    }
#endif
    Relative_X = iqs572.Relative_XY[0] << 8;
    Relative_X |= iqs572.Relative_XY[1];
    Relative_Y = iqs572.Relative_XY[2] << 8;
    Relative_Y |= iqs572.Relative_XY[3];

    Absolute_X = iqs572.Absolute_XY[0] << 8;
    Absolute_X |= iqs572.Absolute_XY[1];
    Absolute_Y = iqs572.Absolute_XY[2] << 8;
    Absolute_Y |= iqs572.Absolute_XY[3];

    if(iqs572.SingleGes & 0x1)
    {
        iqs572.iqsAct = 1;
        iqs572.iqsClick = 1;
        gesture_report_singleTap();
        SetTimerTask(TIME_IQS, UG_TICK_PER_SECOND/2);
        iqs572.iqsClickCnt++;
        if(iqs572.iqsClickCnt >= 2)
        {
            iqs572.iqsClickCnt = 0;
            iqs572.iqsClick = 0;
        }
    }
    else if((Relative_X | Relative_Y) && (1 == iqs572.iqsClick))
    {
        trackpad_data_report(iqs572.Relative_XY, e_touch_move);
        ClrTimerTask(TIME_IQS);
        iqs572.iqsAct = 1;
        iqs572.iqsClick = 1;
        iqs572.dragStatus = 1;
    }
    else
    {
        if(iqs572.dragStatus)
        {
            if( iqs572.SysInfo_0 == 0x80)return;
            iqs572.dragStatus = 0;
            iqs572.iqsAct = 0;
            iqs572.iqsClick = 0;
            iqs572.iqsClickCnt = 0;
        }

        if(Relative_X | Relative_Y)
        {
            iqs572.iqsAct = 1;
            if(TRACK_TYPE_MOUSE == track_mode)
            {
                trackpad_data_report(iqs572.Relative_XY, e_touch_only);
            }
            else
            {
                iqs5xx_track_mode_pro(Absolute_X, Absolute_Y, track_mode);
            }
            SetTimerTask(TIME_IQS, UG_TICK_PER_SECOND/2);
        }
        else if(( iqs572.SysInfo_0 == 0x82) && (0 == iqs572.iqsClick)) //ÊÍ·ÅÍÏ¶¯
        {
            iqs572.iqsClick = 0xff;
            gesture_report_click_end();
            track_status = 0;
            track_posi_count = 0;
            track_minus_count = 0;
        }
    }

}
#ifdef  configUse_IqsInt
static void iqs624_hdr(void *args)
{
    uint8_t scan_mode;

    IQS624_t *iqs = (IQS624_t*)args;
    if(IS_KEY_PRESSED)return;
    if(iqs572.iqsAct)return;

    iqs6xx_buffer_read(&iqs->HallFlags.HallFlags, Hall_UI_Flags, 1);
    // Read Hall Degrees output
    iqs6xx_buffer_read(&iqs->HallDegrees.Hall_Degrees_Low, HALL_DGE_LSB, 2);
    scan_mode = read_hand_pencil_status();
    if(scan_mode < e_pen_status_touch)
        iqs6xx_events_process();
}

static void iqs572_hdr(void *args)
{
    uint8_t scan_mode, track_mode;
    IQS572_t *iqs = (IQS572_t*)args;
    if(IS_KEY_PRESSED)return;
    scan_mode = read_hand_pencil_status();
    track_mode = (uint8_t)config_track_pad_mode_get();

    if(TRACK_TYPE_MOUSE == track_mode)
    {
        if(e_pen_status_idle != scan_mode)return;

    }
    else
    {
        if(scan_mode >= e_pen_status_touch)return;

    }

    iqs5xx_buffer_read(&iqs->SingleGes, GESTURE_EVENTS_0, 13);
    iqs5xx_comm_end();
    iqs5xx_events_process( track_mode, scan_mode);
}

static void iqs_int_install(void)
{
    irq_priority priority =
    {
        .nvic_irq_pre_priority = 0,
        .nvic_irq_sub_priority = 0
    };

    rcu_periph_clock_enable(RCU_GPIOB);
    //rcu_periph_clock_enable(RCU_CFGCMP);

//    pin_in_init(I2C_INT1_PIN_IDX, PIN_MODE_INPUT);
//    pin_in_init(I2C_INT2_PIN_IDX, PIN_MODE_INPUT);

    pin_irq_install(  I2C_INT1_PIN_IDX, PIN_MODE_INPUT_PULLUP,
                      PIN_IRQ_MODE_FALLING,
                      iqs624_hdr,
                      &iqs624,
                      &priority);
    pin_irq_install(  I2C_INT2_PIN_IDX, PIN_MODE_INPUT_PULLDOWN,
                      PIN_IRQ_MODE_RISING,
                      iqs572_hdr,
                      &iqs572,
                      &priority);
}

#else
// The loop function is called in an endless loop
void iqs_loop( void )
{
    uint8_t scan_mode, track_mode;

    if(IS_KEY_PRESSED)return;

    scan_mode = read_hand_pencil_status();
    track_mode = (uint8_t)config_track_pad_mode_get();

    if(RESET == I2C_INT1)
    {
        // handle the loop state
        if(iqs572.iqsAct)return;
#if defined( configUse_ESDTEST )
        iqs6xx_err_scan();
#endif
        if(scan_mode < e_pen_status_touch)
            iqs6xx_events_process();
    }

    if(SET == I2C_INT2)
    {
#if defined( configUse_ESDTEST )
        iqs5xx_err_scan();
#endif
        //if(sys_status.work_mode == e_work_mode_usb)
            iqs5xx_events_process( track_mode, scan_mode);
        //else
        //{
        //    if(iqs_ble_connect)
        //        iqs5xx_events_process( track_mode, scan_mode);
        //}
    }

}
#endif
/********************************iqs572_config end*************************************************/

/**************************************************************************************************/
/*                                                                                                */
/*                    iqs_init                                                                    */
/*                                                                                                */
/**************************************************************************************************/
/**
 * @brief Check which setup we need to run for the IQS624 - capacitive & hall rotation
 * @param   None
 * @retval  None
 */
void iqs_init(void)
{
    ugee_i2c_config();

    iqs624_setup();

    iqs572_setup();

#ifdef  configUse_IqsInt
    iqs_int_install();
#else
    iqs_interrupt_config();
#endif
}




