///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright (C) 2020 HanvonUgee Technology Corp. All rights reserved.
//FileName: hid_core.h
//Brief: usb hid core code header file 
//Platform:GD32F350xxx
//Ver: V1.0.0
//Date:2020.07.16
//Author:Liming
//Modify: Felix_wang
//	1.ÕûÀí´úÂë
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef HID_CORE_H
#define HID_CORE_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include  "usbd_std.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define USB_CUSTOMHID_DATA_SIZE     64

#define USB_CUSTOMHID_DESC_SIZE     9

#define CUSTOMHID_REPORT_DESC_SIZE  29
#define CUSTOMHID_CONFIG_DESC_SIZE  66   //one interface 41 two interface 41+25 


#define HID_RPT01_SIZE          40  



#define HID_DESC_TYPE                   0x21
#define HID_REPORT_DESCTYPE             0x22

#define CUSTOMHID_DESC_TYPE         0x21
#define CUSTOMHID_REPORT_DESCTYPE   0x22

#define GET_REPORT                      0x01
#define GET_IDLE                        0x02
#define GET_PROTOCOL                    0x03
#define SET_REPORT                      0x09
#define SET_IDLE                        0x0A
#define SET_PROTOCOL                    0x0B


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	eUSB_HID0_INTERFACE = 0x0,   
	eUSB_HID1_INTERFACE = 0x1,  
	eUSB_HID2_INTERFACE = 0x2,
}USB_HID_INTERFACE_ENUM;


#pragma pack(1)

typedef struct
{
    usb_descriptor_header_struct Header;  //!< regular descriptor header containing the descriptor's type and length

    uint16_t bcdHID;                      //!< BCD encoded version that the HID descriptor and device complies to
    uint8_t  bCountryCode;                //!< country code of the localized device, or zero if universal
    uint8_t  bNumDescriptors;             //!< total number of HID report descriptors for the interface
    uint8_t  bDescriptorType;             //!< type of HID report */
    uint16_t wDescriptorLength;           //!< length of the associated HID report descriptor, in bytes
} usb_hid_descriptor_hid_struct;

#pragma pack()

typedef struct
{
    usb_descriptor_configuration_struct        Config;
	
	usb_descriptor_interface_struct            hid0_interface;
    usb_hid_descriptor_hid_struct              hid0_vendor;
    usb_descriptor_endpoint_struct             hid0_in_endpoint;
	
    usb_descriptor_interface_struct            HID_Interface;
    usb_hid_descriptor_hid_struct              HID_VendorHID;
    usb_descriptor_endpoint_struct             HID_ReportINEndpoint;
    usb_descriptor_endpoint_struct             HID_ReportOUTEndpoint;
	

} usb_descriptor_configuration_set_struct;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern uint8_t report_buf[USB_CUSTOMHID_DATA_SIZE];
extern uint8_t* usbd_strings[USB_STRING_COUNT];
extern const usb_descriptor_device_struct device_descripter;
extern const usb_descriptor_configuration_set_struct configuration_descriptor;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// function declarations
// initialize the HID device
uint8_t custom_hid_init (void *pudev, uint8_t config_index);

// de-initialize the HID device
uint8_t custom_hid_deinit (void *pudev, uint8_t config_index);

// handle the custom HID class-specific and standard requests
uint8_t custom_hid_req_handler (void *pudev, usb_device_req_struct *req);

// handle data stage
uint8_t custom_hid_data_handler (void *pudev, usb_dir_enum rx_tx, uint8_t ep_id);

// send custom HID report
uint8_t custom_hid_report_send (usb_core_handle_struct *pudev, uint8_t *report, uint16_t Len);

#endif  // HID_CORE_H
