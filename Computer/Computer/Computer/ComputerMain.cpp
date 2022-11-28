#include <iostream>
#include "Computer.h"
/*
#include "Log.h"
#include "Temp.h"
#include "UART.h"
#include "UI.h"
*/
#include <time.h>

using namespace std;

int main()
{
	Log testLog;
	UI testUI;
	UART testUART;
	Temp testTemp(6,29);
	
	// Test

	// addToLog metode test
	//srand(time(0)); testObj.addToLog(rand() % 20);
	
	// Udskriv log test
	//testObj.print();

//	Computer testCmp(UI testUI, UART testUART, Log &testLog, Temp &testTemp());
	Computer testComputer;
	testComputer.LogPrint();
}

