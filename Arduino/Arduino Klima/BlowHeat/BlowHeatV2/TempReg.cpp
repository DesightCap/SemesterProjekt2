#include "TempReg.h"
#include <stdlib.h>
#include <iostream>
#include <exception>


TempReg::TempReg(double minimumTemp, double MaxemumTemp)
{
	isTempRegStarted = false;
	try
	{
	setTempInterval(minimumTemp, MaxemumTemp);
	}
	catch (std::invalid_argument const& ex)
	{
		std::cout << ex.what() << std::endl;
	}

}

TempReg::~TempReg()
{
}

void TempReg::RegTempNow()
{
	isTempRegStarted = true;
	myBlowControl_.TempNed(getTemp());
	myHeatControl_.TempOp(getTemp());

}

double TempReg::getTemp() // getTemp is supprosed to be the current tempratur.
{
	return 0.0;
}

void TempReg::getTempInterval()
{
	std::cout << "Showing tempInterval: " << std::endl;
	std::cout << "Minimum Tempratur: " << myHeatControl_.getTempMin() << std::endl;
	std::cout << "Maximum Tempratur: " << myBlowControl_.getTempMax() << std::endl;
}

void TempReg::setTempInterval(double minimumTemp, double MaxemumTemp)
{
	if (minimumTemp >= MaxemumTemp)
	{
		throw "Interval ERROR: Maximum tempratur below Minimum Tempratur!";
	}
	myBlowControl_.SetMaxTemp(MaxemumTemp);
	myHeatControl_.SetMinTemp(minimumTemp);

}
