#include <iostream>
#include "Computer.h"
#include "Log.h"
#include "Temp.h"
#include "UART.h"
#include "UI.h"

using namespace std;

int main()
{
	Log testObj;
	//testObj.addToLog(80085);
	testObj.print();
}

