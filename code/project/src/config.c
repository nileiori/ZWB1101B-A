#include <stdio.h>
#include <string.h>
#include "gd32f3x0.h"
#include "config.h"
#include "gpio.h"
#include "led.h"
#include "iqs.h"
#include "hid_descriptor.h"

extern const STRUCT_STRING_SD004 sd004;

system_config_t g_sys_cfg;

android_control android_handle = {0,0,0};

const android_control android_procTab[] =
{
    {0,0,0},
    {0,0,0},
    {1,0,0},
    {1,0,0},
    {1,1,1},
};

void config_android_handle_set(uint8_t direction, uint8_t save)
{
    if(direction > 4)return;
    android_handle = android_procTab[direction];
    g_sys_cfg.android_para = android_handle.android_ul;
    if(save)
    {
        config_store();
    }
}

void config_android_handle_get(void)
{
    system_config_t cfg;
    memcpy(&cfg, (uint8_t*)(uint32_t)SYSTEM_CONFIG_ADDRESS, sizeof(system_config_t));
    android_handle.android_ul = cfg.android_para;
    g_sys_cfg.rotate_mode = cfg.rotate_mode;
}


void u16saveu32(uint32_t *dst,const uint16_t *src)
{
    uint8_t tmp;

    tmp = *src++;
    *dst = tmp<<24;
    tmp = *src++;
    *dst |= tmp<<16;
    tmp = *src++;
    *dst |= tmp<<8;
    tmp = *src++;
    *dst |= tmp;
}

static void product_info_calibration(void)
{
    uint32_t addr = PRODUCT_CONFIG_ADDRESS;
    uint32_t vid;
    uint32_t pid;
    uint32_t tmepBuf[8] = {0ul,};
    uint8_t i,j,k;
    
    vid = *(uint32_t*)addr;
    pid = *(uint32_t*)(addr + 4);

    if(vid != MY_VID || pid != MY_PID)
    {
        fmc_unlock();
        
        fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
        
        fmc_page_erase(addr);
        fmc_page_erase(SYSTEM_CONFIG_ADDRESS);
        
        fmc_word_program(addr, MY_VID);
        fmc_word_program(addr + 4, MY_PID);
        j = sizeof(sd004.string)/sizeof(uint16_t);
        k = j % 4;
        j /= 4;
        j += k;
        for(i = 0;i < j;i++)
        {
            u16saveu32(tmepBuf+i,sd004.string+i*4);
        }
        for(i = 0;i < 8;i++)
        {
            fmc_word_program(addr + 8 + i*4, tmepBuf[i]);
        }
        
        fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
        
        fmc_lock();
    }

}

void config_dfu_flag_set(uint8_t flag)
{
    if (g_sys_cfg.dfu_flag != flag)
    {
        g_sys_cfg.dfu_flag = flag;

        config_store();
    }
}

uint8_t config_channel_get(void)
{
    return g_sys_cfg.channel;
}

void config_channel_set(uint8_t channel, uint8_t save)
{
    g_sys_cfg.channel = channel;

    if (save)
    {
        config_store();
    }
}

void config_android_direction_set(uint8_t direction, uint8_t save)
{
    g_sys_cfg.android_direction = direction;

    if (save)
    {
        config_store();
    }
}

uint8_t config_android_direction_get(void)
{
    return g_sys_cfg.android_direction;
}

void config_android_support_set(uint8_t support, uint8_t save)
{
    g_sys_cfg.android_support = support;

    if (save)
    {
        config_store();
    }
}

uint8_t config_android_support_get(void)
{
    return g_sys_cfg.android_support;
}

uint32_t config_footled_duty_get(void)
{
    return g_sys_cfg.footled_duty;
}
uint32_t config_track_pad_duty_get(void)
{
    return g_sys_cfg.trackpad_duty;
}
uint32_t config_track_pad_mode_get(void)
{
    return g_sys_cfg.trackpad_mode;
}

void config_footled_duty_set(uint8_t duty)
{
    uint32_t tmp = g_sys_cfg.channel;
    
    if (g_sys_cfg.footled_duty != duty)
    {
        g_sys_cfg.channel = Default_Report_ID;
        g_sys_cfg.footled_duty = duty;
        
        config_store();
    }
    g_sys_cfg.channel = tmp;
}


