#pragma once

#include <iostream>
#include "UART.h"
#include "Log.h"
#include "Temp.h"
#include "UI.h"

using namespace std;

class Computer
{
public:
	//Computer(testUI, testUART, &testLog, &testTemp)
	Computer(UI* testUI, UART* testUART, Log* testLog, Temp* testTemp);

	void TempReg();
private:
	int temp_;
};

