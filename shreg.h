/* 
 * File:   74hc595.h
 * Author: George Nicolas Kontogiorgos
 *
 * Created on 3 de Dezembro de 2019, 22:25
 */

#ifndef SHREG_H
#define	SHREG_H

#ifdef	__cplusplus
extern "C" {
#endif

//***************************
// Shift register definitions

#define NOT_SRCLR LATBbits.LATB0
#define SRCLK     LATBbits.LATB1
#define RCLK      LATBbits.LATB2
#define NOT_OE    LATBbits.LATB3
#define SER       LATBbits.LATB4

void send_sr(short int * buffer);

#ifdef	__cplusplus
}
#endif

#endif	/* 74HC595_H */

