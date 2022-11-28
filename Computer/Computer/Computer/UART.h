#pragma once

#include <iostream>
#include "UART.h"

using namespace std;

class UART
{
public:
	
	int getTemp();
	void sendOp();
	void sendNed();
	int initUART();
	void send(string);
	char getChar();
};

