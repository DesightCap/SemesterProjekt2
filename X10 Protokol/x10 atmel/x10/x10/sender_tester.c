#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include "x10.h"
#include <util/delay.h>
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
	
volatile int counter = 100;
volatile int* counterPtr = &counter;

//datapakke (global variabel s� den kan ses i ISR-scope). uint8 til test, skal v�re uint32 til system.
uint32_t datapakke = 0;


ISR(INT0_vect) //Interrupt p� int0. Tjek pin nummer
{
	/*Saetter PIN til high / low p� ZC interrupt.Counter saettes til 1 n�r sendx10() kaldes og
	datastroemmen slutter n�r det sidste bit i datapakke er sat p� PORTA*/
	
	if (counter <=4)
	{
		if (start & (0b00001000 >> (counter - 1)))
			sendBurst();
			//PORTA = 0b00000001;
		
	//	else PORTA = 0;
	}
	
	
	if (counter > 4 && counter <= dataSIZE+4)
		{
		if (datapakke & (0b10000000000000000000000000000000 >> (counter - 5))) // set PIN high
			sendBurst();
			//PORTA = 0b00000001;
			
		//else PORTA = 0; //set PIN low
		}
	
	//if (counter > dataSIZE+4)	
		//PORTA = 0;
		
	counter++;

}


int main()
{


	initPort();
	initISR();
	
	while(1)
	{
		
		sendx10(&address, &com, &combined, &encoded, &datapakke, &counter);
		_delay_ms(2000);
	}
	
}
