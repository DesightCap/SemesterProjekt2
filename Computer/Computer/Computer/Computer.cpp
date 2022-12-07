#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
//#include <stdlib.h>

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
	// Diskuter


}

// Metoder merget fra UI klasse


int Computer::tempInitiate()
{
	// Diskuter
	return 0;
}

void Computer::tempStop()
{
	// Diskuter
}

bool Computer::recieveTempInterval()
{
	// Mangler
	return false;
}


void Computer::openMenu()
{
	// Jeg har været nødt til at sætte en sleep ind ved UIinput kald, det ødelægger lidt ideen med at omgå at blokere for kørslen.
	// Vi er evt. nødt til at mixe med multithreading? 
	// Problem burde være løst - prøv at fjerne diverse Sleep() funktioner og se om UI stadig virker

	bool runningTempReg = true;

	this->menuPrint();

	while (runningTempReg)
	{		
		int userSelect = 0;
		switch (UIinput())
		{
		case 1:
			// Hvordan håndtere vi her? 
			system("CLS");
			testLog_->print();
			this->menuPrint(false); // udskriver menu uden rydning af consol
			break;
		case 2:
			{
				testTemp_->setTempInt();
				this->menuPrint();
				cout << "Temperatur interval indstillet." << endl;
			}
			break;
		case 3:
			cout << endl << endl << "Koersels loop afsluttes. ";
			runningTempReg = false;
			break;
		case 4:
		case 5:
			//
			cout << "invalid input ";
			break;
		default:

			char recieve[] = "00,0";
			char toSend;
			//testUART_->getTemp(recieve, 4);
			testUART_->getTemp(recieve, (sizeof(recieve) / sizeof(recieve[0]))-1);
			if (recieve[0] != '0' || recieve[1] != '0' || recieve[2] != '\0')
			{

				double recievedInt = this->tempCharArrayToDouble(recieve);

				testLog_->addToLog(recievedInt);
				cout << "Skrevet til log" 
					<< " - Double: " << recievedInt 
					<< " Char: " << recieve[0] << recieve[1] << recieve[2] << recieve[3] 
					<< endl; // test udskriv for skrivning til log
				switch (this->testTemp_->checkTemp(recievedInt))
				{
				case -1:
					testUART_->sendNed();
					cout << "Saenk temperatur" << endl;
					break;
				case 0:
					cout << "Temp indenfor interval" << endl;
					break;
				case 1:
					testUART_->sendOp();
					cout << "Haev temp" << endl;
					break;
				default:
					break;
				}
				
			}
			else if (recieve[0] == '0' && recieve[1] == '0' && recieve[2] == '\0')
			{
				Sleep(50);
				testUART_->getTemp(recieve, sizeof(recieve) / sizeof(recieve[0]));
				if (recieve[0] != '0' || recieve[1] != '0' || recieve[2] != '\0')
				{
					cout << recieve << endl;
				}
			}
			
			Sleep(ARDUINO_WAIT_TIME);
			break;
		}
		/*
		Sleep(65);

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
		}*/
	}
}

void Computer::menuPrint(bool clear)
{
	if (clear)
		system("CLS");
	for (int j = 48; j < 54; j++)
	{
		GetAsyncKeyState(j);
	}
	cout
		<< "1: Udskriv log" << endl
		<< "2: Saet temperatur interval" << endl
		<< "3: Stop korsel af loop" << endl << endl;

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

string Computer::dataHandler()
{
	Sleep(ARDUINO_WAIT_TIME);
	if (testUART_->isConnected())
	{
		for (int i = 0; i < INPUT_DATA_BYTES; i++)
		{
			inputData_[i] = 0;
		}
		testUART_->getTemp(inputData_, INPUT_DATA_BYTES);
	}

	string inputValStr(inputData_);
	return inputValStr;
}

double Computer::tempCharArrayToDouble(char recieved[])
{
	//double intRecieve;
	double doubleRecieve;
	//stringstream str(recieved);
	stringstream s2d;
	s2d << recieved;
	s2d >> doubleRecieve;

	//str >> intRecieve;
	return doubleRecieve;
}