#include "hid_core.h"
#include "usbd_std.h"
#include "usbd_int.h"
#include "hid_descriptor.h"
#include "preferences.h"


// Note:it should use the C99 standard when compiling the below codes
// USB standard device descriptor

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Note:it should use the C99 standard when compiling the below codes 
// USB standard device descriptor 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

__ALIGN_BEGIN usb_descriptor_device_struct device_descripter __ALIGN_END =
{
    .Header = 
     {
         .bLength = USB_DEVICE_DESC_SIZE, 
         .bDescriptorType = USB_DESCTYPE_DEVICE
     },
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = USB_MAX_EP0_SIZE,
    .idVendor = MY_VID,
    .idProduct = MY_PID,
    .bcdDevice = 0x0000,
    .iManufacturer = USBD_MFC_STR_IDX,
    .iProduct = USBD_PRODUCT_STR_IDX,
    .iSerialNumber = USBD_SERIAL_STR_IDX,
    .bNumberConfigurations = USBD_CFG_MAX_NUM
};


__ALIGN_BEGIN usb_descriptor_configuration_set_struct configuration_descriptor __ALIGN_END = 
{
    .config = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_descriptor_configuration_struct), 
             .bDescriptorType = USB_DESCTYPE_CONFIGURATION 
         },
        .wTotalLength = DESC_CONFIG_WORD(0x005B),
        .bNumInterfaces = 0x03,
        .bConfigurationValue = 0x01,
        .iConfiguration = 0x00,
        .bmAttributes = 0x80,
        .bMaxPower = 0x23
    },

	////////////////////////////////////////interface 0 ////////////////////////////////////////

    .hid0_interface = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_descriptor_interface_struct), 
             .bDescriptorType = USB_DESCTYPE_INTERFACE 
         },
        .bInterfaceNumber = 0x00,
        .bAlternateSetting = 0x00,
        .bNumEndpoints = 0x01,
        .bInterfaceClass = 0x03,
        .bInterfaceSubClass = 0x01,
        .bInterfaceProtocol = 0x02,
        .iInterface = 0x00
    },

    .hid0_vendor = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_hid_descriptor_hid_struct), 
             .bDescriptorType = HID_DESC_TYPE 
         },
        .bcdHID = 0x0100,
        .bCountryCode = 0x00,
        .bNumDescriptors = 0x01,
        .bDescriptorType = HID_REPORT_DESCTYPE,
        .wDescriptorLength =  HID_RPT01_SIZE,
    },

    .hid0_in_endpoint = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_descriptor_endpoint_struct), 
             .bDescriptorType = USB_DESCTYPE_ENDPOINT 
         },
        .bEndpointAddress = EP1_IN,
        .bmAttributes = 0x03,
        .wMaxPacketSize = HID_IN_PACKET,
        .bInterval = 0x03
    },
   //////////////////////////////////////////////interface 1 ///////////////////////////////////////////////
	.hid1_interface = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_descriptor_interface_struct), 
             .bDescriptorType = USB_DESCTYPE_INTERFACE 
         },
        .bInterfaceNumber = 0x01,
        .bAlternateSetting = 0x00,
        .bNumEndpoints = 0x01,
        .bInterfaceClass = 0x03,
        .bInterfaceSubClass = 0x01,
        .bInterfaceProtocol = 0x02,
        .iInterface = 0x00
    },

    .hid1_vendor = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_hid_descriptor_hid_struct), 
             .bDescriptorType = HID_DESC_TYPE 
         },
        .bcdHID = 0x0100,
        .bCountryCode = 0x00,
        .bNumDescriptors = 0x01,
        .bDescriptorType = HID_REPORT_DESCTYPE,
        .wDescriptorLength =  HID_RPT02_SIZE,
    },

    .hid1_in_endpoint = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_descriptor_endpoint_struct), 
             .bDescriptorType = USB_DESCTYPE_ENDPOINT 
         },
        .bEndpointAddress = EP2_IN,
        .bmAttributes = 0x03,
        .wMaxPacketSize = 64,
        .bInterval = 0x03
    },

    ///////////////////////////////////////////////interface 2 ////////////////////////////////////////
	.hid2_interface = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_descriptor_interface_struct), 
             .bDescriptorType = USB_DESCTYPE_INTERFACE 
         },
        .bInterfaceNumber = 0x02,
        .bAlternateSetting = 0x00,
        .bNumEndpoints = 0x02,
        .bInterfaceClass = 0x03,
        .bInterfaceSubClass = 0x00,
        .bInterfaceProtocol = 0x00,
        .iInterface = 0x00
    },

    .hid2_vendor = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_hid_descriptor_hid_struct), 
             .bDescriptorType = HID_DESC_TYPE 
         },
        .bcdHID = 0x0100,
        .bCountryCode = 0x00,
        .bNumDescriptors = 0x01,
        .bDescriptorType = HID_REPORT_DESCTYPE,
        .wDescriptorLength =  HID_RPT03_SIZE,
    },

    .hid2_in_endpoint = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_descriptor_endpoint_struct), 
             .bDescriptorType = USB_DESCTYPE_ENDPOINT 
         },
        .bEndpointAddress = EP3_IN,
        .bmAttributes = 0x03,
        .wMaxPacketSize = 12,
        .bInterval = 0x03
    },

	.hid2_out_endpoint = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_descriptor_endpoint_struct), 
             .bDescriptorType = USB_DESCTYPE_ENDPOINT 
         },
        .bEndpointAddress = EP3_OUT,
        .bmAttributes = 0x03,
        .wMaxPacketSize = 21,
        .bInterval = 0x0A
    }
};

