#include "Heat.h"
#include <stdlib.h>
#include <iostream>

void HeatControl::TempOp(double currentTemp)
{
	if (currentTemp < TempMin_)
	{
		std::cout << "Begin warming system..." << std::endl;
	}
}

void HeatControl::SetMinTemp(double tempMin)
{
	TempMin_ = tempMin;
}

HeatControl::HeatControl(double SetTempMin)
{
	SetMinTemp(SetTempMin);
}

HeatControl::~HeatControl()
{
}

double HeatControl::getTempMin()
{
	return TempMin_;
}
