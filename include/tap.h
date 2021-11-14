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

#ifndef TAP_H
#define TAP_H

enum tap_state {
    TAP_ST8_IDLE,
    TAP_ST8_BUSY,
    TAP_ST8_ERROR,
};

enum tap_error {
    TAP_ERR_NONE,
    TAP_ERR_CART,
    /* Synonymous for read */
    TAP_ERR_PEEK,
    /* Synonymous for write */
    TAP_ERR_POKE,
    TAP_ERR_ADDR,
    TAP_ERR_DATA,
};

enum tap_request {
    /* Status requests */
    TAP_GETST8,
    TAP_GETERR,
    TAP_CLRERR,
    /* Low-level requests */
    TAP_HANDSHK = 11,
    TAP_MBCPEEK,
    TAP_RAMPEEK,
    TAP_R0MPEEK,
    TAP_R1MPEEK,
    TAP_MBCPOKE,
    TAP_RAMPOKE,
    TAP_R0MPOKE,
    TAP_R1MPOKE,
    /* High-level requests */
    TAP_DUMPHDR = 22,
    TAP_DUMPROM,
    TAP_DUMPRAM,
    TAP_DUMPEEP,
};

void tap_set_config(usbd_device *dev, uint16_t wValue);

#endif
