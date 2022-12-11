#include <inttypes.h>
#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include "uart1.h"
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>

char recieved[5] = "00.0";

ISR(USART0_RX_vect)
{
	
	
	char bufferWhole[5];
	char bufferDec[5];
	
	//unsigned char recievedData = UDR0;
	//unsigned char number = recievedData - '0';
	uint8_t recievedData = UDR0;
	uint8_t cleanedData = recievedData & 0b01111111;
	
	
	// Udskrivning over ATMEL -> Data Visualizer -> Terminal
	/*
	uint8_t number = (recievedData >> 1);
	uint8_t decNumber = (recievedData << 7);
	sprintf(bufferWhole, "%d", number);
	if(decNumber)
	sprintf(bufferDec, ".5");
	
	PORTB = recievedData;
	SendString(bufferWhole);
	SendString(bufferDec);
	SendString("\n");
	*/
	
}

ISR(USART0_TX_vect)
{

}


int main(){

	DDRE = 0x00;
	DDRB =0xFF;


	InitUART(9600,8,1);
	sei();

	//PINE0;
	
	_delay_ms(2000);
	while (1)
	{
	}
	return 0;
}