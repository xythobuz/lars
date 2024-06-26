/*
 * main.h
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

#ifndef __MAIN_H__
#define __MAIN_H__

enum hw_versions {
    HW_UNKNOWN = 0,
    HW_PROTOTYPE,
    HW_V2,
};

extern enum hw_versions hw_type;

void main_loop_hw(void);

void reset_to_bootloader(void);
void reset_to_main(void);

#endif // __MAIN_H__
