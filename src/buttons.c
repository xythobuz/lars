/*
 * buttons.c
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
#include "pico/stdlib.h"

#include "buttons.h"

#define DEBOUNCE_DELAY_MS 50

static const uint gpio_num[NUM_BTNS] = {
    8, // BTN_A
    9, // BTN_B
    12, // BTN_C
    14, // BTN_REC
    16, // BTN_CLICK
};

struct button_state {
    uint32_t last_time;
    bool current_state, last_state;
};

static struct button_state buttons[NUM_BTNS];
static void (*callback)(enum buttons, bool) = NULL;

void buttons_init(void) {
    for (uint i = 0; i < NUM_BTNS; i++) {
        gpio_init(gpio_num[i]);
        gpio_set_dir(gpio_num[i], GPIO_IN);
        gpio_pull_up(gpio_num[i]);

        buttons[i].last_time = 0;
        buttons[i].current_state = false;
        buttons[i].last_state = false;
    }
}

void buttons_callback(void (*fp)(enum buttons, bool)) {
    callback = fp;
}

void buttons_run(void) {
    for (uint i = 0; i < NUM_BTNS; i++) {
        bool state = !gpio_get(gpio_num[i]);
        uint32_t now = to_ms_since_boot(get_absolute_time());

        if (state != buttons[i].last_state) {
            buttons[i].last_time = now;
        }

        if ((now - buttons[i].last_time) > DEBOUNCE_DELAY_MS) {
            if (state != buttons[i].current_state) {
                printf("btn %d now %s\n", i, state ? "pressed" : "released");

                buttons[i].current_state = state;
                if (callback) {
                    callback(i, state);
                }
            }
        }

        buttons[i].last_state = state;
    }
}
