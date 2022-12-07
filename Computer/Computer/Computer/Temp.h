#pragma once

#include <iostream>
//#include "Computer.h"

using namespace std;

class Temp
{
public:
	Temp(int min = 5, int max = 30);
	//bool setTempInt(); // 
	int checkTemp(int temp); // 0 == indenfor; + == over; - == under
	//void SletKonsolInputs(); // fra google
	int getMin();
	int getMax();
	void setMin(int);
	void setMax(int);
private:
	int TempMin_;
	int TempMax_;
};

