/* MIT License

  Copyright (c) 2021 Adam Jaremko

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE. */

#include <libopencm3/usb/usbd.h>

#include "winusb_defs.h"
#include "winusb.h"

static struct winusb_compatible_id_descriptor __winusb_cid_desc = {
    .header = {
        .dwLength = sizeof(struct winusb_compatible_id_descriptor_header) + sizeof(struct winusb_compatible_id_function_section),
        .bcdVersion = WINUSB_BCD_VERSION,
        .wIndex = WINUSB_REQ_GET_COMPATIBLE_ID_FEATURE_DESCRIPTOR,
        .bNumSections = 1,
        .reserved = { 0 },
    },
    .functions = {{
        .bInterfaceNumber = 2,
        .reserved0 = { 1 },
        .compatibleId = "WINUSB",
        .subCompatibleId = "",
        .reserved1 = { 0 },
    }}
};

static const struct winusb_extended_property_descriptor __winusb_ex_prop_desc = {
    .header = {
        .dwLength = sizeof(struct winusb_extended_property_descriptor_header) +
                    sizeof(struct winusb_extended_property_feature_descriptor),
        .bcdVersion = WINUSB_BCD_VERSION,
        .wIndex = WINUSB_REQ_GET_EXTENDED_PROPERTIES_OS_FEATURE_DESCRIPTOR,
        .wNumFeatures = 1,
    },
    .features = {{
        .dwSize = sizeof(struct winusb_extended_property_feature_descriptor),
        .dwPropertyDataType = WINUSB_PROP_DATA_TYPE_REG_REG_MULTI_SZ,
        .wPropertyNameLength = sizeof(__winusb_ex_prop_desc.features[0].bPropertyName),
        .bPropertyName = u"DeviceInterfaceGUIDs",
        .dwPropertyDataLength = sizeof(__winusb_ex_prop_desc.features[0].bPropertyData),
        .bPropertyData = u"{73307550-7699-4BDC-970A-7000D5B8D277}\0",
    }}
};

#ifdef PLATFORMIO
static const struct usb_string_descriptor __winusb_str_desc = {
    /* minus .wData null terminator */
    .bLength = 18,
    .bDescriptorType = USB_DT_STRING,
    .wData = u"MSFT100!",
};

static enum usbd_request_return_codes __winusb_descriptor_request(
    __attribute__((unused)) usbd_device *dev, struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
    __attribute__((unused)) usbd_control_complete_callback *complete)
{
    if ((req->bmRequestType & USB_REQ_TYPE_TYPE) != USB_REQ_TYPE_STANDARD) return USBD_REQ_NEXT_CALLBACK;

    if ((req->bRequest == USB_REQ_GET_DESCRIPTOR) && ((req->wValue >> 8) == USB_DT_STRING) && ((req->wValue & 0xff) == 0xee)) {
        *buf = (uint8_t*)(&__winusb_str_desc);
        if (*len > __winusb_str_desc.bLength)
            *len = __winusb_str_desc.bLength;

         return USBD_REQ_HANDLED;
    }

    return USBD_REQ_NEXT_CALLBACK;
}
#endif

static enum usbd_request_return_codes __winusb_control_request(
    __attribute__((unused)) usbd_device *dev, struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
    __attribute__((unused)) usbd_control_complete_callback *complete)
{
    enum usbd_request_return_codes ret = USBD_REQ_NEXT_CALLBACK;

    if (WINUSB_MS_VENDOR_CODE != req->bRequest) return ret;

    if (((req->bmRequestType & USB_REQ_TYPE_RECIPIENT) == USB_REQ_TYPE_DEVICE) &&
        (WINUSB_REQ_GET_COMPATIBLE_ID_FEATURE_DESCRIPTOR == req->wIndex)) {
        *buf = (uint8_t*)&__winusb_cid_desc;
        if (*len > __winusb_cid_desc.header.dwLength)
            *len = __winusb_cid_desc.header.dwLength;

        ret = USBD_REQ_HANDLED;
    } else if (((req->bmRequestType & USB_REQ_TYPE_RECIPIENT) == USB_REQ_TYPE_INTERFACE) &&
               (WINUSB_REQ_GET_EXTENDED_PROPERTIES_OS_FEATURE_DESCRIPTOR == req->wIndex)) {
        *buf = (uint8_t*)&__winusb_ex_prop_desc;
        if (*len > __winusb_ex_prop_desc.header.dwLength)
            *len = __winusb_ex_prop_desc.header.dwLength;

        ret = USBD_REQ_HANDLED;
    }

    return ret;
}

void winusb_set_config(usbd_device *dev, __attribute__((unused)) uint16_t wValue)
{
#ifdef PLATFORMIO
    usbd_register_control_callback(dev, USB_REQ_TYPE_DEVICE, USB_REQ_TYPE_RECIPIENT, __winusb_descriptor_request);
#endif
    usbd_register_control_callback(dev, USB_REQ_TYPE_VENDOR, USB_REQ_TYPE_TYPE, __winusb_control_request);
}
