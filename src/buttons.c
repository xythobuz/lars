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

#include "config.h"
#include "log.h"
#include "main.h"
#include "buttons.h"

static const uint gpio_num_proto[NUM_BTNS] = {
    8, // BTN_A
    9, // BTN_B
    12, // BTN_C

    // not existing on prototype
    0xFF, // BTN_D
    0xFF, // BTN_E
    0xFF, // BTN_F
    0xFF, // BTN_G
    0xFF, // BTN_H

    14, // BTN_CLEAR
    16, // BTN_CLICK
};

#define NUM_ROWS 3
#define NUM_COLS 3

static const uint gpio_num_v2[NUM_BTNS] = {
    // handled as matrix
    0xFF, // BTN_A
    0xFF, // BTN_B
    0xFF, // BTN_C
    0xFF, // BTN_D
    0xFF, // BTN_E
    0xFF, // BTN_F
    0xFF, // BTN_G
    0xFF, // BTN_H
    0xFF, // BTN_CLEAR

    20, // BTN_CLICK
};

static const uint gpio_rows[NUM_ROWS] = {
    0, 2, 3
};

static const uint gpio_cols[NUM_COLS] = {
    1, 4, 5
};

struct button_state {
    uint32_t last_time;
    bool current_state, last_state;
};

static struct button_state buttons[NUM_BTNS];
static void (*callback)(enum buttons, bool) = NULL;
static int last_col = 0;

void buttons_init(void) {
    for (uint i = 0; i < NUM_BTNS; i++) {
        uint n = 0xFF;
        if (hw_type == HW_PROTOTYPE) {
            n = gpio_num_proto[i];
        } else if (hw_type == HW_V2) {
            n = gpio_num_v2[i];
        }

        if (n >= 0xFF) {
            continue;
        }

        gpio_init(n);
        gpio_set_dir(n, GPIO_IN);
        gpio_pull_up(n);

        buttons[i].last_time = 0;
        buttons[i].current_state = false;
        buttons[i].last_state = false;
    }

    if (hw_type != HW_V2) {
        return;
    }

    for (uint i = 0; i < NUM_ROWS; i++) {
        uint n = gpio_rows[i];
        gpio_init(n);
        gpio_set_dir(n, GPIO_IN);
        gpio_pull_up(n);
    }

    for (uint i = 0; i < NUM_COLS; i++) {
        uint n = gpio_cols[i];
        gpio_init(n);
        gpio_set_dir(n, GPIO_IN);
        gpio_disable_pulls(n);
    }
}

void buttons_callback(void (*fp)(enum buttons, bool)) {
    callback = fp;
}

static void button_run_single(bool state, uint i) {
    uint32_t now = to_ms_since_boot(get_absolute_time());

    if (state != buttons[i].last_state) {
        buttons[i].last_time = now;
    }

    if ((now - buttons[i].last_time) > DEBOUNCE_DELAY_MS) {
        if (state != buttons[i].current_state) {
            //debug("btn %d now %s", i, state ? "pressed" : "released");

            buttons[i].current_state = state;
            if (callback) {
                callback(i, state);
            }
        }
    }

    buttons[i].last_state = state;
}

static void button_run_proto(void) {
    for (uint i = 0; i < NUM_BTNS; i++) {
        if (gpio_num_proto[i] >= 0xFF) {
            continue;
        }

        bool state = !gpio_get(gpio_num_proto[i]);
        button_run_single(state, i);
    }
}

static void button_run_matrix(void) {
    gpio_set_dir(gpio_cols[last_col], GPIO_IN);
    last_col = (last_col + 1) % NUM_COLS;
    gpio_set_dir(gpio_cols[last_col], GPIO_OUT);
    gpio_put(gpio_cols[last_col], false);
    sleep_us(2);
    for (uint i = 0; i < NUM_ROWS; i++) {
        button_run_single(!gpio_get(gpio_rows[i]), last_col * NUM_ROWS + i);
    }
}

void buttons_run(void) {
    if (hw_type == HW_PROTOTYPE) {
        button_run_proto();
    } else if (hw_type == HW_V2) {
        button_run_matrix();
        button_run_single(!gpio_get(gpio_num_v2[BTN_CLICK]), BTN_CLICK);
    }
}
