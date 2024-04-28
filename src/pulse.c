/*
 * pulse.c
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

#include "led.h"
#include "log.h"
#include "sequence.h"
#include "pulse.h"

static uint32_t out_time[NUM_CHANNELS] = {0};
static uint32_t led_time[LED_COUNT] = {0};

static void pulse_trigger(uint32_t i, uint32_t t_ms, bool out) {
    uint32_t off_t = t_ms + to_ms_since_boot(get_absolute_time());

    if (out) {
        ch_set(i, true);
        if (out_time[i % NUM_CHANNELS] == 0) {
            out_time[i % NUM_CHANNELS] = off_t;
        } else {
            debug("skip retrigger out %"PRIu32, i);
        }
    } else {
        led_set(i, true);
        if (led_time[i % LED_COUNT] == 0) {
            led_time[i % LED_COUNT] = off_t;
        } else {
            debug("skip retrigger led %"PRIu32, i);
        }
    }
}

void pulse_trigger_out(uint32_t i, uint32_t t_ms) {
    pulse_trigger(i, t_ms, true);
}

void pulse_trigger_led(uint32_t i, uint32_t t_ms) {
    pulse_trigger(i, t_ms, false);
}

void pulse_run(void) {
    uint32_t now = to_ms_since_boot(get_absolute_time());

    for (uint i = 0; i < NUM_CHANNELS; i++) {
        if (out_time[i] != 0) {
            if (out_time[i] <= now) {
                ch_set(i, false);
                out_time[i] = 0;
            }
        }
    }

    for (uint i = 0; i < LED_COUNT; i++) {
        if (led_time[i] != 0) {
            if (led_time[i] <= now) {
                led_set(i, false);
                led_time[i] = 0;
            }
        }
    }
}
