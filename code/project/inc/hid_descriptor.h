
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright (C) 2020 HanvonUgee Technology Corp. All rights reserved.
//FileName: hid_descriptor.h
//Brief: hid_descriptor head file
//Platform:GD32F350xxx
//Ver: V1.0.0
//Date:2020.07.15
//Author: 
//Modify: Felix_wang
//	1.ÕûÀí´úÂë
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef HID_DESCRIPTOR_H
#define HID_DESCRIPTOR_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DESC_CONFIG_WORD(a) (a&0xFF),((a>>8)&0xFF)

// The DESC_CONFIG_DWORD() macro is implemented for convinence.  Since the 
//    configuration descriptor array is a BYTE array, each entry needs to be a
//    BYTE in LSB format.  The DESC_CONFIG_DWORD() macro breaks up a DWORD into 
//    the appropriate BYTE entries in LSB.

#define DESC_CONFIG_DWORD(a) (a&0xFF),((a>>8)&0xFF),((a>>16)&0xFF),((a>>24)&0xFF)

// The DESC_CONFIG_BYTE() macro is implemented for convinence.  The 
//    DESC_CONFIG_BYTE() macro provides a consistant macro for use with a byte
//    when generating a configuratin descriptor when using either the 
//    DESC_CONFIG_WORD() or DESC_CONFIG_DWORD() macros.

#define DESC_CONFIG_BYTE(a) (a)



#define HID_RPT01_SIZE          177 //-75 // changed 286 to 287 ,2009-12-15

#if(Support_Android == 0)
    #define HID_RPT02_SIZE         109//107+23+2// 130//237//
#else
    #define HID_RPT02_SIZE          109//107+2////133//134//245//
#endif

#define HID_RPT03_SIZE          36

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef  struct{
 uint8_t bLength;
 uint8_t bDscType;
 uint16_t string[16];
} STRUCT_STRING_SD004;

typedef enum
{
	eUSB_HID0_INTERFACE = 0x0,   
	eUSB_HID1_INTERFACE = 0x1,  
	eUSB_HID2_INTERFACE = 0x2,
}USB_HID_INTERFACE_ENUM;

typedef struct
{
    uint8_t bLength;
    uint8_t bDscType;
    uint8_t string[6];
}string103;
extern string103 sd103;

typedef struct
{
    uint8_t bLength;
    uint8_t bDscType;
    uint8_t string[2];
}string109;
extern string109 sd109;

typedef struct
{
    uint8_t bLength;
    uint8_t bDscType;
    uint8_t string[10];
}string120;
extern string120 sd120;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern uint8_t* usbd_strings[];

extern  const uint8_t hid_report_desc01[HID_RPT01_SIZE];
extern  const uint8_t hid_report_desc02[HID_RPT02_SIZE];
extern  const uint8_t hid_report_desc03[HID_RPT03_SIZE];



#endif /* HID_DESCRIPTOR_H */


