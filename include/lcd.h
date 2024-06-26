/*
 * lcd.h
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

#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>

#define LCD_WIDTH 128
#define LCD_HEIGHT 64

#define FONT_HEIGHT 8
#define FONT_WIDTH 5

void lcd_init(void);
void lcd_draw(const char *mode, const char *val, const char *bat);
void lcd_draw_bye(void);
void lcd_draw_bitmap(uint8_t *data, int width, int height, int x_off, int y_off);
void lcd_draw_version(void);

void lcd_debug_buttons(void);

#endif // __LCD_H__
