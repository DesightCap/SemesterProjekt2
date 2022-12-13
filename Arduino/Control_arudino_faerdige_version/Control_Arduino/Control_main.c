// control main

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include "x10.h"
#include "uart1.h"
#include <util/delay.h>
#define dataSIZE 30

volatile int counter = 100; //sender counter
volatile int count = 1; // modtager counter
uint16_t temperatureRequest = 0b0000000011111111;
uint16_t com;
uint16_t address = 0b0000000000000100;
uint16_t myAdress = 0b0000000000000001;
uint16_t combined = 0;
uint16_t encoded = 0;
uint8_t start = 0b00001110;
uint8_t startRecieved = 0;
uint32_t datapakkeRecieved = 0x0000;
uint16_t addressRecieved = 0;
uint16_t commandRecieved = 0x00;
uint32_t datapakke = 0;

char charRecieved = 'd';

int main(void)
{
	initPort();
	initISR();
	InitUART(9600,8);
	while (1)
	{
		
		charRecieved = ReadChar();
		
		
		switch(charRecieved)
		{
			
			case 'd': // blow on
			
			address = 0b0000000000000110;
			com = 0b0000000010110000;
			sendx10(&address, &com, &combined, &encoded, &datapakke, &counter);
			
			
			break;
			
			case 'u': // heat on
			
			address = 0b0000000000000110;
			com = 0b0000000011000000;
			sendx10(&address, &com, &combined, &encoded, &datapakke, &counter);
			
			break;
			

			case 'p': ; // change code 
			// OBS ';' indsat for at undgå "a label can only be part of a statement and a declaration is not a statement" fejl

			char pwdRecieved = ReadChar();
			address = 0b0000000000000101;
			com = 0b0000000010000000; // sidste 4 = XXXX?
			com |= (uint16_t)pwdRecieved;
			sendx10(&address, &com, &combined, &encoded, &datapakke, &counter);
			
			break;
			
			default:
			
			// kontinuerlig tempRequest
			address = 0b0000000000000100;
			com = 0b0000000011111111;

			sendx10(&address, &com, &combined, &encoded, &datapakke, &counter);
			
			while(count < dataSIZE +2){}
			
			if(count == dataSIZE +2)
			{
				recievex10(&addressRecieved, &commandRecieved, &combined, &encoded, &datapakkeRecieved, &count);
			}

			if (addressRecieved == myAdress)
			{
				float temp = (commandRecieved >> 1);
				
				if (commandRecieved << 15)
				{
					temp += 0.5;
				}
				char charArray[sizeof(temp)];
				dtostrf(temp, 3, 1, charArray);
				SendString(charArray);
			}

			break;
		}
		
		charRecieved = '0';
		_delay_ms(2000);
		
	}
}


ISR(INT0_vect)
{
	//////////////////////SENDER ISR!///////////////
	/*Saetter PIN til high / low på ZC interrupt.Counter saettes til 1 når sendx10() kaldes og
	datastroemmen slutter når det sidste bit i datapakke er sat på PORTA*/
	
	if (counter <=4)
	{
		if (start & (0b00001000 >> (counter - 1)))
		//sendBurst();
		PORTA = 0b00000001;
		
		else PORTA = 0;
	}
	
	
	if (counter > 4 && counter <= dataSIZE+4)
	{
		if (datapakke & (0b10000000000000000000000000000000 >> (counter - 5))) // set PIN high
		//sendBurst();
		PORTA = 0b00000001;
		
		else PORTA = 0; //set PIN low
	}
	
	if (counter > dataSIZE+4)
	{
		PORTA = 0;
	}
	
	counter++;
	
	
	/////////////////////MODTAGER ISR!///////////////
	

	//_delay_ms(20);
	uint8_t inputPin = (PINC & 0x1);
	uint32_t longInputPin = 0x0000;
	longInputPin|= inputPin;
	
	if(startRecieved != 0b00001110)
	{
		startRecieved |= inputPin;
		startRecieved = startRecieved << 1;
	}
	else if (startRecieved == 0b00001110)

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
			
			count++;
		}
	}
}

