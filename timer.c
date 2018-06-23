/*
 * timer.c
 *
 *  Created on: Jun 22, 2018
 *      Author: zzz
 */

#include <avr/io.h>
#include <avr/interrupt.h>

void (* t0_ov_callback)();

void timer0_ov_isr_init(void (* callback)())
{
    t0_ov_callback = callback;
    // start timer0 in normal mode with no prescaling
    TCCR0 |= _BV(CS00);
    // enable overflow interrupt
    TIMSK = _BV(TOIE0);
    sei();
}

ISR(TIMER0_OVF_vect)
{
    t0_ov_callback();
}
