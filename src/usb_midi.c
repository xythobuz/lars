/*
 * Extended from TinyUSB example code.
 *
 * Copyright (c) 2022 - 2023 Thomas Buck (thomas@xythobuz.de)
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "bsp/board.h"
#include "tusb.h"

#include "log.h"
#include "ui.h"
#include "usb_midi.h"

#define MIDI_CABLE_NUM 0 // MIDI jack associated with USB endpoint

#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_ON 0x90

void usb_midi_tx(uint8_t channel, uint8_t note, uint8_t velocity) {
    uint8_t packet[3] = {
        ((velocity == 0) ? MIDI_NOTE_OFF : MIDI_NOTE_ON) | channel,
        note, velocity,
    };

    tud_midi_stream_write(MIDI_CABLE_NUM, packet, sizeof(packet));
}

void usb_midi_run(void) {
    while (tud_midi_available()) {
        uint8_t packet[4];
        tud_midi_packet_read(packet);
        //debug("rx: %02X %02X %02X %02X", packet[0], packet[1], packet[2], packet[3]);

        uint8_t type = packet[1] & 0xF0;
        uint8_t channel = packet[1] & 0x0F;
        uint8_t note = packet[2];
        uint8_t velocity = packet[3];
        //debug("rx: type=%02X channel=%d note=%d velocity=%d", type, channel, note, velocity);

        if (type == MIDI_NOTE_OFF) {
            ui_midi_set(channel, note, 0);
        } else if (type == MIDI_NOTE_ON) {
            ui_midi_set(channel, note, (velocity == 0) ? 127 : velocity);
        }
    }
}
