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
#include "pico/stdlib.h"

#include "buttons.h"
#include "sequence.h"
#include "ui.h"

static bool rec_held_down = false;
static enum ui_modes ui_mode = 0;
static enum machine_modes machine_mode = 0;

static void ui_redraw(void) {
    switch (ui_mode) {
        case UI_BPM: {
            break;
        }

        case UI_MODE: {
            break;
        }

        case UI_LENGTH: {
            break;
        }

        case UI_BANK: {
            break;
        }

        default: {
            printf("%s: invalid mode: %d\n", __func__, ui_mode);
            ui_mode = 0;
            ui_redraw();
            break;
        }
    }
}

static void ui_buttons_loopstation(enum buttons btn, bool val) {
    switch (btn) {
        case BTN_A:
        case BTN_B:
        case BTN_C: {
            if (val) {
                sequence_handle_button_loopstation(btn, rec_held_down);
            }
            break;
        }

        case BTN_REC: {
            rec_held_down = val;
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
            ui_mode = (ui_mode + 1) % UI_NUM_MODES;
            ui_redraw();
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

void ui_init(void) {
    buttons_callback(ui_buttons);
    ui_redraw();
}
