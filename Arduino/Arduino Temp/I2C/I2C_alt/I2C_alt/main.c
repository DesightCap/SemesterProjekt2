// ================ I2C master uint8_t
// from https://microcontrollerslab.com/lm75-temperature-sensor-module-pinout-interfacing-with-arduino/
#include <inttypes.h>
#include <avr/io.h>
//#include "x10.h"
#include <math.h>
#include "uart1.h"
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000

char recieved[5] = "00.0";

ISR(USART0_RX_vect)
{
	/*char temp[5] = "00.0";

	while(1)
	{
		for (int i = 0; i < 5;i++)
		{
			temp[i] = ReadChar();
		}
	}
	if (temp[0] != '0' && temp[1] != '0' && temp[2] != '.' && temp[3] != '0')
	{
		for (int i = 0;i < 5; i++)
		{
			recieved[i] = temp[i];
		}
	}*/
	
	char buffer[5];
	//unsigned char recievedData = UDR0;
	//unsigned char number = recievedData - '0';
	sprintf(buffer, "%d\r\n", number);
	SendString(buffer);
	
}

ISR(USART0_TX_vect)
{

}


int main(){

	DDRE = 0x00;
	DDRB =0xFF;



	InitUART(9600,8,1);
	sei();

	//PINE0;
	
	_delay_ms(2000);
	while (1)
	{
	}
	return 0;
}