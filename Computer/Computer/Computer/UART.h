#pragma once

#define ARDUINO_WAIT_TIME 2000
#define INPUT_DATA_BYTES 1 // Antallet af bytes vi modtager i string. (Husk \r\n hver tæller for en)
#define OUTPUT_DATA_BYTES 1

#include <iostream>
#include "UART.h"
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class UART
{
public:
	UART(char* port_); // Opsætning - Overvej om det er bedst her eller i initUART
	~UART();
	int getTemperature(char* buffer, unsigned int buf_size);// Modtage temperatur
	void sendUp();
	void sendDown(); 
	bool send(char* buffer, unsigned int buf_size); // Sender et char array på størrelse buf_size // Skal kaldes fra sendUp og ned med relevant data preloadet
	bool isConnected(); // Retunere hvorvidt connected er sat og dermed om vi er forbundet

private:
	char* port_; // Giver int eller char bedre mening til at opbevare COM port?
	HANDLE handleToCOM;
	bool connected; // Er der forbundet til Com port
	COMSTAT status;
	DWORD errors;
};

