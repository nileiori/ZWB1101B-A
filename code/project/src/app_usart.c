#include "gd32f3x0.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
//#include "Gd32f3x0_usart.h"
#include "serial.h"
#include "app_usart.h"
#include "drv_pin.h"
#include "usb.h"
#include "system.h"
#include "led.h"
#include "iqs.h"
#include "system.h"
#include "preferences.h"
#include "hid_descriptor.h"
#include "taskschedule.h"
#include "queue.h"

extern system_config_t g_sys_cfg;
extern unsigned int check_sys;

void byte_to_hex_str( char* dest, const unsigned char* source, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;

    for (i = 0; i < sourceLen; i++)
    {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f ;
        highByte += 0x30;
        if (highByte > 0x39)
            dest[i * 2] = highByte + 0x07;
        else
            dest[i * 2] = highByte;

        lowByte += 0x30;
        if (lowByte > 0x39)
            dest[i * 2 + 1] = lowByte + 0x07;
        else
            dest[i * 2 + 1] = lowByte;
    }
    return ;

}

uint16_t* hex8_conver_hex16(uint16_t *pDst, uint8_t *pStr, uint16_t size)
{
    while(size--)
        *pDst++ = *pStr++;

    return pDst;
}


#ifdef  configUse_wireless


static struct
{
    uint8_t  usart_rx_buffer[USART_SERIAL_RB_BUFSZ];
    uint16_t usart_rx_count;

} usart_rx_data = {0,};
static uint8_t usart_publicBuffer[30] = {0,};

unsigned char GetCheckSumHEX(unsigned char *buf, int len )
{
    unsigned char checksum = 0, i;

    for (i = 0; i < len; i++)
    {
        checksum += buf[i];
    }

    //checksum  = ~checksum + 1;

    return checksum;

}

static ug_err_t usart_get_devInfo(ug_uint8_t *pBuffer, ug_uint16_t lens)
{
    ug_uint8_t buf[20];
    ug_uint16_t tmp;
    ug_uint8_t cmd = *(pBuffer + 2);

    buf[0] = FRAME_HEADER_MSB;
    buf[1] = FRAME_HEADER_LSB;
    buf[2] = cmd + 1;
    buf[3] = 0x0a;
    tmp    = MY_VID;
    buf[4] = tmp;
    buf[5] = tmp >> 8;
    tmp    = MY_PID;
    buf[6] = tmp;
    buf[7] = tmp >> 8;
    tmp    = x_physical_max_value;
    buf[8] = tmp;
    buf[9] = tmp >> 8;
    tmp    = y_physical_max_value;
    buf[10] = tmp;
    buf[11] = tmp >> 8;
    tmp    = PressureLevel;
    buf[12] = tmp;
    buf[13] = tmp >> 8;
    buf[14] = GetCheckSumHEX(buf, 14);

    gd32_usart_write(buf, 15);

    return UG_EOK;
}

static ug_err_t usart_get_chrgSta(ug_uint8_t *pBuffer, ug_uint16_t lens)
{
    ug_uint8_t buf[8] = {0,};
    ug_uint8_t cmd = *(pBuffer + 2);
//    sys_status.chrgSta = *(pBuffer + 4);
    buf[0] = FRAME_HEADER_MSB;
    buf[1] = FRAME_HEADER_LSB;
    buf[2] = cmd + 1;
    buf[3] = 0x1;
    buf[4] = sys_status.chrgSta;
    buf[5] = GetCheckSumHEX(buf, 5);

    gd32_usart_write(buf, 6);

    return UG_EOK;
}


static ug_err_t usart_get_handPad_firmwareVer(ug_uint8_t *pBuffer, ug_uint16_t lens)
{
    ug_uint8_t buf[20] = {0,};
    ug_uint8_t cmd = *(pBuffer + 2);
    ug_uint8_t len;

    buf[0] = FRAME_HEADER_MSB;
    buf[1] = FRAME_HEADER_LSB;
    buf[2] = cmd + 1;
    len = sizeof(UG_HAND_PAD_FIRMWARE_VER);
    buf[3] = len;
    memcpy(buf + 4, UG_HAND_PAD_FIRMWARE_VER, len);
    buf[4 + len] = GetCheckSumHEX(buf, 4 + len);

    gd32_usart_write(buf, 4 + len + 1);

    return UG_EOK;
}

