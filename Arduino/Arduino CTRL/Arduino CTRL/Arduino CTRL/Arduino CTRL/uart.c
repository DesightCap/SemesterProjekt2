/************************************************
* "uart.c":                                     *
* Implementation file for Mega2560 UART driver. *
* Using UART 0.                                 *
* Henning Hargaard, 5/4 2019                    *
*************************************************/
#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"

// Target CPU frequency
#define XTAL 16000000

/*************************************************************************
UART 0 initialization:
    Asynchronous mode.			x
    Baud rate = 9600.			x
    Data bits = 8.				x
    RX and TX enabled.			x
	No interrupts enabled.		x
    Number of Stop Bits = 1.	x
    No Parity.					x
    
*************************************************************************/
void InitUART()
{
	// Opdater 9600 til variable BAUD rate
	// Overvej runding 
	UBRR0 = XTAL/(16*9600)-1;
	UCSR0B &= 0b11111011; // Maskering for at slukke bit 3
	UCSR0B |= 0b00011000; // Maskering for at tænde bit 4 og 5
	UCSR0C |= 0b00000110; // Maskering for at tænde bit 2 og 3
	UCSR0C &= 0b00000111; // Maskering for at slukke bit 8 til og med 4
}

/*************************************************************************
  Returns 0 (FALSE), if the UART has NOT received a new character.
  Returns value <> 0 (TRUE), if the UART HAS received a new character.
*************************************************************************/
unsigned char CharReady()
{
	// Tjek om der er noget på UART 0 
	// Hvis noget modtaget (UART0 != 0) return ikke 0
	// Hvis intet mogtaget (UART0 == 0) return 0 

}

/*************************************************************************
Awaits new character received.
Then this character is returned.
*************************************************************************/
char ReadChar()
{
	// Afvent karakter modtaget på UART0
	// mens CharReady == 0 vent
	// 
/*
	while ((UCSR0A & 0b00100000) != 0)
	{ }
*/
}

/*************************************************************************
Awaits transmitter register ready.
Then send the character.
Parameter :
	Tegn : Character for sending. 
*************************************************************************/
void SendChar(char Tegn)
{
   // <---- Skriv din kode her
}

/*************************************************************************
Sends 0 terminated string.
Parameter:
   Streng: Pointer to the string. 
*************************************************************************/
void SendString(char* Streng)
{
   // <---- Skriv din kode her
}

/*************************************************************************
Converts the integer "Tal" to an ASCII string - and then sends this string
using the USART.
Makes use of the C standard library <stdlib.h>.
Parameter:
    Tal: The integer to be converted and sent. 
*************************************************************************/
void SendInteger(int Tal)
{
   // <---- Skriv din kode her
}

/************************************************************************/