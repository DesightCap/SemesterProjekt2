#include <time.h>
#include <stdlib.h>
#include <stdio.h>


#define STARTSIZE 4

int startBits[STARTSIZE] = { 1, 1, 1, 0 };

#define ADDRESSSIZE 3
#define COMMANDSIZE 8
#define COMBINEDSIZE 11
#define ENCODEDSIZE 15


void hammingEncoding(uint16_t*, uint16_t*);



void hammingEncoding(uint16_t* str, uint16_t* encoded) // (15,11) Hamming encoding
{

	//Parity bit 1
	uint16_t checkP1 = 0b11011010101;
	checkP1 &= *str;

	bool p1 = 0;

	while (checkP1) 
	{
		if (checkP2 & 1)
		{
			p2 = !p2;
		}
		checkP1 >>= 1;
	}

	//Parity bit 2
	uint16_t checkP2 = 0b10110110011;
	checkP2 &= *str;

	uint16_t p2 = 0;

	while (checkP2)
	{
		if (checkP2 & 1)
		{
			p2 = !p2;
		}
		checkP2 >>= 1;
	}

	//Parity bit 3
	uint16_t checkP3 = 0b01110001111;
	checkP3 &= *str;

	uint16_t p3 = 0;

	while (checkP3)
	{
		if (checkP3 & 1)
		{
			p3 = !p3;
		}
		checkP3 >>= 1;
	}

	//Parity bit 4
	uint16_t checkP4 = 0b00001111111;
	checkP4 &= *str;

	uint16_t p4 = 0;

	while (checkP4)
	{
		if (checkP4 & 1)
		{ 
			p4 = !p4; 
		}
		checkP4 >>= 1;
	}

}