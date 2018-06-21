/*
 * USART.c
 *
 * Created: 03.05.2015 22:40:17
 *  Author: Erol
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define USART_BAUDRATE	9600
#define BAUD_PRESCALE	((((F_CPU / 16) + (USART_BAUDRATE / 2)) / (USART_BAUDRATE)) - 1)

char rx_byte;
void (* rcv_handler)();

void USART_init(void (* callback)())
{
	rcv_handler = callback;
	UCSRB = (1 << RXEN) | (1 << TXEN); // Turn on the transmission and reception circuitry 
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // Use 8-bit character sizes
	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR registe
	UCSRB |= (1 << RXCIE); // RX Complete Interrupt Enable
	sei();
}

ISR(USART_RXC_vect)
{ 	
	rcv_handler();
}
