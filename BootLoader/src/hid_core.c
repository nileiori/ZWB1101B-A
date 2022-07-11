///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright (C) 2020 HanvonUgee Technology Corp. All rights reserved.
//FileName: hid_core.c
//Brief: usb hid core code
//Platform:GD32F350xxx
//Ver: V1.0.0
//Date:2020.07.16
//Author:Liming
//Modify: Felix_wang
//	1.ÕûÀí´úÂë
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hid_core.h"
#include "usbd_int.h"
#include "dfu.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define USBD_VID	0x28BD
#define USBD_PID	0xFFF1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t report_buf[USB_CUSTOMHID_DATA_SIZE];
uint8_t usbd_customhid_report_id = 0;
uint8_t flag = 0;

static uint32_t usbd_customhid_altset = 0;
static uint32_t usbd_customhid_protocol = 0;
static uint32_t usbd_customhid_idlestate = 0;

usbd_int_cb_struct *usbd_int_fops = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern uint16_t ReceiveFlag;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Note:it should use the C99 standard when compiling the below codes
// USB standard device descriptor */
__ALIGN_BEGIN const usb_descriptor_device_struct device_descripter __ALIGN_END =
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
    .idVendor = USBD_VID,
    .idProduct = USBD_PID,
    .bcdDevice = 0x0000,
    .iManufacturer = USBD_MFC_STR_IDX,
    .iProduct = USBD_PRODUCT_STR_IDX,
    .iSerialNumber = 0x00,
    .bNumberConfigurations = USBD_CFG_MAX_NUM
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
__ALIGN_BEGIN const usb_descriptor_configuration_set_struct configuration_descriptor __ALIGN_END = 
{
    .Config = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_descriptor_configuration_struct), 
             .bDescriptorType = USB_DESCTYPE_CONFIGURATION 
         },
        .wTotalLength = CUSTOMHID_CONFIG_DESC_SIZE,
        .bNumInterfaces = 0x02,
        .bConfigurationValue = 0x01,
        .iConfiguration = 0x00,
        .bmAttributes = 0x80,
        .bMaxPower = 0x32
    },
    
    ///////////////////////////////////////interface 0 ///////////////////////////////////
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
		 .bInterfaceProtocol = 0x06,
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
		 .wMaxPacketSize = 8,
		 .bInterval = 0x04
	 },
     ////////////////////////////////////////interface 1///////////////////////////////////
    .HID_Interface = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_descriptor_interface_struct), 
             .bDescriptorType = USB_DESCTYPE_INTERFACE 
         },
        .bInterfaceNumber = 0x01,
        .bAlternateSetting = 0x00,
        .bNumEndpoints = 0x02,
        .bInterfaceClass = 0x03,
        .bInterfaceSubClass = 0x00,
        .bInterfaceProtocol = 0x00,
        .iInterface = 0x00
    },

    .HID_VendorHID = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_hid_descriptor_hid_struct),
             .bDescriptorType = CUSTOMHID_DESC_TYPE 
         },
        .bcdHID = 0x0100,
        .bCountryCode = 0x00,
        .bNumDescriptors = 0x01,
        .bDescriptorType = CUSTOMHID_REPORT_DESCTYPE,
        .wDescriptorLength = CUSTOMHID_REPORT_DESC_SIZE,
    },

    .HID_ReportINEndpoint = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_descriptor_endpoint_struct), 
             .bDescriptorType = USB_DESCTYPE_ENDPOINT 
         },
        .bEndpointAddress = CUSTOMHID_IN_EP,
        .bmAttributes = 0x03,
        .wMaxPacketSize = CUSTOMHID_IN_PACKET,
        .bInterval = 0x02
    },

    .HID_ReportOUTEndpoint = 
    {
        .Header = 
         {
             .bLength = sizeof(usb_descriptor_endpoint_struct), 
             .bDescriptorType = USB_DESCTYPE_ENDPOINT
         },
        .bEndpointAddress = CUSTOMHID_OUT_EP,
        .bmAttributes = 0x03,
        .wMaxPacketSize = CUSTOMHID_OUT_PACKET,
        .bInterval = 0x02
    }  
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

