#include <xc.h>
#include <string.h>
#include "usart.h"

//***************************
// UART definitions
#define TX_BUFFER_SIZE 100
#define RX_BUFFER_SIZE 100

//***************************
// UART global variables

char tx_buffer[TX_BUFFER_SIZE];
char rx_buffer[RX_BUFFER_SIZE];

unsigned int tx_buffer_pointer = 0;
unsigned int rx_buffer_pointer = 0;

int tx_transmission_end = 1;
int rx_buffer_full = 0;

void uart_set_tx(void)
{
    //Setting TXSTA
    TXSTAbits.TX9   = 0;
    TXSTAbits.TXEN  = 1;       
    TXSTAbits.SYNC  = 0;
    TXSTAbits.SENDB = 0;
    TXSTAbits.BRGH  = 1;      
}
    
void uart_set_rx(void)
{
    //Setting RCSTA
    RCSTAbits.SPEN  = 1;
    RCSTAbits.RX9   = 0;
    RCSTAbits.SREN  = 0;
    RCSTAbits.CREN  = 1;
    RCSTAbits.ADDEN = 0;    
}

void uart_set_baud_rate(void)
{
    SPBRG=8;
    SPBRGH=2;  
    
    //Setting BAUDCON
    BAUDCONbits.RXDTP = 0;
    BAUDCONbits.TXCKP = 0;    
    BAUDCONbits.BRG16 = 1;
}

void uart_send_string(char * message)
{
    strcpy(tx_buffer,message);
    tx_transmission_end = 0;
    PIE1bits.TXIE = 1;
    while(!tx_transmission_end)
    {
        
    }
    memset(tx_buffer, 0, TX_BUFFER_SIZE);
}

void uart_read_buffer(char * message)
{
    if(rx_buffer_full)
    {
        //Copy buffer to local memory
        strcpy(message,rx_buffer);
        
        //Erase reception buffer
        memset(rx_buffer, 0, RX_BUFFER_SIZE);
        
        //Down reciever full flag
        rx_buffer_full = 0;
    }
}

void uart_rx_isr(void)
{
    if(PIR1bits.RCIF)
    {
        if(!rx_buffer_full)
        {
            //Copy char on receiver register to reception buffer array
            rx_buffer[rx_buffer_pointer] = RCREG;
        
            if(rx_buffer[rx_buffer_pointer] != '\n')
            {
                rx_buffer_pointer++;
            }
            else
            {
                rx_buffer_pointer = 0;
                rx_buffer_full = 1;
            }   
        }
        PIR1bits.RCIF = 0;
    }
}

void uart_tx_isr(void)
{
    if(PIR1bits.TXIF)
    {
        if(tx_buffer[tx_buffer_pointer] == '\0')
        {
            tx_transmission_end = 1;
            tx_buffer_pointer = 0;
            PIE1bits.TXIE = 0;
            PIR1bits.TXIF = 0;
        }
        else
        {
            TXREG = tx_buffer[tx_buffer_pointer];
            tx_buffer_pointer++;
        }
    }
}