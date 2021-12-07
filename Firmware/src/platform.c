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

#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#ifdef DEBUG
#include <libopencm3/stm32/dbgmcu.h>
#endif
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include <libopencmsis/core_cm3.h>

#include "platform.h"

#if defined(DEBUG) && defined(SEMIHOSTING)
extern void initialise_monitor_handles(void);
#endif

volatile uint64_t sys_millis;

void sys_tick_handler(void)
{
    sys_millis++;
}

#ifdef STANDBY_SLEEP
__attribute__((always_inline))
static inline void __plat_sleep_begin(unsigned int msecs)
{
#ifdef USE_PLL_HSI
    systick_set_reload((msecs * 6000) - 1);
#else
    systick_set_reload((msecs * 9000) - 1);
#endif
    (void)systick_get_countflag();
    systick_interrupt_enable();
    systick_counter_enable();
}

__attribute__((always_inline))
static inline void __plat_sleep(void)
{
    CM_ATOMIC_BLOCK() {
        sys_millis = 0;
    }

    while (!sys_millis)
        __WFI();
}

__attribute__((always_inline))
static inline void __plat_sleep_end(void)
{
    systick_counter_disable();
    systick_interrupt_disable();
}
#endif

void msleep(unsigned int msecs)
{
    if (!msecs) return;

#ifdef STANDBY_SLEEP
    __plat_sleep_begin(msecs);
    __plat_sleep();
    __plat_sleep_end();
#else
    uint64_t cont = sys_millis + msecs;

    while (cont > sys_millis) __asm__("nop");
#endif
}

__attribute__((always_inline))
static inline void __gpio_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOC);

    /* LED */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  GPIO13);
}

__attribute__((always_inline))
inline void led_on(void)
{
    gpio_clear(GPIOC, GPIO13); /* LED active low!*/
}

__attribute__((always_inline))
inline void led_off(void)
{
    gpio_set(GPIOC, GPIO13); /* LED active low!*/
}

void plat_setup(void)
{
#ifdef USE_PLL_HSI
    /* 48Hz / 8 = 6000000 counts per second */
    rcc_clock_setup_in_hsi_out_48mhz();
#else
    /* 72Hz / 8 = 9000000 counts per second */
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
#endif
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
#ifndef STANDBY_SLEEP
#ifdef USE_PLL_HSI
    /* 6000000/6000 = 1000 overflows per second ie. interrupt every 1ms */
    systick_set_reload(5999);
#else
    /* 9000000/9000 = 1000 overflows per second ie. interrupt every 1ms */
    systick_set_reload(8999);
#endif

    systick_interrupt_enable();
    systick_counter_enable();
#endif

#ifdef DEBUG
#ifdef STANDBY_SLEEP
    /* Enable debugging during wfi */
    DBGMCU_CR = DBGMCU_CR_STANDBY | DBGMCU_CR_STOP | DBGMCU_CR_SLEEP;
#endif
#ifdef SEMIHOSTING
    initialise_monitor_handles();
#endif
#endif

    __gpio_setup();

    led_off();
}
