/*
 * timer.h
 *
 *  Created on: Jun 22, 2018
 *      Author: zzz
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#define CS_NO_PRESCALE      _BV(CS00)
#define CS_8x_PRESCALE      _BV(CS01)
#define CS_64x_PRESCALE     _BV(CS01) | _BV(CS00)

extern uint8_t t_ov;

extern uint8_t pwm_lut[RESL*2];
extern uint8_t* lut_p;
extern uint8_t lut_bank;

void timer0_isr_init(uint8_t ocr_val,  uint8_t cs_mask);

#endif /* TIMER_H_ */
