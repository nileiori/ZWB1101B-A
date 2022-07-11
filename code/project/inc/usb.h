#ifndef __USB_H__
#define __USB_H__

#include "usb_core.h"

#define USB_RX_BUFF_SIZE        ( 20 + 1 )
#define USB_TX_BUFF_SIZE        64

#define REPORT_CHANNEL_MOUSE        0x01
#define REPORT_CHANNEL_TABLET       0x02
#define REPORT_CHANNEL_USERDEF      0x04


typedef struct
{
    __IO uint8_t ep1_tx_complete;
    __IO uint8_t ep2_tx_complete;
    __IO uint8_t ep3_tx_complete;
    
    __IO uint8_t ep3_rx_complete;
    
} usb_status_t;


extern usb_status_t g_usb_status;
extern uint8_t usb_rx_buffer[];
extern usb_core_handle_struct usbhs_core_dev;

extern uint8_t usb_tx(uint8_t *report, uint16_t len);
extern void usb_rx_proc(void);
extern void usb_config(void);

#endif

