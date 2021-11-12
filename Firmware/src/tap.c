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
#include <strings.h>

#include <libopencm3/stm32/fsmc.h>

#include <libopencm3/usb/usbd.h>

#include "cartridge.h"
#include "fsmc.h"
#include "tap.h"
#include "timer.h"
#include "usb.h"

__attribute__((always_inline))
static inline void __dump_header(uint16_t *buf)
{
    uint8_t rb0 = 0;

#ifdef STRICT
    fsmc_bus_width_8();
#endif

    (void)cart_mbc_peek(REG_ROM_BANK_0, &rb0);
    (void)cart_mbc_poke(REG_ROM_BANK_0, 0xff);

#ifdef STRICT
    fsmc_bus_width_16();
#endif

    for (size_t i = 0; i < 16; i+=2)
        (void)cart_nor_peek(0x2fff0 | i, buf++);

#ifdef STRICT
    fsmc_bus_width_8();
#endif

    (void)cart_mbc_poke(REG_ROM_BANK_0, rb0);

#ifdef STRICT
    fsmc_bus_width_16();
#endif
}

static enum usbd_request_return_codes __tap_control_request(
    usbd_device *dev, struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
    void (**complete)(usbd_device *usbd_dev, struct usb_setup_data *req))
{
    (void)dev;
    (void)complete;

    if (req->wIndex != 2) return USBD_REQ_NEXT_CALLBACK;

    /* Only accept vendor request */
    if((req->bmRequestType & 0x7f) != (USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_INTERFACE)) return USBD_REQ_NOTSUPP;

    switch(req->bRequest) {
    case TAP_HANDSHK: {
        int ret = cart_handshake();

        if (len && *len) {
            bzero(*buf, *len);
            **buf = ret;
            *len = 1;
        }

        return USBD_REQ_HANDLED;
    }
    case TAP_DUMPHDR:
        if (NULL == len || 16 > *len) return USBD_REQ_NOTSUPP;

        bzero(*buf, *len);

        __dump_header((uint16_t *)*buf);

        *len = 16;

        return USBD_REQ_HANDLED;
    }

    return USBD_REQ_NOTSUPP;
}

void tap_set_config(usbd_device *dev, uint16_t wValue)
{
    (void)wValue;

    tim2_3_setup();

    usbd_register_control_callback(dev, USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_INTERFACE,
                                   USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
                                   __tap_control_request);
}