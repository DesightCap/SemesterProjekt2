#include "Temp.h"
#include <Windows.h>

Temp::Temp(int min, int max)
{
	TempMin_ = min;
	TempMax_ = max;
}
void Temp::SletKonsolInputs()
{
	PINPUT_RECORD gammeltKonsolInput = new INPUT_RECORD[100];
	DWORD nyKonsolInput;
	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), gammeltKonsolInput, 100, &nyKonsolInput);
	delete[] gammeltKonsolInput;
}
bool Temp::setTempInt()
{
	SletKonsolInputs(); 
	while (1)
	{
		int minTemp, maxTemp;
		cout << "indtast minimum temperaturen: \n";
		cin >> minTemp;
		cout << "indtast max temperaturen: \n";
		cin >> maxTemp;

		if (minTemp <= maxTemp && 5 <= minTemp && maxTemp <= 30)
		{
			TempMin_ = minTemp;
			TempMax_ = maxTemp;
			return true;
		}
		else
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
