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

	system("CLS");
	int minTemp = -1, maxTemp = -1;

	while (minTemp < 0 || maxTemp < 0)
	{

		try {
			cout << "indtast minimum temperaturen (mindst 0): \n";
			cin >> minTemp;
			if (cin.fail())
			{
				minTemp = -1;
				throw 1;
			}

			cout << "indtast max temperaturen (hoejest 63): \n";
			cin >> maxTemp;
			if (cin.fail())
			{
				maxTemp = -1;
				throw 2;
			}
			else if (maxTemp > 63)
			{
				maxTemp = -1;
				throw 3;
			}
			else if (maxTemp < minTemp)
			{
				maxTemp = -1;
				minTemp = -1;
				throw 4;
			}
		}
		catch (int x) {
			if (x == 1)
			{
				system("CLS");
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				cerr << "Minimum ikke valid vaerdi" << endl << endl;
			}
			else if (x == 2)
			{
				system("CLS");
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				cerr << "Maximum ikke valid vaerdi" << endl << endl;
			}
			else if (x == 3)
			{
				system("CLS");
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				cerr << "Maximum maa ikke vaere over 63" << endl << endl;
			}
			else if (x == 4)
			{
				system("CLS");
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				cerr << "Maximum maa ikke vaere mindre end minimum" << endl << endl;
			}
		};
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