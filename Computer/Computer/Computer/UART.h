#pragma once
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

