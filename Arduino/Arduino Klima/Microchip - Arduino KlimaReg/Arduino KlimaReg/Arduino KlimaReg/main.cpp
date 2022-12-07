#include <avr/io.h>
#include "util/delay.h"

#define F_CPU 16000000UL
#define  pinHot 0b00000100
#define  pinCold 0b00001000

// Skal vi bruge delay/cpu freq?

uint16_t adressRecieved;
uint16_t commandRecieved;

int main(void)
{
	/* Replace with your application code */
	
	// opsæt port/pin
	DDRA = 0xFF;
	// 1 pin til cold - PINA & 0b00000100
	// 1 pin til hot  - PINA & 0b00001000
	int blowTime = 0;
	int hotTime = 0;
	
	int timeMultiplier = 1; // Skal være over 0;
	
	char recieved = '0';
	while (1)
	{
		// Modtag x10
		
		switch (commandRecieved)
		{
			case 0b01110101:
			blowTime = 16000000 * timeMultiplier;
			break;
			case 0b01100100:
			hotTime = 16000000 * timeMultiplier;
			break;
			default:
			break;
			if (blowTime)
			{
				PORTA ^= pinCold;
				blowTime--;
			}
			else
			{
				PORTA &= ~pinCold;
			}
			if (hotTime)
			{
				PORTA ^= pinHot;
				hotTime--;
			}
			else
			{
				PORTA &= !pinHot;
			}
		}
		
	}
	
	while (1)
	{
	}
}

/*
while (1)
{

// implementer modtagelse af x10

// d for down	ASCII 'd' == dec 100 == bin 01100100
// u for up		ASCII 'u' == dec 117 == bin 01110101

for (int i = 16000000; i > 0; i--)
{

if (recieved == 'u')
{

// sæt hot pin høj
PORTA = PORTA ^ pinHot;
recieved = '0';

}
else if (recieved == 'd')
{
// sæt cold pin høj
PORTA = PORTA ^ pinCold;
recieved = '0';
}



}
PORTA = 0x00; // Omskriv til maske til slukning af bits
}

*/