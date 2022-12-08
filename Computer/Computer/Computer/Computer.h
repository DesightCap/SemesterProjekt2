#pragma once

#include <iostream>
#include "UART.h"
#include "Log.h"
#include "temperature.h"
//#include "UI.h" // Udgået og merget in i Computer

using namespace std;

class Computer
{
public:
	//Computer(testUI, testUART, &testLog, &testtemperatureerature)
	Computer(UART* testUART, Log* testLog, Temperature* testtemperature);

	void readToggle();
	void openMenu();
	void menuPrint(bool clear = true);
	int UIinput();
	string dataHandler();
	double temperatureCharArrayToDouble(char recieved[]);
	bool setTemperatureInt();
	void clearInputBuffer();
private:
	UART* testUART_;
	Log* testLog_;
	Temperature* temperatureObject_;
	int temperature_;
	bool writeTemperature;
	char inputData_[INPUT_DATA_BYTES];
};

