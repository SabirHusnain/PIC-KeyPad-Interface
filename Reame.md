## Keypad Interfacing with PIC & Arduino

**Objectives: -**

- To understand what are keypads and how they work.
- To understand the use of keypad with microcontrollers.
- To understand the serial communication in Arduino.
- To interface keypad with Arduino and send data serially to a serial pin.

**Theory: -**

- **Keypad: -**

A keypad is one of the most commonly used input devices in microprocessor applications. In a standard keypad wired as an X-Y switch matrix, normally-open switches connect a row to a column when pressed. If a keypad has 12 keys, it is wired as 3 columns by 4 rows. A 16 key pad would have 4 columns by 4 rows.

![](RackMultipart20220929-1-54qj73_html_feeadf99c93c1efb.png)

**Figure 6.1: Keypad (4 x 3)**

A matrix keypad is a group of switches connected in an X-Y, or row-column, matrix arrangement. Pressing a button will connect a row with a column, and we can then scan the array to determine which button was pressed.

By arranging the push buttons in a matrix as opposed to connecting them directly to input pins we can usually reduce the amount of wiring. Commercial matrix keypads are inexpensive and readily available, which simplifies the wiring even more.

This is a great solution, with the following caveats:

- You need at least six pushbuttons. Fewer than six defeats the wiring advantages, as six pushbuttons will require a 2 x 3 matrix for a total of five wires. A larger number of keys will increase the advantage of using a matrix.
- You don't require users to press two buttons simultaneously. The keypad solution will only accept one input at a time.
- You'll need to use a software routine to determine which button was pressed, this is in addition to the code you'll need to write to actually do something once the keypress is received.
- You'll still need a lot of inputs, not as many as discrete switches (providing you stay over five) but more than the resistive keypad or ASCII keyboard solutions. Once you exceed 25 push buttons, you're better off choosing the ASCII Keyboard solution.

The type of keypad we will be using today is a "_Matrix Membrane Keypad_". This is an X-Y matrix of switches created using layers of plastic and conductive surfaces. These devices are inexpensive and can be styled to match your project, reducing the mechanical work of mounting all of those push buttons. Two extremely common membrane matrix keypads are the 4 x 3 keypad (12 switches) and the 4 x 4 keypad (16 switches). They are laid out in the same fashion as a telephone keypad, which is a common user interface that most people are familiar with. Although I'll be using the 4 X 4 matrix keypad in today's experiments you can easily substitute the 4 x 3 unit with minor wiring and code changes.

- **Keypad Working Principle: -**

To understand how matrix keypads work we'll examine the operation of a basic 4 x 4 keypad. As you can see from the above illustration the keypad consists of four rows and four columns, overlaid by 16 push button switches (represented by circles). Pressing a switch will connect its corresponding row and column lines. In order to use the keypad, we will make all of the column connections inputs and hold them HIGH, so they all have a value of digital one. The rows are configured as outputs, each with a value of zero or LOW.

![](RackMultipart20220929-1-54qj73_html_97fa808dd5e9eaea.jpg) ![](RackMultipart20220929-1-54qj73_html_a3afe370284a6856.jpg)

**Figure 6.2: Internal Mechanism of Keypads**

When a key is pressed, as illustrated above, it will connect its column input to one of the row outputs. This will bring that input LOW, and in doing so it reveals which column the switch belongs to. It also serves as an indication that a switch was pressed. Now we need to determine which row the switch is connected to. To do this we will pull all of the column inputs LOW (technically we really only need to keep our detected column LOW, but it is simpler to control them all). After doing this we begin scanning the rows by raising them HIGH, one-by-one. In the above illustration, we test the top row, which produces no results.

- **Keypad Interfacing with**  **μ**** C: -**

There are tow methods to configure the keypad with any microcontroller and those are: -

- Scanning Method
- Interrupt Method

These are explained in the below text: -

- **Scanning Method: -**

Different approaches of keypad scanning, which is required to identify which key has been pressed.

**Sequential exploration of rows**

- Rows → as output and columns → as input, or vice-versa.
- Column lines (inputs), are pulled-high using internal pull-up resistors
- Therefore, default input to these lines is 1.
- The state for matrix keypad can be explored by turning the rows low sequentially, one at a time, and reading columns.
- For example, set the first row to 0 and read all the columns.
- If any key has been pressed in that row, the corresponding column line must read as 0. Otherwise, go to the next row and set it to 0, and read the columns again.
- This process is repeated until a 0 is found in a column. This determines the row and column for the pressed key, thus giving the exploration code for that key.

**Simultaneous exploration of rows and columns**