// USB language ID Descriptor
__ALIGN_BEGIN const usb_descriptor_language_id_struct usbd_language_id_desc __ALIGN_END =
{
    .Header =
     {
         .bLength = sizeof(usb_descriptor_language_id_struct),
         .bDescriptorType = USB_DESCTYPE_STRING
     },
    .wLANGID = ENG_LANGID
};

#if 0
__ALIGN_BEGIN uint8_t* usbd_strings[] __ALIGN_END =
{
    [USBD_LANGID_STR_IDX] = (uint8_t *)&usbd_language_id_desc,
    [USBD_MFC_STR_IDX] = USBD_STRING_DESC("GigaDevice"),
    [USBD_PRODUCT_STR_IDX] = USBD_STRING_DESC("GD32 USB Keyboard in FS Mode"),
    [USBD_SERIAL_STR_IDX] = USBD_STRING_DESC("GD32F3X0-V1.0.0-3a4b5ec")
};

#endif

//Language code string descriptor
const struct{
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[1];
} sd000 = {
    sizeof(sd000),
    USB_DESCTYPE_STRING,
    {0x0409}
};


#if(OEM_CODE == 1)
    //string 1      //Manufacturer string descriptor
    const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[8];}sd001={
    sizeof(sd001),USB_DESCTYPE_STRING,
    {'U','G','T','A','B','L','E','T'}};
    //string 2      //Product string descriptor
    const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[4];}sd002={
    sizeof(sd002),USB_DESCTYPE_STRING,
    {'E','X','0','8'}};//{'T','A','B','L','E','T'}};
#elif(OEM_CODE == 2)
    //string 1      //Manufacturer string descriptor
    const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[8];}sd001={
    sizeof(sd001),USB_DESCTYPE_STRING,
    {'U','G','T','A','B','L','E','T'}};
    //string 2      //Product string descriptor
    const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[7];}sd002={
    sizeof(sd002),USB_DESCTYPE_STRING,
    {'D','E','C','O',' ','0','1'}};
#else
    //string 1      //Manufacturer string descriptor
    const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[8];}sd001={
    sizeof(sd001),USB_DESCTYPE_STRING,
    {'U','G','T','A','B','L','E','T'}};
    const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[11];}sd002={
    sizeof(sd002),USB_DESCTYPE_STRING,
    //{'B','P','G','1','0','0','2','L','-','B'}};
    {'D','e','c','o',' ','P','r','o',' ','M','W'}};
