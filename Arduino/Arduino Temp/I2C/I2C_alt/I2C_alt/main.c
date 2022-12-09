// ================ I2C master uint8_t
// from https://microcontrollerslab.com/lm75-temperature-sensor-module-pinout-interfacing-with-arduino/
#include <inttypes.h>
#include "HelperHeader.h"
#include <avr/io.h>
//#include "x10.h"
#include <math.h>


float HF(float _t){
	return (floor((_t*2)+0.5)/2);
}

uint8_t sixBitsSend(double inputTemp_)
{
	uint8_t Rval = 0b0000000;
	int Temp = inputTemp_;

	if((inputTemp_ - Temp) > 0)
	{
		Rval |= 0b00000001;
	}
	Rval |= ((uint8_t)Temp)<<1;
	return Rval;
}

int main(){
	DDRA = 0xFF;


	while(1)
	{
		uint8_t helperfuntion = (sixBitsSend(HF(getTemp())));

		
		PORTA = helperfuntion;

	}
	return 0;
}