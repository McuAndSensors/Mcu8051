#include <reg51.h>
#include <intrins.h>

//Works for 11.0592MHz clk
sbit PULSE=P0^2;


void delay_in_us(int us){
	
	while(us--)
  {
    _nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
  }
}

void delay_in_ms(int ms){
	while(ms)
  {
		ms--;
    delay_in_us(50);
  }
}

void ServoMove(unsigned char precent){
	//Value sould be between 0-100, value should represent X%
	char i = 0;
	for(i = 0; i < 15; i++){
		PULSE = 1;
		delay_in_us(18+precent);
		PULSE = 0;
		delay_in_us(1000);
	}
}

void main(){
	
	ServoMove(0);
	delay_in_ms(3000);
	
	ServoMove(100);
	delay_in_ms(3000);
	
	ServoMove(0);
	delay_in_ms(3000);
	
	ServoMove(100);
	delay_in_ms(3000);
	
	ServoMove(50);
	delay_in_ms(3000);
	
	while(1);
}