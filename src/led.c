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

#include "led.h"

#define LED_COUNT 4

static const uint gpio_num[LED_COUNT] = {
    12, 13, 14, 15,
};

void led_init(void) {
    for (uint i = 0; i < LED_COUNT; i++) {
        gpio_init(gpio_num[i]);
        gpio_set_dir(gpio_num[i], GPIO_IN);
    }
}

void led_set(uint32_t i, bool v) {
    i %= LED_COUNT;
    gpio_put(i, v);
}
