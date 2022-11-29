/*
 * x10.h
 *
 * Created: 29-11-2022 08:25:16
 *  Author: sebastian
 */


#ifndef X10_H_
#define X10_H_

#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


class x10
{
public:
	x10();
	void sendx10(char, uint8_t* datapakkePtr, volatile int* counterPtr) const;
	char recievex10() const;
	void initISR() const;
};



#endif /* X10_H_ */