#endif
//string 3      //serial number
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[6];}sd003={
sizeof(sd003),USB_DESCTYPE_STRING,
{'0','0','0','0','0','0'}};
//string 4      // IC & Name string
const STRUCT_STRING_SD004 sd004=
{
    sizeof(sd004),USB_DESCTYPE_STRING,
    {'U','G','9','0','2','_','B','P','G','1','1','0','1','B'}
};

//string 5      //Firmware date descriptor
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[21];}sd005={
sizeof(sd005),USB_DESCTYPE_STRING,
//{'1','9','-','0','5','-','2','1','_','F','a','c','t','o','r','y','0','1'}};
//{'1','9','-','0','6','-','1','8','_','R','e','l','e','a','s','e','0','1'}};
{'2','0','2','2','-','0','2','-','2','2','_','R','e','l','e','a','s','e','0','0','7'}};

//string 20                                                             //test mode 0       //william 17/03/14
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[12];}sd020={
sizeof(sd020),USB_DESCTYPE_STRING,
{'D','e','f','a','u','l','t',' ','m','o','d','e'}};

//string 21                                                             //test mode 1
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[13];}sd021={
sizeof(sd021),USB_DESCTYPE_STRING,
{'X','_','E','n','e','r','g','y',' ','m','o','d','e'}};

//string 22                                                             //test mode 2
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[13];}sd022={
sizeof(sd022),USB_DESCTYPE_STRING,
{'Y','_','E','n','e','r','g','y',' ','m','o','d','e'}};

//string 23                                                             //test mode 3
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[13];}sd023={
sizeof(sd023),USB_DESCTYPE_STRING,
{'P','_','E','n','e','r','g','y',' ','m','o','d','e'}};

//string 24                                                             //test mode 4       //william  17/04/18
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[9];}sd024={
sizeof(sd024),USB_DESCTYPE_STRING,
{'F','e','r','q',' ','m','o','d','e'}};

//string 25                                                             //test mode 4       //william  17/04/18
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[18];}sd025={
sizeof(sd025),USB_DESCTYPE_STRING,
{'J','i','t','t','e','r','_','E','n','e','r','g','y',' ','m','o','d','e'}};

//string 25                                                             //test mode 4       //william  17/04/18
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[11];}sd026={
sizeof(sd026),USB_DESCTYPE_STRING,
{'J','i','t','t','e','r',' ','m','o','d','e'}};

//string 25                                                             //test mode 4       //william  17/04/18
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[11];}sd027={
sizeof(sd027),USB_DESCTYPE_STRING,
{'S','m','o','o','t','h',' ','m','o','d','e'}};

//string 28                                                             //test mode 4       //william  17/04/18
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[11];}sd028={
sizeof(sd028),USB_DESCTYPE_STRING,
{'X','_','T','i','l','t',' ','m','o','d','e'}};

//string 29                                                             //test mode 4       //william  17/04/18
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[11];}sd029={
sizeof(sd029),USB_DESCTYPE_STRING,
{'Y','_','T','i','l','t',' ','m','o','d','e'}};

const struct{
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[1];
} sd030 = {
    sizeof(sd030),
    USB_DESCTYPE_STRING,
    {0xa518}
};

const struct{
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[1];
} sd031 = {
    sizeof(sd031),
    USB_DESCTYPE_STRING,
    {0xa518}
};

//string 98 62H
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[15];}sd098={
sizeof(sd098),USB_DESCTYPE_STRING,
{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}};

//Product info //String 100 64H
const struct{uint8_t bLength;uint8_t bDscType;uint8_t string[12];}sd100={
sizeof(sd100),USB_DESCTYPE_STRING,
{
    DESC_CONFIG_WORD(x_physical_max_value),              //X max.
    DESC_CONFIG_WORD(y_physical_max_value),              //Y max.
    0x08, 0x01,                                                                                             //Key number ,Wheel
    DESC_CONFIG_WORD(PressureLevel),                     //Pressure Level
    DESC_CONFIG_WORD(Line_PerInch),                      //Line Per Inch (LPI)
    0x00,                                               //X max.(over 0xffff)
    0x00                                                //Y max.(over 0xffff)
}};

