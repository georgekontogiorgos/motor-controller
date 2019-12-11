/*
 * File:   controller_main.c
 * Author: Andreia Kontogiorgos
 *
 * Created on 7 de Dezembro de 2019, 06:26
 */


#include <xc.h>
#include "config.h"

void configure_timer0();

void interrupt irs()
{
    
}

void main(void) {
    configure_timer0();
    return;
}

void configure_timer0()
{
    T0CONbits.TMR0ON = 
    T0CONbits.T08BIT = 1;
    T0CONbits.T0CS   = 0;
    T0CONbits.T0SE   = 0;
    T0CONbits.PSA    = 
    T0CONbits.T0PS   =
}