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

enum ui_modes {
    UI_BPM = 0,
    UI_MODE,
    UI_BANK,
    UI_LENGTH,

    UI_NUM_MODES
};

enum machine_modes {
    MODE_LOOPSTATION = 0,
    MODE_DRUMMACHINE,

    MACHINE_NUM_MODES
};

void ui_init(void);
void ui_encoder(int32_t val);

#endif // __UI_H__
