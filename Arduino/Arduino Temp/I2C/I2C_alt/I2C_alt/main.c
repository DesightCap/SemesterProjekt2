/*
 * LAB12_1.c
 *
 * Created: 25-11-2020 10:08:17
 * Author : sebas
 */ 
/*
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart_int.h"
#include "led.h"
#include "i2c.h"
#include "lm75.h"

unsigned char receivedCounter = 0;
unsigned char lastCharReceived = '\0';

ISR(USART0_RX_vect)
{
//	receivedCounter++;
//	writeAllLEDs(receivedCounter);
//	char c = UDR0;
	
	unsigned char c = UDR0;
	unsigned char n = c - '0';
	char str[30];
	if (n >= 0 && n <=7)
	{
		toggleLED(n);
		sprintf(str, "LED nummer %d er toggled\r\n", n);
		SendString(str);
	}
}

ISR(USART0_TX_vect)
{
	
}

int main(void)
{
	initLEDport();
	writeAllLEDs(receivedCounter);
	InitUART(9600, 8, 1);
	LM75_init();
	sei();
	int temperatureTimes2;
	
    while (1) 
    {
			//PORTB = 0xff;
	temperatureTimes2 = LM75_temperature(1);
			PORTB = 0xff;
		SendInteger(temperatureTimes2 >> 1);
		SendChar(',');
		SendChar((temperatureTimes2 & 1) ? '5' : '0');
		SendString(" grader\r\n");
		PORTB = 0xff;
		_delay_ms(1000);
		
    }
}

*/
// ================ I2C master uint8_t
// from https://microcontrollerslab.com/lm75-temperature-sensor-module-pinout-interfacing-with-arduino/
#include <inttypes.h>
#include <avr/io.h>
//#include "x10.h"
#include <math.h>
#include "uart1"
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
	unsigned char recievedData = UDR0;
	unsigned char number = recievedData - '0';
	
	sprintf(buffer, "%d\n", number);
	
	SendString(buffer);
	
}

ISR(USART0_TX_vect)
{

}


int main(){

	DDRE = 0x00;
	DDRB =0xFF;
	PORTB = 0xFF;


	InitUART(9600,8,1);
	sei();

	//PINE0;
	
	_delay_ms(2000);
	while (1)
	{
	}
	return 0;
}