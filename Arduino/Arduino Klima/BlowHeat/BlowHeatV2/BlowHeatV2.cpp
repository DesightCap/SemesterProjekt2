#include <iostream>
#include "TempReg.h"


int main()
{
    std::cout << "Hello World!\n";
    TempReg myTempReg(20, 99);

    myTempReg.getTempInterval();
    myTempReg.setTempInterval(30, 70);
    myTempReg.getTempInterval();
    myTempReg.setTempInterval(66, 33);
}
