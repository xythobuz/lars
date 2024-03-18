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
#include "pulse.h"
#include "sequence.h"

#define MAX_BEATS 32
static const uint32_t channel_times[NUM_CHANNELS] = CH_GPIO_TIMINGS;

static uint32_t ms_per_beat = 500;
static uint32_t beats = 16;
static uint32_t last_t = 0;
static uint32_t last_i = 0;

static enum channels sequence[MAX_BEATS] = {0};

void sequence_init(void) {
    last_t = to_ms_since_boot(get_absolute_time());
    last_i = 0;

    for (uint i = 0; i < MAX_BEATS; i++) {
        sequence[i] = 0;
    }
}

void sequence_set_bpm(uint32_t new_bpm) {
    ms_per_beat = 60000 / new_bpm;
}

void sequence_set_beats(uint32_t new_beats) {
    beats = (new_beats <= MAX_BEATS) ? new_beats : MAX_BEATS;
}

static void sequence_set(uint32_t beat, enum channels ch, bool value) {
    if (beat < MAX_BEATS) {
        if (value) {
            sequence[beat] |= ch;
        } else {
            sequence[beat] &= ~ch;
        }
    }
}

static bool sequence_get(uint32_t beat, enum channels ch) {
    if (beat < MAX_BEATS) {
        return (sequence[beat] & ch) != 0;
    }
    return false;
}

void sequence_handle_button_loopstation(enum buttons btn, bool rec) {
    switch (btn) {
        case BTN_A: {
            pulse_trigger_out(0, channel_times[0]);
            pulse_trigger_led(0, channel_times[0]);
            break;
        }

        case BTN_B: {
            pulse_trigger_out(1, channel_times[1]);
            pulse_trigger_led(1, channel_times[1]);
            break;
        }

        case BTN_C: {
            pulse_trigger_out(2, channel_times[2]);
            pulse_trigger_led(2, channel_times[2]);
            break;
        }

        default: {
            break;
        }
    }

    if (rec) {
        uint32_t beat = 42; // TODO!!

        switch (btn) {
            case BTN_A: {
                sequence_set(beat, CH_KICK, true);
                break;
            }

            case BTN_B: {
                sequence_set(beat, CH_SNARE, true);
                break;
            }

            case BTN_C: {
                sequence_set(beat, CH_HIHAT, true);
                break;
            }

            default: {
                break;
            }
        }
    }
}

void sequence_handle_button_drummachine(enum buttons btn) {
    uint32_t beat = 42; // TODO!!

    switch (btn) {
        case BTN_A: {
            bool val = !sequence_get(beat, CH_KICK);
            sequence_set(beat, CH_KICK, val);
            break;
        }

        case BTN_B: {
            bool val = !sequence_get(beat, CH_SNARE);
            sequence_set(beat, CH_SNARE, val);
            break;
        }

        case BTN_C: {
            bool val = !sequence_get(beat, CH_HIHAT);
            sequence_set(beat, CH_HIHAT, val);
            break;
        }

        default: {
            break;
        }
    }
}

void sequence_run(void) {
    uint32_t now = to_ms_since_boot(get_absolute_time());

    if ((last_t + ms_per_beat) >= now) {
        uint32_t i = last_i + 1;
        if (i >= beats) i = 0;

        led_set(last_i, false);
        led_set(i, true);

        for (uint ch = 0; ch < NUM_CHANNELS; ch++) {
            if (sequence[i] & (1 << ch)) {
                pulse_trigger_out(i, channel_times[ch]);
            }
        }

        last_t = now;
        last_i = i;
    }
}
