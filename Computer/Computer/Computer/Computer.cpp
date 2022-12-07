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
	startStop_ = true;
}


void Computer::readToggle()
{
	startStop_ = !startStop_;
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
			setTempInt();
			this->menuPrint(false);
			cout << "Temperatur interval indstillet." << endl;
		}
		break;
		case 3:
			cout << endl << endl << "Koersels loop afsluttes. ";
			runningTempReg = false;
			break;
		case 4:
			readToggle();
			if (!startStop_)
			{
				cout << "stopper temperatur regulering" << endl;
			}
			else if (startStop_)
			{
				cout << "starter temperatur regulering" << endl;
			}
			break;
		case 5:
			//
			cout << "invalid input ";
			break;
		default:

			char recieve[] = "00.0";
			char toSend;
			while (startStop_ == true)
			{
				testUART_->getTemp(recieve, (sizeof(recieve) / sizeof(recieve[0])) - 1);
				if (recieve[0] != '0' || recieve[1] != '0' || recieve[2] != '\0')
				{

					double recievedDouble = this->tempCharArrayToDouble(recieve);

					testLog_->addToLog(recievedDouble);
					cout << "Skrevet til log"
						<< " - Double: " << recievedDouble
						<< " Char: " << recieve[0] << recieve[1] << recieve[2] << recieve[3]
						<< endl; // test udskriv for skrivning til log
					switch (this->testTemp_->checkTemp(recievedDouble))
					{
					case -1:
						if (startStop_ == true)
							testUART_->sendNed();
						cout << "Saenk temperatur" << endl;
						break;
					case 0:
						if (startStop_ == true)
							cout << "Temp indenfor interval" << endl;
						break;
					case 1:
						if (startStop_ == true)
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
					if (startStop_)
					{
						testUART_->getTemp(recieve, sizeof(recieve) / sizeof(recieve[0]));
					}
					if (recieve[0] != '0' || recieve[1] != '0' || recieve[2] != '\0')
					{
						cout << recieve << endl;
					}
				}
				break;
			}

		}
			Sleep(ARDUINO_WAIT_TIME);
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
		<< "3: Stop korsel af loop" << endl
		<< "4: start/stop temperatur maaling" << endl << endl;

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
	double doubleRecieve;
	stringstream s2d;
	s2d << recieved;
	s2d >> doubleRecieve;

	return doubleRecieve;
}

bool Computer::setTempInt()
{
	SletKonsolInputs();

	system("CLS");
	cout << "Temperatur interval minimum nu: " << testTemp_->getMin() << endl
		<< "Temperatur interval maximum nu: " << testTemp_->getMax() << endl;
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
		testTemp_->setMin(minTemp);
		testTemp_->setMax(maxTemp);
		
		return true;
	}
	else
		cout << "proev igen" << endl;
}

void Computer::SletKonsolInputs()
{
	PINPUT_RECORD gammeltKonsolInput = new INPUT_RECORD[100];
	DWORD nyKonsolInput;
	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), gammeltKonsolInput, 100, &nyKonsolInput);
	delete[] gammeltKonsolInput;
}