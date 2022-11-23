#pragma once

#include <iostream>
#include "UART.h"

using namespace std;


class UI
{
public:
	int tempInitiate();
	void tempStop();
	bool recieveTempInterval();
	void printLog();
	void openMenu();
};

