#pragma once

#include <iostream>
//#include "Computer.h"

using namespace std;

class Temp
{
public:
	Temp(int min = 5, int max = 30);
	bool setTempInt(int min, int max); // 
	int checkTemp(int temp); // 0 == indenfor; + == over; - == under
private:
	int TempMin_;
	int TempMax_;
};

