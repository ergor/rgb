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
#include <avr/interrupt.h>

#include "USART.h"
#include "LCD.h"
#include "main.h"
#include "rx.h"
#include "timer.h"

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
//uint32_t k = 10; // magic number. adjust as needed
uint32_t thr_r = 50, thr_g = 0, thr_b = 0;

static int uart_putchar(char c, FILE *stream);
static FILE uart_fd = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_RW);

uint8_t cycle = 255;

uint8_t mask = PIN_R | PIN_G | PIN_B;
uint8_t inv_mask; 

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
int main()
{
    LCD_init(); // specific to my board - feel free to comment out
    LCD_put_string("   [www.erol.no]   github.com/ergor/rgb");
    LCD_send_cmd(LCD_CMD_SEL_LINE_2);
    LCD_put_string(" clone the project github.com/ergor/rgb");
    LCD_send_cmd(LCD_CMD_CURSOR_OFF);
    
    USART_init(recv);
    stdout = &uart_fd;
    stdin = &uart_fd;
    
    DIR_REG |= mask;
    inv_mask = ~mask;
    
    //uint32_t top = k * (RESL-1);
    timer0_ov_isr_init(pwm);
    
    uint32_t text_cycle = 0;
	while (1) {
	    // poll the receive done flag in rx.c 
	    if (rx_done) {
	        //printf("\n%s OK\n", rx_data);
	        parse_val();
	        rx_done = 0;
	    }
	    
        if (text_cycle == 150000) {
            LCD_send_cmd(LCD_CMD_SHIFTLEFT);
            text_cycle = 0;
        }
        text_cycle++;
	    //PORT &= inv_mask; // turn off all lights by default
	    
	    // and when the threshold is reached, turn on again:
		/*for (uint32_t i = top; i > 0; i--) {
		    if (i == thr_r)
		        PORT |= PIN_R;
		    if (i == thr_g)
                PORT |= PIN_G;
		    if (i == thr_b)
                PORT |= PIN_B;
		}*/
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
    thr_r = /*k * */ strtol(data, &data, 16);
    thr_g = /*k * */ strtol(data, &data, 16);
    thr_b = /*k * */ strtol(data, &data, 16);
    //printf("r: %lu, g: %lu, b: %lu\n", thr_r, thr_g, thr_b);
}

void pwm()
{
    if (cycle == 255)
        PORT &= inv_mask; // turn off all lights by default
    
    if (thr_r == cycle)
        PORT |= PIN_R;
    if (thr_g == cycle)
        PORT |= PIN_G;
    if (thr_b == cycle)
        PORT |= PIN_B;
        
    cycle--;
}
