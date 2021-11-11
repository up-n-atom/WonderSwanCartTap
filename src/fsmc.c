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

#include <libopencm3/stm32/fsmc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "fsmc.h"

__attribute__((always_inline))
static inline void __gpio_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOE);
    rcc_periph_clock_enable(RCC_GPIOF);
    rcc_periph_clock_enable(RCC_GPIOG);

    /* D2, D3, D13, D14, D15, D0, D1 */
    gpio_set_mode(GPIOD, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  GPIO0 | GPIO1 | GPIO8 | GPIO9 | GPIO10 | GPIO14 | GPIO15);
    /* D4, D5, D6, D7, D8, D9, D10, D11, D12 */
    gpio_set_mode(GPIOE, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  GPIO7 | GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 | GPIO14 | GPIO15);
    /* A0..A9 */
    gpio_set_mode(GPIOF, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO12 | GPIO13 | GPIO14 | GPIO15);
    /* A10..A15 */
    gpio_set_mode(GPIOG, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5);
    /* A16..A18 */
    gpio_set_mode(GPIOD, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  GPIO11 | GPIO12 | GPIO13);
    /* A19..A21 */
    gpio_set_mode(GPIOE, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  GPIO3 | GPIO4 | GPIO5 | GPIO6);
    /* NOE, NWE */
    gpio_set_mode(GPIOD, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  GPIO4 | GPIO5);
    /* NE2 */
    gpio_set_mode(GPIOG, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  GPIO9);
}

__attribute__((always_inline))
static inline void __gpio_reset(void)
{
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOE);
    rcc_periph_clock_enable(RCC_GPIOF);
    rcc_periph_clock_enable(RCC_GPIOG);

    /* D2, D3, D13, D14, D15, D0, D1 */
    gpio_set_mode(GPIOD, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO0 | GPIO1 | GPIO8 | GPIO9 | GPIO10 | GPIO14 | GPIO15);
    /* D4, D5, D6, D7, D8, D9, D10, D11, D12 */
    gpio_set_mode(GPIOE, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO7 | GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 | GPIO14 | GPIO15);
    /* A0..A9 */
    gpio_set_mode(GPIOF, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO12 | GPIO13 | GPIO14 | GPIO15);
    /* A10..A15 */
    gpio_set_mode(GPIOG, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5);
    /* A16..A18 */
    gpio_set_mode(GPIOD, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO11 | GPIO12 | GPIO13);
    /* A19..A21 */
    gpio_set_mode(GPIOE, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO3 | GPIO4 | GPIO5 | GPIO6);
    /* NOE, NWE */
    gpio_set_mode(GPIOD, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO4 | GPIO5);
    /* NE2 */
    gpio_set_mode(GPIOG, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO9);
}

void fsmc_setup(void)
{
    __gpio_setup();

    rcc_periph_clock_enable(RCC_FSMC);

    FSMC_BCR2 = 0U | FSMC_BCR_MTYP_NOR | FSMC_BCR_MWID_16BITS | FSMC_BCR_FACCEN | FSMC_BCR_WREN;
    /* Ref. AN2784 - Using the high-density STM32F10xxx FSMC peripheral to drive external memories
       Table 3 */
    FSMC_BTR2 = 0U | FSMC_BTR_ADDSETx(2) | FSMC_BTR_DATASTx(5) | FSMC_BTR_ACCMODx(FSMC_BTx_ACCMOD_B);
    FSMC_BWTR2 = 0x0fffffffU;
    FSMC_BCR2 |= FSMC_BCR_MBKEN;
}

void fsmc_reset(void)
{
    /* No formal peripheral reset */
    FSMC_BCR2 = 0U;
    FSMC_BWTR2 = 0U;
    FSMC_BTR2 = 0U;
    FSMC_BCR2 = 0U;

    rcc_periph_clock_disable(RCC_FSMC);

    __gpio_reset();
}

__attribute__((always_inline))
inline void fsmc_enable(void)
{
    FSMC_BCR2 |= FSMC_BCR_MBKEN;
}

__attribute__((always_inline))
inline void fsmc_disable(void)
{
    FSMC_BCR2 &= ~FSMC_BCR_MBKEN;
}

uint8_t fsmc_toggle_bus_width(void)
{
    fsmc_disable();

    if (FSMC_NOR_PSRAM_16BITS) {
        /* 8-bit mode requires Q15/A-1 to be bitbanged */
        gpio_set_mode(GPIOD, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO10);
        gpio_clear(GPIOD, GPIO10);

        FSMC_BCR2 &= ~FSMC_BCR_MWID_16BITS;
    } else {
        gpio_set_mode(GPIOD, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO10);

        FSMC_BCR2 |= FSMC_BCR_MWID_16BITS;
    }

    fsmc_enable();

    return (uint8_t)(FSMC_BCR2 & FSMC_BCR_MWID_16BITS);
}

inline void fsmc_bus_width_8(void)
{
    if (FSMC_NOR_PSRAM_16BITS)
        (void)fsmc_toggle_bus_width();
}

inline void fsmc_bus_width_16(void)
{
    if (!FSMC_NOR_PSRAM_16BITS)
        (void)fsmc_toggle_bus_width();
}
