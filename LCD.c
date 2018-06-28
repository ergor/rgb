/*
 * LCD.c
 *
 * Created: 03.05.2015 22:32:39
 *  Author: Erol
 */ 

#ifdef PROJECT_BOARD_LCD

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"


/************************************************************************/
/* THE PHYSICAL PIN CONNECTIONS TO THE LCD                              */
/************************************************************************/
#define LCD_E		_BV(PA2)
#define LCD_RS		_BV(PA3)
#define LCD_DATA	0xF0	// pin 4 through 7
#define LCD_NC		_BV(PA0) | _BV(PA1)
#define LCD_OFFSET		4	//amount to bitwise shift data to compensate for 
                            // data not corresponding to pin numbers

void clock_and_reset()
{
	PORTA |= LCD_E;
	_delay_ms(1);
	
	PORTA &= ~(LCD_E);
	_delay_ms(1);
	
	PORTA &= 3;
	_delay_ms(1);
}

void LCD_send_cmd(char cmd)
{
	// NOTE: ONLY WORKS WHEN LCD DATA 4 through 7 is connected to PORTA 4 
    // through 7, in correct order (4 on 4, 5 on 5, etc)
	
	PORTA |= cmd & 0xF0; // high nibble first
	clock_and_reset();
	PORTA |= cmd << LCD_OFFSET; // then lower
	clock_and_reset();
}

void LCD_put_char(char ascii_char)
{
	if (ascii_char != '\r')
	{
		PORTA |= (ascii_char & 0xF0) | LCD_RS; // high nibble first
		clock_and_reset();
		PORTA |= (ascii_char << LCD_OFFSET | LCD_RS); // then lower
		clock_and_reset();
	}
	else
	{
		LCD_send_cmd(LCD_CMD_RETURN);
	}
}

void LCD_put_string(char * ascii_string)
{
	while(*ascii_string)
		LCD_put_char(*ascii_string++);
}

void LCD_init()
{
	DDRA = LCD_E | LCD_RS | LCD_DATA;
	PORTA = 0;
	
	// activate 4 bit mode
	PORTA |= 1 << 5;
	clock_and_reset();
	
	// activate display
	PORTA = 0;
	clock_and_reset();
	PORTA = 0xF << 4;
	clock_and_reset();
	
	// clear display
	LCD_send_cmd(LCD_CMD_CLEAR);
	
	// set for two row operation
	LCD_send_cmd(LCD_CMD_2LINE);
}

#endif
