#include "x10.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#define dataSIZE 16
#define COMBINEDSIZE 11




void sendx10(uint16_t* adr, uint16_t* com, uint16_t* combined, uint16_t* encoded, uint32_t* datapakke, volatile int* counterPtr)
{

	combineBitArrays(adr, com, combined);

	hammingEncoding(combined, encoded);

	doubleEncoded(encoded, datapakke);

	*counterPtr = 1;
}

void recievex10(uint16_t* adr, uint16_t* com, uint16_t* combined, uint16_t* encoded, uint32_t* datapakke, volatile int* counterPtr)
{
		
		if (*counterPtr == dataSIZE+2)
		{
			halfEncoded(encoded, datapakke);
			
			hammingDecoding(encoded, combined);
			
			splitCombine(adr, com, combined);
			
			*encoded = 0;
			*combined = 0;
			*datapakke = 0;
			*counterPtr = 1;
		}
				
		
}

/*
uint8_t recievex10()
{

	return uint8_t;
}*/

void initISR()
{
	EICRA = 0b00000001; // any edge
	EIMSK = 0b00000001; // enable interrupt int 0
	sei(); // Global ISR enabler
}

void initPort()
{
	DDRC = 0x00;
	PORTC = 0x00;
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRA = 0xFF;
}

void combineBitArrays(uint16_t* adr, uint16_t* com, uint16_t* combined) //Combines the address and command into an single int
{


	*combined = ((*adr << 8) | *com);

}


void splitCombine(uint16_t* adr, uint16_t* com, uint16_t* combined) //Splits a combined int back into the address and command
{

	*com = (*combined & 0b0000000011111111);

	*adr = (*combined >> 8);
	
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
			p1 ^= 0b00000001;
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
			p2 ^= 0b00000001;
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
			p3 ^= 0b00000001;
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
			p4 ^= 0b00000001;
		}
		checkP4 >>= 1;
	}

	*encoded = ((*str << 4)) | (p1 << 3) | (p2 << 2) | (p3 << 1) | (p4);
}


void hammingDecoding(uint16_t* str, uint16_t* decoded) // (15,11) Hamming decoding
{
	*decoded = 0;
	*decoded |= (*str >> 4);

	//Parity bit 1
	uint16_t checkP1 = (0b11011010101 & *decoded);
	//Parity bit 2
	uint16_t checkP2 = (0b10110110011 & *decoded);
	//Parity bit 3
	uint16_t checkP3 = (0b01110001111 & *decoded);
	//Parity bit 4
	uint16_t checkP4 = (0b00001111111 & *decoded);

	//The checkSums, initialized with their parity value
	uint16_t checkSum1 = ((*str & 0b000000000001000) >> 3);
	uint16_t checkSum2 = ((*str & 0b000000000000100) >> 2);
	uint16_t checkSum3 = ((*str & 0b000000000000010) >> 1);
	uint16_t checkSum4 = ((*str & 0b000000000000001) >> 0);



	while (checkP1)
	{
		if (checkP1 & 1)
		{
			checkSum1^= 0b00000001;
		}
		checkP1 >>= 1;
	}
	
	while (checkP2)
	{
		if (checkP2 & 1)
		{
			checkSum2^= 0b00000001;
		}
		checkP2 >>= 1;
	}
	
	while (checkP3)
	{
		if (checkP3 & 1)
		{
			checkSum3^= 0b00000001;
		}
		checkP3 >>= 1;
	}
	
	while (checkP4)
	{
		if (checkP4 & 1)
		{
			checkSum4^= 0b00000001;
		}
		checkP4 >>= 1;
	}

	//errors found
	uint16_t error = 0;


	//Check for each bit in message, if an error, add to error array
	if (checkSum1 && checkSum2 && !checkSum3 && !checkSum4)	//m1
	{
		error |= 1 << 0;
	}
	if (checkSum1 && !checkSum2 && checkSum3 && !checkSum4)	//m2
	{
		error |= 1 << 1;
	}
	if (!checkSum1 && checkSum2 && checkSum3 && !checkSum4)	//m3
	{
		error |= 1 << 2;
	}
	if (checkSum1 && checkSum2 && checkSum3 && !checkSum4)	//m4
	{
		error |= 1 << 3;
	}
	if (checkSum1 && !checkSum2 && !checkSum3 && checkSum4)	//m5
	{
		error |= 1 << 4;
	}
	if (!checkSum1 && checkSum2 && !checkSum3 && checkSum4)	//m6
	{
		error |= 1 << 5;
	}
	if (checkSum1 && checkSum2 && !checkSum3 && checkSum4)	//m7
	{
		error |= 1 << 6;
	}
	if (!checkSum1 && !checkSum2 && checkSum3 && checkSum4)	//m8
	{
		error |= 1 << 7;
	}
	if (checkSum1 && !checkSum2 && checkSum3 && checkSum4)	//m9
	{
		error |= 1 << 8;
	}
	if (!checkSum1 && checkSum2 && checkSum3 && checkSum4)	//m10
	{
		error |= 1 << 9;
	}
	if (checkSum1 && checkSum2 && checkSum3 && checkSum4)	//m11
	{
		error |= 1 << 10;
	}

	//Add the error array to the decoded message, flipping any found error.
	*decoded ^= error;

}


void doubleEncoded(uint16_t* encoded, uint32_t* datapakke)
{

	for (int i = 0; i < 15; i++)
	{
		if (*encoded & (0b0100000000000000 >> i))
			*datapakke = (*datapakke | (0b10000000000000000000000000000000 >> (i * 2)));

		else *datapakke = (*datapakke | (0b01000000000000000000000000000000 >> (i * 2)));
	}
}


void halfEncoded(uint16_t* encoded, uint32_t* datapakke)
{

	for (int i = 0; i < 15; i++)
	{
		if(*datapakke & (0b10000000000000000000000000000000 >> (i * 2)))
			*encoded |=  1 << (14-i);
			
		else *encoded &= ~(1 << (14-i));
	}
}