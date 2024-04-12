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
#include <inttypes.h>
#include "pico/stdlib.h"

#include "led.h"
#include "pulse.h"
#include "ui.h"
#include "sequence.h"

static const uint32_t channel_times[NUM_CHANNELS] = CH_GPIO_TIMINGS;

static uint32_t ms_per_beat = 0;
static uint32_t beats = MAX_BEATS;
static uint32_t last_t = 0;
static uint32_t last_i = 0;
static uint32_t bank = 0;
static uint32_t beattimer_start = 0;

static enum channels sequence[MAX_BEATS] = {0};

void sequence_init(void) {
    ms_per_beat = 0;
    beats = MAX_BEATS;
    last_t = to_ms_since_boot(get_absolute_time());
    last_i = 0;

    for (uint i = 0; i < MAX_BEATS; i++) {
        sequence[i] = 0;
    }
}

void sequence_set_bpm(uint32_t new_bpm) {
    ms_per_beat = 60000 / new_bpm / beats;
    printf("bpm now %"PRIu32" = %"PRIu32"ms\n", new_bpm, ms_per_beat);
}

uint32_t sequence_get_bpm(void) {
    return 60000 / (ms_per_beat * beats);
}

uint32_t sequence_get_ms(void) {
    return ms_per_beat;
}

void sequence_set_beats(uint32_t new_beats) {
    beats = (new_beats <= MAX_BEATS) ? new_beats : MAX_BEATS;

    uint32_t max_banks_currently = (beats + (NUM_BTNS - 1)) / NUM_BTNS;
    bank = (bank < max_banks_currently) ? bank : max_banks_currently;
}

uint32_t sequence_get_beats(void) {
    return beats;
}

void sequence_set_bank(uint32_t new_bank) {
    uint32_t b = (new_bank < MAX_BANKS) ? new_bank : MAX_BANKS;

    uint32_t max_banks_currently = (beats + (NUM_BTNS - 1)) / NUM_BTNS;
    bank = (b < max_banks_currently) ? b : max_banks_currently;
}

uint32_t sequence_get_bank(void) {
    return bank;
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
        case BTN_A:
        case BTN_E: {
            pulse_trigger_out(0, channel_times[0]);
            pulse_trigger_led(0, channel_times[0]);
            pulse_trigger_led(4, channel_times[0]);
            break;
        }

        case BTN_B:
        case BTN_F: {
            pulse_trigger_out(1, channel_times[1]);
            pulse_trigger_led(1, channel_times[1]);
            pulse_trigger_led(5, channel_times[1]);
            break;
        }

        case BTN_C:
        case BTN_G: {
            pulse_trigger_out(2, channel_times[2]);
            pulse_trigger_led(2, channel_times[2]);
            pulse_trigger_led(6, channel_times[2]);
            break;
        }

        default: {
            break;
        }
    }

    if (rec) {
        switch (btn) {
            case BTN_A:
            case BTN_E: {
                sequence_set(last_i, CH_KICK, true);
                break;
            }

            case BTN_B:
            case BTN_F: {
                sequence_set(last_i, CH_SNARE, true);
                break;
            }

            case BTN_C:
            case BTN_G: {
                sequence_set(last_i, CH_HIHAT, true);
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
            // TODO kick is wrong here
            bool val = !sequence_get(beat, CH_KICK);
            sequence_set(beat, CH_KICK, val);
            break;
        }

        case BTN_B: {
            // TODO snare is wrong here
            bool val = !sequence_get(beat, CH_SNARE);
            sequence_set(beat, CH_SNARE, val);
            break;
        }

        case BTN_C: {
            // TODO hihat is wrong here
            bool val = !sequence_get(beat, CH_HIHAT);
            sequence_set(beat, CH_HIHAT, val);
            break;
        }

        default: {
            break;
        }
    }
}

void sequence_looptime(bool fin) {
    if (!fin) {
        beattimer_start = to_ms_since_boot(get_absolute_time());
    } else {
        if (ms_per_beat == 0) {
            uint32_t now = to_ms_since_boot(get_absolute_time());
            uint32_t diff = now - beattimer_start;
            ms_per_beat = diff / beats;
            printf("looptime: diff=%"PRIu32"ms per_beat=%"PRIu32"ms\n", diff, ms_per_beat);
        }
    }
}

void sequence_run(void) {
    uint32_t now = to_ms_since_boot(get_absolute_time());

    if ((ms_per_beat > 0) && (now >= (last_t + ms_per_beat))) {
        //printf("trigger\n");

        uint32_t i = last_i + 1;
        if (i >= beats) i = 0;

        if (ui_get_machinemode() == MODE_DRUMMACHINE) {
            led_set(last_i, false);
            led_set(i, true);
        }

        for (uint ch = 0; ch < NUM_CHANNELS; ch++) {
            if (sequence[i] & (1 << ch)) {
                pulse_trigger_out(ch, channel_times[ch]);
                if (ui_get_machinemode() == MODE_LOOPSTATION) {
                    pulse_trigger_led(ch, channel_times[ch]);
                }
            }
        }

        last_t = now;
        last_i = i;
    }
}
