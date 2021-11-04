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

#ifndef FSMC_H
#define FSMC_H

#define FSMC_NOR_PSRAM_WRITE_WORD(A, D) (*(volatile uint16_t *)(A) = (D))
#define FSMC_NOR_PSRAM_WRITE_BYTE(A, D) (*(volatile uint8_t *)(A) = (D))

#define FSMC_NOR_PSRAM_READ_WORD(A) (*(uint16_t *)(A))
#define FSMC_NOR_PSRAM_READ_BYTE(A) (*(uint8_t *)(A))

#define FSMC_BANK1_BASE_NOR1 FSMC_BANK1_BASE
#define FSMC_BANK1_BASE_NOR2 (FSMC_BANK1_BASE + 0x04000000U)

#define FSMC_BCR_MWID_8BITS  (0 << 4)
#define FSMC_BCR_MWID_16BITS (1 << 4)

#define FSMC_NOR_PSRAM_16BITS (FSMC_BCR2 & FSMC_BCR_MWID_16BITS)

#define FSMC_BCR_MTYP_NOR (1 << 3)

void fsmc_setup(void);

void fsmc_reset(void);

void fsmc_enable(void);

void fsmc_disable(void);

uint8_t fsmc_toggle_bus_width(void);

#endif
