#pragma once
#include <iostream>

class x10
{
public:
	x10();
	void sendx10(std::string, uint32_t* datapakkePtr, volatile int* counterPtr) const;
	std::string recievex10() const;
	void initISR() const;
};