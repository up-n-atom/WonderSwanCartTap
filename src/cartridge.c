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
#include <stdint.h>

#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/fsmc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "cartridge.h"
#include "fsmc.h"
#include "platform.h"

__attribute__((always_inline))
static inline uint8_t __cart_read_byte(const uint32_t addr)
{
    return FSMC_NOR_PSRAM_READ_BYTE(FSMC_BANK1_BASE_NOR2 + addr);
}

__attribute__((always_inline))
static inline uint16_t __cart_read_word(const uint32_t addr)
{
    return FSMC_NOR_PSRAM_READ_WORD(FSMC_BANK1_BASE_NOR2 + addr);
}

__attribute__((always_inline))
static inline void __cart_write_byte(const uint32_t addr, const uint8_t val)
{
    FSMC_NOR_PSRAM_WRITE_BYTE(FSMC_BANK1_BASE_NOR2 + addr, val);
}

__attribute__((always_inline))
static inline void __cart_write_word(const uint32_t addr, const uint16_t val)
{
    FSMC_NOR_PSRAM_WRITE_WORD(FSMC_BANK1_BASE_NOR2 + addr, val);
}

int cart_mbc_peek(const uint8_t reg, uint8_t *val)
{
    if (NULL == val) return BADPTR;

    if (REG_MIN > reg) return BADREG;

#ifdef STRICT
    /* enforce prior call to fsmc_toggle_bus_width() */
    if (FSMC_NOR_PSRAM_16BITS) return BADWIDTH;
#endif

    /* SS */
    gpio_clear(GPIOF, GPIO10);
    msleep(1);

#ifndef STRICT
    if (FSMC_NOR_PSRAM_16BITS)
        *val = (uint8_t)__cart_read_word((((reg & 0xf0) << 12) | (reg & 0xf)) << 1);
    else
#endif
        *val = __cart_read_byte(((reg & 0xf0) << 12) | (reg & 0xf));

    /* SS */
    gpio_set(GPIOF, GPIO10);
    msleep(1);

    return FIN;
}

int cart_mbc_poke(const uint8_t reg, const uint8_t val)
{
    if (REG_MIN > reg) return BADREG;

#ifdef STRICT
    /* enforce prior call to fsmc_toggle_bus_width() */
    if (FSMC_NOR_PSRAM_16BITS) return BADWIDTH;
#endif

    /* SS */
    gpio_clear(GPIOF, GPIO10);
    msleep(1);

#ifndef STRICT
    if (FSMC_NOR_PSRAM_16BITS)
        __cart_write_word((((reg & 0xf0) << 12) | (reg & 0xf)) << 1, (uint16_t)val);
    else
#endif
        __cart_write_byte(((reg & 0xf0) << 12) | (reg & 0xf), val);

    /* SS */
    gpio_set(GPIOF, GPIO10);
    msleep(1);

    return FIN;
}

int cart_nor_peek(const uint32_t addr, uint16_t *val)
{
    if (NULL == val) return BADPTR;

    if (!ROM_ADDR_VALID(addr)) return BADADDR;

    if (FSMC_NOR_PSRAM_16BITS) {
        if (addr % 2) return BADADDR;

        *val = __cart_read_word(addr << 1);
    } else {
        /* bitbang Q15/A-1 */
        if (addr & 1) {
            gpio_set(GPIOD, GPIO10);
            msleep(1);
        }

        *val = (uint16_t)__cart_read_byte(addr);

        gpio_clear(GPIOD, GPIO10);
        msleep(1);
    }

    return FIN;
}

int cart_nor_poke(const uint32_t addr, const uint16_t val)
{
    if (!ROM_ADDR_VALID(addr)) return BADADDR;

    if (FSMC_NOR_PSRAM_16BITS) {
#ifdef STRICT
        /* enforce prior calls to fsmc_toggle_bus_width() and cart_port_poke(REG_MEMORY_CTRL, 1) */
        return BADWIDTH;
#else
        if (addr % 2) return BADADDR;

        __cart_write_word(addr << 1, val);
#endif
    } else {
        /* bitbang Q15/A-1 */
        if (addr & 1) {
            gpio_set(GPIOD, GPIO10);
            msleep(1);
        }

        __cart_write_byte(addr, (uint8_t)(val & 0xff));

        gpio_clear(GPIOD, GPIO10);
        msleep(1);
    }

    return FIN;
}

int cart_sram_peek(const uint32_t addr, uint8_t *val)
{
    if (NULL == val) return BADPTR;

    if (!RAM_ADDR_VALID(addr)) return BADADDR;

    if (FSMC_NOR_PSRAM_16BITS)
#ifdef STRICT
        /* enforce prior call to fsmc_toggle_bus_width() */
        return BADWIDTH;
#else
        *val = (uint8_t)__cart_read_word(addr << 1);
#endif
    else
        *val = __cart_read_byte(addr);

    return FIN;
}

