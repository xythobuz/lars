/*
 * usb_midi.h
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

#ifndef __USB_MIDI_H__
#define __USB_MIDI_H__

#include <stdint.h>

#define MIDI_MAX_CH 16

void usb_midi_tx(uint8_t channel, uint8_t note, uint8_t velocity);

void usb_midi_run(void);

#endif // __USB_MIDI_H__
