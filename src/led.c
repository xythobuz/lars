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

#include "pico/stdlib.h"

#include "sequence.h"
#include "led.h"

static const uint led_gpio_num[LED_COUNT] = {
    12, 13, 14, 15,
};

static const uint ch_gpio_num[NUM_CHANNELS] = {
    2, 3, 4,
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
    gpio_put(i, v);
}

void ch_set(uint32_t i, bool v) {
    i %= NUM_CHANNELS;
    gpio_put(i, v);
}
