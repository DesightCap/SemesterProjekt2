#pragma once
#ifndef HEAT_H
#define HEAT_H
//#include <Arduino.h>

class HeatControl{
private:
		double TempMin_;
public:
		void SetMinTemp(double tempMin);
		HeatControl(double SetTempMin = 5);
		~HeatControl();
		double getTempMin();
		void TempOp(double currentTemp);

	
};

#endif