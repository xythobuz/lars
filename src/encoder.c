/*
 * encoder.c
 *
 * Based on https://github.com/mathertel/RotaryEncoder/blob/master/src/RotaryEncoder.cpp
 *
 * Copyright (c) 2024 Thomas Buck (thomas@xythobuz.de)
 *
 * Copyright (c) by Matthias Hertel, http://www.mathertel.de
 * This work is licensed under a BSD 3-Clause style license
 * https://www.mathertel.de/License.aspx.
 */

#include <stdio.h>
#include "pico/stdlib.h"

#include "main.h"
#include "log.h"
#include "encoder.h"

#define LATCH0 0
#define LATCH3 3

#define FOUR3 1 // 4 steps, Latch at position 3 only (compatible to older versions)
#define FOUR0 2 // 4 steps, Latch at position 0 (reverse wirings)
#define TWO03 3 // 2 steps, Latch at position 0 and 3

#define ENCODER_MODE FOUR3

static const uint gpio_num_proto[2] = {
    17, 18
};

static const uint gpio_num_v2[2] = {
    19, 18
};

static const int8_t KNOBDIR[] = {
    0, -1, 1, 0,
    1, 0, 0, -1,
    -1, 0, 0, 1,
    0, 1, -1, 0
};

static int8_t oldState;
static int32_t position;
static int32_t positionExt;
static int32_t positionExtPrev;
static uint32_t positionExtTime;
static uint32_t positionExtTimePrev;

void encoder_init(void) {
    for (uint i = 0; i < 2; i++) {
        if (hw_type == HW_PROTOTYPE) {
            gpio_init(gpio_num_proto[i]);
            gpio_set_dir(gpio_num_proto[i], GPIO_IN);
            gpio_pull_up(gpio_num_proto[i]);
        } else if (hw_type == HW_V2) {
            gpio_init(gpio_num_v2[i]);
            gpio_set_dir(gpio_num_v2[i], GPIO_IN);
            gpio_pull_up(gpio_num_v2[i]);
        }
    }

    if (hw_type == HW_PROTOTYPE) {
        oldState = gpio_get(gpio_num_proto[0]) | (gpio_get(gpio_num_proto[1]) << 1);
    } else if (hw_type == HW_V2) {
        oldState = gpio_get(gpio_num_v2[0]) | (gpio_get(gpio_num_v2[1]) << 1);
    }

    position = 0;
    positionExt = 0;
    positionExtPrev = 0;
    positionExtTime = 0;
    positionExtTimePrev = 0;
}

int32_t encoder_pos(void) {
    return positionExt;
}

// TODO should be adaptive depending on value range to be changed
#define ENCODER_RPM_VALUE_FACTOR 100.0f

int32_t encoder_get_diff(void) {
    int32_t diff = positionExt - positionExtPrev;
    positionExtPrev = positionExt;

#ifdef ENCODER_RPM_VALUE_FACTOR
    if (diff != 0) {
        uint32_t rpm = encoder_get_rpm();
        float f = 1.0f + ((float)rpm / ENCODER_RPM_VALUE_FACTOR);
        //debug("diff=%"PRIi32" rpm=%"PRIu32" result=%.1f", diff, rpm, diff * f);
        return diff * f;
    }
#endif

    return diff;
}

uint32_t encoder_get_rpm(void) {
    // calculate max of difference in time between last position changes or last change and now.
    uint32_t timeBetweenLastPositions = positionExtTime - positionExtTimePrev;
    uint32_t timeToLastPosition = to_ms_since_boot(get_absolute_time()) - positionExtTime;
    uint32_t t = MAX(timeBetweenLastPositions, timeToLastPosition);
    return (60.0f * 1000.0f) / ((float)(t * 20));
}

void encoder_run(void) {
    int8_t thisState = 0;
    if (hw_type == HW_PROTOTYPE) {
        thisState = gpio_get(gpio_num_proto[0]) | (gpio_get(gpio_num_proto[1]) << 1);
    } else if (hw_type == HW_V2) {
        thisState = gpio_get(gpio_num_v2[0]) | (gpio_get(gpio_num_v2[1]) << 1);
    }

    if (oldState != thisState) {
        position += KNOBDIR[thisState | (oldState << 2)];
        oldState = thisState;

        switch (ENCODER_MODE) {
        case FOUR3:
            if (thisState == LATCH3) {
                // The hardware has 4 steps with a latch on the input state 3
                positionExt = position >> 2;
                positionExt = -positionExt;

                positionExtTimePrev = positionExtTime;
                positionExtTime = to_ms_since_boot(get_absolute_time());
            }
            break;

        case FOUR0:
            if (thisState == LATCH0) {
                // The hardware has 4 steps with a latch on the input state 0
                positionExt = position >> 2;
                positionExt = -positionExt;

                positionExtTimePrev = positionExtTime;
                positionExtTime = to_ms_since_boot(get_absolute_time());
            }
            break;

        case TWO03:
            if ((thisState == LATCH0) || (thisState == LATCH3)) {
                // The hardware has 2 steps with a latch on the input state 0 and 3
                positionExt = position >> 1;
                positionExt = -positionExt;

                positionExtTimePrev = positionExtTime;
                positionExtTime = to_ms_since_boot(get_absolute_time());
            }
            break;
        }
    }
}
