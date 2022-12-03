#pragma once
#pragma once
#ifndef BLOW_H
#define BLOW_H
//#include <Arduino.h>

class BlowControl {
private:
	double TempMax_;
public:
	void SetMaxTemp(double tempMax);
	BlowControl(double SetTempMax = 40);
	~BlowControl();
	double getTempMax();
	void TempNed(double currentTemp);


};

#endif