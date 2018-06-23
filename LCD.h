/*
 * LCD.h
 *
 * Created: 03.05.2015 22:39:01
 *  Author: Erol
 */ 


#ifndef LCD_H_
#define LCD_H_

/************************************************************************/
/* COMMANDS                                                             */
/************************************************************************/
#define LCD_CMD_CLEAR		0x1
#define LCD_CMD_2LINE		0b00101000
#define LCD_CMD_RETURN		0x2
#define LCD_CMD_SHIFTLEFT	0b11000
#define LCD_CMD_SHIFTRIGHT	0b11100
#define LCD_CMD_SEL_LINE_1	0x80
#define LCD_CMD_SEL_LINE_2	(0x80 | 0x40)
#define LCD_CMD_CURSOR_OFF  0b1100



/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void LCD_init();
void LCD_put_char(char ascii_char);
void LCD_put_string(char * ascii_string);
void LCD_send_cmd(char cmd);


#endif /* LCD_H_ */
