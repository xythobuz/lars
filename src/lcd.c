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

#include "hardware/i2c.h"

#include "ssd1306.h"

#include "buttons.h"
#include "main.h"
#include "lcd.h"

static i2c_inst_t *gpio_i2c_proto = i2c0;
static const uint gpio_num_proto[2] = { 0, 1 };

static i2c_inst_t *gpio_i2c_v2 = i2c0;
static const uint gpio_num_v2[2] = { 16, 17 };

#define LCD_ADDR 0x3C

static ssd1306_t disp = {0};
static bool buttons[NUM_BTNS] = {0};
static bool changed = true;

static void lcd_debug_buttons_callback(enum buttons btn, bool v) {
    buttons[btn] = v;
    changed = true;
}

void lcd_debug_buttons(void) {
    buttons_callback(lcd_debug_buttons_callback);

    while (1) {
        buttons_run();
        handle_serial_input();

        if (changed) {
            changed = false;

            ssd1306_clear(&disp);
            ssd1306_draw_string(&disp, 0, 0, 3, "Buttons");

            for (uint i = 0; i < NUM_BTNS; i++) {
                if ((hw_type == HW_PROTOTYPE) && (i >= BTN_D) && (i <= BTN_H)) {
                    continue;
                }

                ssd1306_draw_char(&disp,
                                  i * 12, LCD_HEIGHT - 20 - 16 - 1,
                                  2, '0' + i);

                if (buttons[i]) {
                    ssd1306_draw_square(&disp,
                                        i * 12, LCD_HEIGHT - 20 - 1,
                                        10, 20);
                } else {
                    ssd1306_draw_empty_square(&disp,
                                              i * 12, LCD_HEIGHT - 20 - 1,
                                              10, 20);
                }
            }

            ssd1306_show(&disp);
        }
    }
}

void lcd_draw_bitmap(uint8_t *data, int width, int height, int x_off, int y_off) {
    ssd1306_clear(&disp);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const uint pos = y * width + x;
            const uint bit = 7 - (pos % 8);
            if (data[pos / 8] & (1 << bit)) {
                ssd1306_draw_pixel(&disp, x + x_off, y + y_off);
            }
        }
    }

    ssd1306_show(&disp);
}

void lcd_init(void) {
    if (hw_type == HW_PROTOTYPE) {
        i2c_init(gpio_i2c_proto, 2UL * 1000UL * 1000UL);

        for (uint i = 0; i < sizeof(gpio_num_proto) / sizeof(gpio_num_proto[0]); i++) {
            gpio_set_function(gpio_num_proto[i], GPIO_FUNC_I2C);
            gpio_pull_up(gpio_num_proto[i]);
        }

        ssd1306_init(&disp,
                     LCD_WIDTH, LCD_HEIGHT,
                     LCD_ADDR, gpio_i2c_proto);
    } else if (hw_type == HW_V2) {
        i2c_init(gpio_i2c_v2, 2UL * 1000UL * 1000UL);

        for (uint i = 0; i < sizeof(gpio_num_v2) / sizeof(gpio_num_v2[0]); i++) {
            gpio_set_function(gpio_num_v2[i], GPIO_FUNC_I2C);
            gpio_pull_up(gpio_num_v2[i]);
        }

        ssd1306_init(&disp,
                     LCD_WIDTH, LCD_HEIGHT,
                     LCD_ADDR, gpio_i2c_v2);
    }
}

void lcd_draw(const char *mode, const char *val, const char *bat) {
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 0, 0, 2, mode);
    ssd1306_draw_string(&disp, 0, 20, 4, val);
    ssd1306_draw_string(&disp, 0, LCD_HEIGHT - 8, 1, bat);
    ssd1306_show(&disp);
}

void lcd_draw_bye(void) {
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 6, 5, 3, " Boot-");
    ssd1306_draw_string(&disp, 8, LCD_HEIGHT / 2 + 5, 3, "loader");
    ssd1306_show(&disp);
}
