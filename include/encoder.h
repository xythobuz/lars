/*
 * encoder.h
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

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <stdint.h>

void encoder_init(void);
void encoder_run(void);

int32_t encoder_pos(void);
int32_t encoder_get_diff(void);
uint32_t encoder_get_rpm(void);

#endif // __ENCODER_H__
