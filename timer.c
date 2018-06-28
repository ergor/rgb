/*
 * timer.c
 *
 *  Created on: Jun 22, 2018
 *      Author: zzz
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "project.h"

uint8_t tick = 0;

/** <extern vars> **/
uint8_t t_ov = 0;

uint8_t pwm_lut[RESL*2];
uint8_t* lut_p = pwm_lut;
uint8_t lut_bank = 0;
/** </extern vars> **/


void timer0_isr_init(uint8_t ocr_val,  uint8_t cs_mask)
{
#if defined(__AVR_ATmega16__)
//#warning "ATMega16 needs refactoring to 328p standard"
    TCCR0 = cs_mask | _BV(WGM01); // CTC mode
    OCR0 = ocr_val;
    TIMSK = _BV(OCIE0); // OC match interrupt enable
#elif defined(__AVR_ATmega328p__)
#define TIMER0_COMP_vect    TIMER0_COMPA_vect
    TCCR0A |= _BV(WGM01); // CTC mode
    OCR0A = ocr_val;
    TCCR0B |= cs_mask;
    TIMSK0 = _BV(OCIE0A); // OC match interrupt enable
#else
#error "Target implementation needed"
#endif
    sei();
}

/* MOVED TO: xxx-pwm.S (xxx = atmega model)*/
ISR(TIMER0_COMP_vect)
{
    PORT = lut_p[tick++];
    
    // swap bank on overflow and signal main loop
    if (tick == 0){
        lut_bank ^= 1;
        lut_p = pwm_lut + (lut_bank * RESL);
        t_ov = 1;                            
    }
}