static ug_err_t usart_get_ble_firmwareVer(ug_uint8_t *pBuffer, ug_uint16_t lens)
{

    sys_status.fw.status = TRUE;
    memcpy(sys_status.fw.ble_fw, pBuffer + 4, lens);

    return UG_EOK;
}

static ug_err_t usart_get_batt_value(ug_uint8_t *pBuffer, ug_uint16_t lens)
{
    ug_uint8_t buf[15] = {0,};

    sys_status.ble_batt = *(pBuffer + 4);
    if(sys_status.ble_batt > 100)sys_status.ble_batt = 100;
    //sprintf((char*)buf, "%d", sys_status.ble_batt);
    //byte_to_hex_str((char*)dst,sys_status.ble_batt,6);
    //hex8_conver_hex16(sd109.string + 9, (ug_uint8_t*)buf, 3);
    sd109.string[1] = sys_status.ble_batt;
    buf[0] = 0x02;
    buf[1] = 0xf2;
    buf[2] = 0x01;
    buf[3] = sys_status.ble_batt;
    buf[4] = sys_status.chrgSta;
    if(sys_status.work_mode == e_work_mode_usb)
    {
        usb_tx(buf, 12);
    }
    else
    {
        memmove(buf + 4, buf, 5);
        BLE_TRANS_CMD_FORM(buf, 0xf3, 5);
    }

    return UG_EOK;
}

static ug_err_t usart_get_devName(ug_uint8_t *pBuffer, ug_uint16_t lens)
{
    ug_uint8_t buf[20] = {0,};
    ug_uint8_t cmd = *(pBuffer + 2);
    ug_uint8_t len;

    buf[0] = FRAME_HEADER_MSB;
    buf[1] = FRAME_HEADER_LSB;
    buf[2] = cmd + 1;
    len = strlen(sys_status.ble_name) + 1;
    buf[3] = len;
    memcpy(buf + 4, sys_status.ble_name, len);
    buf[4 + len] = GetCheckSumHEX(buf, 4 + len);

    gd32_usart_write(buf, 4 + len + 1);

    return UG_EOK;
}

static ug_err_t usart_get_ble_mac(ug_uint8_t *pBuffer, ug_uint16_t lens)
{
    ug_uint8_t *p = pBuffer + 4;
    ug_uint16_t tmp;

    sys_status.ble_mac.status = TRUE;
    memcpy(sys_status.ble_mac.addr, p, 6);
    memcpy(sd103.string, p, 6);

    tmp             = MY_VID;
    sd120.string[0] = tmp >> 8;
    sd120.string[1] = tmp;
    tmp             = MY_PID;
    sd120.string[2] = tmp >> 8;
    sd120.string[3] = tmp;
    memcpy(sd120.string + 4, p, 6);
    //byte_to_hex_str((char*)dst, sys_status.ble_mac.addr, 6);
    //hex8_conver_hex16(sd103.string + 4, dst, 12);

    return UG_EOK;
}

static ug_err_t usart_get_ble_matches(ug_uint8_t *pBuffer, ug_uint16_t lens)
{
    sys_status.ble_match_en = *(pBuffer + 4);

    return UG_EOK;
}

static ug_err_t usart_get_issue_mac(ug_uint8_t *pBuffer, ug_uint16_t lens)
{
    uint8_t data[12] = {0};
    ug_uint8_t result = *(pBuffer + 4);

    data[0] = 0x02;
    data[1] = 0xb4;
    data[2] = 0x11;
    data[3] = 0x12;
    data[4] = 0x01;
    data[5] = 1;//result;

    usb_tx(data, 12);
    if(0 == result)
    {
        sys_status.dongle_mac.status = TRUE;
    }
    return UG_EOK;
}

