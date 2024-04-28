/*
 * ui.c
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
#include <math.h>
#include "pico/stdlib.h"

#include "adc.h"
#include "buttons.h"
#include "lcd.h"
#include "led.h"
#include "sequence.h"
#include "usb.h"
#include "usb_midi.h"
#include "ui.h"

#define BAT_FETCH_MS 66

enum ui_settings {
    SETTING_MODE = 0,

    // loop station
    SETTING_SPEED,

    // drum machine
    SETTING_BPM,
    SETTING_BANK,
    SETTING_LENGTH,

    // midi
    SETTING_CH_RX,
    SETTING_CH_TX,

    SETTING_NUM_MODES
};

static bool allowed_settings[MACHINE_NUM_MODES][SETTING_NUM_MODES] = {
    // MODE_LOOPSTATION
    {
        true, true,
        false, false, false,
        false, false,
    },

    // MODE_DRUMMACHINE
    {
        true, false,
        true, true, true,
        false, false,
    },

    // MODE_MIDI
    {
        true, false,
        false, false, false,
        true, true,
    },
};

static bool rec_held_down = false;
static enum ui_settings ui_setting = 0;
static enum machine_modes machine_mode = 0;
static uint32_t last_bat_fetch = 0;
static float last_voltage = 0.0f;
static float last_percentage = 0.0f;
static uint8_t midi_rx = 0, midi_tx = 0;

enum machine_modes ui_get_machinemode(void) {
    return machine_mode;
}

static void ui_redraw(void) {
    char mode[64] = {0};
    char val[64] = {0};
    char bat[64] = {0};

    switch (ui_setting) {
        case SETTING_BPM: {
            snprintf(mode, sizeof(mode) - 1, "BPM:");
            snprintf(val, sizeof(val) - 1, "%"PRIu32, sequence_get_bpm());
            break;
        }

        case SETTING_MODE: {
            if ((machine_mode == MODE_LOOPSTATION) && (sequence_get_ms() != 0)) {
                snprintf(mode, sizeof(mode) - 1, "Mode: %"PRIu32"ms", sequence_get_ms());
            } else {
                snprintf(mode, sizeof(mode) - 1, "Mode:");
            }

            switch (machine_mode) {
                case MODE_LOOPSTATION: {
                    snprintf(val, sizeof(val) - 1, "Loop");
                    break;
                }

                case MODE_DRUMMACHINE: {
                    snprintf(val, sizeof(val) - 1, "Drum");
                    break;
                }

                case MODE_MIDI: {
                    snprintf(val, sizeof(val) - 1, "MIDI");
                    break;
                }

                default: {
                    printf("%s: invalid machine mode: %d\n", __func__, machine_mode);
                    machine_mode = 0;
                    ui_redraw();
                    return;
                }
            }

            break;
        }

        case SETTING_LENGTH: {
            snprintf(mode, sizeof(mode) - 1, "Length:");
            snprintf(val, sizeof(val) - 1, "%"PRIu32, sequence_get_beats());
            break;
        }

        case SETTING_BANK: {
            snprintf(mode, sizeof(mode) - 1, "Bank:");
            snprintf(val, sizeof(val) - 1, "%"PRIu32, sequence_get_bank());
            break;
        }

        case SETTING_SPEED: {
            snprintf(mode, sizeof(mode) - 1, "Speed:");
            snprintf(val, sizeof(val) - 1, "%"PRIu32, sequence_get_ms());
            break;
        }

        case SETTING_CH_RX: {
            snprintf(mode, sizeof(mode) - 1, "Rx-Ch:");
            snprintf(val, sizeof(val) - 1, "%"PRIu8, midi_rx + 1);
            break;
        }

        case SETTING_CH_TX: {
            snprintf(mode, sizeof(mode) - 1, "Tx-Ch:");
            snprintf(val, sizeof(val) - 1, "%"PRIu8, midi_tx + 1);
            break;
        }

        default: {
            printf("%s: invalid setting: %d\n", __func__, ui_setting);
            ui_setting = 0;
            ui_redraw();
            return;
        }
    }

    snprintf(bat, sizeof(bat) - 1, "Bat: %.1f%% (%.2fV)", last_percentage, last_voltage);
    lcd_draw(mode, val, bat);
}

static void ui_buttons_loopstation(enum buttons btn, bool val) {
    switch (btn) {
        case BTN_A:
        case BTN_B:
        case BTN_C:
        case BTN_E:
        case BTN_F:
        case BTN_G: {
            if (val) {
                sequence_handle_button_loopstation(btn, rec_held_down);
            }
            break;
        }

        case BTN_REC: {
            // reset sequence
            sequence_init();
            ui_redraw();
            break;
        }

        case BTN_D:
        case BTN_H: {
            rec_held_down = val;
            sequence_looptime(!val);
            if (!val) {
                ui_redraw();
            }
            break;
        }

        default: {
            printf("%s: invalid btn: %d\n", __func__, btn);
            break;
        }
    }
}

static void ui_buttons_drummachine(enum buttons btn, bool val) {
    switch (btn) {
        case BTN_A:
        case BTN_B:
        case BTN_C:
        case BTN_D:
        case BTN_E:
        case BTN_F:
        case BTN_G:
        case BTN_H:
        case BTN_REC: {
            if (val) {
                sequence_handle_button_drummachine(btn);
            }
            break;
        }

        default: {
            printf("%s: invalid btn: %d\n", __func__, btn);
            break;
        }
    }
}

static void ui_buttons_midi(enum buttons btn, bool val) {
    // TODO
    (void)btn;
    (void)val;
}

static void ui_buttons(enum buttons btn, bool val) {
    switch (btn) {
        case BTN_CLICK: {
            if (val) {
                // only allow settings for this mode
                do {
                    ui_setting = (ui_setting + 1) % SETTING_NUM_MODES;
                } while (!allowed_settings[machine_mode][ui_setting]);

                ui_redraw();
            }
            break;
        }

        default: {
            switch (machine_mode) {
                case MODE_LOOPSTATION: {
                    ui_buttons_loopstation(btn, val);
                    break;
                }

                case MODE_DRUMMACHINE: {
                    ui_buttons_drummachine(btn, val);
                    break;
                }

                case MODE_MIDI: {
                    ui_buttons_midi(btn, val);
                    break;
                }

                default: {
                    printf("%s: invalid mode: %d\n", __func__, machine_mode);
                    machine_mode = 0;
                    ui_buttons(btn, val);
                    break;
                }
            }
            break;
        }
    }
}

void ui_encoder(int32_t val) {
    if (val == 0) {
        return;
    }

    switch (ui_setting) {
        case SETTING_BPM: {
            sequence_set_bpm(sequence_get_bpm() + val);
            break;
        }

        case SETTING_MODE: {
            int32_t tmp = machine_mode + val;

            while (tmp < 0) {
                tmp += MACHINE_NUM_MODES;
            }
            while (tmp >= MACHINE_NUM_MODES) {
                tmp -= MACHINE_NUM_MODES;
            }

            // midi only when connected to pc
            if ((tmp == MODE_MIDI) && !usb_is_connected()) {
                tmp = (tmp + 1) % MACHINE_NUM_MODES;
            }

            enum machine_modes prev_mode = machine_mode;
            machine_mode = tmp;

            printf("mode add %"PRIi32" now %d\n", val, machine_mode);

            if (prev_mode != machine_mode) {
                // reset sequence
                sequence_init();

                // turn off all LEDs
                for (uint i = 0; i < LED_COUNT; i++) {
                    led_set(i, false);
                }

                if (machine_mode == MODE_LOOPSTATION) {
                    sequence_set_beats(MAX_BEATS);

                    // enable static LEDs in loopstation mode
                    led_set(NUM_CHANNELS, true);
                    led_set(NUM_CHANNELS + 4, true);
                } else if (machine_mode == MODE_DRUMMACHINE) {
                    sequence_set_beats(LED_COUNT);
                }
            }
            break;
        }

        case SETTING_LENGTH: {
            sequence_set_beats(sequence_get_beats() + val);
            break;
        }

        case SETTING_BANK: {
            sequence_set_bank(sequence_get_bank() + val);
            break;
        }

        case SETTING_SPEED: {
            sequence_set_ms(sequence_get_ms() + val);
            break;
        }

        case SETTING_CH_RX: {
            midi_rx = (uint8_t)(midi_rx + val) % MIDI_MAX_CH;
            break;
        }

        case SETTING_CH_TX: {
            midi_tx = (uint8_t)(midi_tx + val) % MIDI_MAX_CH;
            break;
        }

        default: {
            printf("%s: invalid setting: %d\n", __func__, ui_setting);
            ui_setting = 0;
            ui_encoder(val);
            return;
        }
    }

    ui_redraw();
}

void ui_midi_set(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (machine_mode != MODE_MIDI) {
        return;
    }

    if (channel != midi_rx) {
        return;
    }

    note = note % NUM_CHANNELS;
    if (velocity > 0) {
        sequence_handle_button_loopstation(BTN_A + note, false);
    }
}

void ui_init(void) {
    buttons_callback(ui_buttons);

    // enable static LEDs in loopstation mode
    machine_mode = MODE_LOOPSTATION;
    led_set(NUM_CHANNELS, true);
    led_set(NUM_CHANNELS + 4, true);

    ui_redraw();
}

void ui_run(void) {
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (now >= (last_bat_fetch + BAT_FETCH_MS)) {
        last_bat_fetch = now;

        float volt = bat_get();
        float percentage = bat_to_percent(volt);
        if ((fabsf(volt - last_voltage) >= 0.01f)
                || (fabsf(percentage - last_percentage) >= 0.1f)) {
            last_voltage = volt;
            last_percentage = percentage;
            ui_redraw();
        }
    }
}
