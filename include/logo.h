/*
 * logo.h
 *
 * Copyright (c) 2022 - 2023 Thomas Buck (thomas@xythobuz.de)
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

#ifndef __LOGO_H__
#define __LOGO_H__

#include <stdint.h>

#define LOGO_HEIGHT 64
#define LOGO_WIDTH 128

extern uint8_t logo_data[LOGO_HEIGHT * LOGO_WIDTH / 8];

#endif // __LOGO_H__
