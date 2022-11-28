#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
//#include "Computer.h"

using namespace std;

class Log
{
public:
	Log(string filename = "Log.txt");
	void print();
	void addToLog(int temp);
private:
	string filename_;
};