static ug_err_t usart_get_ble_status(ug_uint8_t *pBuffer, ug_uint16_t lens)
{

    sys_status.ble_connect = *(pBuffer + 4);
    if(sys_status.pdCtrl.flag != 3)return UG_EOK;
    if(sys_status.ble_connect)
    {
        sys_status.sleep.usTime = SLEEP_COUNT_30MINUTES;
        config_android_handle_set(0,0);
    }
    else
    {
        sys_status.sleep.usTime = SLEEP_COUNT_5MINUTES;
        sys_status.ble_match_en = 0;
    }
    return UG_EOK;
}

static ug_err_t usart_ble_led_light_set_response(ug_uint8_t *pBuffer, ug_uint16_t lens)
{
    sys_status.led_light = *(pBuffer + 4);

    return UG_EOK;
}

static ug_err_t usart_ble_led_light_get_response(ug_uint8_t *pBuffer, ug_uint16_t lens)
{
    uint8_t buf[12] = {0};
    //g_sys_cfg.trackpad_duty = *(pBuffer + 4);

    buf[0] = 0x02;
    buf[1] = 0xb4;
    buf[2] = 0x01;
    buf[3] = 0x00;
    buf[4] = 0x01;
    buf[5] = g_sys_cfg.trackpad_duty;

    if(sys_status.work_mode == e_work_mode_usb)
    {
        usb_tx(buf, 12);
    }
    else
    {
        memmove(buf + 4, buf, 6);
        BLE_TRANS_CMD_FORM(buf, 0xf3, 6);
    }
    return UG_EOK;
}


static void __cmd_channel_set(uint8_t *pBuffer, uint8_t *pDst)
{
    uint8_t channel;

    channel = pBuffer[2] & 0x7;
    if (channel != REPORT_CHANNEL_MOUSE && channel != REPORT_CHANNEL_TABLET && channel != REPORT_CHANNEL_USERDEF)
    {
        channel = Default_Report_ID;
    }

    config_channel_set(channel, (pBuffer[2] & 0x80));
	
    pDst[0] = 0x02;
    pDst[1] = 0xb0;
    pDst[2] = 0x01;
	//pDst[3] = channel;
}

static void __cmd_android_set(uint8_t *pBuffer, uint8_t *pDst)
{
    uint8_t direction;
    uint8_t tmp;

    if (pBuffer[3] & 0x01)
    {
        config_channel_set(Default_Report_ID, 0);
        tmp = 1;

        // 开启安卓支持
        direction = pBuffer[4];
        config_android_direction_set(direction, 0);
        config_android_support_set(1, 0);
        config_android_handle_set(3, 0);
        config_trackpad_rotate_set(tmp, 1);

        pDst[0] = 0x02;
        pDst[1] = 0xb2;
        pDst[2] = 0x01;
        pDst[3] = 0x01;
    }
    else if (pBuffer[3] & 0x10)
    {
        check_sys = 1;
    }
    else if (pBuffer[3] & 0x20)
    {
        check_sys = 2;
        if(0 == g_sys_cfg.save)
        {
            android_handle = android_procTab[3];
            g_sys_cfg.rotate_mode = e_TrackPad_rotate_degree_90;
        }
    }
    else
    {
        config_channel_set(Default_Report_ID, 0);
        tmp = 0;

        // 关闭安卓支持
        config_android_support_set(0, 0);
        config_android_handle_set(0, 0);
        config_trackpad_rotate_set(tmp, 1);
        pDst[0] = 0x02;
        pDst[1] = 0xb2;
        pDst[2] = 0x00;
        pDst[3] = 0x01;

    }
}

