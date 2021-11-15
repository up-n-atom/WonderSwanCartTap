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

#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#define ADDRMASK 0xfffffUL

#define ROM_ADDR_VALID(A) ((A & ADDRMASK) >> 17) /* 0x20000..0xfffff */

#define RAM_ADDR_VALID(A) (1 == ((A & ADDRMASK) >> 16)) /* 0x10000..0x1ffff */

#define FIN      0
#define BADPTR   1
#define BADADDR  2
#define BADREG   3
#define BADWIDTH 4

#define CCLK 384

#define REG_LINEAR_ADDR_OFF   0xc0
#define REG_RAM_BANK          0xc1
#define REG_ROM_BANK_0        0xc2
#define REG_ROM_BANK_1        0xc3
#define REG_OUT_SERIAL_DATA_L 0xc4
#define REG_OUT_SERIAL_DATA_H 0xc5
#define REG_OUT_SERIAL_COM_L  0xc6
#define REG_OUT_SERIAL_COM_H  0xc7
#define REG_OUT_SERIAL_CTRL_L 0xc8
#define REG_OUT_SERIAL_CTRL_H 0xc9
#define REG_RTC_CTRL          0xca
#define REG_RTC_DATA          0xcb
#define REG_IO_CTRL           0xcc
#define REG_IO_SCAN           0xcd
#define REG_MEMORY_CTRL       0xce
#define REG_RAM_BANK_L        0xd0
#define REG_RAM_BANK_H        0xd1
#define REG_ROM_BANK_0_L      0xd2
#define REG_ROM_BANK_0_H      0xd3
#define REG_ROM_BANK_1_L      0xd4
#define REG_ROM_BANK_1_H      0xd5

#define REG_MIN REG_LINEAR_ADDR_OFF
#define REG_MAX 0xff

#define SRAM_BASE 0x10000
#define ROM0_BASE 0x20000
#define ROM1_BASE 0x30000

#define HANDSHAKE_SECRET 0x5140
#define HANDSHAKE_SUCCESS 0
#define HANDSHAKE_FAILURE 1

#define XFER_WIDTH 24
#define SECRET_WIDTH 18

void cart_clock_start(void);

void cart_clock_stop(void);

void cart_delay(uint32_t cycles);

int cart_mbc_peek(const uint8_t reg, uint8_t *val);

int cart_mbc_poke(const uint8_t reg, const uint8_t val);

int cart_nor_peek(const uint32_t addr, uint16_t *val);

int cart_nor_poke(const uint32_t addr, const uint16_t val);

int cart_sram_peek(const uint32_t addr, uint8_t *val);

int cart_sram_poke(const uint32_t addr, const uint8_t val);

void cart_2k3_nor_poke_enable(void);

void cart_2k3_nor_poke_disable(void);

int cart_handshake(void);

#if 0
int cart_2k1_eeprom_erase(const uint32_t addr, const uint8_t val);

int cart_2k1_eeprom_peek(const uint32_t addr, uint8_t *val);

int cart_2k1_eeprom_poke(const uint32_t addr, const uint8_t val);
#endif

#endif
