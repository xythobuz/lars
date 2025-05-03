/*
 * led.c
 *
 * Copyright (c) 2024 - 2025 Thomas Buck (thomas@xythobuz.de)
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

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "log.h"
#include "main.h"
#include "sequence.h"
#include "led.h"

/*
 * Beware:
 * PWM driving the LED outputs assumes that the pins are not on
 * shared PWM slices / channels!
 * For RP2040: all GPIO numbers < 16 are safe
 *
 * Unfortunately the output channels on 26 and 27 are in conflict
 * with the LEDs on pins 10 and 11, in both hardware revisions.
 * So we only use PWM for the LEDs.
 */

static const uint led_gpio_num_proto[MAX_LED_COUNT] = {
    10, 11, 13, 15,
};

static const uint led_gpio_num_v2[MAX_LED_COUNT] = {
    6, 7, 8, 9, 10, 11, 12, 13,
};

static const uint ch_gpio_num[NUM_CHANNELS] = {
    22, 26, 27,
};

uint8_t led_count(void) {
    if (hw_type == HW_PROTOTYPE) {
        return 4;
    }
    return MAX_LED_COUNT;
}

void led_init(void) {
    for (uint i = 0; i < led_count(); i++) {
        uint p = 0xFF;
        if (hw_type == HW_PROTOTYPE) {
            p = led_gpio_num_proto[i];
        } else if (hw_type == HW_V2) {
            p = led_gpio_num_v2[i];
        }
        if (p >= 0xFF) {
            continue;
        }

        gpio_init(p);
        gpio_set_dir(p, GPIO_OUT);
        gpio_set_function(p, GPIO_FUNC_PWM);

        uint slice_num = pwm_gpio_to_slice_num(p);
        pwm_set_wrap(slice_num, UINT8_MAX);
        pwm_set_chan_level(slice_num, pwm_gpio_to_channel(p), 0);
        pwm_set_enabled(slice_num, true);
    }

    for (uint i = 0; i < NUM_CHANNELS; i++) {
        gpio_init(ch_gpio_num[i]);
        gpio_set_dir(ch_gpio_num[i], GPIO_OUT);
    }
}

void led_set(uint32_t i, bool v) {
    led_dim(i, v ? UINT8_MAX : 0);
}

void led_dim(uint32_t i, uint8_t v) {
    i %= led_count();

    uint p = 0xFF;
    if (hw_type == HW_PROTOTYPE) {
        p = led_gpio_num_proto[i];
    } else if (hw_type == HW_V2) {
        p = led_gpio_num_v2[i];
    }
    if (p >= 0xFF) {
        return;
    }

    pwm_set_chan_level(pwm_gpio_to_slice_num(p), pwm_gpio_to_channel(p), v);
    //debug("led %"PRIu32" now %d", i, v);
}

void ch_set(uint32_t i, bool v) {
    i %= NUM_CHANNELS;
    gpio_put(ch_gpio_num[i], v);
    //debug("ch %"PRIu32" now %d", i, v);
}
