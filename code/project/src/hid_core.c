#include "hid_core.h"
#include "usbd_std.h"
#include "usbd_int.h"
#include "hid_descriptor.h"
#include "usb.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static uint32_t usbd_hid_altset = 0U;
static uint32_t usbd_hid_protocol = 0U;
static uint32_t usbd_hid_idlestate  = 0U;
usbd_int_cb_struct *usbd_int_fops = NULL;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   \brief      initialize the HID device
//    \param[in]  pudev: pointer to USB device instance
//    \param[in]  config_index: configuration index
//    \param[out] none
//    \retval     USB device operation status
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t  usbd_hid_init (void *pudev, uint8_t config_index)
{
    // initialize Tx endpoint
    usbd_ep_init(pudev, &(configuration_descriptor.hid0_in_endpoint));
    usbd_ep_init(pudev, &(configuration_descriptor.hid1_in_endpoint));
    usbd_ep_init(pudev, &(configuration_descriptor.hid2_in_endpoint));
    usbd_ep_init(pudev, &(configuration_descriptor.hid2_out_endpoint));

    // prepare receive data
    usbd_ep_rx(pudev, EP3_OUT, usb_rx_buffer, USB_RX_BUFF_SIZE);

    return USBD_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    \brief      de-initialize the HID device
//    \param[in]  pudev: pointer to USB device instance
//    \param[in]  config_index: configuration index
//    \param[out] none
//    \retval     USB device operation status
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t  usbd_hid_deinit (void *pudev, uint8_t config_index)
{
    // deinitialize HID endpoints
    usbd_ep_deinit (pudev, EP1_IN);
    usbd_ep_deinit (pudev, EP2_IN);
    usbd_ep_deinit (pudev, EP3_IN);
    usbd_ep_deinit (pudev, EP3_OUT);

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
uint8_t usbd_hid_classreq_handle (void *pudev, usb_device_req_struct *req)
{
    uint16_t len = 0U;
    uint8_t *pbuf = NULL;

    switch (req->bmRequestType & USB_REQ_MASK)
    {
    case USB_CLASS_REQ:
        switch (req->bRequest)
        {
        case GET_REPORT:
            // no use for this driver
            usbd_ctltx(pudev, (uint8_t *)&usbd_hid_idlestate, 1U);  //just return data for android MLI 200722
            break;
        case GET_IDLE:
            usbd_ctltx(pudev, (uint8_t *)&usbd_hid_idlestate, 1U);
            break;

        case GET_PROTOCOL:
            usbd_ctltx(pudev, (uint8_t *)&usbd_hid_protocol, 1U);
            break;

        case SET_REPORT:
            // no use for this driver
            break;

        case SET_IDLE:
            usbd_hid_idlestate = (uint8_t)(req->wValue >> 8);
            break;

        case SET_PROTOCOL:
            usbd_hid_protocol = (uint8_t)(req->wValue);
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
            switch (req->wValue >> 8)
            {
            case HID_REPORT_DESCTYPE:
                switch( req->wIndex)
                {
                case eUSB_HID0_INTERFACE:
                    len = USB_MIN((uint16_t)HID_RPT01_SIZE, req->wLength);
                    pbuf = (uint8_t *)&hid_report_desc01;
                    break;
                case eUSB_HID1_INTERFACE:
                    len = USB_MIN((uint16_t)HID_RPT02_SIZE, req->wLength);
                    pbuf = (uint8_t *)&hid_report_desc02;
                    break;
                case eUSB_HID2_INTERFACE:
                    len = USB_MIN((uint16_t)HID_RPT03_SIZE, req->wLength);
                    pbuf = (uint8_t *)&hid_report_desc03;
                    break;
                }
                break;
            case HID_DESC_TYPE:
                switch( req->wIndex)
                {
                case eUSB_HID0_INTERFACE:
                    len = USB_MIN((uint16_t)USB_HID_DESC_SIZE, req->wLength);
                    pbuf = (uint8_t *)(&(configuration_descriptor.hid0_vendor));
                    break;
                case eUSB_HID1_INTERFACE:
                    len = USB_MIN((uint16_t)USB_HID_DESC_SIZE, req->wLength);
                    pbuf = (uint8_t *)(&(configuration_descriptor.hid1_vendor));
                    break;
                case eUSB_HID2_INTERFACE:
                    len = USB_MIN((uint16_t)USB_HID_DESC_SIZE, req->wLength);
                    pbuf = (uint8_t *)(&(configuration_descriptor.hid2_vendor));
                    break;
                }
                break;
            default:
                break;
            }
            usbd_ctltx (pudev, pbuf, len);
            break;
        case USBREQ_GET_INTERFACE:
            usbd_ctltx (pudev, (uint8_t *)&usbd_hid_altset, 1U);
            break;
        case USBREQ_SET_INTERFACE:
            usbd_hid_altset = (uint8_t)(req->wValue);
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
//    \brief      handle data stage
//    \param[in]  pudev: pointer to USB device instance
//    \param[in]  rx_tx: data transfer direction:
//    \arg        USBD_TX
//    \arg        USBD_RX
//    \param[in]  ep_id: endpoint identifier
//    \param[out] none
//    \retval     USB device operation status
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t  usbd_hid_data_handler (void *pudev, usb_dir_enum rx_tx, uint8_t ep_id)
{
    uint8_t ep_addr;

    if (USB_TX == rx_tx)
    {
        ep_addr = ep_id | 0x80;
        if (ep_addr == EP1_IN)
        {
            g_usb_status.ep1_tx_complete = 1;
        }
        else if (ep_addr == EP2_IN)
        {
            g_usb_status.ep2_tx_complete = 1;
        }
        else if (ep_addr == EP3_IN)
        {
            g_usb_status.ep3_tx_complete = 1;
        }

        return USBD_OK;
    }
    else if ((USB_RX == rx_tx) && (EP3_OUT == ep_id))
    {
        g_usb_status.ep3_rx_complete = 1;

        return USBD_OK;
    }

    return USBD_FAIL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      send keyboard report
//    \param[in]  pudev: pointer to USB device instance经
//    \param[in]  report: pointer to HID report
//    \param[in]  len: data length
//    \param[out] none
//    \retval     USB device operation status
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t usbd_hid_report_send (usb_core_handle_struct *pudev, uint8_t *report, uint16_t len)
{
    uint8_t ep_addr;

    do
    {
        if (report[0] == 0x02)
        {
            if (!g_usb_status.ep3_tx_complete)
                break;

            ep_addr = EP3_IN;
            g_usb_status.ep3_tx_complete = 0;
        }
        else if (report[0] == 0x07)
        {
            if (!g_usb_status.ep2_tx_complete)
                break;

            ep_addr = EP2_IN;
            g_usb_status.ep2_tx_complete = 0;
        }
        else if (report[0] == 0x09 || report[0] == 0x06 || report[0] == 0x01)
        {
            if (!g_usb_status.ep1_tx_complete)
                break;

            ep_addr = EP1_IN;
            g_usb_status.ep1_tx_complete = 0;
        }
        else
        {
            break;
        }

        usbd_ep_tx(pudev, ep_addr, report, len);

        return USBD_OK;

    } while (0);

    return USBD_FAIL;
}
