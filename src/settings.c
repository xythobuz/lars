/*
 * settings.c
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

#include <stdio.h>
#include <inttypes.h>

#include "pico/stdlib.h"
#include "hardware/watchdog.h"

#include "config.h"
#include "buttons.h"
#include "console.h"
#include "encoder.h"
#include "lcd.h"
#include "log.h"
#include "mem.h"
#include "sequence.h"
#include "ui.h"
#include "usb.h"
#include "settings.h"

static uint32_t setting = 0;

static void settings_redraw(void) {
    char mode[64] = {0};
    char val[64] = {0};
    char bat[64] = {0};

    if (setting >= (NUM_CHANNELS + 1)) {
        snprintf(mode, sizeof(mode) - 1, "Memory");
        snprintf(val, sizeof(val) - 1, "Save");
    } else if (setting == NUM_CHANNELS) {
        snprintf(mode, sizeof(mode) - 1, "Boot Anim");
        snprintf(val, sizeof(val) - 1, "%"PRIu32"ms", mem_data()->boot_anim_ms);
    } else {
        snprintf(mode, sizeof(mode) - 1, "Channel %"PRIu32, setting);
        snprintf(val, sizeof(val) - 1, "%"PRIu32"ms", mem_data()->ch_timings[setting]);
    }

    lcd_draw(mode, val, bat);
}

static void settings_buttons(enum buttons btn, bool val) {
    switch (btn) {
        case BTN_CLICK: {
            if (val) {
                setting = (setting + 1) % (NUM_CHANNELS + 2);
                settings_redraw();
            }
            break;
        }

        default: {
            break;
        }
    }
}

static bool settings_encoder(int32_t val) {
    if (val == 0) {
        return false;
    }

    if (setting >= (NUM_CHANNELS + 1)) {
        mem_write();
        lcd_draw("Save", "Ok!", "");
        return true;
    } else if (setting == NUM_CHANNELS) {
        int32_t tmp = mem_data()->boot_anim_ms + val;
        KEEP_IN_RANGE(tmp, 0, 10000);
        mem_data()->boot_anim_ms = tmp;
    } else {
        int32_t tmp = mem_data()->ch_timings[setting] + val;
        KEEP_IN_RANGE(tmp, 1, 100);
        mem_data()->ch_timings[setting] = tmp;
    }

    settings_redraw();
    return false;
}

void settings_init(void) {
    buttons_callback(settings_buttons);
    settings_redraw();
}

void settings_run(void) {
    while (1) {
        watchdog_update();
        buttons_run();
        encoder_run();
        usb_run();
        cnsl_run();

        if (settings_encoder(encoder_get_diff())) {
            break;
        }
    }
}
