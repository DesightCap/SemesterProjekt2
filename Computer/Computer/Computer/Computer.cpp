#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
//#include <stdlib.h>

#include "Computer.h"


Computer::Computer(UART* testUART, Log* testLog, Temperature* testTemperature)
{
	temperature_ = 0;
	testUART_ = testUART;
	testLog_ = testLog;
	temperatureObject_ = testTemperature;
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

	bool runningTemperatureReg = true;

	this->menuPrint();
	while (runningTemperatureReg)
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
			setTemperatureInt();
			this->menuPrint(false);
			cout << "Temperatur interval indstillet." << endl;
		}
		break;
		case 3:
			cout << endl << endl << "Koersels loop afsluttes. ";
			runningTemperatureReg = false;
			break;
		case 4:
			readToggle();
			if (!startStop_)
			{
				cout << "Stopper temperatur regulering" << endl;
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
				testUART_->getTemperature(recieve, (sizeof(recieve) / sizeof(recieve[0])) - 1);
				if (recieve[0] != '0' || recieve[1] != '0' || recieve[2] != '\0')
				{
					
					double recievedDouble = this->temperatureCharArrayToDouble(recieve);

					testLog_->addToLog(recievedDouble);
					cout << "Skrevet til log"
						<< " - Double: " << recievedDouble
						<< " Char: " << recieve[0] << recieve[1] << recieve[2] << recieve[3]
						<< endl; // test udskriv for skrivning til log
					switch (this->temperatureObject_->checkTemperature(recievedDouble))
					{
					case -1:
						if (startStop_ == true)
							testUART_->sendNed();
						cout << "Saenk temperatur" << endl;
						break;
					case 0:
						if (startStop_ == true)
							cout << "Temperature indenfor interval" << endl;
						break;
					case 1:
						if (startStop_ == true)
							testUART_->sendOp();
						cout << "Haev Temperature" << endl;
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
						testUART_->getTemperature(recieve, sizeof(recieve) / sizeof(recieve[0]));
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
		testUART_->getTemperature(inputData_, INPUT_DATA_BYTES);
	}

	string inputValStr(inputData_);
	return inputValStr;
}

double Computer::temperatureCharArrayToDouble(char recieved[])
{
	double doubleRecieve;
	stringstream s2d;
	s2d << recieved;
	s2d >> doubleRecieve;

	return doubleRecieve;
}

bool Computer::setTemperatureInt()
{
	clearInputBuffer();

	system("CLS");
	cout << "Temperatur interval minimum nu: " << temperatureObject_->getMin() << endl
		<< "Temperatur interval maximum nu: " << temperatureObject_->getMax() << endl;
	int minTemperature = -1, maxTemperature = -1;

	while (minTemperature < 0 || maxTemperature < 0)
	{

		try {
			cout << "indtast minimum temperaturen (mindst 0): \n";
			cin >> minTemperature;
			if (cin.fail())
			{
				minTemperature = -1;
				throw 1;
			}

			cout << "indtast max temperaturen (hoejest 63): \n";
			cin >> maxTemperature;
			if (cin.fail())
			{
				maxTemperature = -1;
				throw 2;
			}
			else if (maxTemperature > 63)
			{
				maxTemperature = -1;
				throw 3;
			}
			else if (maxTemperature < minTemperature)
			{
				maxTemperature = -1;
				minTemperature = -1;
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

	if (minTemperature <= maxTemperature && 0 <= minTemperature && maxTemperature <= 63)
	{
		temperatureObject_->setMin(minTemperature);
		temperatureObject_->setMax(maxTemperature);
		
		return true;
	}
	else
		cout << "proev igen" << endl;
}

void Computer::clearInputBuffer()
{
	PINPUT_RECORD gammeltKonsolInput = new INPUT_RECORD[100];
	DWORD nyKonsolInput;
	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), gammeltKonsolInput, 100, &nyKonsolInput);
	delete[] gammeltKonsolInput;
}