//Product info //String 101 65H
const struct{uint8_t bLength;uint8_t bDscType;uint8_t string[2];}sd101={
sizeof(sd101),USB_DESCTYPE_STRING,
{
    0x00,                           //OEM Code
    0x67,                           //Model Number
}};

//string 102 66H
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[21];}sd102={
sizeof(sd102),USB_DESCTYPE_STRING,
{'U','G','E','E',' ','T','e','c','h','n','o','l','o','g','y',' ','C','o','r','p','.'}};

//string 103 addr
string103 sd103 =
{
    sizeof(sd103),
    USB_DESCTYPE_STRING,
    {0,}
};

//string 120 addr
string120 sd120 =
{
    sizeof(sd120),
    USB_DESCTYPE_STRING,
    {0,}
};

//string 104 pen infomation
const struct{uint8_t bLength;uint8_t bDscType;uint8_t string[4];}sd104={
sizeof(sd104),USB_DESCTYPE_STRING,
{
    0x02,      //pen button num, default 2
    0x00,     //pen with eraser 0:not ,1 have
    0x01,     //pen type default 0: normal type, 1:PE1 2:PE2 5:PA3 6:PA6
    0x00
}};

//string 105 android_direction
const struct{uint8_t bLength;uint8_t bDscType;uint8_t string[2];}sd105={
sizeof(sd105),USB_DESCTYPE_STRING,
{
    0x01,
    0x00
}};

//string 109 6DH Battery Capacity
string109 sd109={
sizeof(sd109),USB_DESCTYPE_STRING,
{100,100}};


//Work Size Option //String 110 6eH
const struct{uint8_t bLength;uint8_t bDscType;uint8_t string[8];}sd110={
sizeof(sd110),USB_DESCTYPE_STRING,
{
    0x00,//system or not
    0x10,// Borad,LCD,DONGLE,BIGBoard,BIG LCD
    0x00,//key type
    0x0d,//key num(hotkey & wheel)
    0x08,//HOTKEY
    0x01,//WHEEL
    0x01,//touch num
    0x0b,//other high
//    0x00 //other low
}};

//IC Characteristic //String 111 6fH
const struct{uint8_t bLength;uint8_t bDscType;uint8_t string[4];}sd111={
sizeof(sd111),USB_DESCTYPE_STRING,
{
    0x05,//device id size//180725-M Zih issue: RF Get Device ID
    0x00,//flash size(H)
    0xff, //flash size(L)
}};


//Expand Mode Name //string 121 79H
const struct{uint8_t bLength;uint8_t bDscType;uint8_t string[14];}sd121={
sizeof(sd121),USB_DESCTYPE_STRING,
{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}};

//Expand Information //string 122 7aH
const struct{uint8_t bLength;uint8_t bDscType;uint8_t string[6];}sd122={
sizeof(sd122),USB_DESCTYPE_STRING,
{
    0x00,                       //Hot Key Version
    0x00,                                 //Hot Key Number
    0x00,                                 //Touch Pad Number
    0x00,                                  //Expand Flag
    0x00,                       //Reserved
    0x00,                       //Reserved
}};

//Software Hot Key ON //string 123 7bH
const struct{uint8_t bLength;uint8_t bDscType;uint8_t string[10];}sd123={
sizeof(sd123),USB_DESCTYPE_STRING,
{
    0x48,                      //H
    0x00,                      //
    0x4b,                      //K
    0x00,                      //
    0x20,                      //
    0x00,                      //
    0x4f,                      //O
    0x00,                      //
    0x6e,                      //n
    0x00,                      //
}};

//Software Hot Key ON //string 124 7cH
const struct{uint8_t bLength;uint8_t bDscType;uint8_t string[12];}sd124={
sizeof(sd124),USB_DESCTYPE_STRING,
{
    0x48,                      //H
    0x00,                      //
    0x4b,                      //K
    0x00,                      //
    0x20,                      //
    0x00,                      //
    0x4f,                      //O
    0x00,                      //
    0x66,                      //f
    0x00,                      //
    0x66,                      //f
    0x00,                      //
}};

