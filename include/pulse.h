/*
 * pulse.h
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

#ifndef __PULSE_H__
#define __PULSE_H__

#include <stdint.h>

void pulse_trigger_out(uint32_t i, uint32_t t_ms);
void pulse_trigger_led(uint32_t i, uint32_t t_ms);
void pulse_run(void);

#endif // __PULSE_H__
