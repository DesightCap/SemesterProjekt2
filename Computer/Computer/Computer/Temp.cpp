#include "Temp.h"
#include <Windows.h>

Temp::Temp(int min, int max)
{
	TempMin_ = min;
	TempMax_ = max;
}
void Temp::SletKonsolInputs() // REN COPY PASTE FUNKTION FRA GOOGLE https://stackoverflow.com/questions/20222277/delete-key-press-historygetkeystate-c
{
	PINPUT_RECORD gammeltKonsolInput = new INPUT_RECORD[100];
	DWORD nyKonsolInput;
	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), gammeltKonsolInput, 100, &nyKonsolInput);
	delete[] gammeltKonsolInput;
}
bool Temp::setTempInt()
{
	SletKonsolInputs(); // REN COPY PASTE FRA NOGET FRA GOOGLE
	while (1)
	{
		int minTemp, maxTemp;
		cout << "indtast minimum temperaturen: \n";
		cin >> minTemp;
		cout << "indtast max temperaturen: \n";
		cin >> maxTemp;

		if (minTemp <= maxTemp && 5 <= minTemp && maxTemp <= 30) // lavet min/max validering
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
