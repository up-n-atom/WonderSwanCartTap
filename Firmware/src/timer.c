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

#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "timer.h"

__attribute__((always_inline))
static inline void __gpio_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_AFIO);

    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON, AFIO_MAPR_TIM2_REMAP_PARTIAL_REMAP1 | AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP);

    /* CLK - TIM2_CH2, RST - TIM3_CH2 */
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  GPIO3 | GPIO5);
}

__attribute__((always_inline))
static inline void __gpio_reset(void)
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_AFIO);

    /* CLK - TIM2_CH2, RST - TIM3_CH2 */
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                  GPIO3 | GPIO5);
}

__attribute__((always_inline))
static inline void __tim2_setup(void)
{
    rcc_periph_clock_enable(RCC_TIM2);

    rcc_periph_reset_pulse(RST_TIM2);

    timer_set_master_mode(TIM2, TIM_CR2_MMS_UPDATE);
    /* PSC + ARR calculated using https://github.com/v0idv0id/STM32-Scaler
       ./timescale.py -c 48000000 -f 384000 -d 50 -e 0.5
    */
    timer_set_prescaler(TIM2, 4);
    timer_set_period(TIM2, 24);
    /* 50% duty cycle */
    timer_set_oc_value(TIM2, TIM_OC2, 12);
    /* PWM mode 1 - output high if CNT > CCR1 */
    timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM1);
    timer_set_oc_idle_state_unset(TIM2, TIM_OC2);
    timer_set_oc_fast_mode(TIM2, TIM_OC2);
    timer_enable_oc_output(TIM2, TIM_OC2);
}

__attribute__((always_inline))
static inline void __tim3_setup(void)
{
    rcc_periph_clock_enable(RCC_TIM3);

    rcc_periph_reset_pulse(RST_TIM3);

    /* Default period is 65535 */

    /* /RST - deassert after 1 cycles */
    timer_set_oc_value(TIM3, TIM_OC2, 1);
    timer_set_oc_mode(TIM3, TIM_OC2, TIM_OCM_ACTIVE);
    timer_enable_oc_output(TIM3, TIM_OC2);
    /* Connect TIM2_TRGO to TIM3_TRGI
       Ref. RM0008 - Table 86 */
    timer_slave_set_mode(TIM3, TIM_SMCR_SMS_ECM1);
    timer_slave_set_trigger(TIM3, TIM_SMCR_TS_ITR1);
}

void tim2_3_setup(void)
{
    __gpio_setup();
    /* 384kHz clock source */
    __tim2_setup();
    /* RST and DMA triggers, and delay counter */
    __tim3_setup();
}

void tim2_3_reset(void)
{
    rcc_periph_reset_pulse(RST_TIM3);
    rcc_periph_reset_pulse(RST_TIM2);

    rcc_periph_clock_disable(RCC_TIM3);
    rcc_periph_clock_disable(RCC_TIM2);

    __gpio_reset();
}
