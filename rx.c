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
volatile char rx_done = 0;
char rx_buf[RX_BUF_SZ] = {0};
char rx_data[RX_BUF_SZ];

void recv()
{
    char rx = UDR;
    if (rx == '\r') {
        memcpy(rx_data, rx_buf, RX_BUF_SZ);
        memset(rx_buf, 0, RX_BUF_SZ);
        idx = 0;
        rx_done = 1;
    }
    else if (idx < RX_BUF_SZ - 1) { // -1 for 0-character
        rx_buf[idx++] = rx;
        loop_until_bit_is_set(UCSRA, UDRE);
        UDR = rx; // echo
    }
    //printf("\nidx=%d, rx_done=%d\nrx_buf: %s\n", idx, rx_done, rx_buf);
}