static void __cmd_led_set(uint8_t *pBuffer, uint8_t *pDst)
{
    uint8_t duty;

    if (pBuffer[3] == 0x01)
    {
        // 设置灯光亮度
        if(pBuffer[4] == 0x01)
        {
            duty = pBuffer[5];

            //track_pad_duty_set(duty);
            //通知蓝牙板点灯
            //sys_status.enterIntoPdTimer = 0;
            sys_status.led_switch = 1;
            sys_status.led_light = (duty != 0);
            //BLE_TRANS_CMD_FORM(data,0x19,1);
            config_track_pad_duty_set(duty);

            pDst[0] = 0x02;
            pDst[1] = 0xb4;
            pDst[2] = 0x01;
            pDst[3] = 0x01;
            pDst[4] = 0x01;
            pDst[5] = 0x01;
//            usb_tx(data, 12);
        }
        else if(pBuffer[4] == 0x02)
        {
            duty = pBuffer[5];

            led_foot_duty_set(duty);
            config_footled_duty_set(duty);

            pDst[0] = 0x02;
            pDst[1] = 0xb4;
            pDst[2] = 0x01;
            pDst[3] = 0x01;
            pDst[4] = 0x02;
            pDst[5] = duty;
        }
    }
    else
    {
        // 读取灯光亮度
        if(pBuffer[4] == 0x01)
        {
            duty = config_track_pad_duty_get();

            pDst[0] = 0x02;
            pDst[1] = 0xb4;
            pDst[2] = 0x01;
            pDst[3] = 0x00;
            pDst[4] = 0x01;
            pDst[5] = 0x01;

//            usb_tx(data, 12);
            //led灯状态获取
            //INQUIRY_BLE_STATUS(data,0x21);
            sys_status.led_light_get = 1;
        }
        else if(pBuffer[4] == 0x02)
        {
            duty = config_footled_duty_get();

            pDst[0] = 0x02;
            pDst[1] = 0xb4;
            pDst[2] = 0x01;
            pDst[3] = 0x00;
            pDst[4] = 0x02;
            pDst[5] = duty;
        }
    }
}

