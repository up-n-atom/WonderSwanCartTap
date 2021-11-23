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

#include <stddef.h>

#include <libopencm3/stm32/desig.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include <libopencm3/usb/cdc.h>
#include <libopencm3/usb/dfu.h>
#include <libopencm3/usb/usbd.h>

#include "platform.h"
#include "usb.h"

extern void tap_set_config(usbd_device *dev, uint16_t wValue);
extern void cdcacm_set_config(usbd_device *dev, uint16_t wValue);

static const struct usb_device_descriptor __dev_desc = {
    .bLength = USB_DT_DEVICE_SIZE,
    .bDescriptorType = USB_DT_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0xef, /* Miscellaneous Device */
    .bDeviceSubClass = 2, /* Common Class */
    .bDeviceProtocol = 1, /* Interface Association Descriptor */
    .bMaxPacketSize0 = 64,
#if 1
    /* Todo: Register own id @ https://pid.codes/ */
    .idVendor = 0x0483,
    .idProduct = 0xDF11,
    .bcdDevice = 0x0200,
#endif
    .iManufacturer = 1,
    .iProduct = 2,
    .iSerialNumber = 3,
    .bNumConfigurations = 1,
};

static const struct usb_endpoint_descriptor __uart_comm_endp[] = {{
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = 0x82,
    .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
    .wMaxPacketSize = 16,
    .bInterval = 255,
}};

static const struct usb_endpoint_descriptor __uart_data_endp[] = {{
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = 0x01,
    .bmAttributes = USB_ENDPOINT_ATTR_BULK,
    .wMaxPacketSize = 64,
    .bInterval = 1,
}, {
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = 0x81,
    .bmAttributes = USB_ENDPOINT_ATTR_BULK,
    .wMaxPacketSize = 64,
    .bInterval = 1,
}};

static const struct {
    struct usb_cdc_header_descriptor header;
    struct usb_cdc_call_management_descriptor call_mgmt;
    struct usb_cdc_acm_descriptor acm;
    struct usb_cdc_union_descriptor cdc_union;
} __attribute__((packed)) __uart_cdcacm_func_descs = {
    .header = {
        .bFunctionLength = sizeof(struct usb_cdc_header_descriptor),
        .bDescriptorType = CS_INTERFACE,
        .bDescriptorSubtype = USB_CDC_TYPE_HEADER,
        .bcdCDC = 0x0110,
    },
    .call_mgmt = {
        .bFunctionLength = sizeof(struct usb_cdc_call_management_descriptor),
        .bDescriptorType = CS_INTERFACE,
        .bDescriptorSubtype = USB_CDC_TYPE_CALL_MANAGEMENT,
        .bmCapabilities = 0,
        .bDataInterface = 1,
    },
    .acm = {
        .bFunctionLength = sizeof(struct usb_cdc_acm_descriptor),
        .bDescriptorType = CS_INTERFACE,
        .bDescriptorSubtype = USB_CDC_TYPE_ACM,
        .bmCapabilities = 0,
    },
    .cdc_union = {
        .bFunctionLength = sizeof(struct usb_cdc_union_descriptor),
        .bDescriptorType = CS_INTERFACE,
        .bDescriptorSubtype = USB_CDC_TYPE_UNION,
        .bControlInterface = 0,
        .bSubordinateInterface0 = 1,
    }
};

static const struct usb_interface_descriptor __uart_comm_iface[] = {{
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 0,
    .bAlternateSetting = 0,
    .bNumEndpoints = 1,
    .bInterfaceClass = USB_CLASS_CDC,
    .bInterfaceSubClass = USB_CDC_SUBCLASS_ACM,
    .bInterfaceProtocol = USB_CDC_PROTOCOL_AT,
    .iInterface = 4, /* Strings index + 1 */
    .endpoint = __uart_comm_endp,
    .extra = &__uart_cdcacm_func_descs,
    .extralen = sizeof(__uart_cdcacm_func_descs)
}};

