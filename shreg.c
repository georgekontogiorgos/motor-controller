#include <xc.h>
#include "shreg.h"

void send_sr(short int * buffer)
{
    // SRCLR' - B0
    // SRCLK  - B1
    // RCLK   - B2
    // OE'    - B3
    // SER    - B4
    
    int i;

    NOT_SRCLR = 1;
        
    for(i=0; i<8; i++)
    {
        SER = buffer[i];
        SRCLK = 1;
        SRCLK = 0;       
    }
    
    RCLK = 0;
    RCLK = 1;    
    NOT_SRCLR = 0;
}