static ug_uint8_t __dongle_protocolParse(uint8_t *pBuffer, uint8_t *pDst)
{
    uint8_t type, ret = 0;
    uint8_t buf[12] = {0,};
    uint16_t tmp;
    char *str;

    if((pBuffer[0] != 0x02) && (pBuffer[0] != 0x80) && (pBuffer[0] != 0xfd))return 0;
    if(pBuffer[0] == 0x02)
    {
        switch (pBuffer[1])
        {
        case 0xb0:
            // 数据通道设置
            __cmd_channel_set(pBuffer, pDst);
            config_trackpad_rotate_set(0, 0);
            //询问电池电量,上报一次电量数据,非必要,只是为了让winqc尽早显示电量....
            //INQUIRY_BLE_STATUS(buf, 0x09);

            break;

        case 0xb2:
            if (pBuffer[2] & 0x20)
            {
                // 安卓支持
                __cmd_android_set(pBuffer, pDst);
            }

            break;

        case 0xb4:
            if (pBuffer[2] & 0x01)
            {
                // 灯光控制
                __cmd_led_set(pBuffer, pDst);
            }
            else if (pBuffer[2] & 0x02)
            {
                if(pBuffer[3] & 0x01)
                {
                    if(pBuffer[5] <= 0x03)
                    {                       
                        config_track_pad_mode_set(pBuffer[5]);
                        iqs5xx_track_mode_start();
						pDst[0] = 0x02;
                        pDst[1] = 0xb4;
                        pDst[2] = 0x02;
                        pDst[3] = 0x01;
                        pDst[4] = 0x01;
                        //pDst[6] = 0x01;
                        //pDst[7] = 0x00;
                    }
                    config_trackpad_rotate_set(*(pBuffer + 6), 0);
                }
                else
                {
                    pDst[0] = 0x02;
                    pDst[1] = 0xb4;
                    pDst[2] = 0x02;
                    pDst[3] = 0x01;
                    pDst[4] = 0x00;
                    pDst[5] = config_track_pad_mode_get();
                    pDst[6] = config_get_rotate_mode();

                }
            }
            else if (pBuffer[2] & 0x08)//休眠时长
            {
                pDst[0] = pBuffer[0];
                pDst[1] = pBuffer[1];
                pDst[2] = pBuffer[2];
                pDst[3] = pBuffer[3];
                pDst[4] = sys_status.sleep.ucTime[0];
                pDst[5] = sys_status.sleep.ucTime[1];

            }
            /*else if (pBuffer[2] == 0x10)
            {
                pDst[0] = pBuffer[0];
                pDst[1] = pBuffer[1];
                pDst[2] = pBuffer[2];
                pDst[3] = 0x00;
                pDst[4] = sys_status.ble_batt;
            }*/

            break;
        case 0xb8:
            if (pBuffer[2] & 0x04)//ble batt
            {
                //pDst[0] = 0x02;
                //pDst[1] = 0xf2;
                //pDst[2] = 0x01;
                //pDst[3] = sys_status.ble_batt;
                //pDst[4] = sys_status.chrgSta;
                //询问电池电量
                INQUIRY_BLE_STATUS(buf,0x09);
                return 0;
            }
            break;
        }
        ret = 12;
    }
    else if(pBuffer[0] == 0x80)
    {
        memcpy(pDst, pBuffer, 3);
        pDst[3] = 0x01;
        type = *(pBuffer + 2);
        switch(type)
        {
        case 0x02:
            memcpy(pDst + 4, UG_DEV_NAME, sizeof(UG_DEV_NAME));
            break;
        case 0x03:
            //memcpy(pDst+4,UG_DEV_SERIAL_NUM,sizeof(UG_DEV_SERIAL_NUM));
            memcpy(pDst + 4, sys_status.dev_serialNo, sizeof(sys_status.dev_serialNo));
            break;
        case 0x04:
            str = strchr((char*)sys_status.fw.ble_fw, '.');
            str = strchr(str + 1, '.');
            memcpy(pDst + 4, sys_status.fw.ble_fw, str - (char*)sys_status.fw.ble_fw);
            break;
        case 0x05:
            str = strchr((char*)sys_status.fw.ble_fw, '.');
            str = strchr(str + 1, '.');
            str++;
            memcpy(pDst + 4, str, 8);
            break;
        case 0x64:
            tmp         = x_physical_max_value;
            pDst[4]     = tmp;
            pDst[5]     = tmp >> 8;
            tmp         = y_physical_max_value;
            pDst[6]     = tmp;
            pDst[7]     = tmp >> 8;
            pDst[8]     = 0;
            pDst[9]     = 0;
            tmp         = PressureLevel;
            pDst[10]    = tmp;
            pDst[11]    = tmp >> 8;
            tmp         = Line_PerInch;
            pDst[12]    = tmp;
            pDst[13]    = tmp >> 8;
            pDst[14]     = 0;
            pDst[15]     = 0;
            break;
        case 0x67:
            memcpy(pDst + 4, sys_status.ble_mac.addr, 6);
            break;
        case 0x6e:
            pDst[4] = 0x10;////Dev type 0x10:board  0x30:screen
            pDst[5] = 0x00;
            pDst[6] = 0x0b;
            pDst[7] = 0x08;////key num
            pDst[8] = 0x01;////wheel num
            pDst[9] = 0x01;////touch num
            pDst[10] = 0x03;////extend
            break;
        case 0x83:
            memcpy(pDst + 4, (uint8_t*)UG_HAND_PAD_PROTOCOL_VER, sizeof(UG_HAND_PAD_PROTOCOL_VER));
            break;
        case 0xf1:
            pDst[4] = DATA_PACK_LENS;
            break;
        default:
            break;
        }
        ret = 15;
    }
    else if(pBuffer[0] == 0xfd)
    {
        pDst[0] = 0xfd;
        pDst[1] = 0xfd;
        pDst[2] = 0x02;
        pDst[3] = 0xf1;
        memcpy(pDst + 10, sys_status.dev_serialNo, 10);
        ret = 20;
    }

    return ( ret );
}

static ug_err_t usart_dongle_parse(ug_uint8_t *pBuffer, ug_uint16_t bufferLen)
{
    ug_uint8_t buf[30] = {0,};
    ug_uint8_t cmd = *(pBuffer + 2);
    ug_uint8_t len;

    buf[0] = FRAME_HEADER_MSB;
    buf[1] = FRAME_HEADER_LSB;
    buf[2] = cmd + 1;
    memset(usart_publicBuffer, 0, sizeof(usart_publicBuffer));
    len = __dongle_protocolParse(pBuffer + 4, usart_publicBuffer);
    if(0 == len)
        return UG_ERROR;
    buf[3] = len;
    memcpy(buf + 4, usart_publicBuffer, len);
    buf[4 + len] = GetCheckSumHEX(buf, 4 + len);

    gd32_usart_write(buf, 4 + len + 1);

    return UG_EOK;
}


