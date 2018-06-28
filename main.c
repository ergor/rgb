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

#include "project.h"
#include "USART.h"
#include "main.h"
#include "rx.h"
#include "timer.h"

/******************************************************************************/
/* GLOBAL VARIABLES & DECLARATIONS                                            */
/******************************************************************************/
uint32_t thr_r = 50, thr_g = 0, thr_b = 0; // pwm levels

static int uart_putchar(char c, FILE *stream);
static FILE uart_fd = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_RW);


/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/

int main()
{
#ifdef PROJECT_BOARD_LCD // specific to my board
#include "LCD.h"
    LCD_init(); 
    LCD_put_string("   [www.netb.st]   github.com/ergor/rgb");
    LCD_send_cmd(LCD_CMD_SEL_LINE_2);
    LCD_put_string(" clone the project github.com/ergor/rgb");
    LCD_send_cmd(LCD_CMD_CURSOR_OFF);
#endif
    
    stdout = &uart_fd;
    stdin = &uart_fd;
    USART_init(recv);
    
    DIR_REG |= PIN_R | PIN_G | PIN_B;
    
    // oc_val=100: refresh rate > 50 HZ (156 for ~50Hz)
    timer0_isr_init(100, CS_8x_PRESCALE); 
    
    uint32_t text_cycle = 0;
	while (1) {

	    // case: timer has overflowed and swapped bank,
	    // the old bank is now available for generating new pwm sequence:
	    if (t_ov) {
	        gen_pwm();
	        t_ov = 0;
	    }
	        
	    // a complete string is ready for reading
	    if (data_ready) {
	        parse_val();
	        data_ready = 0;
	    }
	    // else poll the receive done flag in rx.c 
	    if (rx_done) {
	        //printf("\n%s OK\n", rx_data);
	        //UDR0 = rx_char;
	        uart_putchar(rx_char, NULL);
	        recv();
	        rx_done = 0;
	    }
#ifdef PROJECT_BOARD_LCD	    
        if (text_cycle == 150000) {
            LCD_send_cmd(LCD_CMD_SHIFTLEFT);
            text_cycle = 0;
        }
        text_cycle++;
#endif
	}
}

static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n')
        uart_putchar('\r', stream);
        //c = '\r';
#ifdef __AVR_ATmega16__
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
#elif __AVR_ATmega328p__
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
#else
#error "Target implementation needed"
#endif
    return 0;
}

void parse_val()
{
    char* data = rx_data;
    thr_r = /*k * */ strtol(data, &data, 16);
    thr_g = /*k * */ strtol(data, &data, 16);
    thr_b = /*k * */ strtol(data, &data, 16);
    //thr_r = thr_r == 0 ? 255 : 0;
    printf("r: %03lu, g: %03lu, b: %03lu\n", thr_r, thr_g, thr_b);
}

void gen_pwm()
{
    // lut_bank is the current bank in use; we must use the opposite bank
    uint8_t bank = lut_bank ^ 1;
    uint8_t* ptr = pwm_lut + (bank * RESL);
    uint8_t mask;
    
    for (uint16_t i = 0; i < RESL; i++) {
        mask = 0;
        
        if (i < thr_r)
            mask |= PIN_R;
        if (i < thr_g)
            mask |= PIN_G;
        if (i < thr_b)
            mask |= PIN_B;
        
        ptr[i] = mask;
    }
}
