#pragma once

#include <iostream>
//#include "Computer.h"

using namespace std;


class UI
{
public:
	//UI();
	int tempInitiate();
	void tempStop();
	bool recieveTempInterval();
	void printLog();
	void openMenu();
	
	friend class Computer;
};

