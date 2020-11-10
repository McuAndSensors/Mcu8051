#include<reg52.h> 
#include "I2C.h"
#include "delay.h"
#include "LCD16x2.h"

unsigned char convertVal60(unsigned char val);
 
char i;
unsigned char sec[2],min[2],hour[2],dow=0,dom[2],month[2],year[2];
unsigned char a[8];
char _hour, _min;
typedef enum { false, true } bool; 
 

 

 
int Dec_To_BCD(int dec){
      return( (dec/10*16) + (dec%10) );
    }


void readAllReg(){
	I2CStart();										//Start comunication
	I2CSend(0xD0);								//Send 0xD0
	I2CSend(0x00);								//Sent 0 address to begain
	I2CStart();	
	I2CSend(0xD1);
	for (i = 0; i < 8; i++) {
		a[i] = I2CRead();
		if(i == 7)
			I2CNak();
		else
			I2CAck();
	}
	I2CStop();		
}
		
void setTime(int Sec, int Min, int Hour,int Dow, int Dom, int Month, int Year)
{
	I2CStart();	
	I2CSend(0xD0);	
	I2CSend(0x00);
		
	I2CSend(Dec_To_BCD(Sec)&0x7f);
	I2CSend(Dec_To_BCD(Min)&0x7f);
	I2CSend(Dec_To_BCD(Hour)&0x3f);
	I2CSend(Dec_To_BCD(Dow)&0x07);
	I2CSend(Dec_To_BCD(Dom)&0x3f);
	I2CSend(Dec_To_BCD(Month)&0x1f);
	I2CSend(Dec_To_BCD(Year)&0xff);

	I2CStop();
}	

void readTimeOnly()
{
	//Read Seconds only
	I2CStart();						//Apply start signal
	I2CSend(0xD0);				//Send Slave address
	I2CSend(0x00);				//Set Address to begin
	I2CStart();						//
	I2CSend(0xD1);				//Sent read request
	for (i = 0; i < 3; i++) {
		a[i] = I2CRead();
		if(i != 2)
			I2CAck();
	}
	I2CNak();
	I2CStop();						//Stop comunication
	a[0]=convertVal60(a[0]);
	a[1]=convertVal60(a[1]);
	a[2]=((a[2]&0x0f))+(((a[2]>>4)&0x03)*10);
}	

unsigned char convertVal60(unsigned char val){
	return (val&0x0f)+(((val>>4)&0x07)*10);	//This function conver min & sec to 0-60 range 
}

void displayTime()
{	//This function, display the time on the 16x2 LCD screen
	LCD_Command(0x01);
	LCD_Command(0x80);
	
  readAllReg();	//Read all data from RTC

	//Hndle 
	a[0]=convertVal60(a[0]);													//First data is Sec, convert to 0-60 range
	sec[0]=(a[0]/10)+48;sec[1]=(a[0]%10)+48;					//Set the values in the sec array "00"
	a[1]=convertVal60(a[1]);													//Second data is Min, convert to 0-60 range
	min[0]=a[1]/10+48;min[1]=a[1]%10+48;							//Set the values in the sec array "00"
	a[2]=((a[2]&0x0f))+(((a[2]>>4)&0x03)*10);					//Thiered data is Hour, conver to 0-24 base
	hour[0]=a[2]/10+48;hour[1]=a[2]%10+48;						//Set the values in the sec array "00"
	LCD_Data(hour[0]);LCD_Data(hour[1]);LCD_Data(':'); 	//Print hour
	LCD_Data(min[0]);LCD_Data(min[1]);LCD_Data(':');		//Print min
	LCD_Data(sec[0]);LCD_Data(sec[1]);LCD_Data(' ');		//Print sec
	
	//Conver value of day-of-week, day-of-month, month, year
	a[3]=(a[3]&0x07);dow=a[3];							//day-of-week
	a[4]=(a[4]&0x0f)+(((a[4]>>4)&0x03)*10);	//day-of-month
	a[5]=(a[5]&0x0f)+(((a[5]>>4)&0x01)*10);	//month
	a[6]=(a[6]&0x0f)+(((a[6]>>4)&0x0f)*10);	//year
	LCD_String(" Day:");LCD_PrintNum(a[3]);
	
	LCD_Command(0xc0);								//Get to next line
	LCD_PrintNum(a[4]);LCD_Data('/');	//print day-of-month
	LCD_PrintNum(a[5]);LCD_Data('/');	//print month
	LCD_PrintNum(a[6]);LCD_Data(' ');	//print year
}

void setStartTime(){
	readTimeOnly();
	_min = a[1];
	_hour = a[2];
}

unsigned short getTotalTimeInMin(){
	char min_in_hour = 60;
	readTimeOnly();
	return((min_in_hour*a[2]+a[1]) - (min_in_hour*_hour+_min));
}

void main()
{
	LCD_Init();
	setStartTime();						//Save current time

	//		sec,min,h,DOW,d,m,y
	//setTime(00,20,12,4,19,8,20); 
	while(1)
	{
		Delay_ms(600);
		LCD_Clear();
		displayTime();											//Show time + day + date
		LCD_PrintNum(getTotalTimeInMin());	//Report how much time has past since time is saved
	}	
}