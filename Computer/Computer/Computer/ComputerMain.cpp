#include <iostream>
#include "Computer.h"
#include <time.h>
#include <iomanip> // til at sætte præcion 
#include <string> // string til double via stod()

void testFunction_writeToLog(Log);
void testFunction_writeToArduino(UART);
void testFunction_ArdunoConnected(UART);

using namespace std;

//char inputData[INPUT_DATA_BYTES]; // INPUT_DATA_BYTES er defineret i UART.h
//double inputValue = 0.0;

// Vi antager at Ardrino tilsluttes port mellem COM1 og COM9, da vi ellers skal formatere porten som \\\\.\\COM3 som Win32 vil have den // Kan evt. udvides efterfølgende
char comport[] = "COM7";
//char comport[] = "\\\\.\\COM5";
char* port = comport;

int main()
{
	int userPort;
	cout << "Indtast COM port nummer som kommunikationsenhed er tilsluttet: ";
	cin >> userPort;
	comport[3] = (char)userPort + 48;

	Log LogObj;
	UART UARTObj(port); // Med UART der opretter forbindelse i constructoren, kunne Complex(d).print(); som vist i OOP være en ide til at hente data ud?
	Temperature TemperatureObj;
	Computer ComputerObj(&UARTObj, &LogObj, &TemperatureObj);
	ComputerObj.openMenu();
}



void testFunction_writeToLog(Log LogObj)
{
	// addToLog metode test
	srand(time(0)); LogObj.addToLog(rand() % 20);
}

void testFunction_ArdunoConnected(UART UARTObj)
{
	cout << "Arduino forbundet: " << boolalpha << UARTObj.isConnected() << endl;
	if (UARTObj.isConnected())
		cout << endl << "Forbundet til port " << port << endl;
}

void testFunction_writeToArduino(UART UARTObj)
{
	// Test skrivning til Arduino
	char testOutput = 1;
	while (true)
	{
		if (UARTObj.send(&testOutput, 1))
		{
			cout << "SUCCESS" << endl;

		}
		else
		{
			cout << "FAILIURE" << endl;
		}
		Sleep(60);
	}

}
