/*
 * rx.h
 *
 *  Created on: Jun 21, 2018
 *      Author: zzz
 */

#ifndef RX_H_
#define RX_H_

#define RX_BUF_SZ   16

extern char rx_done;
extern char rx_data[];

void recv(void);

#endif /* RX_H_ */
