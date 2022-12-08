#pragma once

#include <iostream>
//#include "Computer.h"

using namespace std;

class Temperature
{
public:
	Temperature(int min = 5, int max = 30);
	int checkTemperature(int Temperature); // 0 == indenfor; + == over; - == under
	int getMin();
	int getMax();
	void setMin(int);
	void setMax(int);
private:
	int TemperatureMin_;
	int TemperatureMax_;
};

