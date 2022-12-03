#pragma once
#pragma once
#ifndef BLOW_H
#define BLOW_H
//#include <Arduino.h>

class BlowControl {
private:
	double TempMax_;
	void SetMinTemp(double tempMax);
public:
	BlowControl(double SetTempMax);
	~BlowControl();
	double getTempMax();
	void TempNed(double currentTemp);


};

#endif