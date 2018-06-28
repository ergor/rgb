/*
 * rx.c
 *
 *  Created on: Jun 21, 2018
 *      Author: Erol
 */

#include <string.h>
#include <stdlib.h>
#include <avr/io.h>

#include "main.h"
#include "rx.h"

int idx = 0;
char rx_done = 0;
char data_ready = 0;
char rx_char;
char rx_buf[RX_BUF_SZ] = {0};
char rx_data[RX_BUF_SZ];

void recv()
{    
    if (rx_char == '\r') {
        memcpy(rx_data, rx_buf, RX_BUF_SZ);
        memset(rx_buf, 0, RX_BUF_SZ);
        idx = 0;
        data_ready = 1;
    }
    else if (idx < RX_BUF_SZ - 1) { // -1 for 0-character
        rx_buf[idx++] = rx_char;
    }
    else return;
    
    //loop_until_bit_is_set(UCSRA, UDRE);
    //UDR = rx; // echo
}
