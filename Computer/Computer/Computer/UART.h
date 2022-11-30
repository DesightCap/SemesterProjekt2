#pragma once

#include <iostream>
#include "UART.h"

using namespace std;

class UART
{
public:
	int initUART(char*);
	
	int getTemp(); // Modtage temperatur
	void sendOp();
	void sendNed();
	void send(string);
	char getChar();

private:
	char* port_; // Giver int eller char bedre mening til at opbevare COM port?

};

