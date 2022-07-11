#ifndef __MESSAGE_H__
#define __MESSAGE_H__


#include "config.h"

#define MESSAGE_LENS    ( 10 )
typedef struct
{
    uint8_t type;
    uint8_t value;
}message_info,*message_info_t;

typedef struct 
{
    message_info msg_buf[MESSAGE_LENS]; // 缓冲数据, .buffer = msg
    int size; // 队列大小，使用的时候给出稍大的size，可以减少进入内核态的操作
    int lget; // 取队列数据的偏移量
    int lput; // 放队列数据的偏移量
    int nData; // 队列中数据的个数,用来判断队列满/空

}message,*message_t;

typedef void (*msg_handle)(uint8_t msg_val);

#define msg_rev             message_receive()
#define msg_type            pMsg->type
#define msg_value           pMsg->value

enum
{
    e_msg_type_x,
    e_msg_type_y,
    e_msg_type_freq,
    e_msg_type_sys_halt,
    e_msg_type_max,
};


#ifdef __cplusplus
extern "C" {
#endif

void message_init( void );
message_info_t message_receive( void );
void message_send( message_info data );
void message_transmit(uint8_t type,uint8_t value);
void message_process( void );
void message_exit(void);


#ifdef __cplusplus
}
#endif

#endif





