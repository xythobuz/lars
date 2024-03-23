/*
 * sequence.h
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

#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__

#include <stdint.h>
#include "buttons.h"

enum channels {
    CH1 = (1 << 0),
    CH2 = (1 << 1),
    CH3 = (1 << 2),

    CH_KICK = CH1,
    CH_SNARE = CH2,
    CH_HIHAT = CH3,

    NUM_CHANNELS = 3
};

#define CH_GPIO_TIMINGS { 42, 42, 42 } // in milliseconds

#define MAX_BEATS 32
#define MAX_BANKS (MAX_BEATS / NUM_BTNS)

void sequence_init(void);

void sequence_set_bpm(uint32_t new_bpm);
uint32_t sequence_get_bpm(void);

void sequence_set_beats(uint32_t new_beats);
uint32_t sequence_get_beats(void);

void sequence_set_bank(uint32_t new_bank);
uint32_t sequence_get_bank(void);

void sequence_handle_button_loopstation(enum buttons btn, bool rec);
void sequence_handle_button_drummachine(enum buttons btn);

void sequence_run(void);

#endif // __SEQUENCE_H__
