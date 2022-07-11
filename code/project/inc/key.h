#ifndef __KEY_H__
#define __KEY_H__

#include "config.h"

#define KEY_QUEUE_SIZE      10

#define IS_KEY_PRESSED  key_status_read()

typedef struct
{
    uint8_t  value;      // ¼üÖµ
    uint8_t  ctrl;       // ¿ØÖÆ¼ü
    
} hotkey_t;


typedef enum
{    
    USER_BUTTON_1,
    USER_BUTTON_2,
    USER_BUTTON_3,
    USER_BUTTON_4,
    USER_BUTTON_5,
    USER_BUTTON_6,
    USER_BUTTON_7,
    USER_BUTTON_8,
    USER_BUTTON_MAX
} user_button_t;

void key_report(void);
void key_init(void);
EventStatus  key_scan(void);
void user_button_init(void);
EventStatus button_scan(void);
uint8_t key_status_read(void);
EventStatus key_longPressReset(void);



#endif