static void config_default_set(void)
{    
    
    memset(&g_sys_cfg, 0, sizeof(system_config_t));
    
    g_sys_cfg.android_support = 1;
    g_sys_cfg.channel = Default_Report_ID;
    g_sys_cfg.footled_duty = DEFAULTFOOTLIGHT;
    g_sys_cfg.trackpad_duty = DEFAULTTRACKPADLIGHT;
    g_sys_cfg.trackpad_mode = TRACK_TYPE_MOUSE;  
    g_sys_cfg.rotate_mode = e_TrackPad_rotate_degree_0;
    android_handle = android_procTab[0];
    g_sys_cfg.android_para = android_handle.android_ul;

    //crc32 = crc_block_data_calculate(&g_sys_cfg.dfu_flag,( sizeof(g_sys_cfg) / sizeof(uint32_t) ) - 1);
    //g_sys_cfg.crc = crc32;//END_FLAG_VALUE;
    
    config_store();
}

void config_store(void)
{
    uint32_t addr = SYSTEM_CONFIG_ADDRESS;
    uint32_t *pdata;
    uint32_t len;
    uint32_t i;

    crc_data_register_reset();
    g_sys_cfg.crc = crc_block_data_calculate(&g_sys_cfg.dfu_flag,( sizeof(g_sys_cfg) / sizeof(uint32_t) ) - 1);
    pdata = (uint32_t*)&g_sys_cfg;
    
    fmc_unlock();
    
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);

    fmc_page_erase(addr);

    len = sizeof(system_config_t);
    for (i = 0; i < len; i += 4) 
    {
        fmc_word_program(addr, *pdata);
        addr += 4;
        pdata += 1;
    }

    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);

    fmc_lock();
}

void config_load(void)
{    
    uint32_t crc32;
    memcpy(&g_sys_cfg, (uint8_t*)(uint32_t)SYSTEM_CONFIG_ADDRESS, sizeof(system_config_t));
    crc32 = crc_block_data_calculate(&g_sys_cfg.dfu_flag,( sizeof(g_sys_cfg) / sizeof(uint32_t) ) - 1);
    
     if(crc32 != g_sys_cfg.crc)
    {     
        config_default_set();
    }
}

void track_pad_duty_set(uint16_t duty)
{    
    if (duty > 99)
    {
        duty = 99;
    }
    else if (duty < 10)
    {
        duty = 0;
    }    
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_2, duty);
}

void config_track_pad_duty_set(uint16_t duty)
{
    //uint32_t tmp = g_sys_cfg.channel;
    
    if (g_sys_cfg.trackpad_duty != duty)
    {
        //g_sys_cfg.channel = Default_Report_ID;
        g_sys_cfg.trackpad_duty = duty;
        
        //config_store();
    }
    //g_sys_cfg.channel = tmp;
}

void config_track_pad_mode_set(uint8_t mode)
{
    uint32_t tmp = g_sys_cfg.channel;
    
    if (g_sys_cfg.trackpad_mode != mode)
    {
        g_sys_cfg.channel = Default_Report_ID;
        g_sys_cfg.trackpad_mode = mode;
        
        config_store();
    }
    g_sys_cfg.channel = tmp;
}

void config_init(void)
{
    rcu_periph_clock_enable(RCU_CRC);
    crc_deinit();
    crc_input_data_reverse_config(CRC_INPUT_DATA_NOT);
    //crc_reverse_output_data_enable();
    product_info_calibration();

    config_load();
}

void config_led_foot_duty_set(void)
{
    led_foot_duty_set(g_sys_cfg.footled_duty);
}

void config_led_track_pad_duty_set(void)
{
    track_pad_duty_set(g_sys_cfg.trackpad_duty);
}

uint8_t config_get_rotate_mode(void)
{
    return g_sys_cfg.rotate_mode;
}

void config_trackpad_rotate_set(uint8_t usbRev,uint8_t save)
{
    g_sys_cfg.rotate_mode = usbRev;
    uint32_t tmp;

    switch(g_sys_cfg.rotate_mode)
    {
    case e_TrackPad_rotate_degree_90:

        break;
    case e_TrackPad_rotate_degree_180:

        break;
    case e_TrackPad_rotate_degree_270:

        break;
    default:
        g_sys_cfg.rotate_mode = e_TrackPad_rotate_degree_0;
        break;
    }
    if(save)
    {
        tmp = g_sys_cfg.channel;
        g_sys_cfg.channel = Default_Report_ID;  
        g_sys_cfg.save = 1;
        config_store();
        g_sys_cfg.channel = tmp;
    }
    
}

void soft_reset(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}


