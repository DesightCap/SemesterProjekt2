#include "Temp.h"


Temp::Temp(int min, int max)
{
	TempMin_ = min;
	TempMax_ = max;
}

bool Temp::setTempInt(int min, int max)
{
	// Undersøg hvad vi skal validere vedr. temperatur interval

	if (min <= max)
	{
		TempMin_ = min;
		TempMax_ = max;
		return true;
	}
	else
		return false;
}


int Temp::checkTemp(int temp)
{
	return 0;
}
