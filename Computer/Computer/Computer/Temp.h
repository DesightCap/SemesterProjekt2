#pragma once

#include <iostream>
#include "UART.h"

using namespace std;

class Temp
{
public:
	bool setTempInt(int min, int max);
	int checkTemp(int temp); 
private:
	int TempMin_;
	int TempMax_;
};

