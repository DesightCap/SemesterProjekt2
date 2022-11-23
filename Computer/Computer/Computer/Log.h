#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

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

