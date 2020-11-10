#include <reg51.h>															/* Include reg51 header file */
#include "LCD16x2.h"
#define Data_Port P1														/* Define data port for GLCD */

sbit RS = P2^0;																	/* Set control bits pins */
sbit RW = P2^1;
sbit E = P2^2;

void delay(k)																		/* Delay of msec with xtal = 11.0592MHz */
{
	int i,j;
	for (i=0;i<k;i++)
		for (j=0;j<112;j++);
}

void LCD_Command(char Command)									/* LCD command function */
{
	Data_Port = Command;
	RS= 0;
	RW= 0;
	E=1;
	delay(1);
	E=0;
	delay(5);
}

void LCD_Clear(){
	LCD_Command (0x01);								/* clear display */
}

void LCD_PrintNum(unsigned long int Num){
	char temp = 0, index = 11;
	char arr[12];
	arr[11] = 0;
	while(Num){
		temp = Num%10;
		Num /= 10;
		arr[--index] = temp + '0';
	}
	LCD_String(arr+index);
}

void LCD_Data(char Data)												/* GLCD data function */
{
	Data_Port = Data;
	RS= 1;
	RW= 0;
	E=1;
	delay(1);
	E=0;
	delay(5);
}

void LCD_String (char *str)							/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)							/* Send each char of string till the NULL */
	{
		LCD_Data (str[i]);							/* Call LCD data write */
	}
}

void LCD_Init (void)								/* LCD Initialize function */
{
	delay(20);									/* LCD Power ON Initialization time >15ms */
	
	LCD_Command (0x38);								/* Initialization of 16X2 LCD in 8bit mode */
	LCD_Command (0x0C);								/* Display ON Cursor OFF */
	LCD_Command (0x06);								/* Auto Increment cursor */
	LCD_Command (0x01);								/* clear display */
	LCD_Command (0x80);								/* cursor at home position */
}