__ALIGN_BEGIN uint8_t* usbd_strings[] __ALIGN_END = 
{
    [USBD_LANGID_STR_IDX] = (uint8_t *)&usbd_language_id_desc,
    [USBD_MFC_STR_IDX]      = USBD_STRING_DESC("UGTABLET"),
    [USBD_PRODUCT_STR_IDX]  = USBD_STRING_DESC("TABLET_DFU200718"),
    [USBD_SERIAL_STR_IDX]   = USBD_STRING_DESC("0x0000")
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// USB custom HID device report descriptor
__ALIGN_BEGIN const uint8_t customhid_report_descriptor[CUSTOMHID_REPORT_DESC_SIZE] __ALIGN_END =
{
		 
	0x06,00,0xFF,     //vendor defined
	0x09, 0x01,       //useage undefined(01)
	0xA1, 0x01,       //collection Application 0x01
	0x19, 0x01,       //usage undefined 0x01
	0x29, 0x40,       //usage maximum undefined 0x40
	0x15, 0x00,        //logical min 0
	0x26, 0xff,0x00,   //logical max 255
	0x75, 0x08,        //report size  8
	0x95, 0x40,        //report count 64
	0x81, 0x00,        //input data 0x00
	0x19, 0x01,        //usage undefined 0x01
	0x29, 0x40,        //usage maximum undefined 0x40
	0x91, 0x00,        //output data 0x00
	0xC0,              //end collection
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
__ALIGN_BEGIN const uint8_t hid_report_desc01[HID_RPT01_SIZE] __ALIGN_END ={            
///////////////////Interfce 1 === Keyboard //////////////////////////////// 
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
	0xC0                  //      End Collection  	40
};// end of HID  report descriptor  



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      initialize the HID device
//    \param[in]  pudev: pointer to USB device instance
//    \param[in]  config_index: configuration index
//    \param[out] none
//    \retval     USB device operation status
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t custom_hid_init (void *pudev, uint8_t config_index)
{
	// initialize Tx endpoint

	usbd_ep_init(pudev, &(configuration_descriptor.hid0_in_endpoint));

	usbd_ep_init(pudev, &(configuration_descriptor.HID_ReportINEndpoint));

	// initialize Rx endpoint//
	usbd_ep_init(pudev, &(configuration_descriptor.HID_ReportOUTEndpoint));

	// prepare receive data
	usbd_ep_rx(pudev, CUSTOMHID_OUT_EP, report_buf, USB_CUSTOMHID_DATA_SIZE);

	return USBD_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      de-initialize the HID device
//    \param[in]  pudev: pointer to USB device instance
//    \param[in]  config_index: configuration index
//    \param[out] none
//    \retval     USB device operation status
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t  custom_hid_deinit (void *pudev, uint8_t config_index)
{
	// deinitialize HID endpoints
	usbd_ep_deinit (pudev, CUSTOMHID_IN_EP);
	usbd_ep_deinit (pudev, CUSTOMHID_OUT_EP);
	usbd_ep_deinit (pudev, EP1_IN);

	return USBD_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      handle the HID class-specific requests
//    \param[in]  pudev: pointer to USB device instance
//    \param[in]  req: device class-specific request
//    \param[out] none
//    \retval     USB device operation status
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t custom_hid_req_handler (void *pudev, usb_device_req_struct *req)
{
	uint16_t len = 0;
	uint8_t *pbuf = NULL;
	uint8_t usbd_customhid_report_length = 0;

	switch (req->bmRequestType & USB_REQ_MASK) 
	{
		case USB_CLASS_REQ:
			switch (req->bRequest) 
			{
				case GET_REPORT:
					// no use for this driver
					break;

				case GET_IDLE:
					usbd_ctltx (pudev, (uint8_t *)&usbd_customhid_idlestate, 1);
					break;

				case GET_PROTOCOL:
					usbd_ctltx (pudev, (uint8_t *)&usbd_customhid_protocol, 1);
					break;
				case SET_REPORT:
					flag = 1;
					usbd_customhid_report_id = (uint8_t)(req->wValue);
					usbd_customhid_report_length = (uint8_t)(req->wLength);
					usbd_ctlrx (pudev, report_buf, usbd_customhid_report_length);
					break;
				case SET_IDLE:
					usbd_customhid_idlestate = (uint8_t)(req->wValue >> 8);
					break;
				case SET_PROTOCOL:
					usbd_customhid_protocol = (uint8_t)(req->wValue);
					break;

				default:
					usbd_enum_error (pudev, req);
					return USBD_FAIL; 
			}
			break;
		case USB_STANDARD_REQ:
			// standard device request
			switch(req->bRequest) 
			{
				case USBREQ_GET_DESCRIPTOR:
				switch(req->wValue >> 8) 
				{
					case CUSTOMHID_REPORT_DESCTYPE:        
						switch( req->wIndex)
						{
							case eUSB_HID0_INTERFACE:
								len = USB_MIN((uint16_t)HID_RPT01_SIZE, req->wLength);
								pbuf = (uint8_t *)&hid_report_desc01;
								break;
							case eUSB_HID1_INTERFACE:
								len = USB_MIN(CUSTOMHID_REPORT_DESC_SIZE, req->wLength);
								pbuf = (uint8_t *)&customhid_report_descriptor;
								break;			
						}
						break;
					case CUSTOMHID_DESC_TYPE:
						switch( req->wIndex)
							{
								case eUSB_HID0_INTERFACE:
									len = USB_MIN((uint16_t)USB_CUSTOMHID_DESC_SIZE, req->wLength);
									pbuf = (uint8_t *)(&(configuration_descriptor.hid0_vendor));
									break;
								case eUSB_HID1_INTERFACE:
									len = USB_MIN(USB_CUSTOMHID_DESC_SIZE, req->wLength);
									pbuf = (uint8_t *)(&(configuration_descriptor.HID_VendorHID));
									break;
							}
							break;
					default:
						break;
				}
				usbd_ctltx (pudev, pbuf, len);
				break;
				case USBREQ_GET_INTERFACE:
					usbd_ctltx (pudev, (uint8_t *)&usbd_customhid_altset, 1);
					break;
				case USBREQ_SET_INTERFACE:
					usbd_customhid_altset = (uint8_t)(req->wValue);
					break;
				default:
					break;
			}

			break;
		default:
			break;	   
    }

    return USBD_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      handle data stage
//    \param[in]  pudev: pointer to USB device instance
//    \param[in]  rx_tx: data transfer direction:
//      \arg        USBD_TX
//      \arg        USBD_RX
//    \param[in]  ep_id: endpoint identifier
//    \param[out] none
//    \retval     USB device operation status
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t  custom_hid_data_handler (void *pudev, usb_dir_enum rx_tx, uint8_t ep_id)
{
    if((USB_TX == rx_tx) && ((CUSTOMHID_IN_EP & 0x7F) == ep_id || (EP1_IN & 0x7F) == ep_id  )) 
	{
		set_ugUSBStatusSend(eStatus_IDLE,ep_id|0x80);
        return USBD_OK;
    } 
    else if ((USB_RX == rx_tx) && (CUSTOMHID_OUT_EP == ep_id)) 
    {    
		set_ugUSBStatusRecv(eStatus_IDLE);

        return USBD_OK;
    }

    return USBD_FAIL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      send custom HID report
//    \param[in]  pudev: pointer to USB device instance
//    \param[in]  report: pointer to HID report
//    \param[in]  len: data length
//    \param[out] none
//    \retval     USB device operation status
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t  custom_hid_report_send (usb_core_handle_struct *pudev, uint8_t *report, uint16_t len)
{
    usbd_ep_tx (pudev, CUSTOMHID_IN_EP, report, len);

    return USBD_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void USBD_ReceiveFunc(uint8_t *pUSER, uint16_t length)
{
    uint16_t i;
    
    for (i = 0; i < length; i ++)
    {
        *(pUSER+i) =  report_buf[i];
    }

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////



