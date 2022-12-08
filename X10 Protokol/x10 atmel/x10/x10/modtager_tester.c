#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include "x10.h"
#include <util/delay.h>
#define dataSIZE 30

volatile int count = 100;

uint8_t startRecieved = 0;
uint32_t datapakkeRecieved = 0;
uint32_t longPortA;

ISR(INT0_vect) 
{
	if (count <=4)
		startRecieved = startRecieved | (PORTA << (count - 1));
	
	if (count >4 && startRecieved == 0b00001110)
	{
		longPortA = PORTA;
		if (count <= dataSIZE)
		{
			datapakkeRecieved = datapakkeRecieved | (longPortA << (count - 5));
		}
	}
	
	else if (count>=4 && startRecieved != 0b00001110)
		{
			startRecieved = 0;
			datapakkeRecieved = 0;
		
		}
	
	count++;
}

int main()
{
	initPort();
	initISR();

	while(1)
	{
		if(PORTA!=0)
		recievex10(&count);
		_delay_ms(10000);
	}
}