//Data Specification Verser
const struct{uint8_t bLength;uint8_t bDscType;uint8_t string[4];}sd131={
sizeof(sd131),USB_DESCTYPE_STRING,
{
    0x01,                      //
    0x04,                      //
    0x00,                      //
    0x04                      ////180725-M Zih issue: RF Get Device ID
}};

//string 168 A8H
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[8];}sd168={
sizeof(sd168),USB_DESCTYPE_STRING,
{'2','0','1','8','0','1','0','4'}};

//string 250                                                             //freq adjust
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[11];}sd250={
sizeof(sd250),USB_DESCTYPE_STRING,
{'A','D','J','U','S','T','_','K','0','H','I'}};

//string 251                                                            
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[11];}sd251={
sizeof(sd251),USB_DESCTYPE_STRING,
{'A','D','J','U','S','T','_','K','2','H','I'}};

//string 252                                                             //freq adjust
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[11];}sd252={
sizeof(sd252),USB_DESCTYPE_STRING,
{'A','D','J','U','S','T','_','K','4','H','I'}};

//string 253                                                            
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[12];}sd253={
sizeof(sd253),USB_DESCTYPE_STRING,
{'A','D','J','U','S','T','_','K','0','_','K','5'}};

//string 254                                                            
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[12];}sd254={
sizeof(sd254),USB_DESCTYPE_STRING,
{'A','D','J','U','S','T','_','K','1','M','A','X'}};

//string 255                                                            
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[13];}sd255={
sizeof(sd255),USB_DESCTYPE_STRING,
{'E','X','I','T','_','F','R','E','Q','_','A','D','J'}};


