/*
 * encoder.c
 *
 * Based on https://github.com/mathertel/RotaryEncoder/blob/master/src/RotaryEncoder.cpp
 */

#include <stdio.h>
#include "pico/stdlib.h"

#include "encoder.h"

#define LATCH0 0
#define LATCH3 3

#define FOUR3 1 // 4 steps, Latch at position 3 only (compatible to older versions)
#define FOUR0 2 // 4 steps, Latch at position 0 (reverse wirings)
#define TWO03 3 // 2 steps, Latch at position 0 and 3

#define ENCODER_MODE TWO03

static const uint gpio_num[2] = {
    9, 10
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

void encoder_init(void) {
    for (uint i = 0; i < 2; i++) {
        gpio_init(gpio_num[i]);
        gpio_set_dir(gpio_num[i], GPIO_IN);
        gpio_pull_up(gpio_num[i]);
    }

    oldState = gpio_get(gpio_num[0]) | (gpio_get(gpio_num[1]) << 1);
    position = 0;
    positionExt = 0;
    positionExtPrev = 0;
}

int32_t encoder_pos(void)
{
    return positionExt;
}

void encoder_run(void) {
    int8_t thisState = gpio_get(gpio_num[0]) | (gpio_get(gpio_num[1]) << 1);

    if (oldState != thisState) {
        position += KNOBDIR[thisState | (oldState << 2)];
        oldState = thisState;

        switch (ENCODER_MODE) {
        case FOUR3:
            if (thisState == LATCH3) {
                // The hardware has 4 steps with a latch on the input state 3
                positionExt = position >> 2;
            }
            break;

        case FOUR0:
            if (thisState == LATCH0) {
                // The hardware has 4 steps with a latch on the input state 0
                positionExt = position >> 2;
            }
            break;

        case TWO03:
            if ((thisState == LATCH0) || (thisState == LATCH3)) {
                // The hardware has 2 steps with a latch on the input state 0 and 3
                positionExt = position >> 1;
            }
            break;
        }
    }
}
