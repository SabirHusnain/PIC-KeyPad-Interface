#line 1 "main.c"
#line 1 "main.c"



void SerTX(unsigned char x);
void RBIF_ISR(void);
void MSDelay(unsigned int millisecs);
unsigned char keypad[4][4] = {'0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F'};












void __interrupt() chk_int () {
    if (INTCONbits.RBIF == 1)
        RBIF_ISR();
}


void main() {
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

void RBIF_ISR(void) {
    unsigned char temp, COL = 0, ROW = 4;
    MSDelay(15);
    temp = PORTB;
    temp ^= 0xF0;
    if (!temp) return;
    while (temp <<= 1) COL++;
    PORTB = 0xFE;
    if (PORTB != 0xFE)
        ROW = 0;
    else {
        PORTB = 0xFD;
        if (PORTB != 0xFD)
            ROW = 1;
        else {
            PORTB = 0xFB;
            if (PORTB != 0xFB)
                ROW = 2;
            else {
                PORTB = 0xF7;
                if (PORTB != 0xF7)
                    ROW = 3;
            }
        }
    }
    if (ROW < 4)
        SerTX(keypad[ROW][COL]);
    while (PORTB != 0xF0) PORTB = 0xF0;
    INTCONbits.RBIF = 0;
}

void SerTX(unsigned char x) {
    while (PIR1bits.TXIF != 1);
    TXREG = x;
}

void MSDelay(unsigned int millisecs) {
    unsigned int i, j;
    for (i = 0; i < millisecs; i++)
        for (j = 0; j < 135; j++) {
        }
}