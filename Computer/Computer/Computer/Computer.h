#pragma once

#include <iostream>
#include "UART.h"

using namespace std;

class Computer
{
public:
	void TempReg();
private:
	int temp_;
};

