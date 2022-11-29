#include "x10.h"
using namespace std;
#include <stdio.h>
#include <stdlib.h>

x10::x10()
{
}

void x10::sendx10(char command, uint8_t* datapakkePtr, volatile int* counterPtr) const
{

	//simplificeret forslag med on/off. Kunne være switch-case med heatOn, heatOff, blowOn, blowOff, tempSomething osv.
	if (command == y)
		*datapakkePtr = 0b11111111;

	else if (command == z)
		*datapakkePtr = 0b00000000;

	*counterPtr = 1;
}

char x10::recievex10() const
{
	return char();
}

void x10::initISR() const
{
	EICRA = 0b00000001; // any edge
	EIMSK = 0b00000001; // enable interrupt int 0
	sei(); // Global ISR enabler
}

