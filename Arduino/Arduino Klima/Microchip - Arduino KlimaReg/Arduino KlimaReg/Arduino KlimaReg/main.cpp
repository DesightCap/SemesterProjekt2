#include <avr/io.h>

#define  pinHot 0b00000100
#define  pinCold 0b00001000

// Skal vi bruge delay/cpu freq?

uint16_t adressRecieved;
uint16_t commandRecieved;

 void StartBlow(int hotTime_);
 void StartHeat(int blowTime_);


int main(void)
{
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
						StartHeat(hotTime);
						StartBlow(blowTime);
						break;
			}
	}
	while (1)
	{
	}
}
void StartBlow(int blowTime_)
{
	if (blowTime_)
	{
		PORTA ^= pinCold;
		blowTime_--;
	}
	else
	{
		PORTA &= ~pinCold;
	}
}
void StartHeat(int hotTime_)
{
	if (hotTime_)
	{
		PORTA ^= pinHot;
		hotTime_--;
	}
	else
	{
		PORTA &= !pinHot;
	}
}
