#include <iostream>
#include "Computer.h"
/*
#include "Log.h"
#include "Temp.h"
#include "UART.h"
#include "UI.h"
*/
#include <time.h>
#include <iomanip> // til at sætte præcion 
#include <thread>
#include <string> // string til double via stod()


using namespace std;

char inputData[INPUT_DATA_BYTES]; // INPUT_DATA_BYTES er defineret i UART.h
double inputValue = 0.0;

// Vi antager at Ardrino tilsluttes port mellem COM1 og COM9, da vi ellers skal formatere porten som \\\\.\\COM3 som Win32 vil have den // Kan evt. udvides efterfølgende
char comport[] = "COM5";
//char comport[] = "\\\\.\\COM5";
char* port = comport;


int main()
{

	Log testLog;
	//	UI testUI;
	UART testUART(port); // Med UART der opretter forbindelse i constructoren, kunne Complex(d).print(); som vist i OOP være en ide til at hente data ud?
	Temp testTemp(5, 10);

	// Test

	// addToLog metode test
	//srand(time(0)); testLog.addToLog(rand() % 20);

	// Udskriv log test
	//testObj.print();

//	Computer testCmp(UI testUI, UART testUART, Log &testLog, Temp &testTemp());

	Computer testComputer(&testUART, &testLog, &testTemp);
	//testComputer.openMenu();
	/*
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
	//*/
	
	//*


	//cout << "Arduino forbundet: " << boolalpha << testUART.isConnected() << endl << "Forbundet til port " << port;
	while (testUART.isConnected())
	{
		char toSend;
		cin >> toSend;
		testUART.send(&toSend, 1);
		cout << testComputer.dataHandler() << endl;
	}
	/*
	while (testUART.isConnected())
	{
		if (testUART.isConnected())
		{
			for (int i = 0; i < INPUT_DATA_BYTES; i++)
			{
				inputData[i] = 0;
			}
			testUART.getTemp(inputData, INPUT_DATA_BYTES);
		}
		string inputValStr(inputData); // Kan man evt bare caste til string i stedet for at oprette en ny string?
		//inputValue = stod(inputValStr); // String til double

		//cout << fixed << setprecision << inputValue << '\r';

		cout << endl << "Data: " << inputData;

		//this_thread::sleep_for(chrono::microseconds(500)); // Sov baseret på hvor ofte arduino sender data
		Sleep(ARDUINO_WAIT_TIME); //Alternativ sleep
	}
	//*/

}

