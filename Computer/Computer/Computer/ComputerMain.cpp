#include <iostream>
#include "Computer.h"
#include <time.h>
#include <iomanip> // til at s�tte pr�cion 


#include <string> // string til double via stod()

//[HKEY_LOCAL_MACHINE\HARDWARE\]
//Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Ports

void testFunction_writeToLog(Log);
void testFunction_writeToArduino(UART);
void testFunction_ArdunoConnected(UART);


using namespace std;

//char inputData[INPUT_DATA_BYTES]; // INPUT_DATA_BYTES er defineret i UART.h
//double inputValue = 0.0;





// Vi antager at Ardrino tilsluttes port mellem COM1 og COM9, da vi ellers skal formatere porten som \\\\.\\COM3 som Win32 vil have den // Kan evt. udvides efterf�lgende
char comport[] = "COM7";
//char comport[] = "\\\\.\\COM5";
char* port = comport;

int main()
{
	int userPort;
	cout << "Indtast COM port nummer som kommunikationsenhed er tilsluttet: ";
	cin >> userPort;
	comport[3] = (char)userPort + 48;
	
	Log testLog;
	UART testUART(port); // Med UART der opretter forbindelse i constructoren, kunne Complex(d).print(); som vist i OOP v�re en ide til at hente data ud?
	Temperature testtemperature;
	Computer testComputer(&testUART, &testLog, &testtemperature);
	testComputer.openMenu();
for (int i = 0; i < 10; i++)
	{

		//Sleep(ARDUINO_WAIT_TIME);
		
	// kan nu sende besked korrekt til arudino, dog med sm� forstyrrelser p� output pins, n�r pinnen er low.
		char x = 'd';
		char y = 'u';
		char z = 'a';
	//	testUART.send(&x, 1);
		//Sleep(200);
	//	cout << testUART.send(&x, 1) << endl;
		//testUART.send(&z, 1);
		char recieve[] = "00.0";

		testUART.getTemperature(recieve, (sizeof(recieve) / sizeof(recieve[0])) - 1);
		double temp = testComputer.temperatureCharArrayToDouble(recieve);

		testLog.addToLog(temp);
		Sleep(500);
	//	testUART.send(&y, 1);
	//	Sleep(200);	
	//	cout << testUART.send(&y, 1) << endl;
	//testUART.send(&z, 1);
	//cout << testUART.send(&z, 1) << endl;

		//testFunction_writeToArduino(testUART);


	}
		testLog.print();
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
