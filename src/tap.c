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

#include <string.h>

#include <libopencm3/stm32/fsmc.h>

#include <libopencm3/usb/usbd.h>

#include "cartridge.h"
#include "fsmc.h"
#include "tap.h"
#include "timer.h"
#include "usb.h"

static struct {
    struct {
        uint8_t buf[USB_CONTROL_BUF_SIZE];
        uint16_t len;
    } dat;
    enum tap_state st8;
    enum tap_error err;
} __ctx = {
    .dat = {{0}, 0},
    .st8 = TAP_ST8_IDLE,
    .err = TAP_ERR_NONE,
};

__attribute__((always_inline))
static inline enum tap_state __get_state(void)
{
    return __ctx.st8;
}

__attribute__((always_inline))
static inline void __set_state(enum tap_state st8)
{
    __ctx.st8 = st8;
}

__attribute__((always_inline))
static inline enum tap_error __get_error(void)
{
    return __ctx.err;
}

__attribute__((always_inline))
static inline void __set_error(enum tap_error err)
{
    __ctx.err = err;
}

__attribute__((always_inline))
static inline void __set_error_state(enum tap_error err)
{
    __set_error(err);

    if (err) {
        __set_state(TAP_ST8_ERROR);
    } else {
        if (__get_state() == TAP_ST8_ERROR)
            __set_state(TAP_ST8_IDLE);
    }
}

__attribute__((always_inline))
static inline int __dump_header(uint8_t *buf, uint16_t len)
{
    struct cart_header *hdr = (struct cart_header *)buf;
    uint8_t rb0 = 0;

    if (sizeof(struct cart_header) > len) {
        __set_error_state(TAP_ERR_DATA);
        return 0;
    } else {
        len = sizeof(struct cart_header);
    }

#ifdef STRICT
    fsmc_bus_width_8();
#endif
    (void)cart_mbc_peek(REG_ROM_BANK_0, &rb0);
    (void)cart_mbc_poke(REG_ROM_BANK_0, 0xff);
#ifdef STRICT
    fsmc_bus_width_16();
#endif

    for (uint8_t i = 0; i < len; i+=2)
        (void)cart_nor_peek(0x2fff0 | i, (uint16_t *)(buf + i));

    if (hdr->flags.bwidth) {
        fsmc_bus_width_8();
        for (uint8_t i = 1; i < len; i+=2)
            (void)cart_nor_peek(0x2fff0 | i, (uint16_t *)(buf + i));
    }

#ifdef STRICT
    fsmc_bus_width_8();
#endif
    (void)cart_mbc_poke(REG_ROM_BANK_0, rb0);
#ifdef STRICT
    fsmc_bus_width_16();
#endif

    return len;
}

__attribute__((always_inline))
static inline int __dump_rom(const uint32_t idx, uint8_t *buf, uint16_t len)
{
    struct cart_header *hdr = (struct cart_header *)&__ctx.dat.buf;

    if (USB_CONTROL_BUF_SIZE != len) {
        __set_error_state(TAP_ERR_DATA);
        return 0;
    }

    if (!idx) {
         (void)__dump_header((uint8_t *)hdr, sizeof(struct cart_header));

        if ((hdr->jmpf.opcode != 0xea) || (hdr->rom_sz > (sizeof(cart_rom_sz) / sizeof(cart_rom_sz[0])))) {
            bzero(__ctx.dat.buf, sizeof(struct cart_header));
            __set_error_state(TAP_ERR_PEEK);
            return 0;
        }
    }

    const uint32_t abs_addr = idx << 10;

    if (cart_rom_sz[hdr->rom_sz] <= abs_addr) {
        bzero(__ctx.dat.buf, sizeof(struct cart_header));
        return 0;
    } else if (cart_rom_sz[hdr->rom_sz] < ((abs_addr + len) - 1)) {
        len = cart_rom_sz[hdr->rom_sz] - abs_addr;
    }

    uint32_t rel_addr = (uint16_t)abs_addr;

    if (!rel_addr)
        (void)cart_mbc_poke(REG_ROM_BANK_0, ((256 - (cart_rom_sz[hdr->rom_sz] >> 16)) + (abs_addr >> 16)));

    rel_addr |= ROM0_BASE;

    if (hdr->flags.bwidth) {
        fsmc_bus_width_8();
        for (uint16_t i = 0; i < len; ++i)
            (void)cart_nor_peek(rel_addr++, (uint16_t *)(buf + i));
        fsmc_bus_width_16();
    } else {
        for (uint16_t i = 0; i < len; i+=2)
            (void)cart_nor_peek(rel_addr + i, (uint16_t *)(buf + i));
    }

    return len;
}

