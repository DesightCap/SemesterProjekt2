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
	// Initialize LED port
	initLEDport();
	// Initialize UART: Baud = 9600, 8 data bits, No Parity
	InitUART(9600,8);
	initSwitchPort();
	
	turnOffLED(2);
	turnOnLED(3);
	turnOffLED(4);
	turnOnLED(5);
	
	
	while (1)
	{
		//_delay_ms(2000);
		
		if ( switchStatus() != 0 )
		{
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
		}
		else
		{
			char recieved = ReadChar();
			if (recieved == 'u' || recieved == 'U')
			{
				tempChange('u');
			}
			else if (recieved == 'd'||recieved == 'D')
			{
				tempChange('d');
			}	
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
