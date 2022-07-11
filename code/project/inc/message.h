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
    message_info msg_buf[MESSAGE_LENS]; // ��������, .buffer = msg
    int size; // ���д�С��ʹ�õ�ʱ������Դ��size�����Լ��ٽ����ں�̬�Ĳ���
    int lget; // ȡ�������ݵ�ƫ����
    int lput; // �Ŷ������ݵ�ƫ����
    int nData; // ���������ݵĸ���,�����ж϶�����/��

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





