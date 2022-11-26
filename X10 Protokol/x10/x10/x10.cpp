#include "x10.h"
using namespace std;

x10::x10()
{
}

void x10::sendx10(string command, uint32_t* datapakkePtr, volatile int* counterPtr) const
{

	//simplificeret forslag med on/off. Kunne være switch-case med heatOn, heatOff, blowOn, blowOff, tempSomething osv.
	if (command == "on")
		*datapakkePtr = 0b00000000000011110000111100001111;

	else if (command == "off")
		*datapakkePtr = 0b00000000000011110000111100000000;

	*counterPtr = 1;
}

std::string x10::recievex10() const
{
	return std::string();
}

void x10::initISR() const
{
	EICRA = 0b00000001; // any edge
	EIMSK = 0b00000001; // enable interrupt int 0
	sei(); // Global ISR enabler
}

