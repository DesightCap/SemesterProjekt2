#pragma once

#include <iostream>
#include "UART.h"
#include "Log.h"
#include "Temp.h"
#include "UI.h"


using namespace std;

class Computer : Log, Temp, UART, UI
{
public:
	//Computer(testUI, testUART, &testLog, &testTemp)
	Computer();

	void TempReg();
	void consol();
	void LogPrint() {
		print();
	}



private:
	int temp_;
};

