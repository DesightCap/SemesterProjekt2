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
	Computer(UART* UARTObj, Log* LogObj, Temperature* TemperatureObj);

	void readToggle();
	void openMenu();
	void menuPrint(bool clear = true);
	int UIinput();
	string dataHandler();
	double temperatureCharArrayToDouble(char recieved[]);
	bool setTemperatureInt();
	void clearInputBuffer();
	void seeCurrentTemperature(char[]); 
private:
	UART* UARTObject_;
	Log* LogObject_;
	Temperature* temperatureObject_;
	int temperature_;
	bool writeTemperature_;
	char inputData_[INPUT_DATA_BYTES];
};

