/*
 * sequence.c
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

#include "led.h"
#include "sequence.h"

#define MAX_BEATS 32

static uint32_t ms_per_beat = 500;
static uint32_t beats = 16;
static uint32_t last_t = 0;
static uint32_t last_i = 0;

static bool sequence[MAX_BEATS] = {0};

void sequence_init(void) {
    last_t = to_ms_since_boot(get_absolute_time());
    last_i = 0;
}

void sequence_set_bpm(uint32_t new_bpm) {
    ms_per_beat = 60000 / new_bpm;
}

void sequence_set_beats(uint32_t new_beats) {
    beats = (new_beats <= MAX_BEATS) ? new_beats : MAX_BEATS;
}

static void sequence_set(uint32_t beat, bool value) {
    if (beat < MAX_BEATS) {
        sequence[beat] = value;
    }
}

void sequence_handle_button(enum buttons btn, bool rec) {
    // TODO trigger gpio impulse

    if (rec) {

    }
}

void sequence_run(void) {
    uint32_t now = to_ms_since_boot(get_absolute_time());

    if ((last_t + ms_per_beat) >= now) {
        uint32_t i = last_i + 1;
        if (i >= beats) i = 0;

        led_set(last_i, false);
        led_set(i, true);

        if (sequence[i]) {
            // TODO trigger GPIO impulse
        }

        last_t = now;
        last_i = i;
    }
}
