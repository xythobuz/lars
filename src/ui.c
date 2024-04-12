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
#include "pico/stdlib.h"

#include "adc.h"
#include "buttons.h"
#include "lcd.h"
#include "led.h"
#include "sequence.h"
#include "ui.h"

#define REDRAW_MS 2000

static bool rec_held_down = false;
static enum ui_modes ui_mode = 0;
static enum machine_modes machine_mode = 0;
static uint32_t last_redraw = 0;

enum machine_modes ui_get_machinemode(void) {
    return machine_mode;
}

static void ui_redraw(void) {
    char mode[64] = {0};
    char val[64] = {0};
    char bat[64] = {0};

    switch (ui_mode) {
        case UI_BPM: {
            snprintf(mode, sizeof(mode) - 1, "BPM:");
            snprintf(val, sizeof(val) - 1, "%"PRIu32, sequence_get_bpm());
            break;
        }

        case UI_MODE: {
            if ((machine_mode == MODE_LOOPSTATION) && (sequence_get_ms() != 0)) {
                snprintf(mode, sizeof(mode) - 1, "Mode: %"PRIu32"ms", sequence_get_ms());
            }else {
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

                default: {
                    printf("%s: invalid machine mode: %d\n", __func__, machine_mode);
                    machine_mode = 0;
                    ui_redraw();
                    return;
                }
            }
            break;
        }

        case UI_LENGTH: {
            snprintf(mode, sizeof(mode) - 1, "Length:");
            snprintf(val, sizeof(val) - 1, "%"PRIu32, sequence_get_beats());
            break;
        }

        case UI_BANK: {
            snprintf(mode, sizeof(mode) - 1, "Bank:");
            snprintf(val, sizeof(val) - 1, "%"PRIu32, sequence_get_bank());
            break;
        }

        default: {
            printf("%s: invalid mode: %d\n", __func__, ui_mode);
            ui_mode = 0;
            ui_redraw();
            return;
        }
    }

    snprintf(bat, sizeof(bat) - 1, "Bat: %.2fV", bat_get());
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

        case BTN_REC:
        case BTN_D:
        case BTN_H: {
            rec_held_down = val;
            sequence_looptime(!val);
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

static void ui_buttons(enum buttons btn, bool val) {
    switch (btn) {
        case BTN_CLICK: {
            if (val) {
                ui_mode = (ui_mode + 1) % UI_NUM_MODES;

                // allow other ui modes only in drumkit mode
                if (machine_mode == MODE_LOOPSTATION) {
                    ui_mode = 0;
                }

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

    switch (ui_mode) {
        case UI_BPM: {
            sequence_set_bpm(sequence_get_bpm() + val);
            break;
        }

        case UI_MODE: {
            int32_t tmp = machine_mode + val;
            while (tmp < 0) {
                tmp += MACHINE_NUM_MODES;
            }
            while (tmp >= MACHINE_NUM_MODES) {
                tmp -= MACHINE_NUM_MODES;
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

                // enable static LEDs in loopstation mode
                if (machine_mode == MODE_LOOPSTATION) {
                    led_set(NUM_CHANNELS, true);
                    led_set(NUM_CHANNELS + 4, true);
                }
            }
            break;
        }

        case UI_LENGTH: {
            sequence_set_beats(sequence_get_beats() + val);
            break;
        }

        case UI_BANK: {
            sequence_set_bank(sequence_get_bank() + val);
            break;
        }

        default: {
            printf("%s: invalid mode: %d\n", __func__, ui_mode);
            ui_mode = 0;
            ui_encoder(val);
            return;
        }
    }

    ui_redraw();
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
    if (now >= (last_redraw + REDRAW_MS)) {
        ui_redraw();
        last_redraw = now;
    }
}
