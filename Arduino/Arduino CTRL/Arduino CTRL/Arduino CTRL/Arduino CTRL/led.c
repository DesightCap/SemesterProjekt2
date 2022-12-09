/*************************************************
* "LED.C"                                        *
* Implementation file for "Mega2560 LED driver"  *
* Henning Hargaard, 22/9 2015                    *
**************************************************/
#include <avr/io.h>
#define MAX_LED_NR 7

void initLEDport()
{
	// Implementering OK
	// S�t port til output. 
	DDRB = 0xFF;
	// Sluk alle LED
	PORTB = 0x00;
}

void writeAllLEDs(unsigned char pattern)
{
	// Implementering OK
	// Parameteren pattern skal v�re formateret som 0b01010101
	// Hent parameteren og skriv til lysdioderne
	PORTB = pattern;   
}

void turnOnLED(unsigned char led_nr)
{
	// Implementering OK
	// Lokal variabel
	unsigned char mask;
	// Vi skal kun lave noget, hvis led_nr < 8
	if (led_nr <= MAX_LED_NR)
	{
		// Dan maske p� basis af parameteren (led_nr)
		mask = 0b00000001 << led_nr;
		// T�nd den aktuelle lysdiode (de andre �ndres ikke)
		PORTB = PINB | mask;
	}
}

void turnOffLED(unsigned char led_nr)
{
	//Implementering OK
	// Vi skal kun lave noget, hvis led_nr < 8
	if (led_nr <= MAX_LED_NR)
	{
		PORTB &= ~(1 << led_nr);
	}	
}

void toggleLED(unsigned char led_nr)
{
	//Implementering OK
	// L�s nuv�rende LED, brug EX-OR operator til at vende efterspurgt bit  
	unsigned char mask;
	// Vi skal kun lave noget, hvis led_nr < 8
	if (led_nr <= MAX_LED_NR)
	{
		// Dan maske p� basis af parameteren (led_nr)
		mask = 0b00000001 << led_nr;
		// T�nd den aktuelle lysdiode (de andre �ndres ikke)
		PORTB = PINB ^ mask;
		}

}