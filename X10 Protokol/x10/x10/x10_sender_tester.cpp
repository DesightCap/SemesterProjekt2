#include <iostream>
using namespace std;
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include "x10.h"
#include <iostream>
using namespace std;

DDRA = 0xFF;
PORTA = 0x00;

//counter for hvert ZC interrupt (global variabel s� den kan ses i ISR-scope)
volatile int counter = 100;
volatile int* counterPtr = &counter;

//datapakke (gobal variabel s� den kan ses i ISR-scope). uint8 til test, skal v�re uint32 til system.
uint8_t datapakke;
uint8_t* datapakkePtr = &datapakke;

ISR(INT0_vect) //Interrupt p� int0. Tjek pin nummer
{
	/*Saetter PIN til high / low p� ZC interrupt.Counter saettes til 1 n�r sendx10() kaldes og
	datastroemmen slutter n�r det sidste bit i datapakke er sat p� PIN A0*/

	if (counter <= sizeof(datapakke);
	{
		if (datapakke & (1 << (counter - 1))) // set PIN high
			PORTA = 0b00000001;

		else //set PIN low
			PORTA = 0b00000000;
	}
	counter++;
}


int main()
{
	x10 x10senderObject;

	x10senderObject.initISR();

	x10senderObject.sendx10('y', &datapakke, &counter);
}
