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
	void tempStop();
	bool recieveTempInterval();
	void printLog();
	void openMenu();
	int UIinput();
private:
	UART* testUART_;
	Log* testLog_;
	Temp* testTemp_;
	int temp_;
};

