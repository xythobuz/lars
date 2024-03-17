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

#include "pico/stdlib.h"

#include "buttons.h"
#include "ui.h"

static bool rec_held_down = false;

static void ui_buttons(enum buttons btn, bool val) {
    switch (btn) {
        case BTN_A:
        case BTN_B:
        case BTN_C:
            if (val) {
                sequence_handle_button(btn, rec_held_down);
            }
            break;

        case BTN_REC:
            rec_held_down = val;
            break;

        case BTN_CLICK:
            break;

        default:
            break;
    }
}

void ui_init(void) {
    buttons_callback(ui_buttons);
}

void ui_run(void) {

}
