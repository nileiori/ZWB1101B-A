#include "gd32f3x0_gpio.h"
#include "gpio.h"

#include "preferences.h"


void YOFF_4051_Chip(void)
{
    uint8_t i, num;

#if defined  ( configUse_74hc138_Y)
    (void)num;
    for(i = 0; i < 3; i++)
    {
        pin_out_on(hc4051_y_pin.en[i]);
    }
#else
    num = (ALL_Y_ANTANA_NUM / 8) + (ALL_Y_ANTANA_NUM % 8 != 0);
    for (i = 0; i < num; i++)
    {
        pin_out_on(hc4051_y_pin[i].en);
    }
#endif
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void XOFF_4051_Chip(void)
{
    uint8_t i, num;

#if defined  ( configUse_74hc138_X)
    (void)num;
    for(i = 0; i < 3; i++)
    {
        pin_out_on(hc4051_x_pin.en[i]);
    }
#else
    num = (ALL_X_ANTANA_NUM / 8) + (ALL_X_ANTANA_NUM % 8 != 0);
    for (i = 0; i < num; i++)
    {
        pin_out_on(hc4051_x_pin[i].en);
    }
#endif
}


void ALLOFF_4051_Chip(void)
{
    YOFF_4051_Chip();
    XOFF_4051_Chip();
}

void antenna_open(uint8_t x_ant, uint8_t y_ant)
{
    uint8_t i, ant_num;

    ALLOFF_4051_Chip();

#if defined  ( configUse_74hc138_X )
    if (x_ant > 0 && x_ant <= ALL_X_ANTANA_NUM)
    {
        //选地址
        ant_num = x_ant % 8;
        ant_num = ant_num ? ant_num : 8;
        gd32_pin_write(x_ant_pin[ant_num].p_hc4051->addr0, x_ant_pin[ant_num].no & 0x01);
        gd32_pin_write(x_ant_pin[ant_num].p_hc4051->addr1, (x_ant_pin[ant_num].no >> 1) & 0x01);
        gd32_pin_write(x_ant_pin[ant_num].p_hc4051->addr2, (x_ant_pin[ant_num].no >> 2) & 0x01);

        //选使能脚
        ant_num = (x_ant - 1) / 8;
        for(i = 0; i < 3; i++)
        {
            if(ant_num & (0x1 << i))
            {
                pin_out_on(hc4051_x_pin.en[i]);
            }
            else
            {
                pin_out_off(hc4051_x_pin.en[i]);
            }
        }

    }
#else
		(void)i;(void)ant_num;
        if (x_ant > 0 && x_ant <= ALL_X_ANTANA_NUM)
        {
            gd32_pin_write(x_ant_pin[x_ant].p_hc4051->addr0, x_ant_pin[x_ant].no & 0x01);
            gd32_pin_write(x_ant_pin[x_ant].p_hc4051->addr1, (x_ant_pin[x_ant].no >> 1) & 0x01);
            gd32_pin_write(x_ant_pin[x_ant].p_hc4051->addr2, (x_ant_pin[x_ant].no >> 2) & 0x01);
            pin_out_off(x_ant_pin[x_ant].p_hc4051->en);
        }

#endif
#if defined  ( configUse_74hc138_Y )
    if (y_ant > 0 && y_ant <= ALL_Y_ANTANA_NUM)
    {
        //选地址
        ant_num = y_ant % 8;
        ant_num = ant_num ? ant_num : 8;
        gd32_pin_write(y_ant_pin[ant_num].p_hc4051->addr0, y_ant_pin[ant_num].no & 0x01);
        gd32_pin_write(y_ant_pin[ant_num].p_hc4051->addr1, (y_ant_pin[ant_num].no >> 1) & 0x01);
        gd32_pin_write(y_ant_pin[ant_num].p_hc4051->addr2, (y_ant_pin[ant_num].no >> 2) & 0x01);

        //选使能脚
        ant_num = (x_ant - 1) / 8;
        for(i = 0; i < 3; i++)
        {
            if(ant_num & (0x1 << i))
            {
                pin_out_on(hc4051_y_pin.en[i]);
            }
            else
            {
                pin_out_off(hc4051_y_pin.en[i]);
            }
        }

    }

#else
		(void)i;(void)ant_num;
    if (y_ant > 0 && y_ant <= ALL_Y_ANTANA_NUM)
    {

        gd32_pin_write(y_ant_pin[y_ant].p_hc4051->addr0, y_ant_pin[y_ant].no & 0x01);
        gd32_pin_write(y_ant_pin[y_ant].p_hc4051->addr1, (y_ant_pin[y_ant].no >> 1) & 0x01);
        gd32_pin_write(y_ant_pin[y_ant].p_hc4051->addr2, (y_ant_pin[y_ant].no >> 2) & 0x01);
        pin_out_off(y_ant_pin[y_ant].p_hc4051->en);
    }
#endif
}


