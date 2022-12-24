#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
//#include <stdlib.h>

#include "Computer.h"


Computer::Computer(UART* testUART, Log* testLog, Temperature* testTemperature)
{
	temperature_ = 0;
	UARTObject_ = testUART;
	LogObject_ = testLog;
	temperatureObject_ = testTemperature;
	writeTemperature_ = true;
}


void Computer::readToggle()
{
	writeTemperature_ = !writeTemperature_;
}

void Computer::openMenu()
{
	bool runningTemperatureReg = true;
	this->menuPrint();
	while (runningTemperatureReg)
	{
		Sleep(ARDUINO_WAIT_TIME);
		int userSelect = 0;
		switch (UIinput())
		{
		case 1:
			system("CLS");
			LogObject_->print();
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
			UARTObject_->changePassword((char)pwdInt);
			cout << endl << endl;
			break;
		}
		default:

			char recieve[] = "00.0";

			while (writeTemperature_)
			{
				UARTObject_->getTemperature(recieve, (sizeof(recieve) / sizeof(recieve[0])) - 1);
				if (recieve[2] == '.' && (recieve[1] != '0' || recieve[0] != '0'))
				{
					double recievedDouble = this->temperatureCharArrayToDouble(recieve);

					LogObject_->addToLog(recievedDouble);
					seeCurrentTemperature(recieve);

				//	cout << "Skrevet til log: " << recieve[0] << recieve[1] << recieve[2] << recieve[3] << endl; // tester

					switch (this->temperatureObject_->checkTemperature(recievedDouble))
					{
					case -1:
						if (writeTemperature_)
							UARTObject_->sendUp();
						//		cout << "Haev Temperatur" << endl; // tester
						break;
					case 0:
						if (writeTemperature_)
						{
							char x = 'x';
							UARTObject_->send(&x, 1); // tror ikke det her giver mening, for vores CTRL arudino har slet ikke en 'x' case
													  // enten skal hele casen slettes, eller have den med, men det inde i casen skal slettes.
						}
						//			cout << "Temperatur indenfor interval" << endl; // tester
						break;
					case 1:
						if (writeTemperature_)
							UARTObject_->sendDown();
						//		cout << "Saenk temperatur " << endl; // tester
						break;
					default:
						break;

					}
				}
				else
				{

					char x = 'x';
					UARTObject_->send(&x, 1);
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
	if (UARTObject_->isConnected())
	{
		for (int i = 0; i < INPUT_DATA_BYTES; i++)
		{
			inputData_[i] = 0;
		}
		UARTObject_->getTemperature(inputData_, INPUT_DATA_BYTES);
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
