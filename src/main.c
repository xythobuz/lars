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
#define LOGO_INIT_MS 1500

static const uint gpio_hw_detect = 21;
enum hw_versions hw_type = HW_UNKNOWN;

static void reset_to_bootloader(void) {
#ifdef PICO_DEFAULT_LED_PIN
    reset_usb_boot(1 << PICO_DEFAULT_LED_PIN, 0);
#else // ! PICO_DEFAULT_LED_PIN
    reset_usb_boot(0, 0);
#endif // PICO_DEFAULT_LED_PIN
}

int main(void) {
    stdio_init_all();

    gpio_init(gpio_hw_detect);
    gpio_set_dir(gpio_hw_detect, GPIO_IN);
    gpio_pull_up(gpio_hw_detect);
    if (gpio_get(gpio_hw_detect)) {
        hw_type = HW_PROTOTYPE;
    } else {
        hw_type = HW_V2;
    }

    bat_init();
    buttons_init();
    encoder_init();
    lcd_init();
    led_init();

    // show splash for a bit and animate LEDs
    for (uint i = 0; i < LED_COUNT; i++) {
        led_set(i, true);
        sleep_ms(LOGO_INIT_MS / LED_COUNT);
    }

    // turn off LEDs at end of init
    for (uint i = 0; i < LED_COUNT; i++) {
        led_set(i, false);
    }

    sequence_init();
    ui_init();

    printf("init done\n");
    watchdog_enable(WATCHDOG_PERIOD_MS, 1);

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
