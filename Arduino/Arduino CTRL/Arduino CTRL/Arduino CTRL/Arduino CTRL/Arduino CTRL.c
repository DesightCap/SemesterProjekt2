#define F_CPU 16000000UL

#include <avr/io.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "uart1.h"
#include "led.h"
#include "switch.h"
#include <util/delay.h>

char recieved = '0';
char* toSend = "00";

void tempChange(char);


int main()
{
	//char tegn;
	//char TestStreng[8] = "2.55 \n\r";

	// Initialize LED port
	initLEDport();
	// Initialize UART: Baud = 9600, 8 data bits, No Parity
	InitUART(9600,8);
	initSwitchPort();
	
	turnOffLED(1);
	turnOnLED(2);
	turnOffLED(3);
	turnOnLED(4);
	srand(time(NULL));
	while (1)
	{
		char recieved = ReadChar();
		//_delay_ms(2000);
		
		if (recieved == 'u' || recieved == 'U')
		{
			tempChange('u');
		}
		else if (recieved == 'd'||recieved == 'D')
		{
			tempChange('d');
		}
		if(switchOn(7))
		{
			toSend = "63";
			toggleLED(7);
			SendString(toSend);
			_delay_ms(2000);
		}
		else if(switchOn(0))
		{
			toSend = "20";
			toggleLED(0);
			SendString(toSend);
			_delay_ms(2000);
		}
		recieved = '0';
	}
	/*
	while (i != 10)
	{
	i ++;
	
	for (int i = 0;  TestStreng[i] != '\n'; i++)
	{
	//SendInteger(i);
	SendChar(TestStreng[i]);
	
	}
	
	
	}
	*/
	while (1)
	{
		// for en sikkeheds skyld
	}
}

void tempChange(char t)
{
	if (t == 'u')
	{
		writeAllLEDs(0b11100000);
	}
	else if (t == 'd')
	{
		writeAllLEDs(0b00000111);
	}
}
