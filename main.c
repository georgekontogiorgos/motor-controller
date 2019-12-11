#include "config.h"

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "usart.h"
#include "shreg.h"


//H bridge shift register positions
#define M1_IN1 7
#define M1_IN2 6
#define M1_IN3 5
#define M1_IN4 4

#define M2_IN1 3
#define M2_IN2 2
#define M2_IN3 1
#define M2_IN4 0

int pwm = 0;
int pwm_counter = 0;

int motor_1_ia;

void configure_tmr2();

void configure_pwm();

interrupt void IRS()
{
    uart_rx_isr();
    uart_tx_isr();
    
    if(PIR1bits.ADIF)
    {
        motor_1_ia = ADRES;
        ADCON0bits.GO = 1;
        PIR1bits.ADIF = 0;
    }
}

void main(void)
{
    TRISB = 0;
    TRISD = 0;
    TRISC = 0;
    
    uart_set_tx();
    uart_set_rx();
    uart_set_baud_rate();
    
    //***************************
    // Interrupt configuration
    
    // Enable interrupts register
    PIE1bits.RCIE = 1;
    PIE1bits.ADIE = 1;
    
    // Clear interrupt flags at startup
    PIR1bits.RCIF     = 0;
    PIR1bits.TXIF     = 0;
    PIR1bits.ADIF     = 0;
    INTCONbits.TMR0IF = 0;
    
    INTCONbits.GIE  = 1;
    INTCONbits.PEIE = 1;
    
    INTCONbits.TMR0IE = 0;
    
    INTCON2bits.TMR0IP = 0;

    //**************************
    // ADC configuration
    
    //Setting ADCON0        
    ADCON0bits.CHS=0;
    
    ADCON0bits.ADON = 1;
            
    //Setting ADCON1
    ADCON1bits.VCFG = 3;
    ADCON1bits.PCFG = 11;

    
    //Setting ADCON2
    ADCON2bits.ADFM = 1;
    ADCON2bits.ACQT = 7;
    ADCON2bits.ADCS = 6;
    
    uart_send_string("MOTOR CONTROLLER\n");
    uart_send_string("Starting system...\n");
    
    char raw_command[100];
    char * command;   
    char * arg1;
    char * arg2;
    
    short int L298_inputs[]={0, 0, 0, 0, 0, 0, 0, 0};
    
    // Clear all H bridge inputs
    send_sr(L298_inputs);
    NOT_OE = 0;

    configure_pwm();    
    configure_tmr2();
    
    CCPR1L = 0;
    
    ADCON0bits.GO = 1;
    
    while(1)
    {
        memset(raw_command, 0, 100);
        
        uart_read_buffer(raw_command);
        
        command = strtok(raw_command, " \n");
        arg1 = strtok(NULL, " \n");
        arg2 = strtok(NULL, " \n");
        
        if(strcmp(command,"idn") == 0)
        {
            uart_send_string("Controller - V0.0.0\n");
        }
        
        if(strcmp(command,"en") == 0)
        {
            if(strcmp(arg2,"1") == 0)
            {
                L298_inputs[M1_IN1]=1;
                send_sr(L298_inputs);
                uart_send_string("M1 IN1 enable\n");                
            }
            if(strcmp(arg2,"2") == 0)
            {
                L298_inputs[M1_IN2]=1;
                send_sr(L298_inputs);
                uart_send_string("M1 IN2 enable\n");                     
            }
        }

        if(strcmp(command,"dis") == 0)
        {
            if(strcmp(arg2,"1") == 0)
            {
                L298_inputs[M1_IN1]=0;
                send_sr(L298_inputs);
                uart_send_string("M1 IN1 disable\n");                
            }
            if(strcmp(arg2,"2") == 0)
            {
                L298_inputs[M1_IN2]=0;
                send_sr(L298_inputs);
                uart_send_string("M1 IN2 disable\n");                   
            }
        }
   
        if(strcmp(command,"pwm") == 0)
        {
            uart_send_string("pwm enabled\n");
            CCPR1L=atoi(arg2);
        }

        if(strcmp(command,"imeas") == 0)
        {
            if(strcmp(arg2,"1") == 0)
            {
                char send_current[6];
                sprintf(send_current, "%0.3f A\n", (float)motor_1_ia*0.00207779255);
                uart_send_string(send_current);
            }
        }
    }
    return;
}

void configure_tmr2()
{
    PR2 = 1023;
    T2CONbits.TOUTPS = 0;
    T2CONbits.TMR2ON = 1;
    T2CONbits.T2CKPS = 0;
}

void configure_pwm()
{
    CCP1CONbits.CCP1M = 12;
}