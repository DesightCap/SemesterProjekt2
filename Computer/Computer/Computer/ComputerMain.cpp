#include <iostream>
#include "Computer.h"
#include "Log.h"
#include "Temp.h"
#include "UART.h"
#include "UI.h"

#include <time.h>

using namespace std;

int main()
{
	Log testObj;
	
	// Test

	// addToLog metode test
	srand(time(0)); testObj.addToLog(rand() % 20);
	


	// Udskriv log test
	//testObj.print();
}

