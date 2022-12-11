#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "x10.h"
#include <util/delay.h>



#define pinHot 0b00000100
#define pinCold 0b00000001
#define F_CPU 16000000
#define dataSIZE 30

void runBlower();
void runHeater();

volatile int count = 1;

uint8_t startRecieved = 0;
uint32_t datapakkeRecieved = 0x0000;
uint16_t addressRecieved = 0;
uint16_t commandRecieved = 0x00;
uint16_t encoded = 0;
uint16_t combined = 0;

uint16_t my_adr = 0b00000101;

uint32_t blowTime = 0;
uint32_t hotTime = 0;


int main(void)
{
	// opsæt port/pin
	DDRL = 0xFF;
	// 1 pin til cold - PINA & 0b00000100
	// 1 pin til hot  - PINA & 0b00001000
	int timeMultiplier = 1; // Skal være over 0;

	initPort();
	initISR();
	
	startRecieved = 0;
	datapakkeRecieved = 0x0000;
	addressRecieved = 0;
	commandRecieved = 0x00;
	encoded = 0;
	combined = 0;
	
	while (1)
	{
		// Modtag x10
		if (count == (dataSIZE+2))
		{
			recievex10(&addressRecieved, &commandRecieved, &combined, &encoded, &datapakkeRecieved, &count);
			//PORTB = commandRecieved;
		}
			
		//PORTB = addressRecieved;
		//if (my_adr = addressRecieved)
		
		
		
		switch (commandRecieved)
		{
			case 0b0000000011110000:
			blowTime = 1000000 * timeMultiplier;
			break;
			
			case 0b0000000000001111:
			hotTime = 1000000 * timeMultiplier;
			
			break;
			default:
			
			break;
		}
		
		
		addressRecieved = 0;
		commandRecieved = 0;
		
		
		runBlower();
		runHeater();
			
		
	}
	
}

void runBlower()
{
		if(blowTime > 0)
		{
			PORTL |= pinCold;
			_delay_us(100);
			blowTime--;
		}
			
		else
		{
			PORTL &= ~pinCold;
		}
}

void runHeater()
{
		if(hotTime > 0)
		{
			PORTL |= pinHot;
			_delay_us(100);
			hotTime--;
		}
		
		else
		{
			PORTL &= ~pinHot;
		}
}




ISR(INT0_vect)
{

	_delay_us(20);
	uint8_t inputPin = (PINC & 0x1);
	uint32_t longInputPin = 0x0000;
	longInputPin|= inputPin;
	
	if(startRecieved != 0b00001110)
	{
		startRecieved |= inputPin;
		startRecieved = startRecieved << 1;
	}
	else
	{
		if(count <= dataSIZE)
		{
			datapakkeRecieved |= longInputPin;
			datapakkeRecieved = (datapakkeRecieved << 1);
			count++;
		}
		else if (count == (dataSIZE+1))
		{
			datapakkeRecieved = datapakkeRecieved << 2;
			startRecieved = 0;
			//PORTB = (datapakkeRecieved);
			count++;
		}

		
	}

}