__attribute__((always_inline))
static inline int __dump_ram(const uint32_t idx, uint8_t *buf, uint16_t len)
{
    struct cart_header *hdr = (struct cart_header *)&__ctx.dat.buf;

    if (USB_CONTROL_BUF_SIZE != len) {
        __set_error_state(TAP_ERR_DATA);
        return 0;
    }

    if (!idx) {
        (void)__dump_header((uint8_t *)hdr, sizeof(struct cart_header));

        if ((hdr->jmpf.opcode != 0xea) || (hdr->sav_sz > (sizeof(cart_sav_sz) / sizeof(cart_sav_sz[0])))) {
            bzero(__ctx.dat.buf, sizeof(struct cart_header));
            __set_error_state(TAP_ERR_PEEK);
            return 0;
        }
    }

    const uint32_t abs_addr = idx << 10;

    if (cart_sav_sz[hdr->sav_sz] <= abs_addr) {
        bzero(__ctx.dat.buf, sizeof(struct cart_header));
        return 0;
    } else if (cart_sav_sz[hdr->sav_sz] < ((abs_addr + len) - 1)) {
        len = cart_sav_sz[hdr->sav_sz] - abs_addr;
    }

    uint32_t rel_addr = (uint16_t)abs_addr;

    if (!rel_addr)
        (void)cart_mbc_poke(REG_RAM_BANK, ((256 - (cart_sav_sz[hdr->sav_sz] >> 16)) + (abs_addr >> 16)));

    rel_addr |= SRAM_BASE;

    for (uint16_t i = 0; i < len; ++i)
        (void)cart_sram_peek(rel_addr + i, buf + i);

    return len;
}

