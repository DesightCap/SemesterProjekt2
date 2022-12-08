#include "Temperature.h"
#include <Windows.h>

Temperature::Temperature(int min, int max)
{
	TemperatureMin_ = min;
	TemperatureMax_ = max;
}

int Temperature::getMin()
{
	return TemperatureMin_;
}

int Temperature::getMax()
{
	return TemperatureMax_;
}

void Temperature::setMin(int min)
{
	TemperatureMin_ = min;
}

void Temperature::setMax(int max)
{
	TemperatureMax_ = max;
}


int Temperature::checkTemperature(int Temperature)
{
	if (Temperature >= TemperatureMin_ && Temperature <= TemperatureMax_)
	{
		return 0;
	}
	else if (Temperature < TemperatureMin_)
	{
		return -1;
	}
	else if (Temperature > TemperatureMax_)
	{
		return 1;
	}
}