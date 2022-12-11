//#include <inttypes.h> // Test om skal bruges
#include <avr/io.h>
#include <stdio.h>
//#include <math.h> // Test om skal bruges
#include "uart1.h"
#include <avr/interrupt.h>
#include "x10.h"
#include <stdlib.h>
#define F_CPU 16000000
#include <util/delay.h>
#define dataSIZE 30


//char recieved[5] = "00.0";
uint16_t cleanedData;

// X10 variabler
volatile int counter = 100; //sender counter
volatile int count = 1; // modtager counter
uint16_t myAddress = 0b0000000000000100; // Min adresse
uint16_t com;
uint16_t address = 0b0000000000000001;
uint16_t combined = 0;
uint16_t encoded = 0;
uint8_t start = 0b00001110;
uint8_t startRecieved = 0;
uint32_t datapakkeRecieved = 0x0000;
uint16_t addressRecieved = 0;
uint16_t commandRecieved = 0x00;
uint32_t datapakke = 0;
char charRecieved;
uint16_t requestTemperature = 0b0000000011111111;

ISR(USART0_RX_vect)
{
	
	uint8_t recievedData = UDR0;
	cleanedData = recievedData;
	cleanedData &= 0b0000000001111111;
	PORTB = cleanedData;
	
	// Udskrivning over ATMEL -> Data Visualizer -> Terminal
	/*
	char bufferWhole[5];
	char bufferDec[5];
	
	uint8_t number = (recievedData >> 1);
	uint8_t decNumber = (recievedData << 7);
	sprintf(bufferWhole, "%d", number);
	if(decNumber)
	sprintf(bufferDec, ".5");
	
	PORTB = recievedData;
	SendString(bufferWhole);
	SendString(bufferDec);
	SendString("\n");
	//*/
	
}

ISR(USART0_TX_vect)
{

}

ISR(INT0_vect)
{
	//////////////////////SENDER ISR!///////////////
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
	}
	
	counter++;
	
	
	/////////////////////MODTAGER ISR!///////////////
	

	//_delay_ms(20);
	uint8_t inputPin = (PINC & 0x1);
	uint32_t longInputPin = 0x0000;
	longInputPin|= inputPin;
	
	if(startRecieved != 0b00001110)
	{
		startRecieved |= inputPin;
		startRecieved = startRecieved << 1;
	}
	else if (startRecieved == 0b00001110)

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
			
			count++;
		}
	}
}

int main(){

	DDRE = 0x00;
	DDRB =0xFF;


	InitUART(9600,8,1);
	sei();
	
	initPort();
	
	while (1)
	{
		recievex10(&addressRecieved,&commandRecieved,&combined,&encoded,&datapakkeRecieved,&count);
		
		if (addressRecieved == myAddress)
		{
			if (commandRecieved == requestTemperature)
				sendx10(&address,&cleanedData,&combined,&encoded,&datapakke,&counter);
			_delay_ms(2000);
		}
	}
}