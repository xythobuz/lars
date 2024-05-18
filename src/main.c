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

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/watchdog.h"

#include "config.h"
#include "adc.h"
#include "buttons.h"
#include "console.h"
#include "encoder.h"
#include "lcd.h"
#include "led.h"
#include "log.h"
#include "logo.h"
#include "mem.h"
#include "pulse.h"
#include "sequence.h"
#include "settings.h"
#include "ui.h"
#include "usb.h"
#include "main.h"

static const uint gpio_hw_detect = 21;

enum hw_versions hw_type = HW_UNKNOWN;

static bool debug_buttons[NUM_BTNS] = {0};

static void debug_buttons_callback(enum buttons btn, bool v) {
    debug_buttons[btn] = v;
}

static uint32_t debug_count_buttons(void) {
    uint32_t cnt = 0;
    for (uint32_t i = 0; i < NUM_BTNS; i++) {
        if (debug_buttons[i]) {
            cnt++;
        }
    }
    return cnt;
}

void reset_to_bootloader(void) {
    lcd_draw_bye();

#ifdef PICO_DEFAULT_LED_PIN
    reset_usb_boot(1 << PICO_DEFAULT_LED_PIN, 0);
#else // ! PICO_DEFAULT_LED_PIN
    reset_usb_boot(0, 0);
#endif // PICO_DEFAULT_LED_PIN
}

void reset_to_main(void) {
    watchdog_enable(1, 0);
    while (1);
}

static void sleep_ms_wd(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        watchdog_update();
        sleep_ms(1);
    }
}

static void hw_type_detection(void) {
    gpio_init(gpio_hw_detect);
    gpio_set_dir(gpio_hw_detect, GPIO_IN);
    gpio_pull_up(gpio_hw_detect);

    if (gpio_get(gpio_hw_detect)) {
        hw_type = HW_PROTOTYPE;
    } else {
        hw_type = HW_V2;
    }
}

static void animate_boot_combos(void) {
    // read out button state for debug options
    buttons_callback(debug_buttons_callback);
    for (uint i = 0; i < (DEBOUNCE_DELAY_MS + 5); i++) {
        watchdog_update();
        buttons_run();
        usb_run();
        cnsl_run();
        sleep_ms_wd(1);
    }

    uint32_t cnt = debug_count_buttons();

    // handle special button combos on boot
    if ((cnt == 2) && debug_buttons[BTN_CLEAR] && debug_buttons[BTN_CLICK]) {
        lcd_debug_buttons();
    } else if ((cnt == 1) && debug_buttons[BTN_CLEAR]) {
        // enter settings menu
        settings_init();
        settings_run();
        sleep_ms_wd(mem_data()->boot_anim_ms);
    } else if (cnt == 3) {
        // skip splash screen
    } else if ((cnt == 1) && debug_buttons[BTN_CLICK]) {
        // show version info
        lcd_draw_version();

        // wait until button is released
        uint32_t last = to_ms_since_boot(get_absolute_time());
        bool state = false;
        while (debug_buttons[BTN_CLICK]) {
            watchdog_update();
            buttons_run();
            usb_run();
            cnsl_run();
            uint32_t now = to_ms_since_boot(get_absolute_time());
            if ((now - last) >= 250) {
                state = !state;
                last = now;
                led_set(0, state);
            }
        }
        led_set(0, false);
    } else {
        // show splash for a bit and animate LEDs
        for (uint i = 0; i < LED_COUNT; i++) {
            watchdog_update();
            usb_run();
            cnsl_run();
            led_set(i, true);
            sleep_ms_wd(mem_data()->boot_anim_ms / LED_COUNT);
        }
    }

    // turn off LEDs at end of init
    for (uint i = 0; i < LED_COUNT; i++) {
        led_set(i, false);
    }
}

void main_loop_hw(void) {
    watchdog_update();

    usb_run();
    cnsl_run();
    buttons_run();
    encoder_run();
    sequence_run();
    pulse_run();
    ui_run();

    ui_encoder(encoder_get_diff());
}

int main(void) {
    watchdog_enable(WATCHDOG_PERIOD_MS, 1);

    cnsl_init();
    usb_init();
    mem_load();

    hw_type_detection();

    bat_init();
    buttons_init();
    encoder_init();
    lcd_init();
    led_init();

    // show logo
    lcd_draw_bitmap(logo_data,
                    LOGO_WIDTH, LOGO_HEIGHT,
                    0, 0);

    animate_boot_combos();

    sequence_init();
    ui_init();

    debug("init done\n");

    while (1) {
        main_loop_hw();
    }

    return 0;
}
