/*
 * adc.c
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
#include "hardware/adc.h"

#include "adc.h"

#define ADC_NUM 2
#define ADC_PIN (26 + ADC_NUM)

#define ADC_VREF 3.3
#define ADC_RANGE (1 << 12)
#define ADC_CONVERT (ADC_VREF / (ADC_RANGE - 1))

#define BAT_R1 10000.0f
#define BAT_R2 18000.0f

void bat_init(void) {
    adc_init();
    adc_gpio_init( ADC_PIN);
    adc_select_input( ADC_NUM);
}

float bat_get(void) {
    float v_adc = adc_read() * ADC_CONVERT;

    // Vadc = Vbat * R2 / (R1 + R2)
    float v_bat = v_adc / (BAT_R2 / (BAT_R1 + BAT_R2));
    return v_bat;
}
