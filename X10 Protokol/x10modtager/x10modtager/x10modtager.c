#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include "x10.h"
#include <util/delay.h>
#define dataSIZE 30

volatile int count = 1;

uint8_t startRecieved = 0;
uint32_t datapakkeRecieved = 0x0000;
uint16_t addressRecieved = 0;
uint16_t commandRecieved = 0x00;
uint16_t encoded = 0;
uint16_t combined = 0;

uint16_t my_adr = 0b00000101;


ISR(INT0_vect)
{

	_delay_ms(20);
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

int main()
{
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

		//datapakkeRecieved = 0b10011010011001100110010110011000; //FOR TESTING ONLINE
		
		if (count == (dataSIZE+2))
		{	
			recievex10(&addressRecieved, &commandRecieved, &combined, &encoded, &datapakkeRecieved, &count);
		}
		PORTB = addressRecieved;
		
	}
}