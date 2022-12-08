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
uint16_t com = 0b0000000010101010;
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
	// Initialize LED port
	//initLEDport();
	// Initialize UART: Baud = 9600, 8 data bits, No Parity
	InitUART(9600,8);
	//initSwitchPort();
	initPort();
	initISR();
	
	
	while (1)
	{
		
		// Placer kode/funktionskald til modtagelse af x10 her
		_delay_ms(2000);
		
		uint16_t x10recievedTemp = (com << 9);
		x10recievedTemp = (x10recievedTemp >> 9);
		

		//SendChar((char)x10recievedTemp);
		
		float temp = (x10recievedTemp >> 1);
		if (x10recievedTemp << 15)
		{
			temp += 0.5;
		}
		if (PORTC != 0)
		{
			char charArray[sizeof(temp)];
			dtostrf(temp, 5, 1, charArray);
			SendString(charArray);
		}
		else
		{
			recieved = ReadChar();
			if (recieved == 'u' || recieved == 'U')
			{
				temperatureChange('u');
			}
			else if (recieved == 'd'||recieved == 'D')
			{
				temperatureChange('d');
			}
		}
		
		recieved = '0';
	}
	
	while (1)
	{
		// for en sikkeheds skyld
	}
}

void temperatureChange(char t)
{
	if (t == 'u')
	{
		com = 0b0000000000001111;
		sendInProcess = sendx10(&address, &com, &combined, &encoded, &datapakke, &counter);

	}
	else if (t == 'd')
	{
		com = 0b0000000011110000;		
		sendInProcess = sendx10(&address, &com, &combined, &encoded, &datapakke, &counter);
		
	}
}

void temperatureRequest()
{
	com = 0b0000000000111100;
	sendInProcess = sendx10(&address, &com, &combined, &encoded, &datapakke, &counter);

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
	else if (!sendInProcess)
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
	
}