const static  struct
{
    ug_uint8_t cmd;
    ug_err_t (*respone)(ug_uint8_t *pBuffer, ug_uint16_t lens);
} usart_cmds[] =
{
    {0x01,      usart_get_devInfo},
    {0x03,      usart_get_devName},
    {0x05,      usart_get_handPad_firmwareVer},
    {0x08,      usart_get_ble_firmwareVer},
    {0x0a,      usart_get_batt_value},
    {0x0b,      usart_get_chrgSta},
    {0x0e,      usart_get_ble_mac},
    {0x10,      usart_get_ble_matches},
    {0x12,      usart_get_issue_mac},
    {0x14,      usart_get_ble_status},
    {0x20,      usart_ble_led_light_set_response},
    {0x22,      usart_ble_led_light_get_response},
    {0xf0,      usart_dongle_parse},
    {0xf2,      usart_dongle_parse}
};

ug_err_t usart_data_analyze(ug_uint8_t *pBuffer, ug_int16_t bufferLen)
{
    ug_uint8_t i, cmd, frameLen,checksum, calChecksum;
    ug_err_t ret = UG_ERROR;
    ug_uint8_t *p = pBuffer;
    ug_uint8_t *pData = NULL;
	
	//ug_hw_interrupt_disable();
    do
    {
        while(*p != FRAME_HEADER_MSB)
        {
            p++;
            bufferLen--;
            if(bufferLen == 0)return UG_ERROR;
        }
        pData = p;
        p++;

        if(*p++ != FRAME_HEADER_LSB)
            return UG_ERROR;

        cmd = *p++;
        frameLen = *p++;
        if(frameLen > USART_SERIAL_RB_BUFSZ)
            return UG_ERROR;
        checksum = *(p + frameLen);
        calChecksum = GetCheckSumHEX(pData, 4 + frameLen);       
        if(checksum != calChecksum)
            return UG_ERROR;

        for(i = 0; i < ITEM_NUM(usart_cmds); i++)
        {
            if(usart_cmds[i].cmd == cmd)
            {
                ret = usart_cmds[i].respone(pData, frameLen);
                break;
            }
        }
		
		if(bufferLen > 0)
		{
			bufferLen -= (frameLen + 5);        
			p += (frameLen + 1);
		}
    }
    while(bufferLen > 0);
	//ug_hw_interrupt_enable();

    return ret;

}

#ifdef configUse_usart_queue_rev

static uint8_t m_uart_queue_buffer[USART_SERIAL_RB_BUFSZ];
static uint8_t m_uart_rx_buffer[USART_SERIAL_RB_BUFSZ];
static Queue	m_queue = NULL;

int QueueIsEmpty( Queue Q )
{
    return Q->Size == 0;
}

int QueueIsFull( Queue Q )
{
    return Q->Size == Q->Capacity;
}

void QueueMakeEmpty(Queue Q)
{
    Q->Size = 0;
    Q->Front = 1;
    Q->Rear = 0;
}

Queue QueueCreate(int MaxElements)
{
    Queue Q;

    Q = (Queue)malloc( sizeof(struct QueueRecord));
    if( Q == NULL )
        return NULL;

    Q->Array = m_uart_queue_buffer;

    if( Q->Array == NULL )
        return NULL;

    Q->Capacity = MaxElements;

    QueueMakeEmpty( Q );

    return Q;
}

int QueueSucc(int Value, Queue Q)
{
    if( ++Value == Q->Capacity )
        Value = 0;
    return Value;
}

void QueueIn(uint8_t X)
{
    if(QueueIsFull(m_queue))
    {
        return ;
    }
    else
    {
        m_queue->Size++;
        m_queue->Rear = QueueSucc( m_queue->Rear, m_queue );
        m_queue->Array[ m_queue->Rear ] = X;
    }
}

