#include "gd32f3x0.h"
#include "gpio.h"
#include "string.h"
#include "stdlib.h"
#include "system.h"
#include "message.h"
#include "scan.h"


static message_t msg = NULL;

void message_init( void )
{
    msg = (message_t)malloc(sizeof(message));
    configASSERT( msg );
    memset(msg->msg_buf,0,sizeof(msg->msg_buf));
    msg->size  = MESSAGE_LENS;
    msg->lget  = 0;
    msg->lput  = 0;
    msg->nData = 0;
}

void message_exit(void)
{
    //release memory space
    free(msg);
    msg = NULL;

}

message_info_t message_receive( void )
{
    message_info_t pData;

    if(NULL == msg)
    {
        return NULL;
    }
    if(msg->lget == msg->lput && 0 == msg->nData)
    {
        return NULL;
    }   

    pData = &msg->msg_buf[msg->lget++];
    msg->lget %= msg->size;    
    msg->nData--;

    return pData;
}


void message_send( message_info data )
{

    if(msg->lget == msg->lput && msg->nData)
    {
        return ;
    }
    msg->msg_buf[msg->lput++] = data;
    msg->lput %= msg->size;
    msg->nData++;

}

void message_transmit(uint8_t type,uint8_t value)
{
    message_info msg_info;
    msg_info.type  = type;
    msg_info.value = value;

    message_send(msg_info);
}

#include "fct.h"
void message_process_ant_x(uint8_t msg_val)
{
#ifdef  configUse_fct
    fct_single_ant_test(msg_val, 0);
#endif
}

void message_process_ant_y(uint8_t msg_val)
{
    #ifdef  configUse_fct
    fct_single_ant_test(0,msg_val);
    #endif
}

void message_process_ant_freq(uint8_t msg_val)
{
    #ifdef  configUse_fct
    fct_pwm_period_set_by_idx(msg_val);
    #endif
}

void message_process_sys_halt(uint8_t msg_val)
{
    (void)msg_val;
}

const msg_handle message_func_tab[e_msg_type_max] =
{
    message_process_ant_x,
    message_process_ant_y,
    message_process_ant_freq,
    message_process_sys_halt
};
void message_process( void )
{
    message_info_t pMsg;
#ifdef  configUse_fct
    if(fct_is_destroy())
    {
        message_exit();
        fct_exit();
        return;
    }
#endif
    pMsg = msg_rev;
    if( pMsg )
    {
        configASSERT( msg_type < e_msg_type_max );
        message_func_tab[msg_type](msg_value);
    }
}

