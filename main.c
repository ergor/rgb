/*
 * main.c
 *
 *  Created on: Jun 19, 2018
 *      Author: Erol
 */

/******************************************************************************/
/* INCLUDES                                                                   */
/******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#include "USART.h"
#include "LCD.h"
#include "main.h"
#include "rx.h"

/******************************************************************************/
/* DEFINES                                                                    */
/******************************************************************************/
#define DIR_REG		DDRC
#define PORT		PORTC
#define PIN_R		1   // pin 0
#define PIN_G       2   // pin 1
#define PIN_B       4   // pin 2

#define RESL        256

/******************************************************************************/
/* GLOBAL VARIABLES & DECLARATIONS                                            */
/******************************************************************************/
// k = amount to scale the resolution (higher CPU freq dictates higher k)
uint32_t k = 10; // magic number. adjust as needed
uint32_t thr_r, thr_g, thr_b;

static int uart_putchar(char c, FILE *stream);
static FILE uart_fd = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_RW);

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
int main()
{
    LCD_init(); // specific to my board - feel free to comment out
    
    USART_init(&recv);
    stdout = &uart_fd;
    stdin = &uart_fd;
    
    uint8_t mask = PIN_R | PIN_G | PIN_B;
    uint8_t inv_mask = ~mask; 
    
    DIR_REG |= mask;
    

    uint32_t top = k * (RESL-1);
    
	while (1) {
	    // poll the receive done flag in rx.c 
	    if (rx_done) {
	        printf("\n%s OK\n", rx_data);
	        parse_val();
	        rx_done = 0;
	    }
	    
	    PORT &= inv_mask; // turn off all lights by default
	    
	    // and when the threshold is reached, turn on again:
		for (uint32_t i = top; i > 0; i--) {
		    if (i == thr_r)
		        PORT |= PIN_R;
		    if (i == thr_g)
                PORT |= PIN_G;
		    if (i == thr_b)
                PORT |= PIN_B;
		}
	}
}

static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n')
        uart_putchar('\r', stream);
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
    return 0;
}

void parse_val()
{
    char* data = rx_data;
    thr_r = k * strtol(data, &data, 16);
    thr_g = k * strtol(data, &data, 16);
    thr_b = k * strtol(data, &data, 16);
    printf("r: %lu, g: %lu, b: %lu\n", thr_r, thr_g, thr_b);
}
