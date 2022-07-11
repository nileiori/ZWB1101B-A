///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \file  usbd_conf.h
//    \brief the header file of USBFS device-mode configuration
//
//    \version 2017-06-06, V1.0.0, firmware for GD32F3x0
//    \version 2019-06-01, V2.0.0, firmware for GD32F3x0
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef USBD_CONF_H
#define USBD_CONF_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "usb_conf.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define USBD_CFG_MAX_NUM             1
#define USBD_ITF_MAX_NUM             2
#define USB_STR_DESC_MAX_SIZE        64

// USB feature -- self powered 
#define USBD_SELF_POWERED

// USB user string supported 
// #define USB_SUPPORT_USER_STRING_DESC 

#define USBD_DYNAMIC_DESCRIPTOR_CHANGE_ENABLED

#define USB_STRING_COUNT             4

#define CUSTOMHID_IN_EP                    EP2_IN
#define CUSTOMHID_OUT_EP                   EP2_OUT

#define CUSTOMHID_IN_PACKET                USB_CUSTOMHID_DATA_SIZE
#define CUSTOMHID_OUT_PACKET               USB_CUSTOMHID_DATA_SIZE
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // USBD_CONF_H 
