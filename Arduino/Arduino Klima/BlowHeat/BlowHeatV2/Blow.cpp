#include "Blow.h"
#include <stdlib.h>
#include <iostream>

BlowControl::BlowControl(double SetTempMax)
{
	SetMinTemp(SetTempMax);
}

BlowControl::~BlowControl()
{
}

void BlowControl::SetMinTemp(double tempMax)
{
	TempMax_ = tempMax;
}


double BlowControl::getTempMax()
{
	return TempMax_;
}

void BlowControl::TempNed(double currentTemp)
{
	if (currentTemp > TempMax_)
	{
		std::cout << "begin cooling system..." << std::endl;
	}
}