static enum usbd_request_return_codes __tap_control_request(
    __attribute__((unused)) usbd_device *dev, struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
    __attribute__((unused)) usbd_control_complete_callback *complete)
{
    if (2 != req->wIndex) return USBD_REQ_NEXT_CALLBACK;

    /* Only accept vendor request */
    if((req->bmRequestType & (USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)) != (USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_INTERFACE))
        return USBD_REQ_NOTSUPP;

    /* Ignore any leveled requests until the error is acknowledged and cleared */
    if ((__get_state() == TAP_ST8_ERROR) && (req->bRequest >= TAP_HANDSHK)) {
        *len = 0;
        return USBD_REQ_HANDLED;
    }

    switch(req->bRequest) {
    case TAP_GETST8:
        if (len && *len) {
            **buf = __get_state();
            *len = 1;
        } /* else... No errors generated */
        return USBD_REQ_HANDLED;
    case TAP_GETERR:
        if (len && *len) {
            **buf = __get_error();
            *len = 1;
        } /* else... No error override */
        return USBD_REQ_HANDLED;
    case TAP_CLRERR:
        __set_error_state(TAP_ERR_NONE);
        return USBD_REQ_HANDLED;
    case TAP_HANDSHK:
        if (cart_handshake())
            __set_error_state(TAP_ERR_CART);
        return USBD_REQ_HANDLED;
    case TAP_MBCPEEK: {
        if ((NULL == len) || (0 == *len)) {
            __set_error_state(TAP_ERR_DATA);
            return USBD_REQ_HANDLED;
        }

        uint8_t port = (uint8_t)req->wValue;

        if (REG_MIN > port) {
            __set_error_state(TAP_ERR_ADDR);
            *len = 0;
            return USBD_REQ_HANDLED;
        }

        if (REG_MAX < (port + *len - 1))
            *len = REG_MAX - port;

#ifdef STRICT
        fsmc_bus_width_8();
#endif
        for (uint8_t i = 0; i < *len; ++i)
            (void)cart_mbc_peek(port++, *buf + i);
#ifdef STRICT
        fsmc_bus_width_16();
#endif

        return USBD_REQ_HANDLED;
    }
    case TAP_RAMPEEK: {
        if ((NULL == len) || (0 == *len)) {
            __set_error_state(TAP_ERR_DATA);
            return USBD_REQ_HANDLED;
        }

        uint32_t addr = req->wValue;

        if (REGION_SIZE < (addr + *len - 1))
            *len = REGION_SIZE - addr;

        addr |= SRAM_BASE;

#ifdef STRICT
        fsmc_bus_width_8();
#endif
        for (uint16_t i = 0; i < *len; ++i)
            (void)cart_sram_peek(addr++, *buf + i);
#ifdef STRICT
        fsmc_bus_width_16();
#endif

        return USBD_REQ_HANDLED;
    }
    case TAP_R0MPEEK:
        __attribute__((__fallthrough__));
    case TAP_R1MPEEK: {
        if (NULL == len || 2 > *len) {
            __set_error_state(TAP_ERR_DATA);
            *len = 0;
            return USBD_REQ_HANDLED;
        }

        uint32_t addr = req->wValue;

        if (addr % 2) {
            __set_error_state(TAP_ERR_ADDR);
            *len = 0;
            return USBD_REQ_HANDLED;
        }

        if (REGION_SIZE < (addr + *len - 2))
            *len = REGION_SIZE - addr;

        addr |= req->bRequest == TAP_R1MPEEK ? ROM1_BASE : ROM0_BASE;

        for (uint16_t i = 0; i < *len; i+=2)
            (void)cart_nor_peek(addr + i, (uint16_t *)(*buf + i));

        return USBD_REQ_HANDLED;
    }
    case TAP_MBCPOKE: {
        if ((NULL == len) || (0 == *len)) {
            __set_error_state(TAP_ERR_DATA);
            return USBD_REQ_HANDLED;
        }

        uint8_t port = (uint8_t)req->wValue;

        if (REG_MIN > port) {
            __set_error_state(TAP_ERR_ADDR);
            return USBD_REQ_HANDLED;
        }

        if (REG_MAX < (port + *len - 1))
            *len = REG_MAX - port;

#ifdef STRICT
        fsmc_bus_width_8();
#endif
        for (uint8_t i = 0; i < *len; ++i)
            (void)cart_mbc_poke(port++, *(*buf + i));
#ifdef STRICT
        fsmc_bus_width_16();
#endif

        return USBD_REQ_HANDLED;
    }
    case TAP_RAMPOKE: {
        if ((NULL == len) || (0 == *len)) {
            __set_error_state(TAP_ERR_DATA);
            return USBD_REQ_HANDLED;
        }

        uint32_t addr = req->wValue;

        if (REGION_SIZE < (addr + *len - 1))
            *len = REGION_SIZE - addr;

        addr |= SRAM_BASE;

#ifdef STRICT
        fsmc_bus_width_8();
#endif
        for (uint16_t i = 0; i < *len; ++i)
            (void)cart_sram_poke(addr++, *(*buf + i));
#ifdef STRICT
        fsmc_bus_width_16();
#endif

        return USBD_REQ_HANDLED;
    }
    case TAP_DUMPHDR:
        *len = __dump_header(*buf, *len);
        return USBD_REQ_HANDLED;
    case TAP_DUMPROM:
        *len = __dump_rom(req->wValue, *buf, *len);
        return USBD_REQ_HANDLED;
    case TAP_DUMPRAM:
        *len = __dump_ram(req->wValue, *buf, *len);
        return USBD_REQ_HANDLED;
    }

    return USBD_REQ_NOTSUPP;
}

void tap_set_config(usbd_device *dev, __attribute__((unused)) uint16_t wValue)
{
    tim2_3_setup();

    usbd_register_control_callback(dev, USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_INTERFACE,
                                   USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
                                   __tap_control_request);
}
