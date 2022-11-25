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
	Computer(UI, UART, Log*, Temp*);
	void TempReg();
private:
	int temp_;
};

