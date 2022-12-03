#pragma once
#ifndef TEMPREG_H
#define TEMPREG_H
#include "Blow.h"
#include "Heat.h"
//#include <Arduino.h>

class TempReg
{
private:
	BlowControl myBlowControl_;
	HeatControl myHeatControl_;
	bool isTempRegStarted = false;
public:
	TempReg(double minimumTemp, double MaxemumTemp);
	~TempReg();
	void RegTempNow();
	double getTemp(); // getTemp is supprosed to be the current tempratur.
	void getTempInterval();
	void setTempInterval(double minimumTemp, double MaxemumTemp);
};
#endif
