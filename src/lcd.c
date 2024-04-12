/*
 * lcd.c
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
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "ssd1306.h"
#include "logo.h"
#include "main.h"
#include "lcd.h"

static i2c_inst_t *gpio_i2c_proto = i2c0;
static const uint gpio_num_proto[2] = { 0, 1 };

static i2c_inst_t *gpio_i2c_v2 = i2c0;
static const uint gpio_num_v2[2] = { 16, 17 };

#define LCD_ADDR 0x3C

static ssd1306_t disp;

void lcd_init(void) {
    if (hw_type == HW_PROTOTYPE) {
        i2c_init(gpio_i2c_proto, 1000 * 1000);
        for (uint i = 0; i < sizeof(gpio_num_proto) / sizeof(gpio_num_proto[0]); i++) {
            gpio_set_function(gpio_num_proto[i], GPIO_FUNC_I2C);
            gpio_pull_up(gpio_num_proto[i]);
        }

        disp.external_vcc = false;
        ssd1306_init(&disp, LCD_WIDTH, LCD_HEIGHT, LCD_ADDR, gpio_i2c_proto);
    } else if (hw_type == HW_V2) {
        i2c_init(gpio_i2c_v2, 1000 * 1000);
        for (uint i = 0; i < sizeof(gpio_num_v2) / sizeof(gpio_num_v2[0]); i++) {
            gpio_set_function(gpio_num_v2[i], GPIO_FUNC_I2C);
            gpio_pull_up(gpio_num_v2[i]);
        }

        disp.external_vcc = false;
        ssd1306_init(&disp, LCD_WIDTH, LCD_HEIGHT, LCD_ADDR, gpio_i2c_v2);
    }

    ssd1306_clear(&disp);
    for (uint y = 0; y < LOGO_HEIGHT; y++) {
        for (uint x = 0; x < LOGO_WIDTH; x++) {
            const uint pos = y * LOGO_WIDTH + x;
            const uint bit = 7 - (pos % 8);
            if (logo_data[pos / 8] & (1 << bit)) {
                ssd1306_draw_pixel(&disp, x, y);
            }
        }
    }
    ssd1306_show(&disp);
}

void lcd_draw(const char *mode, const char *val, const char *bat) {
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 0, 0, 2, mode);
    ssd1306_draw_string(&disp, 0, 20, 4, val);
    ssd1306_draw_string(&disp, 0, LCD_HEIGHT - 1 - 10, 1, bat);

    ssd1306_show(&disp);
}