__ALIGN_BEGIN const uint8_t hid_report_desc01[HID_RPT01_SIZE] __ALIGN_END ={
 //=====================Interfce 1 === Mouse =======================
    0x05, 0x01,           //      Usage Page (Generic Desktop)
    0x09, 0x02,           //      Usage (Mouse)
    0xA1, 0x01,           //      Collection (Application)
    0x85, 0x09,           //      Report ID(9)
    0x09, 0x01,           //      Usage (Pointer)
    0xA1, 0x00,           //      Collection (Physical)
    0x05, 0x09,           //      Usage Page (Button)
    0x19, 0x01,           //      Usage MINIMUM (Button 1)
    0x29, 0x03,           //      Usage MAXIMUM (Button 3)
    0x15, 0x00,           //      LOGICAL_MINIMUM (0)
    0x25, 0x01,           //      LOGICAL_MAXIMUM (1)
    0x95, 0x03,           //      Report Count (3)
    0x75, 0x01,           //      Report Size (1)
    0x81, 0x02,           //      INPUT (Data,Var,Abs)
    0x95, 0x05,           //      Report Count (5)
    0x81, 0x01,           //      INPUT (Cnst,Ary,Abs)
    0x05, 0x01,           //      Usage Page (Generic Desktop)
    0x09, 0x30,           //      Usage(X)
    0x09, 0x31,           //      Usage(Y)
    0x26, 0xFF, 0x7F,     //      LOGICAL_MAXIMUM (32767)
    0x95, 0x02,           //      Report Count (2)
    0x75, 0x10,           //      Report Size (16)
    0x81, 0x02,           //      INPUT (Data,Var,Abs)
    0x05, 0x0D,           //       USAGE_PAGE (Digitizers)
    0x09, 0x30,           //       USAGE (Tip Pressure)
    0x26, 0xFF, 0x07,     //       LOGICAL_MAXIMUM (32767)
    0x95, 0x01,           //      Report Count (2)
    0x75, 0x10,           //      Report Size (16)
    0x81, 0x02,           //      INPUT (Data,Var,Abs)
    0xC0,                 //      End Collection
    0xC0,                 //      End Collection
 //=====================Interfce 1 === END =======================

// //=====================Interfce 2 === Mouse =======================
    0x05, 0x01,           //      Usage Page (Generic Desktop)
    0x09, 0x02,           //      Usage (Mouse)
    0xA1, 0x01,           //      Collection (Application)
    0x09, 0x01,           //      Usage (Pointer)
    0xA1, 0x00,           //      Collection (Physical)
    0x85, 0x01,           //      Report ID(1)
    0x05, 0x09,           //      Usage Page (Button)
    0x19, 0x01,           //      Usage MINIMUM (Button 1)
    0x29, 0x05,           //      Usage MAXIMUM (Button 5)
    0x95, 0x05,           //      Report Count (5)
    0x75, 0x01,           //      Report Size (1)
    0x15, 0x00,           //      LOGICAL_MINIMUM (0)
    0x25, 0x01,           //      LOGICAL_MAXIMUM (1)
    0x81, 0x02,           //      INPUT (Data,Var,Abs)
    0x95, 0x03,           //      Report Count (3)
    0x81, 0x01,           //      INPUT (Cnst,Ary,Abs)
    0x05, 0x01,           //      Usage Page (Generic Desktop)
    0x09, 0x30,           //      Usage(X)
    0x09, 0x31,           //      Usage(Y)
    0x95, 0x02,           //      Report Count (2)
    0x75, 0x10,           //      Report Size (16)
    0x16, 0x00, 0x80,     //      LOGICAL_MINIMUM (-32767)
    0x26, 0xFF, 0x7F,     //      LOGICAL_MAXIMUM (32767)
    0x81, 0x06,           //      INPUT (Data,Var,Rel)
//  //=====================Interfce 2 === Wheel ==================
    0x09, 0x38,           //      Usage (Wheel)
    0x15, 0x81,           //      LOGICAL_MINIMUM (-127)
    0x25, 0x7F,           //      LOGICAL_MAXIMUM (127)
    0x95, 0x01,           //      Report Count (1)
    0x75, 0x08,           //      Report Size (8)
    0x81, 0x06,           //      INPUT (Data,Var,Rel)
    0x05, 0x0C,           //      Usage Page (Consumer Devices)
    0x0A, 0x38, 0x02,     //      Usage (AC Pen)
    0x95, 0x01,           //      Report Count (1)
    0x75, 0x08,           //      Report Size (8)
    0x81, 0x06,           //      INPUT (Data,Var,Rel)
    0xC0,                 //      End Collection
    0xC0,                 //      End Collection
// //=====================Interfce 2 === END =======================
 //=====================Interfce 2 === Keyboard =======================
    0x05, 0x01,           //      Usage Page (Generic Desktop)
    0x09, 0x06,           //      Usage (Keyboard)
    0xA1, 0x01,           //      Collection (Application)
    0x85, 0x06,           //      Report ID(6)
    0x05, 0x07,           //      Usage Page (Keyboard/Keypad)
    0x19, 0xE0,           //      Usage MINIMUM (Keyboard Left Control)
    0x29, 0xE7,           //      Usage MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,           //      LOGICAL_MINIMUM (0)
    0x25, 0x01,           //      LOGICAL_MAXIMUM (1)
    0x75, 0x01,           //      Report Size (1)
    0x95, 0x08,           //      Report Count (8)
    0x81, 0x02,           //      INPUT (Cnst,Var,Abs)
    0x05, 0x07,           //      Usage Page (Keyboard/Keypad)
    0x19, 0x00,           //      Usage MINIMUM (Undefined)
    0x29, 0xFF,           //      Usage MAXIMUM
    0x26, 0xFF, 0x00,     //      LOGICAL_MAXIMUM (255)
    0x75, 0x08,           //      Report Size (8)
    0x95, 0x06,           //      Report Count (6)
    0x81, 0x00,           //      INPUT (Data,Ary,Abs)
    0xC0                  //      End Collection    40
};// end of HID  report descriptor