- All rows and columns are explored simultaneously in two phases.
- First, rows → as output and columns → as input. (Internal pullups are enabled on the column lines.)
- Then all rows → set to 0 and columns are read. If any key has been pressed, corresponding column will be read as 0. This detects column but not its row.
- The whole process is reversed next. Rows → as input and columns as output. (Internal weak pull-ups are now enabled on row lines.)
- All columns are set to 0, and rows are read. Row that reads 0 contains pressed key.
- This gives both the row and column for the pressed key.

**Note:** This approach of exploring the keypad is relatively faster than sequential exploration approach.

- **Interrupt Method: -**

Rows of keypad are connected to PORTB.Low (RB3-RB0) & Columns are connected to PORTB.High (RB7-RB4 (PORTBChange interrupt)).

- Any change causes interrupt which means a key is pressed, In ISR, make sure key was not erroneous & wait for at least 20ms to prevent multiple interrupt from same key
- To make sure preceding key is released, send 0s to rows & check column until all becomes high.
- Detect row in which key is pressed, ground one row at a time and check column if all any column has 0 signal. Row detection confirms the starting address for look-up table.
- To identify key press, rotate column bits one bit at a time into carry flag to check if it is low. Column detection confirms the exact code in the look-up table.

**Program: -**

- **Code: -**

_// PIC18F452 Configuration Bit Settings_

_// 'C' source line config statements_

_// CONFIG1H_

#pragma config OSC = XT   _// Oscillator Selection bits (XT oscillator)_

#pragma config OSCS = OFF _// Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))_

_// CONFIG2L_

#pragma config PWRT = OFF _// Power-up Timer Enable bit (PWRT disabled)_

#pragma config BOR = ON   _// Brown-out Reset Enable bit (Brown-out Reset enabled)_

#pragma config BORV = 20  _// Brown-out Reset Voltage bits (VBOR set to 2.0V)_

_// CONFIG2H_

#pragma config WDT = OFF   _// Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))_

#pragma config WDTPS = 128 _// Watchdog Timer Postscale Select bits (1:128)_

_// CONFIG3H_

#pragma config CCP2MUX = ON _// CCP2 Mux bit (CCP2 input/output is multiplexed with RC1)_

_// CONFIG4L_

#pragma config STVR = OFF _// Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will not cause RESET)_

#pragma config LVP = OFF  _// Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)_

_// CONFIG5L_

#pragma config CP0 = OFF _// Code Protection bit (Block 0 (000200-001FFFh) not code protected)_

#pragma config CP1 = OFF _// Code Protection bit (Block 1 (002000-003FFFh) not code protected)_

#pragma config CP2 = OFF _// Code Protection bit (Block 2 (004000-005FFFh) not code protected)_

#pragma config CP3 = OFF _// Code Protection bit (Block 3 (006000-007FFFh) not code protected)_

_// CONFIG5H_

#pragma config CPB = OFF _// Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)_

#pragma config CPD = OFF _// Data EEPROM Code Protection bit (Data EEPROM not code protected)_

_// CONFIG6L_

#pragma config WRT0 = OFF _// Write Protection bit (Block 0 (000200-001FFFh) not write protected)_

#pragma config WRT1 = OFF _// Write Protection bit (Block 1 (002000-003FFFh) not write protected)_

#pragma config WRT2 = OFF _// Write Protection bit (Block 2 (004000-005FFFh) not write protected)_

#pragma config WRT3 = OFF _// Write Protection bit (Block 3 (006000-007FFFh) not write protected)_

_// CONFIG6H_

#pragma config WRTC = OFF _// Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)_

#pragma config WRTB = OFF _// Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)_

#pragma config WRTD = OFF _// Data EEPROM Write Protection bit (Data EEPROM not write protected)_

_// CONFIG7L_

#pragma config EBTR0 = OFF _// Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)_

#pragma config EBTR1 = OFF _// Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)_

#pragma config EBTR2 = OFF _// Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)_

#pragma config EBTR3 = OFF _// Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)_

_// CONFIG7H_

#pragma config EBTRB = OFF _// Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)_

_// #pragma config statements should precede project file includes._

_// Use project enums instead of #define for ON and OFF._

#include \<xc.h\>

#include \<pic18f452.h\>

void SerTX(unsigned char x);

void RBIF\_ISR(void);

void MSDelay(unsigned int millisecs);

unsigned char keypad[4][4] = {

    {'7', '8', '9', '/'},

    {'4', '5', '6', '\*'},

    {'1', '2', '3', '-'},

    {'C', '0', '=', '+'}};

void \_\_interrupt(high\_priority) chk\_int()

