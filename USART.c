/*
 * USART.c
 *
 * Created: 03.05.2015 22:40:17
 *  Author: Erol
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "rx.h"

#define USART_BAUDRATE	9600
//#define BAUD_PRESCALE	((((F_CPU / 16) + (USART_BAUDRATE / 2)) / (USART_BAUDRATE)) - 1)
#define BAUD_PRESCALE   ((F_CPU / (16UL*USART_BAUDRATE)) - 1UL) 

void (* rcv_handler)();

void USART_init(void (* callback)())
{
    rcv_handler = callback;
#if defined(__AVR_ATmega16__)
//#warning "ATMega16 needs refactoring to 328p standard"
	UCSRB = (1 << RXEN) | (1 << TXEN); // Turn on the transmission and reception circuitry 
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // Use 8-bit character sizes
	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR registe
	UCSRB |= (1 << RXCIE); // RX Complete Interrupt Enable
#elif defined(__AVR_ATmega328p__)
#define USART_RXC_vect  USART_RX_vect
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Turn on the transmission and reception circuitry 
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); // Use 8-bit character sizes
    UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
    UBRR0L = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR registe
    UCSR0B |= (1 << RXCIE0); // RX Complete Interrupt Enable
#else
#error "Target implementation needed"
#endif
	sei();
	//printf("UBRR: %lu", BAUD_PRESCALE);
}


ISR(USART_RXC_vect)
{
    rx_char = UDR;
    rx_done = 1;
    //rcv_handler();
}
