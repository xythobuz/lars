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
#include "log.h"
#include "mem.h"
#include "pulse.h"
#include "ui.h"
#include "sequence.h"

static uint64_t us_per_beat = 0;
static uint64_t last_t = 0;
static uint64_t beattimer_start = 0;

static uint32_t beats = MAX_BEATS;
static uint32_t last_i = 0;
static uint32_t bank = 0;
static uint32_t max_banks_currently = 0;
static uint32_t channel = 0;
static bool button_held[NUM_BTNS] = {0};

static enum channels sequence[MAX_BEATS] = {0};

void sequence_init(void) {
    us_per_beat = 0;
    beats = MAX_BEATS;
    last_t = to_us_since_boot(get_absolute_time());
    last_i = 0;
    max_banks_currently = (beats + (NUM_BTNS - 1)) / NUM_BTNS;

    for (uint i = 0; i < MAX_BEATS; i++) {
        sequence[i] = 0;
    }
}

uint32_t sequence_get_max_banks(void) {
    return max_banks_currently;
}

void sequence_set_bpm(uint32_t new_bpm) {
    us_per_beat = (60UL * 1000UL * 1000UL) / new_bpm;
    debug("bpm @ %"PRIu64"us = %"PRIu64"us", us_per_beat, us_per_beat / beats);
}

uint32_t sequence_get_bpm(void) {
    if (us_per_beat == 0) {
        return 0;
    }
    return (60UL * 1000UL * 1000UL) / us_per_beat;
}

void sequence_set_beats(uint32_t new_beats) {
    beats = (new_beats <= MAX_BEATS) ? new_beats : MAX_BEATS;
    debug("beats @ %"PRIu64"us = %"PRIu64"us", us_per_beat, us_per_beat / beats);

    max_banks_currently = (beats + (NUM_BTNS - 1)) / NUM_BTNS;
    bank = (bank < max_banks_currently) ? bank : 0;
}

uint32_t sequence_get_beats(void) {
    return beats;
}

void sequence_set_bank(uint32_t new_bank) {
    uint32_t b = (new_bank < MAX_BANKS) ? new_bank : MAX_BANKS;
    bank = (b < max_banks_currently) ? b : 0;
}

uint32_t sequence_get_bank(void) {
    return bank;
}

void sequence_set_channel(uint32_t new_channel) {
    channel = (new_channel < NUM_CHANNELS) ? new_channel : 0;
}

uint32_t sequence_get_channel(void) {
    return channel;
}

uint64_t sequence_get_us(void) {
    return us_per_beat;
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

void sequence_handle_button_loopstation(enum buttons btn, bool val) {
    button_held[btn] = val;

    switch (btn) {
        case BTN_A: {
            // only react to button down events
            if (!val) {
                break;
            }

            // trigger outputs and LEDs
            pulse_trigger_out(0, mem_data()->ch_timings[0]);
            pulse_trigger_led(0, mem_data()->ch_timings[0]);
            pulse_trigger_led(4, mem_data()->ch_timings[0]);
            break;
        }

        case BTN_B: {
            // only react to button down events
            if (!val) {
                break;
            }

            // trigger outputs and LEDs
            pulse_trigger_out(1, mem_data()->ch_timings[1]);
            pulse_trigger_led(1, mem_data()->ch_timings[1]);
            pulse_trigger_led(5, mem_data()->ch_timings[1]);
            break;
        }

        case BTN_C: {
            // only react to button down events
            if (!val) {
                break;
            }

            // trigger outputs and LEDs
            pulse_trigger_out(2, mem_data()->ch_timings[2]);
            pulse_trigger_led(2, mem_data()->ch_timings[2]);
            pulse_trigger_led(6, mem_data()->ch_timings[2]);
            break;
        }

        case BTN_D:
        case BTN_H: {
            sequence_looptime(!val);
            if (!val) {
                ui_redraw();
            }
            break;
        }

        case BTN_CLEAR: {
            // only react to button down events
            if (!val) {
                break;
            }

            if (button_held[BTN_H]) {
                // right REC: clear all loops in all banks
                // TODO other banks
                sequence_init();
            } else if (button_held[BTN_D]) {
                // left REC: clear the current loop, including the length
                sequence_init();
            } else if (button_held[BTN_E] || button_held[BTN_F] || button_held[BTN_G]) {
                // channel mute buttons: clear only this channel in the current loop
                // TODO
            } else {
                // on its own: clear all channels of the current loop, keeping the set length
                // TODO
            }

            ui_redraw();
            break;
        }

        default: {
            break;
        }
    }

    // set marker in sequence when one of the REC buttons was held
    if ((button_held[BTN_D] || button_held[BTN_H]) && val) {
        switch (btn) {
            case BTN_A: {
                sequence_set(last_i, CH_KICK, true);
                break;
            }

            case BTN_B: {
                sequence_set(last_i, CH_SNARE, true);
                break;
            }

            case BTN_C: {
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
    switch (btn) {
        case BTN_A:
        case BTN_B:
        case BTN_C:
        case BTN_D:
        case BTN_E:
        case BTN_F:
        case BTN_G:
        case BTN_H: {
            uint32_t beat = (btn - BTN_A) + bank * LED_COUNT;
            bool val = !sequence_get(beat, 1 << channel);
            sequence_set(beat, 1 << channel, val);
            break;
        }

        default: {
            break;
        }
    }
}

void sequence_looptime(bool fin) {
    if (!fin) {
        beattimer_start = to_us_since_boot(get_absolute_time());
    } else {
        if (us_per_beat == 0) {
            uint64_t now = to_us_since_boot(get_absolute_time());
            uint64_t diff = now - beattimer_start;
            us_per_beat = diff;
            debug("looptime @ %"PRIu64"us = %"PRIu64"us", us_per_beat, us_per_beat / beats);
        }
    }
}

void sequence_run(void) {
    uint64_t now = to_us_since_boot(get_absolute_time());
    uint64_t us = us_per_beat / beats;

    if ((us > 0) && (now >= (last_t + us))) {
        //debug("trigger");

        uint32_t i = last_i + 1;
        if (i >= beats) i = 0;

        if (ui_get_machinemode() == MODE_DRUMMACHINE) {
            led_set(last_i, false);
            led_set(i, true);
        }

        for (uint ch = 0; ch < NUM_CHANNELS; ch++) {
            // skip muted channels
            if ((ui_get_machinemode() == MODE_LOOPSTATION) && button_held[BTN_E + ch]) {
                continue;
            }

            if (sequence[i] & (1 << ch)) {
                // trigger channel solenoid
                pulse_trigger_out(ch, mem_data()->ch_timings[ch]);

                // flash LEDs in loopstation mode
                if (ui_get_machinemode() == MODE_LOOPSTATION) {
                    pulse_trigger_led(ch, mem_data()->ch_timings[ch]);
                    pulse_trigger_led(ch + 4, mem_data()->ch_timings[ch]);
                }
            }
        }

        last_t += us;
        last_i = i;
    }
}
