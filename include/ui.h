/*
 * ui.h
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

#ifndef __UI_H__
#define __UI_H__

#include <stdint.h>

#define KEEP_IN_RANGE(val, min, len) { \
    while (val > (len - min)) {        \
        val -= len;                    \
    }                                  \
    while (val < min) {                \
        val += len;                    \
    }                                  \
}

enum machine_modes {
    MODE_LOOPSTATION = 0,
    MODE_DRUMMACHINE,
    MODE_MIDI,

    MACHINE_NUM_MODES
};

void ui_init(void);
void ui_encoder(int32_t val);
void ui_run(void);

void ui_midi_set(uint8_t channel, uint8_t note, uint8_t velocity);

enum machine_modes ui_get_machinemode(void);

#endif // __UI_H__