{

    if (INTCONbits.RBIF == 1)

        RBIF\_ISR();

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

void RBIF\_ISR(void)

{

    unsigned char temp, COL = 0, ROW = 4;

    MSDelay(15);

    temp = PORTB;

    temp ^= 0xF0;

    if (!temp)

        return;

    while (temp \<\<= 1)

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

    if (ROW \< 4)

        SerTX(keypad[ROW][COL]);

    while (PORTB != 0xF0)

        PORTB = 0xF0;

    INTCONbits.RBIF = 0;

}

void SerTX(unsigned char x)

{

    while (PIR1bits.TXIF != 1);

    TXREG = x;

    PORTD = x;

}

void MSDelay(unsigned int millisecs)

{

    unsigned int i, j;

    for (i = 0; i \< millisecs; i++)

        for (j = 0; j \< 135; j++)

        {

        }

}

- **Explanation: -**

In this code interrupt method is being carried out to detect the keypress on keypad. An interrupt causes to find out which key has been pressed by turning LOW and HIGH on each columns of the keypad. In this code same concept is being used and when a key is pressed it causes an interrupt and that interrupt goes to ISR.

In ISR the interrupt is analyzed and it is tested from which row the interrupt is coming and then every column is tested with row by turning it HIGH when column matches the row the that specific value is picked from the array formed at the start of the program.

That value is serially sent to the Serial Port and the PORTD the ASCII code of the character appears in parallel manner on PORTB and in serial manner on RC6 or TX pin of the PIC18F452.

Then this data is analyzed by the serial monitor or virtual terminal available in proteus simulator.

**Circuit: -**

![](RackMultipart20220929-1-54qj73_html_f3e24bd84a2f158f.gif)

**Figure 6.3: Keypad Circuit with PIC in Proteus**

![](RackMultipart20220929-1-54qj73_html_88478065f4595a6.png)

**Figure 6.4: Results on Virtual Terminal or Serial Monitor**

**Home Task: -**

-
# Write an Arduino code for keypad interfacing with Arduino and display the results.

**Program: -**

- **Code: -**

#include \<DFRobot\_LCD.h\>

#include \<Wire.h\>

#include \<Key.h\>

#include \<Keypad.h\>

const byte rows = 4;

const byte cols = 3;

char myConfig[rows][cols] = {

    {'1', '2', '3'},

    {'4', '5', '6'},

    {'7', '8', '9'},

    {'\*', '0', '#'}};

byte row\_Pins[rows] = {2, 3, 4, 5};

byte col\_Pins[cols] = {6, 7, 8};

Keypad myKeyPad = Keypad(makeKeymap(myConfig), row\_Pins, col\_Pins, rows, cols);

DFRobot\_LCD myLCD(16, 2);

void setup()

{

  Serial.begin(9600);

  myLCD.setRGB(255, 0, 0);

  myLCD.init();

}

void loop()

{

  myLCD.setCursor(0, 0);

  myLCD.print("Sabir Husnain");

  char getKey = myKeyPad.getKey();

  if (getKey != NO\_KEY)

  {

    Serial.println(getKey);

    myLCD.setCursor(0, 1);

    myLCD.print(getKey);

    delay(10);

  }

}

- **Explanation: -**

In the code of keypad interfacing a keypad library is included in the file. Array for the character recognition is created and the object of the Keypad class is initialized with its constructor by passing it the key configuration array, number of rows and columns and pins on which rows and columns will be attached.

The setup function contains the serial initialization of data with a baud rate of 9600 which is commonly used with Arduino Uno.

The loop contains the function of class Keypad getKey() which is used to detect the character automatically in case of a keypress. If some one pressed a key the key will be saved in the char type variable and will be displayed in the serial monitor.

**Circuit: -**

![](RackMultipart20220929-1-54qj73_html_1856adb19685c3fe.gif)

**Figure 6.5: Keypad Interfacing with Arduino**

![](RackMultipart20220929-1-54qj73_html_fe1455313a168ede.gif) ![](RackMultipart20220929-1-54qj73_html_cd7e0eb14d98e563.png)

**Figure 6.6: Displaying results on Serial Monitor**

**Conclusion: -**

- In this lab session I learnt about the keypads, about their hardware and how to use them.
- Keypad was configured with PIC using C language and it was seen that how to use it with PIC microcontroller.
- The data serialization was also studied and it was learnt that how to send serial data using PIC.
- Keypad was also configured with the Arduino and I see that how to use Arduino to use keypad.
- It was also noted that how to work with Arduino libraries and how easy it is with respect to the PIC.
- It also differs the Arduino from the PIC that the Arduino provides enormous libraries for working with it but PIC didn't.