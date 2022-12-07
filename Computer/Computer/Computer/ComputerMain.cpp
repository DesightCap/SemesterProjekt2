#include <iostream>
#include "Computer.h"
#include <time.h>
#include <iomanip> // til at sætte præcion 
//#include <thread>
#include <string> // string til double via stod()

//[HKEY_LOCAL_MACHINE\HARDWARE\]
//Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Ports

void testFunktionSkrivTilLog();
void testFunktionSkrivTilArduino(UART);
void testFunktionArdionoConnected(UART);


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
	Temp testTemp;

	Computer testComputer(&testUART, &testLog, &testTemp);
	testComputer.openMenu();
}

void testFunktionSkrivTilLog(Log testLog)
{
	// addToLog metode test
	srand(time(0)); testLog.addToLog(rand() % 20);
}

void testFunktionArdionoConnected(UART testUART)
{
	cout << "Arduino forbundet: " << boolalpha << testUART.isConnected() << endl;
	if (testUART.isConnected())
		cout << endl << "Forbundet til port " << port << endl;
}

void testFunktionSkrivTilArduino(UART testUART)
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
