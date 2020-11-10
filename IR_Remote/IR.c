#include <reg51.h>
#include "LCD16x2.h"


unsigned long int bitPattern=0,newKey=0;
unsigned char timerValue;
unsigned char msCount=0;
char pulseCount=0;

//16753245 = 1
//16736925 = 2
//16769565 = 3
//16720605 = 4
//16712445 = 5
//16761405 = 6
//16769055 = 7
//16754775 = 8
//16748655 = 9
//16750695 = 0
//16738455 = *
//16756815 = #
//16718055 = ^
//16730805 = v
//16734885 = >
//16716015 = <
//16726215 = ok


unsigned long int _1 = 16753245;
unsigned long int _2 = 16736925;
unsigned long int _3 = 16769565;
unsigned long int _4 = 16720605;
unsigned long int _5 = 16712445;
unsigned long int _6 = 16761405;
unsigned long int _7 = 16769055;
unsigned long int _8 = 16754775;
unsigned long int _9 = 16748655;
unsigned long int _0 = 16750695;
unsigned long int _star = 16738455;
unsigned long int _ddd = 16756815;
unsigned long int _up = 16718055;
unsigned long int _down = 16730805;
unsigned long int _right = 16734885;
unsigned long int _left = 16716015;
unsigned long int _ok = 16726215;

void timer0_isr() interrupt 1
{
    if(msCount<50)
        msCount++;

    TH0 = 0xFC;   // Reload the timer value for 1ms Delay
    TL0 = 0x67;
}

void externalIntr1_ISR()  interrupt 2
{
	LCD_Clear();
	LCD_String("In intteryupt");
}

void externalIntr0_ISR()  interrupt 0
{
    timerValue = msCount;
    msCount = 0;

    TH0 = 0xFC;  // Reload the timer value for 1ms Delay
    TL0 = 0x67;

    pulseCount++;

    if((timerValue>=50)) // If the pulse width is greater than 50ms, this will mark the SOF
    { 
        pulseCount = -2; // First 2 counts needs to be skipped hence pulse count is set to -2 
        bitPattern = 0;
    }
    else if((pulseCount>=0) && (pulseCount<32)) //Accumulate the bit values between 0-31.
    {  
        if(timerValue>=2)                      //pulse width greater than 2ms is considered as LOGIC1
        {
            bitPattern |=(unsigned long int)1<<(31-pulseCount);
        }
        else
        {

        }
    }
    else if(pulseCount>=32)                  //This will mark the End of frame as 32 pulses are received
    {
        newKey = bitPattern;                // Copy the newKey(patter) and set the pulse count to 0;
        pulseCount = 0;
    }
}

void check(unsigned long int val){
	LCD_Clear();
	LCD_String("Val: ");
	
	if(val == _0)
		LCD_String("zero");
	if(val == _1)
		LCD_String("one");
	if(val == _2)
		LCD_String("two");
	if(val == _3)
		LCD_String("three");
	if(val == _4)
		LCD_String("four");
	if(val == _5)
		LCD_String("five");
	if(val == _6)
		LCD_String("six");
	if(val == _7)
		LCD_String("seven");
	if(val == _8)
		LCD_String("eghit");
	if(val == _9)
		LCD_String("nine");
	if(val == _star)
		LCD_String("*");
	if(val == _ddd)
		LCD_String("#");
	if(val == _up)
		LCD_String("Up");
	if(val == _down)
		LCD_String("Down");
	if(val == _right)
		LCD_String(">");
	if(val == _left)
		LCD_String("<");
	if(val == _ok)
		LCD_String("OK");
}

void main()
{
    LCD_Init();

    TMOD |= 0x01; // Timer0 MODE1(16-bit timer)
    TH0 = 0xFC;   //Timer value for 1ms at 11.0592Mhz clock
    TL0 = 0x67;
    TR0 = 1;       // Start the Timer
    ET0 = 1;       // Enable the Timer0 Interrupt
    
    IT0 = 1;       // Configure INT0 falling edge interrupt
    EX0 = 1;       // Enable the INT0 External Interrupt
	
		IT1 = 0;       // Configure INT1 falling edge interrupt
    EX1 = 1;       // Enable the INT1 External Interrupt
    
    EA  = 1;       // Enable the Global Interrupt bit

    LCD_String("Init Done");

    while(1)
    {
        if(newKey!=0)		 //Wait till newKey press is detected
        {  
						check(newKey);
            //LCD_Clear();	 //Clear the LCD and Display the new Key Code
            //LCD_PrintNum(newKey);
            newKey = 0;		 //CLear the newKey value for next cycle
        }
    }		
}	