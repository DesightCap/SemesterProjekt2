#include "x10.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#define dataSIZE 16
#define COMBINEDSIZE 11
#include <util/delay.h>



void sendx10(uint16_t *adr, uint16_t* com, uint16_t* combined, uint16_t* encoded, uint32_t* datapakke, volatile int* counterPtr)
{
	
combineBitArrays(adr, com, combined);

hammingEncoding(combined, encoded);

doubleEncoded(encoded, datapakke);


	*counterPtr = 1;
}

void recievex10(volatile int* count)
{
	*count = 1;
	
}



void initISR() 
{
	EICRA = 0b00000001; // any edge
	EIMSK = 0b00000001; // enable interrupt int 0
	sei(); // Global ISR enabler
}

void initPort()
{
	DDRA = 0xFF;
	PORTA = 0x00;
	
	DDRB = 0xFF;
	PORTB = 0xFF;
}

void combineBitArrays(uint16_t* adr, uint16_t* com, uint16_t* combined) //Generating a new bitString by append one bitString to another
{
	
	
	*combined = ((*adr << 8) | *com);

}

int correctModulo(int x, int m) //Modulo in C is not entirely correct, this implementation fixes that
{
	return (x % m + m) % m;
}

void hammingEncoding(uint16_t* str, uint16_t* encoded) // (15,11) Hamming encoding
{

	//Parity bit 1
	uint16_t checkP1 = 0b11011010101;
	checkP1 &= *str;

	char p1 = 0;

	while (checkP1)
	{
		if (checkP1 & 1)
		{
			p1^= 0b00000001;
		}
		checkP1 >>= 1;
	}

	//Parity bit 2
	uint16_t checkP2 = 0b10110110011;
	checkP2 &= *str;

	char p2 = 0;

	while (checkP2)
	{
		if (checkP2 & 1)
		{
			p2^= 0b00000001;
		}
		checkP2 >>= 1;
	}

	//Parity bit 3
	uint16_t checkP3 = 0b01110001111;
	checkP3 &= *str;

	char p3 = 0;

	while (checkP3)
	{
		if (checkP3 & 1)
		{
			p3^= 0b00000001;
		}
		checkP3 >>= 1;
	}

	//Parity bit 4
	uint16_t checkP4 = 0b00001111111;
	checkP4 &= *str;

	char p4 = 0;

	while (checkP4)
	{
		if (checkP4 & 1)
		{
			p4^= 0b00000001;
		}
		checkP4 >>= 1;
	}

	*encoded = ((*str << 4)) | (p1 << 3) | (p2 << 2) | (p3 << 1) | (p4);
}

void doubleEncoded(uint16_t* encoded, uint32_t* datapakke)
{
	
	for(int i=0; i<15; i++)
	{
		if (*encoded & (0b0100000000000000 >> i))
			*datapakke = (*datapakke|(0b10000000000000000000000000000000 >> (i*2)));
			
		else *datapakke = (*datapakke|(0b01000000000000000000000000000000 >> (i*2)));
	}
}

void sendBurst()
{
	//timer 0 ctc mode no prescale
	 TCCR1A = 0b01000000;
	 TCCR1B = 0b00001001;
	 
	 //frekvens = 120000hz
	 OCR1A = 66;
	 
	 _delay_ms(1);
	  TCCR1B = 0b00000000;
	  TCCR1A = 0b00000000;
	 
}