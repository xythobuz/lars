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
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "adc.h"

#define LIPO_USE_PERCENTAGE_CURVE

#define ADC_NUM 2
#define ADC_PIN (26 + ADC_NUM)

#define ADC_VREF 3.3
#define ADC_RANGE (1 << 12)
#define ADC_CONVERT (ADC_VREF / (ADC_RANGE - 1))

#define BAT_R1 10000.0f
#define BAT_R2 18000.0f

#define FILTER_OLD 0.999f
#define FILTER_NEW (1.0f - FILTER_OLD)

#ifndef LIPO_USE_PERCENTAGE_CURVE
static const float full_battery = 4.1f;
static const float empty_battery = 3.2f;
#endif // ! LIPO_USE_PERCENTAGE_CURVE

static float filtered = 0.0f;

static float bat_read(void) {
    float v_adc = adc_read() * ADC_CONVERT;

    // Vadc = Vbat * R2 / (R1 + R2)
    float v_bat = v_adc / (BAT_R2 / (BAT_R1 + BAT_R2));
    return v_bat;
}

void bat_init(void) {
    adc_init();
    adc_gpio_init( ADC_PIN);
    adc_select_input( ADC_NUM);
    filtered = bat_read();
}

float bat_get(void) {
    filtered = (filtered * FILTER_OLD) + (bat_read() * FILTER_NEW);
    return filtered;
}

float bat_to_percent(float voltage) {
    float percentage = 0.0f;

#ifdef LIPO_USE_PERCENTAGE_CURVE
    /*
     * Try to linearize the LiPo discharge curve.
     * https://electronics.stackexchange.com/a/551667
     *
     * Seems to work relatively well, although
     * "stopping" at 3.5V feels a bit high to me.
     */
    percentage = 123.0f - (123.0f / powf(1.0f + powf(voltage / 3.7f, 80.0f), 0.165f));
#else // LIPO_USE_PERCENTAGE_CURVE
    percentage = 100.0f * ((voltage - empty_battery) / (full_battery - empty_battery));
#endif // LIPO_USE_PERCENTAGE_CURVE

    return MIN(MAX(percentage, 0.0f), 100.0f);
}
