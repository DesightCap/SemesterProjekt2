#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "x10.h"
#include <util/delay.h>
#define F_CPU 16000000
#define dataSIZE 30



void setCode(uint16_t);


volatile int count = 1;

uint8_t startRecieved = 0;
uint32_t datapakkeRecieved = 0x0000;
uint16_t addressRecieved = 0;
uint16_t commandRecieved = 0x00;
uint16_t encoded = 0;
uint16_t combined = 0;
uint16_t myAddress = 0b0000000000000101;

uint8_t changeCode = 0;



int main(void)
{
	
	DDRA = 0xFF;
	PORTA = 0x00;
	DDRB = 0xFF; //For testing, can delete
	PORTB = 0x00; //For testing, can delete
	
	initPort();
	initISR();
    
	//commandRecieved = 0b0000000010001100; //For testing only
	
	while (1) 
    {
		
		if (count == (dataSIZE+2))
		{
			recievex10(&addressRecieved, &commandRecieved, &combined, &encoded, &datapakkeRecieved, &count);
		}
		
		if(addressRecieved == myAddress)
		{
			changeCode = (commandRecieved & 0x80) >>7;
			
			switch(changeCode)
			{
				case 1:
				setCode(commandRecieved);
				break;
				
				default:
				break;

			}	
			
			
			addressRecieved = 0;
			commandRecieved = 0;
		}
	
		
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
			count++;
		}

		
	}

}



void setCode(uint16_t command)
{
	uint8_t newCode = (uint8_t) command;
	newCode &= 0b00001111;
	
	PORTA |= newCode;
	PORTB = PORTA; //For testing, can delete
	_delay_ms(100);
	
	PORTA |= 0b10000000;
	PORTB = PORTA; //For testing, can delete
	_delay_ms(100);
	
	PORTA &= 0b01111111;
	PORTB = PORTA; //For testing, can delete
	_delay_ms(100);
	
	PORTA = 0b00000000;
	PORTB = PORTA; //For testing, can delete
}