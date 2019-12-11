/* 
 * File:   usart.h
 * Author: George Nicolas Kontogiorgos
 *
 * Created on 23 de Novembro de 2019, 00:58
 */

#ifndef USART_H
#define	USART_H

#ifdef	__cplusplus
extern "C" {
#endif

void uart_set_tx(void);
    
void uart_set_rx(void);

void uart_set_baud_rate(void);

void uart_send_string(char * message);

void uart_read_buffer(char * message);

void uart_rx_isr(void);

void uart_tx_isr(void);

#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */

