/*
 * encoder.h
 *
 * Based on https://github.com/mathertel/RotaryEncoder/blob/master/src/RotaryEncoder.cpp
 */

#ifndef __ENCODER_H__
#define __ENCODER_H__

void encoder_init(void);
int32_t encoder_pos(void);
void encoder_run(void);

#endif // __ENCODER_H__