static const struct usb_interface_descriptor __uart_data_iface[] = {{
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 1,
    .bAlternateSetting = 0,
    .bNumEndpoints = 2,
    .bInterfaceClass = USB_CLASS_DATA,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = 0,
    .endpoint = __uart_data_endp,
}};

static const struct usb_iface_assoc_descriptor __uart_assoc = {
    .bLength = USB_DT_INTERFACE_ASSOCIATION_SIZE,
    .bDescriptorType = USB_DT_INTERFACE_ASSOCIATION,
    .bFirstInterface = 0,
    .bInterfaceCount = 2,
    .bFunctionClass = USB_CLASS_CDC,
    .bFunctionSubClass = USB_CDC_SUBCLASS_ACM,
    .bFunctionProtocol = USB_CDC_PROTOCOL_AT,
    .iFunction = 0,
};

/* Mimic DFU descriptor */
static const struct usb_tap_descriptor {
    uint8_t bLength;
    /* todo: define a descriptor type */
    uint8_t bDescriptorType;
    /* todo: define attributes */
    uint8_t bmAttributes;
    uint16_t wTransferSize;
    uint16_t bcdTAPVersion;
} __attribute__((packed)) __tap_func = {
    .bLength = sizeof(struct usb_tap_descriptor),
    .bDescriptorType = 0xff,
    .bmAttributes = 0xff,
    .wTransferSize = USB_CONTROL_BUF_SIZE,
    .bcdTAPVersion = 0x0100,
};

static const struct usb_interface_descriptor __tap_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 2,
    .bAlternateSetting = 0,
    .bNumEndpoints = 0,
    .bInterfaceClass = USB_CLASS_VENDOR,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = 5, /* Strings index + 1 */
    .endpoint = NULL,
    .extra = &__tap_func,
    .extralen = sizeof(__tap_func),
};

static const struct usb_interface __ifaces[] = {{
    .num_altsetting = 1,
    .iface_assoc = &__uart_assoc,
    .altsetting = __uart_comm_iface,
}, {
    .num_altsetting = 1,
    .altsetting = __uart_data_iface,
}, {
    .num_altsetting = 1,
    .altsetting = &__tap_iface,
}};

static const struct usb_config_descriptor __config = {
    .bLength = USB_DT_CONFIGURATION_SIZE,
    .bDescriptorType = USB_DT_CONFIGURATION,
    .wTotalLength = 0,
    .bNumInterfaces = 3,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = 0x80,
    .bMaxPower = 0x32,
    .interface = __ifaces,
};

__attribute__((always_inline))
static inline void __usb_reenumerate(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);

    /* No USB disconnect - drive PA12 connected to the USB D+ */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, GPIO12);
    gpio_clear(GPIOA, GPIO12);
    msleep(3);
}

static char serial_no[13];

static const char *__strings[] = {
    "Eleven, Twenty-two",
    "WonderSwan Cartridge Tap",
    serial_no,
    "WS Cart Tap UART",
    "WS Cart Tap TAP"
};

static uint8_t __control_buffer[USB_CONTROL_BUF_SIZE] __attribute__ ((aligned (2)));

usbd_device* usb_setup(void)
{
#ifdef USE_PLL_HSI
#pragma GCC warning "Reliability of USB peripheral is not guaranteed... switch to HSE source."
#endif

    usbd_device *dev;

    rcc_periph_clock_enable(RCC_USB);

    __usb_reenumerate();

    rcc_periph_reset_pulse(RST_USB);

    desig_get_unique_id_as_dfu(serial_no);

    dev = usbd_init(&st_usbfs_v1_usb_driver, &__dev_desc, &__config,
                    __strings, sizeof(__strings) / sizeof(char*),
                    __control_buffer, sizeof(__control_buffer));

    usbd_register_set_config_callback(dev, tap_set_config);
    usbd_register_set_config_callback(dev, cdcacm_set_config);

    return dev;
}