int cart_sram_poke(const uint32_t addr, const uint8_t val)
{
    if (!RAM_ADDR_VALID(addr)) return BADADDR;

    if (FSMC_NOR_PSRAM_16BITS)
#ifdef STRICT
        /* enforce prior call to fsmc_toggle_bus_width() */
        return BADWIDTH;
#else
        __cart_write_word(addr, (uint16_t)val);
#endif
    else
        __cart_write_byte(addr, val);

    return FIN;
}

void cart_2k3_nor_poke_enable(void)
{
    uint8_t val;

    if (FSMC_NOR_PSRAM_16BITS)
        (void)fsmc_toggle_bus_width();

    (void)cart_mbc_peek(REG_MEMORY_CTRL, &val);

    if (!(val & 1))
        (void)cart_mbc_poke(REG_MEMORY_CTRL, val | (uint8_t)1);
}

void cart_2k3_nor_poke_disable(void)
{
    uint8_t val;

    if (FSMC_NOR_PSRAM_16BITS)
        (void)fsmc_toggle_bus_width();

    (void)cart_mbc_peek(REG_MEMORY_CTRL, &val);

    if (val & 1)
        (void)cart_mbc_poke(REG_MEMORY_CTRL, val & (uint8_t)~1);

    if (!FSMC_NOR_PSRAM_16BITS)
        (void)fsmc_toggle_bus_width();
}

void cart_clock_start(void)
{
    timer_enable_oc_output(TIM2, TIM_OC2);
    timer_enable_counter(TIM2);
    timer_set_counter(TIM2, 0);

    timer_enable_oc_output(TIM3, TIM_OC2);
    timer_enable_counter(TIM3);
    timer_set_counter(TIM3, 0);
    timer_clear_flag(TIM3, TIM_SR_CC2IF);

    /* Wait for /RST to be deasserted - 1 cycle
       Ref. RM0008 - Section 14.3.9 */
    while (!timer_get_flag(TIM3, TIM_SR_CC2IF)) __asm__("nop");
}

void cart_clock_stop(void)
{
    timer_disable_oc_output(TIM3, TIM_OC2);
    timer_disable_counter(TIM3);

    timer_disable_oc_output(TIM2, TIM_OC2);
    timer_disable_counter(TIM2);
}

void cart_delay(uint32_t cycles)
{
    if (TIM_CR1(TIM3) & TIM_CR1_CEN) {
        timer_set_counter(TIM3, 0);
        while (timer_get_counter(TIM3) < cycles) __asm__("nop");
    } else {
        while (cycles--) __asm__("nop");
    }
}

__attribute__((always_inline))
static inline void __gpio_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOE);
    rcc_periph_clock_enable(RCC_GPIOF);
    rcc_periph_clock_enable(RCC_GPIOG);

    /* A0..A3 */
    gpio_set_mode(GPIOF, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  GPIO0 | GPIO1 | GPIO2 | GPIO3);
    /* A16..A18 */
    gpio_set_mode(GPIOD, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  GPIO11 | GPIO12 | GPIO13);
    /* A19 */
    gpio_set_mode(GPIOE, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  GPIO3);
    /* /OE, /WE */
    gpio_set_mode(GPIOD, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  GPIO4 | GPIO5);
    /* /CE */
    gpio_set_mode(GPIOG, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  GPIO9);
    /* /SS */
    gpio_set_mode(GPIOF, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  GPIO10);
    /* SO */
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                  GPIO4);
    /* Activate internal weak pull-down - Used as cart detect */
    gpio_clear(GPIOB, GPIO4);
}

__attribute__((always_inline))
static inline void __gpio_reset(void)
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOE);
    rcc_periph_clock_enable(RCC_GPIOF);
    rcc_periph_clock_enable(RCC_GPIOG);

    /* A0..A3 */
    gpio_set_mode(GPIOF, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO0 | GPIO1 | GPIO2 | GPIO3);
    /* A16..A18 */
    gpio_set_mode(GPIOD, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO11 | GPIO12 | GPIO13);
    /* A19 */
    gpio_set_mode(GPIOE, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO3);
    /* /OE, /WE */
    gpio_set_mode(GPIOD, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO4 | GPIO5);
    /* /CE */
    gpio_set_mode(GPIOG, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO9);
    /* /SS */
    gpio_set_mode(GPIOF, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO10);
    /* SO */
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO4);
}

static volatile uint8_t __ibuf[XFER_WIDTH] = {0};

