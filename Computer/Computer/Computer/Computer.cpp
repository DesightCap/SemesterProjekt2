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
	writeTemperature_ = true;
}


void Computer::readToggle()
{
	writeTemperature_ = !writeTemperature_;
}

void Computer::openMenu()
{
	//char x = 'd';
	//testUART_->send(&x, 1);
	// Jeg har været nødt til at sætte en sleep ind ved UIinput kald, det ødelægger lidt ideen med at omgå at blokere for kørslen.
	// Vi er evt. nødt til at mixe med multithreading? 
	// Problem burde være løst - prøv at fjerne diverse Sleep() funktioner og se om UI stadig virker

	// char recieve[] = "00.0"; // tester der hører sammen med recieve[0]++ i loopet

	bool runningTemperatureReg = true;
	this->menuPrint();
	while (runningTemperatureReg)
	{
		Sleep(ARDUINO_WAIT_TIME);
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
			if (!writeTemperature_)
			{
				cout << "Stopper temperatur maaling" << endl;
			}
			else if (writeTemperature_)
			{
				cout << "starter temperatur maaling" << endl;
			}
			break;
		case 5:
		{
			clearInputBuffer();
			bool pwd[4]{ false };
			cout << "Indtast ny kode:" << endl;
			for (int i = 0; i < 4; i++)
			{
				cout << "Cifer " << i + 1 << ": ";
				int c;
				cin >> c;
				while (cin.fail())
				{

					cout << endl << "Fejl!" << endl << "Godtager 0 eller tal derover. ";
					cin.clear();
					cin.ignore(256, '\n');
					cin >> c;
				}
				if (c)
					pwd[i] = true;
			}
			system("CLS");
			this->menuPrint(false);
			cout << "Kode indstilles til: " << endl;
			for (int i = 0; i < 4; i++)
			{
				cout << pwd[i] << endl;
			}
			uint8_t pwdInt = 0;
			for (int i = 0; i < 4; i++)
			{
				if (pwd[i])
				{
					pwdInt = (pwdInt << 1) + 1;
				}
				else
				{
					pwdInt = (pwdInt << 1);

				}
			}
			testUART_->changePassword((char)pwdInt);
			cout << endl << endl;
			break;
		}
		default:

			char recieve[] = "00.0"; // Sat høj for test
			char toSend;

			while (writeTemperature_)
			{
				testUART_->getTemperature(recieve, (sizeof(recieve) / sizeof(recieve[0])) - 1);
				if (recieve[2] == '.' && recieve[1] != '0' && recieve[0] != '0')
				{
					double recievedDouble = this->temperatureCharArrayToDouble(recieve);

					testLog_->addToLog(recievedDouble);
					seeCurrentTemperature(recieve);
					//recieve[0]++; // tester der hører sammen med ovenstående char recieve[] = "00.0";

				//	cout << "Skrevet til log: " << recieve[0] << recieve[1] << recieve[2] << recieve[3] << endl; // tester

					switch (this->temperatureObject_->checkTemperature(recievedDouble))
					{
					case -1:
						if (writeTemperature_)
							testUART_->sendUp();
						//		cout << "Haev Temperatur" << endl; // tester
						break;
					case 0:
						if (writeTemperature_)
						{
							char x = 'x';
							testUART_->send(&x, 1);
						}
						//			cout << "Temperatur indenfor interval" << endl; // tester
						break;
					case 1:
						if (writeTemperature_)
							testUART_->sendDown();
						//		cout << "Saenk temperatur " << endl; // tester
						break;
					default:
						break;

					}
				}
				else
				{

					char x = 'x';
					testUART_->send(&x, 1);
				}
				break;
			}
		}
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
		<< "4: Start/Stop temperatur maaling" << endl
		<< "5: Skift kode" << endl
		<< endl;
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
	{
		cout << "proev igen" << endl;
	}
}

void Computer::clearInputBuffer()
{
	PINPUT_RECORD gammeltKonsolInput = new INPUT_RECORD[100];
	DWORD nyKonsolInput;
	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), gammeltKonsolInput, 100, &nyKonsolInput);
	delete[] gammeltKonsolInput;
}

void Computer::seeCurrentTemperature(char arr1[])
{
	cout << "nuvaerende temperatur: " << arr1[0] << arr1[1] << arr1[2] << arr1[3] << "\r\b\b\b\b";
}
