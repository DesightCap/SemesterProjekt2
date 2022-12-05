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

DDRA = 0x00F;
DDRB = 0xFF;

//modtag på PORTA


volatile int count = 1;

uint8_t datapakke = 0;

ISR(INT0_vect) //Interrupt på int0. Tjek pin nummer
{

	if (count <= sizeof(datapakke);
	{
		datapakke = datapakke | (PORTA << (count - 1));

	}
	count++;
}


int main()

{
	while (count >= 1)
	{
		PORTB = datapakke;
	}

}


