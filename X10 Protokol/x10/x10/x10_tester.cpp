#include <iostream>
using namespace std;
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include "x10.h"

//counter for hvert ZC interrupt
volatile int counter = 100;
volatile int* counterPtr = &counter;


//data
uint32_t datapakke;
uint32_t* datapakkePtr = &datapakke;

ISR(INT0_vect) //Interrupt på int0
{
	/*Saetter PIN til high / low på ZC interrupt.Counter saettes til 1 hver gang sendx10() kaldes og
	datastroemmen slutter når counter < datasize, altsaa naar hele pakken er sendt*/

	if (counter <= sizeof(datapakke));
	{
		if (datapakke & (1 << (counter - 1)))
			//set PIN high

		else //set PIN low
	}
	counter++;
}

int main()
{
	x10 x10senderObject;
	x10senderObject.sendx10();
	initISR();

	//UART signal fra computer f.eks. "heatOn"
	//x10object.sendx10("heatOn")
}

