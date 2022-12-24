#include <avr/io.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "uart1.h"
#include "led.h"
#include "switch.h"
#include <util/delay.h>
#include "x10.h"
#include <avr/interrupt.h>
#include <string.h>


#define F_CPU 16000000
#define dataSIZE 30
#define ADDRESSSIZE 3
#define COMMANDSIZE 8
#define COMBINEDSIZE 11
#define ENCODEDSIZE 15


uint16_t address = 0b0000000000000101;
uint16_t com;
uint16_t combined = 0;
uint16_t encoded = 0;
uint8_t start = 0b00001110;
uint8_t startRecieved = 0;
uint32_t datapakkeRecieved = 0x0000;
uint16_t addressRecieved = 0;
uint16_t commandRecieved = 0x00;

uint16_t my_adr = 0b00000001;


volatile int counter = 100; // Til at modtage
volatile int* counterPtr = &counter;

volatile int count = 1; // Til at sende

//datapakke (global variabel så den kan ses i ISR-scope). uint8 til test, skal være uint32 til system.
uint32_t datapakke = 0;

char recieved = '0';
char* toSend = "00.0";
int sendInProcess = 0;

uint16_t addressRecieved;
uint16_t commandRecieved;

void temperatureChange(char);
void temperatureRequest();



int main()
{
	// Initialize UART: Baud = 9600, 8 data bits, No Parity
	InitUART(9600,8);
	//initSwitchPort();
	initPort();
	initISR();
	
	while (1)
	{
		recieved = ReadChar();
		
		if (recieved == 'u' || recieved == 'U')
		{
			//	temperatureChange('u');
			PORTB = 0b11111111;
			if ( PORTL == 0b00000001 || PORTL == 0b00000100)
			{
				PORTL = 0b00000000;
			}
			PORTL |= 1; // blow eksempel
			//com = 0b1111111100000000;

		}

		if (recieved == 'd'||recieved == 'D')
		{
			PORTB = 0b00000001;
			
			if ( PORTL == 0b00000001 || PORTL == 0b00000100)
			{
				PORTL = 0b00000000;
			}
			PORTL |=1<<2; // heat eksempel
			//	com = 0b0000000011111111;

			//temperatureChange('d');
		}
	}
}

void temperatureChange(char t)
{
	if (t == 'u')
	{
		toggleLED(0b11001100);
		//com = 0b0000000000001111;
		//sendInProcess = sendx10(&address, &com, &combined, &encoded, &datapakke, &counter);

	}
	else if (t == 'd')
	{
		
		toggleLED(0b00110011);
		//com = 0b0000000011110000;
		//sendInProcess = sendx10(&address, &com, &combined, &encoded, &datapakke, &counter);
		
	}
}

void temperatureRequest()
{
	com = 0b0000000000111100;
	//sendInProcess = sendx10(&address, &com, &combined, &encoded, &datapakke, &counter);

}

ISR(INT0_vect) //Interrupt på int0. Tjek pin nummer
{
	if (sendInProcess)
	{
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
			sendInProcess = 0;
		}
		counter++;

	}
}