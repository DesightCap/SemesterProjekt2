#pragma once

#include <iostream>
#include "UART.h"
#include "Log.h"
#include "Temp.h"
//#include "UI.h" // Udgået og merget in i Computer

using namespace std;

class Computer
{
public:
	//Computer(testUI, testUART, &testLog, &testTemp)
	Computer(UART* testUART, Log* testLog, Temp* testTemp);

	void TempReg();
	// Metoder merget ind fra UI klasse
	int tempInitiate();
	void readEnable();
	void readToggle();
	bool recieveTempInterval();
	void printLog();
	void openMenu();
	void menuPrint(bool clear = true);
	int UIinput();
	string dataHandler();
	double tempCharArrayToDouble(char recieved[]);
private:
	UART* testUART_;
	Log* testLog_;
	Temp* testTemp_;
	int temp_;
	bool startStop_ = true;
	//char comport_[] = "COM5"; // Alternativ: char comport[] = "\\\\.\\COM5";
	//char* port_ = comport;
	char inputData_[INPUT_DATA_BYTES];

};

