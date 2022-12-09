#include <iostream>
#include "Computer.h"
#include <time.h>
#include <iomanip> // til at sætte præcion 


#include <string> // string til double via stod()

//[HKEY_LOCAL_MACHINE\HARDWARE\]
//Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Ports

void testFunction_writeToLog(Log);
void testFunction_writeToArduino(UART);
void testFunction_ArdunoConnected(UART);


using namespace std;

//char inputData[INPUT_DATA_BYTES]; // INPUT_DATA_BYTES er defineret i UART.h
//double inputValue = 0.0;





// Vi antager at Ardrino tilsluttes port mellem COM1 og COM9, da vi ellers skal formatere porten som \\\\.\\COM3 som Win32 vil have den // Kan evt. udvides efterfølgende
char comport[] = "COM5";
//char comport[] = "\\\\.\\COM5";
char* port = comport;

int main()
{
	
	Log testLog;
	UART testUART(port); // Med UART der opretter forbindelse i constructoren, kunne Complex(d).print(); som vist i OOP være en ide til at hente data ud?
	Temperature testtemperature;

	Computer testComputer(&testUART, &testLog, &testtemperature);
	//testComputer.openMenu();
	for (int i = 0; ; i++)
	{

		Sleep(ARDUINO_WAIT_TIME);
		
		char buffer[2]{ 0 };

		char x = 'd';

		testFunction_writeToArduino(testUART);

		testUART.send(&x, 1);
		cout << i << endl;

	}
}

void testFunction_writeToLog(Log testLog)
{
	// addToLog metode test
	srand(time(0)); testLog.addToLog(rand() % 20);
}

void testFunction_ArdunoConnected(UART testUART)
{
	cout << "Arduino forbundet: " << boolalpha << testUART.isConnected() << endl;
	if (testUART.isConnected())
		cout << endl << "Forbundet til port " << port << endl;
}

void testFunction_writeToArduino(UART testUART)
{
	// Test skrivning til Arduino
	char testOutput = 1;
	while (true)
	{
		if (testUART.send(&testOutput, 1))
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
