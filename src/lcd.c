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
#include "lcd.h"

#define LCD_I2C i2c0
static const uint gpio_num[2] = { 0, 1 };
#define LCD_ADDR 0x3C

static ssd1306_t disp;

void lcd_init(void) {
    i2c_init(LCD_I2C, 1000 * 1000);
    for (uint i = 0; i < sizeof(gpio_num) / sizeof(gpio_num[0]); i++) {
        gpio_set_function(gpio_num[i], GPIO_FUNC_I2C);
        gpio_pull_up(gpio_num[i]);
    }

    disp.external_vcc = false;
    ssd1306_init(&disp, LCD_WIDTH, LCD_HEIGHT, LCD_ADDR, LCD_I2C);
}
