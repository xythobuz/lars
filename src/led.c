/*
 * led.c
 *
 * Copyright (c) 2024 Thomas Buck (thomas@xythobuz.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * See <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <inttypes.h>
#include "pico/stdlib.h"

#include "sequence.h"
#include "log.h"
#include "led.h"

#if 0
static const uint led_gpio_num[LED_COUNT] = {
    10, 11, 13, 15,
};
#else
static const uint led_gpio_num[LED_COUNT] = {
    6, 7, 8, 9, 10, 11, 12, 13,
};
#endif

static const uint ch_gpio_num[NUM_CHANNELS] = {
    22, 26, 27,
};

void led_init(void) {
    for (uint i = 0; i < LED_COUNT; i++) {
        gpio_init(led_gpio_num[i]);
        gpio_set_dir(led_gpio_num[i], GPIO_OUT);
    }
    for (uint i = 0; i < NUM_CHANNELS; i++) {
        gpio_init(ch_gpio_num[i]);
        gpio_set_dir(ch_gpio_num[i], GPIO_OUT);
    }
}

void led_set(uint32_t i, bool v) {
    i %= LED_COUNT;
    gpio_put(led_gpio_num[i], v);
    //debug("led %"PRIu32" now %d", i, v);
}

void ch_set(uint32_t i, bool v) {
    i %= NUM_CHANNELS;
    gpio_put(ch_gpio_num[i], v);
    //debug("ch %"PRIu32" now %d", i, v);
}
