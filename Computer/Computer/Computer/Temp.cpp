#include "Temp.h"
#include <Windows.h>

Temp::Temp(int min, int max)
{
	TempMin_ = min;
	TempMax_ = max;
}

int Temp::getMin()
{
	return TempMin_;
}

int Temp::getMax()
{
	return TempMax_;
}

void Temp::setMin(int min)
{
	TempMin_ = min;
}

void Temp::setMax(int max)
{
	TempMax_ = max;
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
/*		int minTemp, maxTemp;
		cout << "indtast minimum temperaturen: \n";
		cin >> minTemp;

		switch (minTemp)
		{
		case (0 <= 63):
			break;

		case (!(0 <= 63)):
			cout << "proev igen" << endl;
		default:
			break;
		}


		cin >> minTemp;
		cout << "indtast max temperaturen: \n";
		cin >> maxTemp;
		if (!(maxTemp))
		{

		}
		if (minTemp <= maxTemp && 0 <= minTemp && maxTemp <= 63)
		{

			TempMin_ = minTemp;
			TempMax_ = maxTemp;
			return true;
		}
		else
			cout << "proev igen" << endl;
	}
}*/
/*
for (int i = 0; i <= maxTemp[2]; i++)
{
	if (isdigit(maxTemp[i] + 48) == false)
		cout << "du skal skrive et tal" << endl;
}

for (int i = 0; i <= minTemp[2]; i++)
{
	if (isdigit(minTemp[i] + 48) == false)
		cout << "du skal skrive et tal" << endl;
}*/