#include <windows.h>
#include <iostream>

#include "Computer.h"

Computer::Computer(UART* testUART, Log* testLog, Temp* testTemp)
{
	temp_ = 0;
	testUART_ = testUART;
	testLog_ = testLog;
	testTemp_ = testTemp;
}

void Computer::TempReg()
{

	
}

// Metoder merget fra UI klasse


int Computer::tempInitiate()
{
	return 0;
}

void Computer::tempStop()
{
}

bool Computer::recieveTempInterval()
{

	return false;
}

void Computer::printLog()
{
}

void Computer::openMenu()
{
	bool runningTempReg = true;

	cout
		<< "1: Udskriv log" << endl
		<< "2: Saet temperatur interval" << endl
		<< "3: Stop korsel af loop" << endl << endl;

	while (runningTempReg)
	{

		int userSelect = 0;
		switch (UIinput())
		{
		case 1:

			testLog_->print();
			break;
		case 2:
		{
			cout << " Skriv min temp: ";
			int tempIntMin = UIinput();
			cout << endl << "Skriv max temp: ";
			int tempIntMax = UIinput();
			testTemp_->setTempInt(tempIntMin, tempIntMax);
		}
		break;
		case 3:
			runningTempReg = false;
			break;
		case 4:

			break;
		default:

			testUART_->getTemp();

		}
		{
			int tempValidate = testTemp_->checkTemp(temp_);
			if (tempValidate < 0)
			{
				testUART_->sendNed();
			}
			else if (tempValidate < 0)
			{
				testUART_->sendOp();
			}
		}
	}
}

int Computer::UIinput()
{
	for (int i = 49; i < 54; i++)
	{
		if (GetAsyncKeyState(i))
		{
			for (int j = i; j < 54; j++)
			{
				GetAsyncKeyState(j);
			}
			return i - 48;
		}
	}
	return 0;
}