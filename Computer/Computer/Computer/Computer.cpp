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
	// Jeg har været nødt til at sætte en sleep ind ved UIinput kald, det ødelægger lidt ideen med at omgå at blokere for kørslen.
	// Vi er evt. nødt til at mixe med multithreading? 

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
			// Hvordan håndtere vi her? 
			testLog_->print();
			Sleep(100);
			break;
		case 2:
			Sleep(100);
		{
			// Jeg tror her at vi skal refere til en seperat funktion
			// På den funktion bliver vi evt. nødt til at sætte multithreading op
			cout << " Skriv min temp: ";
			int tempIntMin = UIinput();
			Sleep(1000);
			cout << endl << "Skriv max temp: ";
			Sleep(1000);
			int tempIntMax = UIinput();
			testTemp_->setTempInt(tempIntMin, tempIntMax);
		}
		break;
		case 3:
			cout << "Koersels loop afsluttes. "; 
			runningTempReg = false;
			break;
		case 4:
		case 5:
			Sleep(65);
			cout << "invalid input ";
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
// Overvej at ændre, så metoden kaldes med antallet af tal vi kigger efter
// det burde bare være at ændre til noget i stil med (int i = 49; i < (49 + maxValue); i++)
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
	// Overvej at implementere sleep i UIinput(), i stedet for hvor den bliver brugt
}