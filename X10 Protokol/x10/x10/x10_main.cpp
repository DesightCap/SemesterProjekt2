#include <iostream>
using namespace std;
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 16000000

volatile int counter = 0;

ISR(INT0_vect) //Interrupt på refleksbrik 1
{
	//counter
	counter++; // 
}

int main()
{
	initISR();






}














void initISR()
{
	EICRA = 0b00000001; // any edge
	EIMSK = 0b00000001; // enable interrupt int 0
	sei(); // Global ISR enabler
}