/*
 * main.c
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
#include "pico/stdlib.h"
#include "hardware/watchdog.h"

#include "buttons.h"
#include "encoder.h"
#include "lcd.h"
#include "sequence.h"
#include "ui.h"
#include "main.h"

#define WATCHDOG_PERIOD_MS 100

int main(void) {
    watchdog_enable(WATCHDOG_PERIOD_MS, 1);
    stdio_init_all();
    buttons_init();
    lcd_init();
    sequence_init();
    ui_init();
    printf("init done\n");

    while (1) {
        watchdog_update();
        buttons_run();
        encoder_run();
        sequence_run();
        ui_run();
    }

    return 0;
}
