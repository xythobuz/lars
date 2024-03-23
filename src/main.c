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
#include "pico/bootrom.h"
#include "hardware/watchdog.h"

#include "adc.h"
#include "buttons.h"
#include "encoder.h"
#include "lcd.h"
#include "led.h"
#include "pulse.h"
#include "sequence.h"
#include "ui.h"
#include "main.h"

#define WATCHDOG_PERIOD_MS 100

static void reset_to_bootloader(void) {
#ifdef PICO_DEFAULT_LED_PIN
    reset_usb_boot(1 << PICO_DEFAULT_LED_PIN, 0);
#else // ! PICO_DEFAULT_LED_PIN
    reset_usb_boot(0, 0);
#endif // PICO_DEFAULT_LED_PIN
}

int main(void) {
    //watchdog_enable(WATCHDOG_PERIOD_MS, 1);
    stdio_init_all();
    bat_init();
    buttons_init();
    encoder_init();
    lcd_init();
    led_init();

    // show splash for a bit
    sleep_ms(500);

    sequence_init();
    ui_init();
    printf("init done\n");

    int32_t last_epos = 0;

    while (1) {
        watchdog_update();
        buttons_run();
        encoder_run();
        sequence_run();
        pulse_run();
        ui_run();

        int32_t epos = encoder_pos();
        if (epos != last_epos) {
            ui_encoder(epos - last_epos);
            last_epos = epos;
        }

        int c = getchar_timeout_us(0);
        if (c == 0x18) {
            reset_to_bootloader();
        } else if (c != PICO_ERROR_TIMEOUT) {
            printf("%c", c);
        }
    }

    return 0;
}