__attribute__((always_inline))
static inline void __dma_setup(void)
{
    rcc_periph_clock_enable(RCC_DMA1);

    /* DMA1_CH6 - TIM3_TRIG
       Ref. RM0008 - Table 78 */
    dma_channel_reset(DMA1, DMA_CHANNEL6);

    dma_set_peripheral_address(DMA1, DMA_CHANNEL6, (uint32_t)&GPIO_IDR(GPIOB));
    dma_set_memory_address(DMA1, DMA_CHANNEL6, (uint32_t)__ibuf);
    dma_set_number_of_data(DMA1, DMA_CHANNEL6, XFER_WIDTH);
    dma_set_memory_size(DMA1, DMA_CHANNEL6, DMA_CCR_MSIZE_8BIT);
    dma_set_peripheral_size(DMA1, DMA_CHANNEL6, DMA_CCR_MSIZE_8BIT);
    dma_set_read_from_peripheral(DMA1, DMA_CHANNEL6);
    dma_set_priority(DMA1, DMA_CHANNEL6, DMA_CCR_PL_VERY_HIGH);
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL6);
    dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL6);
}

__attribute__((always_inline))
static inline void __dma_reset(void)
{
    dma_channel_reset(DMA1, DMA_CHANNEL6);

    rcc_periph_clock_disable(RCC_DMA1);
}

__attribute__((always_inline))
static inline void __dma_xfer_begin(void)
{
    timer_enable_irq(TIM3, TIM_DIER_TDE);
    dma_enable_channel(DMA1, DMA_CHANNEL6);
}

__attribute__((always_inline))
static inline void __dma_xfer_end(void)
{
    dma_disable_channel(DMA1, DMA_CHANNEL6);
    timer_disable_irq(TIM3, TIM_DIER_TDE);
}

__attribute__((always_inline))
static inline void __dma_xfer_blocking(void)
{
    __dma_xfer_begin();

    while (!dma_get_interrupt_flag(DMA1, DMA_CHANNEL6, DMA_TCIF)) __asm__("nop");

    dma_clear_interrupt_flags(DMA1, DMA_CHANNEL6, DMA_TCIF);

    __dma_xfer_end();
}

__attribute__((always_inline))
static inline uint32_t __cart_get_secret(void)
{
    size_t i, j;
    uint32_t sec = 0;

    /* Read PB4 - 1 cycle setup + XFER_WIDTH cycles */
    __dma_xfer_blocking();
    __dma_reset();

    /* Isolate PB4 input */

    /* Find 1st falling edge */
    for (i = 0; i < XFER_WIDTH; ++i) {
        if (!__ibuf[i]) continue;
        if (!((__ibuf[i] >> 4) & 1)) break;
        __ibuf[i] = 0;
    }
    /* Build handshake secret */
    for (j = 0; i < XFER_WIDTH && j < SECRET_WIDTH; ++i, ++j) {
        sec |= (uint32_t)(((__ibuf[i] >> 4) & 1) << j);
        __ibuf[i] = 0;
    }
    /* Clear remaining buffer */
    while (i < XFER_WIDTH) {
        __ibuf[i++] = 0;
    }

    return sec;
}

int cart_handshake(void)
{
    cart_clock_stop();

    fsmc_reset();

    __gpio_setup();

    msleep(3);

    /* Check if cart inserted */
    if (!(gpio_get(GPIOB, GPIO4) & GPIO4)) return HANDSHAKE_FAILURE;

    __dma_setup();

    /* bitbang time...*/

    /* /RST */
    gpio_clear(GPIOF, GPIO11);
    /* OE, WE*/
    gpio_set(GPIOD, GPIO4 | GPIO5);
    /* SS */
    gpio_set(GPIOF, GPIO10);
    /* CE */
    gpio_set(GPIOG, GPIO9);
    /* A0..A3 = 5 */
    gpio_set(GPIOF, GPIO1 | GPIO3);
    gpio_clear(GPIOF, GPIO0 | GPIO2);
    /* A16..A19 = A */
    gpio_set(GPIOD, GPIO11 | GPIO13);
    gpio_clear(GPIOD, GPIO12);
    gpio_clear(GPIOE, GPIO3);

    cart_clock_start();

    cart_delay(4);

    /* A0..A3 = A */
    gpio_clear(GPIOF, GPIO1 | GPIO3);
    gpio_set(GPIOF, GPIO0 | GPIO2);
    /* A16..A19 = 5 */
    gpio_clear(GPIOD, GPIO11 | GPIO13);
    gpio_set(GPIOD, GPIO12);
    gpio_set(GPIOE, GPIO3);

#if 0 /* Incorporated delay cycles into secret retrieval */
    cart_delay(4);
#endif

    /* Ref. AN4666 - Parallel synchronous transmission using GPIO and DMA */
    if (__cart_get_secret() != HANDSHAKE_SECRET) {
        __gpio_reset();
        return HANDSHAKE_FAILURE;
    }

    fsmc_setup();

    return HANDSHAKE_SUCCESS;
}