void QueueInBuffer(uint8_t *pBuffer, int BufferLen, Queue Q)
{
    int i = 0;
    uint8_t X;

    if((Q->Capacity - Q->Size) > (BufferLen))
    {
        for(i=0; i<BufferLen; i++)
        {
            X = *(pBuffer+i);
            Q->Size++;
            Q->Rear = QueueSucc( Q->Rear, Q );
            Q->Array[ Q->Rear ] = X;
        }
    }
}

uint8_t QueueFrontAndOut2(Queue Q, uint8_t *pFlag)
{
    uint8_t X = 0;

    if(QueueIsEmpty(Q))
    {
        *pFlag = 0;
        return X;
    }

    Q->Size--;
    X = Q->Array[ Q->Front ];
    Q->Front = QueueSucc( Q->Front, Q );

    *pFlag = 1;
    return X;
}

int  QueueOutBuffer(uint8_t *pBuffer, Queue Q)
{
    int count = 0;
    uint8_t *p = NULL;

    if(NULL != p)
    {
        return 0;
    }

    p = pBuffer;

    while( 1 )
    {
        if(QueueIsEmpty(Q))
        {
            break;
        }
        else
        {
            Q->Size--;
            *p++ = Q->Array[ Q->Front ];
            Q->Front = QueueSucc( Q->Front, Q );
            count++;
        }

    }
    return count;
}

void usart_queue_init(void)
{
    m_queue = QueueCreate(USART_SERIAL_RB_BUFSZ);
}
#endif

EventStatus usart_dispose_recvDataTask(void)
#ifdef configUse_usart_queue_rev
{
    uint8_t  i, cmd, frameLen,checksum, calChecksum;
    uint8_t  flag;//队列数据有效标志，1为有效，0为无效（已达到队列尾）
    uint8_t  data;
    uint8_t  length = 0;
    uint8_t  *p = NULL;

    while( 1 )
    {
        data = QueueFrontAndOut2(m_queue, &flag);
        if(flag)
        {
            if(FRAME_HEADER_MSB == data)
            {
                p = m_uart_rx_buffer;
                length = 0;
                *p++ = data;
                length++;
            }
            else
            {
                if(p == NULL)
                {
                    continue;
                }
                *p++ = data;
                length++;
                if(length > 4)
                {
                    cmd = m_uart_rx_buffer[2];
                    frameLen = m_uart_rx_buffer[3];

                    if(length == (frameLen + 5))
                    {
                        checksum = *(m_uart_rx_buffer + 4 + frameLen);
                        calChecksum = GetCheckSumHEX(m_uart_rx_buffer, 4 + frameLen);

                        if(checksum != calChecksum)
                        {
                            break;
                        }

                        for(i = 0; i < ITEM_NUM(usart_cmds); i++)
                        {
                            if(usart_cmds[i].cmd == cmd)
                            {
                                usart_cmds[i].respone(m_uart_rx_buffer, frameLen);
                                p = NULL;
                                break;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            break;
        }
    }

    return ENABLE;
}
#else
{
    static ug_uint8_t   count = 0;
    ug_size_t datalen;
	
    datalen = gd32_usart_read(usart_rx_data.usart_rx_buffer + usart_rx_data.usart_rx_count, USART_SERIAL_RB_BUFSZ - usart_rx_data.usart_rx_count);
    if(0 != datalen)
    {
        usart_rx_data.usart_rx_count += datalen;
        count = 0;
        if(usart_rx_data.usart_rx_count >= USART_SERIAL_RB_BUFSZ)
        {
            usart_rx_data.usart_rx_count = 0;
        }
    }
    else
    {
        count++;
        if(count >= 2)
        {
            count = 0;
            if(usart_rx_data.usart_rx_count >= 5)
            {
                usart_data_analyze(usart_rx_data.usart_rx_buffer, usart_rx_data.usart_rx_count);
#ifdef configUse_SEGGER_RTT
                {
                    ug_uint8_t i;
                    for(i = 0; i < usart_rx_data.usart_rx_count; i++)
                        SEGGER_RTT_printf(0, "%x ", usart_rx_data.usart_rx_buffer[i]);
                    SEGGER_RTT_printf(0, "\r\n");
                }
#endif
            }
            usart_rx_data.usart_rx_count = 0;
        }
    }

    return ENABLE;
}
#endif

#endif

