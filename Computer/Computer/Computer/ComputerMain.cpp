#include <iostream>
#include "Computer.h"
#include <time.h>
#include <iomanip> // til at sætte præcion 
#include <thread>
#include <string> // string til double via stod()


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
	//	UI testUI;
	UART testUART(port); // Med UART der opretter forbindelse i constructoren, kunne Complex(d).print(); som vist i OOP være en ide til at hente data ud?
	Temp testTemp;
	cout << "Arduino forbundet: " << boolalpha << testUART.isConnected() << endl << "Forbundet til port " << port << endl;

	// Test

	// addToLog metode test
	//srand(time(0)); testLog.addToLog(rand() % 20);

	// Udskriv log test
	//testObj.print();
	
//	Computer testCmp(UI testUI, UART testUART, Log &testLog, Temp &testTemp());

	Computer testComputer(&testUART, &testLog, &testTemp);
	testComputer.openMenu();
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


	//cout << "Arduino forbundet: " << boolalpha << testUART.isConnected() << endl << "Forbundet til port " << port << endl;
	while (testUART.isConnected())
	{
		//string buffer{'0','0','\0'};
		char recieve[] = "00";
		char toSend;
		/*int input = testComputer.UIinput();
		if (input == 1)
		{
			testUART.sendNed();
		}
		else if (input == 2)
		{
			testUART.sendOp();
		}
		else if (input == 3)
		{
			testLog.print();
		}
		else*/
		{

			//testUART.getTemp((char*)&buffer, sizeof(buffer) / sizeof(buffer[0]));
			//testUART.getTemp(recieve, 3);
			testUART.getTemp(recieve, sizeof(recieve) / sizeof(recieve[0]));

			if (recieve[0] != '0' || recieve[1] != '0' || recieve[2] != '\0')
			{

				int recievedInt = testComputer.tempCharArrayToInt(recieve);

				testLog.addToLog(recievedInt);
				cout << "Skrevet til log" << endl;
				switch (testTemp.checkTemp(recievedInt))
				{
				case -1:
					testUART.sendNed();
					cout << "Saenk temperatur" << endl;
					break;
				case 0:
					cout << "Temp indenfor interval" << endl;
					break;
				case 1:
					testUART.sendOp();
					cout << "Haev temp" << endl;
					break;
				default:
					break;
				}
			}
			else if (recieve[0] == '0' && recieve[1] == '0' && recieve[2] == '\0')
			{
				Sleep(50);
				testUART.getTemp(recieve, sizeof(recieve) / sizeof(recieve[0]));
				if (recieve[0] != '0' || recieve[1] != '0' || recieve[2] != '\0')
				{
					cout << recieve << endl;
				}
			}
			else
			{
				cout << "Ingen input." << endl;
			}
		}
		Sleep(ARDUINO_WAIT_TIME);
	}


	//while (testUART.isConnected())
	//{
	//	char toSend;
	//	cout << "Data to send: ";
	//	cin >> toSend;
	//	testUART.send(&toSend, 2);
	//	
	//	if (toSend == '8')
	//	{
	//		testUART.send(&toSend, 1);
	//		string stringRecieved = testComputer.dataHandler();

	//		try
	//		{
	//			cout << "Data recieved: " << stoi(stringRecieved) << endl;

	//		}
	//		catch (invalid_argument const& error)
	//		{
	//			cout << "String til Int daarligt argument: " << error.what() << endl;
	//		}
	//		catch (out_of_range const& error)
	//		{
	//			cout << "String til Int uden for range: " << error.what() << endl;
	//		}
	//	}
	//	else
	//	{
	//		cout << "Data recieved: " << testComputer.dataHandler() << endl;
	//	}
	//}


	/*
	while (testUART.isConnected())
	{

		Sleep(ARDUINO_WAIT_TIME); //Alternativ sleep
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
		cout << "Data recieved: " << inputData << endl << endl;



		//this_thread::sleep_for(chrono::microseconds(500)); // Sov baseret på hvor ofte arduino sender data

	}
	//*/

}
