#pragma once
#ifndef HEAT_H
#define HEAT_H
//#include <Arduino.h>

class HeatControl{
private:
		double TempMin_;
		void SetMinTemp(double tempMin);
public:
		HeatControl(double SetTempMin);
		~HeatControl();
		double getTempMin();
		void TempOp(double currentTemp);

	
};

#endif