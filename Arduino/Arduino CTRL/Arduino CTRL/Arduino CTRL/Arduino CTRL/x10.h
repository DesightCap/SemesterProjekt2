

#ifndef X10_H_
#define X10_H_

#pragma once
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
//#define F_CPU 16000000


	void sendx10(uint16_t *adr, uint16_t* com, uint16_t* combined, uint16_t* encoded, uint32_t* datapakke, volatile int* counterPtr);
	//uint8_t recievex10();
	void initISR();
	void initPort();
	
	void combineBitArrays(uint16_t* adr, uint16_t* com, uint16_t* combined);
	int correctModulo(int, int);
	void hammingEncoding(uint16_t* str, uint16_t* encoded);
	void doubleEncoded(uint16_t* encoded, uint32_t* datapakke);
	void sendBurst();
	
	void hammingDecoding(uint16_t* str, uint16_t* encoded);
	void halfEncoded(uint16_t* encoded, uint32_t* datapakke);
	void recievex10(uint16_t* adr, uint16_t* com, uint16_t* combined, uint16_t* encoded, uint32_t* datapakke, volatile int* counterPtr);
	void splitCombine(uint16_t* adr, uint16_t* com, uint16_t* combined);
	



#endif /* X10_H_ */