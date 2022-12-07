#include <avr/io.h>
#include "util/delay.h"

#define F_CPU 16000000UL
#define  pinHot 0b00000100
#define  pinCold 0b00001000

// Skal vi bruge delay/cpu freq?

int main(void)
{
	/* Replace with your application code */
	
	// opsæt port/pin
	DDRA = 0xFF;
	// 1 pin til cold - PINA & 0b00000100
	// 1 pin til hot  - PINA & 0b00001000
	
	char recieved = '0';
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
	
	while (1)
	{
	}
}

/*

while (1)
{
if ((PINA & 0b10000000) == 0)
_delay_us(956);
else if ((PINA & 0b01000000) == 0)
_delay_us(851);
else if ((PINA & 0b00100000) == 0)
_delay_us(758);
else if ((PINA & 0b00010000) == 0)
_delay_us(716);
else if ((PINA & 0b00001000) == 0)
_delay_us(638);
else if ((PINA & 0b00000100) == 0)
_delay_us(568);
else if ((PINA & 0b00000010) == 0)
_delay_us(506);
else if ((PINA & 0b00000001) == 0)
_delay_us(478);

if (PINA != 0xFF)
PORTB = ~PINB;
}

*/