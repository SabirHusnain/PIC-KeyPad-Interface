// PIC18F452 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = XT   // Oscillator Selection bits (XT oscillator)
#pragma config OSCS = OFF // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

// CONFIG2L
#pragma config PWRT = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON   // Brown-out Reset Enable bit (Brown-out Reset enabled)
#pragma config BORV = 20  // Brown-out Reset Voltage bits (VBOR set to 2.0V)

// CONFIG2H
#pragma config WDT = OFF   // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128 // Watchdog Timer Postscale Select bits (1:128)

// CONFIG3H
#pragma config CCP2MUX = ON // CCP2 Mux bit (CCP2 input/output is multiplexed with RC1)

// CONFIG4L
#pragma config STVR = OFF // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will not cause RESET)
#pragma config LVP = OFF  // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

// CONFIG5L
#pragma config CP0 = OFF // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
#pragma config CP1 = OFF // Code Protection bit (Block 1 (002000-003FFFh) not code protected)
#pragma config CP2 = OFF // Code Protection bit (Block 2 (004000-005FFFh) not code protected)
#pragma config CP3 = OFF // Code Protection bit (Block 3 (006000-007FFFh) not code protected)

// CONFIG5H
#pragma config CPB = OFF // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
#pragma config CPD = OFF // Data EEPROM Code Protection bit (Data EEPROM not code protected)

// CONFIG6L
#pragma config WRT0 = OFF // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
#pragma config WRT1 = OFF // Write Protection bit (Block 1 (002000-003FFFh) not write protected)
#pragma config WRT2 = OFF // Write Protection bit (Block 2 (004000-005FFFh) not write protected)
#pragma config WRT3 = OFF // Write Protection bit (Block 3 (006000-007FFFh) not write protected)

// CONFIG6H
#pragma config WRTC = OFF // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
#pragma config WRTB = OFF // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
#pragma config WRTD = OFF // Data EEPROM Write Protection bit (Data EEPROM not write protected)

// CONFIG7L
#pragma config EBTR0 = OFF // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR1 = OFF // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR2 = OFF // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR3 = OFF // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic18f452.h>

void SerTX(unsigned char x);
void RBIF_ISR(void);
void MSDelay(unsigned int millisecs);
unsigned char keypad[4][4] = {
    {'7', '8', '9', '/'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'C', '0', '=', '+'}};

void __interrupt(high_priority) chk_int()
{
    if (INTCONbits.RBIF == 1)
        RBIF_ISR();
}

void main()
{
    TRISD = 0;
    INTCON2bits.RBPU = 0;
    TRISB = 0xF0;
    PORTB = 0xF0;
    while (PORTB != 0xF0);
    TXSTA = 0x20;
    SPBRG = 15;
    TXSTAbits.TXEN = 1;
    RCSTAbits.SPEN = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.GIE = 1;
    while (1);
}

void RBIF_ISR(void)
{
    unsigned char temp, COL = 0, ROW = 4;
    MSDelay(15);
    temp = PORTB;
    temp ^= 0xF0;
    if (!temp)
        return;
    while (temp <<= 1)
        COL++;
    PORTB = 0xFE;
    if (PORTB != 0xFE)
        ROW = 0;
    else
    {
        PORTB = 0xFD;
        if (PORTB != 0xFD)
            ROW = 1;
        else
        {
            PORTB = 0xFB;
            if (PORTB != 0xFB)
                ROW = 2;
            else
            {
                PORTB = 0xF7;
                if (PORTB != 0xF7)
                    ROW = 3;
            }
        }
    }
    if (ROW < 4)
        SerTX(keypad[ROW][COL]);
    while (PORTB != 0xF0)
        PORTB = 0xF0;
    INTCONbits.RBIF = 0;
}

void SerTX(unsigned char x)
{
    while (PIR1bits.TXIF != 1)
        ;
    TXREG = x;
    PORTD = x;
}

void MSDelay(unsigned int millisecs)
{
    unsigned int i, j;
    for (i = 0; i < millisecs; i++)
        for (j = 0; j < 135; j++)
        {
        }
}