__ALIGN_BEGIN const uint8_t hid_report_desc02[HID_RPT02_SIZE] __ALIGN_END={
 //=====================Interfce 1 ==============================
    0x05, 0x0D,           //       USAGE_PAGE (Digitizers)
    0x09, 0x02,           //       USAGE (Digitizer) Integrated Pen
    0xA1, 0x01,           //       COLLECTION (Application)
    0x85, 0x07,           //       REPORT_ID (7)    //When the firmware wants to send a HID report (containing multi-touch contact information), the Report ID byte should match this number
    0x09, 0x20,           //       USAGE (Stylus)
    0xA1, 0x00,           //       COLLECTION (Physical)
    0x09, 0x42,           //       USAGE (Tip Switch)
    0x09, 0x44,           //       USAGE (Barrel Switch)
    0x09, 0x45,           //       USAGE (Eraser Switch)
    0x09, 0x3C,           //       USAGE (Invert )
 //   0x09, 0x43,           //       USAGE (Second Tip Switch)
 //   0x09, 0x44,           //       USAGE (Barrel Switch)
    0x15, 0x00,           //       LOGICAL_MINIMUM (0)
    0x25, 0x01,           //       LOGICAL_MAXIMUM (1)
    0x75, 0x01,           //       REPORT_SIZE (1)
    0x95, 0x04,           //       REPORT_COUNT (3)
    0x81, 0x02,           //       INPUT (Data,Var,Abs)
    0x95, 0x01,           //       REPORT_COUNT (2)
    0x81, 0x03,           //       INPUT (Cnst,Ary,Abs)
    0x09, 0x32,           //       USAGE (In Range)
    0x95, 0x01,           //       REPORT_COUNT (1)
    0x81, 0x02,           //       INPUT (Data,Var,Abs)
    0x95, 0x02,           //       REPORT_COUNT (2)
    0x81, 0x03,           //       INPUT (Cnst,Ary,Abs)
    0x75, 0x10,           //       REPORT_SIZE (16)
    0x95, 0x01,           //       REPORT_COUNT (1)
    0x35, 0x00,           //       PHYSICAL_MINIMUM (0)                 48
    0xa4,
    0x05, 0x01,           //       USAGE_PAGE (Generic Desktop)
    0x09, 0x30,           //       USAGE (X)
    0x65, 0x13,           //       UNIT (Inch,EngLinear)
    0x55, 0x0D,           //       UNIT_EXPONENT (-3)
    0x46,
    DESC_CONFIG_WORD(x_size),//0xe4, 0x34,//DESC_CONFIG_WORD(X_PHYSICAL_MAX >> 16),
    0x26,
    0xFF, 0x7F,//DESC_CONFIG_WORD(X_PHYSICAL_MAX),//0x72, 0x59,     //       LOGICAL_MAXIMUM (15897)
    0x81, 0x02,
    0x09, 0x31,
    0x46,
    DESC_CONFIG_WORD(y_size),//0xC4, 0x1D,//DESC_CONFIG_WORD(Y_PHYSICAL_MAX >> 16),
    0x26,
    0xFF, 0x7F,//DESC_CONFIG_WORD(Y_PHYSICAL_MAX),     //       LOGICAL_MAXIMUM (9908)
    0x81, 0x02,           //       INPUT (Data,Var,Abs)
    0xB4,
    0x09, 0x30,           //       USAGE (Tip Pressure)
    0x45, 0x00,
    0x26,DESC_CONFIG_WORD(PressureLevel),     //        LOGICAL_MAXIMUM (8191)
    0x81, 0x42,           //       INPUT (Data,Var,Abs)
  //=====================Interfce 0 === Tilt ============================
    0x09, 0x3D,           //       USAGE( X Tilt)
    0x15, 0x81,           //       Logical Minimum (-127)
    0x25, 0x7F,           //       Logical Maximum (127)
    0x75, 0x08,           //       REPORT_SIZE (8)
    0x95, 0x01,           //       REPORT_COUNT (1)
    0x81, 0x02,           //       INPUT (Data,Var,Abs)
    0x09, 0x3E,           //       USAGE( Y Tilt)
    0x15, 0x81,           //       Logical Minimum (-127)
    0x25, 0x7F,           //       Logical Maximum (127)
    0x81, 0x02,           //       INPUT (Data,Var,Abs)
    0xC0,                 //       END_COLLECTION
    0xC0,                 //       END_COLLECTION                               87
 //=====================Interfce 1 ==============================
    #if(Support_Android == 0)
    0x06, 0x03, 0xff,
    0x09, 0x00,
    0xa1, 0x01,
    0x85, 0x0b,
    0x15, 0x00,
    0x26, 0xff, 0x00,
    0x09, 0x00,
    0x75, 0x08,
    0x95, 0x3f,
    0x81, 0x02,
    0xc0
     #endif
};// end of HID 2  report descriptor

