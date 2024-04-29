/*
 * mem.h
 *
 * Copyright (c) 2023 - 2024 Thomas Buck (thomas@xythobuz.de)
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

#ifndef __MEM_H__
#define __MEM_H__

#include "hardware/flash.h"

#include <stdint.h>
#include <stdbool.h>

#include "sequence.h"

/*
 * We use the last flash page for our persistent storage.
 * This is kept clear by our custom linker script.
 */
#define EEPROM_FLASH_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)

// to migrate settings when struct changes between releases
#define MEM_VERSION 0

struct mem_data {
    uint32_t boot_anim_ms;

    uint32_t ch_timings[NUM_CHANNELS];
    uint32_t ch_count; // to invalidate if number of channels changes
};

// .ch_timings is assigned in mem_load_defaults
#define MEM_DATA_INIT {           \
    .boot_anim_ms = LOGO_INIT_MS, \
    .ch_count = NUM_CHANNELS,     \
}

void mem_load(void);
void mem_write(void);
struct mem_data *mem_data(void);
void mem_load_defaults(void);

#endif // __MEM_H__
