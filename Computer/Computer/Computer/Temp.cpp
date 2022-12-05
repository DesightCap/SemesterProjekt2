#include "Temp.h"


Temp::Temp(int min, int max)
{
	TempMin_ = min;
	TempMax_ = max;
}

bool Temp::setTempInt()
{
	// Undersøg hvad vi skal validere vedr. temperatur interval
	while (1)
	{
		int minTemp, maxTemp;
		cout << "indtast minimum temperaturen: \n" << endl;
		cin >> minTemp;
		cout << "indtast max temperaturen: \n " << endl;
		cin >> maxTemp;
		if (minTemp <= maxTemp)
		{
			TempMin_ = minTemp;
			TempMax_ = maxTemp;
			return true;
		}
		cout << "proev igen" << endl;
	}
}



int Temp::checkTemp(int temp)
{
	if (temp >= TempMin_ && temp <= TempMax_)
	{
		return 0;
	}
	else if (temp < TempMin_)
	{
		return -1;
	}
	else if (temp > TempMax_)
	{
		return 1;
	}
}