__ALIGN_BEGIN const uint8_t hid_report_desc03[HID_RPT03_SIZE] __ALIGN_END={
    0x06, 0x0A, 0xFF,       //        Usage Page (Vendor-Defined 11)
    0x09, 0x01,           //        Usage (Vendor-Defined 11)
    0xA1, 0x01,           //        Collection (Application)
    0x85, 0x02,           //        REPORT_ID (mouse)
    0x09, 0x02,           //        Usage (Vendor-Defined 1)
    0x75, 0x08,           //         Report Size (8)
    0x95, 0x0b,           //         Report Count (20)
    0x15, 0x00,           //        Usage Minimum (0)
    0x26, 0xFF, 0x00,     //        Usage Maximum (255)                                             20
    0x81, 0x02,                     //Output (Data,Var,Abs)
    0x09, 0x03,           //        Usage (Vendor-Defined 3)
    0x75, 0x08,           //         Report Size (8)
    0x95, 0x14,           //         Report Count (20)
    0x15, 0x00,           //        Logical Minimum (0)
    0x26, 0xFF, 0x00,     //        Logical Maximum (255)
    0x91, 0x02,                     //input (Data,Var,Abs)
    0xC0                        //        END_COLLECTION                                                        16
};// end of HID 3  report descriptor

#if 1
__ALIGN_BEGIN uint8_t* usbd_strings[] __ALIGN_END =
{
    (uint8_t *)&sd000,          //+0
    (uint8_t *)&sd001,          //+1
    (uint8_t *)&sd002,          //+2
    (uint8_t *)&sd003,          //+3
    (uint8_t *)&sd004,          //+4
    (uint8_t *)&sd005,          //+5
    (uint8_t *)&sd098,          //+6 62
    (uint8_t *)&sd100,          //+7 64
    (uint8_t *)&sd101,          //+8 65
    (uint8_t *)&sd102,          //+9 66
    (uint8_t *)&sd109,          //+10 6D
    (uint8_t *)&sd110,          //+11 6e
    (uint8_t *)&sd111,          //+12 6f
    (uint8_t *)&sd121,          //+13 79
    (uint8_t *)&sd122,          //+14 7a
    (uint8_t *)&sd123,          //+15 7b
    (uint8_t *)&sd124,          //+16 7c
    (uint8_t *)&sd131,          //+17 83
    (uint8_t *)&sd168,           //+18 A8
    (uint8_t *)&sd020,          //+19   14      //william  17/04/18
    (uint8_t *)&sd021,          //+20 15
    (uint8_t *)&sd022,          //+21 16
    (uint8_t *)&sd023,           //+22 17
    (uint8_t *)&sd024,           //+23 18
    (uint8_t *)&sd025,           //+24   19
    (uint8_t *)&sd026,           //+25   20
    (uint8_t *)&sd027,           //+26   21
    (uint8_t *)&sd028,           //+27   22
    (uint8_t *)&sd029,           //+28   23
    (uint8_t *)&sd103,            //+29   mac addr
    (uint8_t *)&sd104,            //+30  68
    (uint8_t *)&sd105,            //+31  68
    (uint8_t *)&sd120,            //+32  EXIT FREQ ADJUST
    (uint8_t *)&sd030,            //+33  蓝牙DTM测试模式
    (uint8_t *)&sd031             //+34  定频测试模式
};

#endif

