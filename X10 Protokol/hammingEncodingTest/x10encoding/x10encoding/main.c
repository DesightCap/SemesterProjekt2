#include <time.h>
#include <stdlib.h>
#include <stdio.h>


#define STARTSIZE 4

int startBits[STARTSIZE] = { 1, 1, 1, 0 };

#define ADDRESSSIZE 3
#define COMMANDSIZE 8
#define COMBINEDSIZE 11
#define ENCODEDSIZE 15


void randomBits(int*, int);
void print(int*, int);
void combineBitArrays(int*, int*, int*);

int correctModulo(int, int);
void hammingEncoding(int*, int*);
void hammingDecoding(int*, int*);

void main()
{
	//////////////////////////////////////////
	//Address bits
	//////////////////////////////////////////
	int addressTest[ADDRESSSIZE];

	//Test case address
	addressTest[0] = 1;
	addressTest[1] = 0;
	addressTest[2] = 1;

	//Random bits, comment out for test case
	randomBits(&addressTest, ADDRESSSIZE);

	printf_s("Address: \n");
	print(&addressTest, ADDRESSSIZE);



	//////////////////////////////////////////
	//Command bits
	//////////////////////////////////////////
	int commandTest[COMMANDSIZE];

	//Test case command
	commandTest[0] = 1;
	commandTest[1] = 0;
	commandTest[2] = 0;
	commandTest[3] = 0;
	commandTest[4] = 1;
	commandTest[5] = 1;
	commandTest[6] = 0;
	commandTest[7] = 1;

	//Random bits, comment out for test case
	randomBits(&commandTest, COMMANDSIZE);

	printf_s("Command: \n");
	print(&commandTest, COMMANDSIZE);



	//////////////////////////////////////////
	//Combine address and command strings
	//////////////////////////////////////////
	int combinedTest[COMBINEDSIZE];
	combineBitArrays(&addressTest, &commandTest, &combinedTest);
	printf_s("Combined: \n");
	print(&combinedTest, COMBINEDSIZE);



	//////////////////////////////////////////
	//Encode the bits with Hamming coding
	//////////////////////////////////////////
	int encodedTest[ENCODEDSIZE];
	hammingEncoding(&combinedTest, &encodedTest);
	printf_s("Hamming encoded: \n");
	print(&encodedTest, ENCODEDSIZE);



	//////////////////////////////////////////
	//Generating error for testing of decoding
	//////////////////////////////////////////
	int forcedErr[ENCODEDSIZE];

	for (int i = 0; i < ENCODEDSIZE; i++)
	{
		forcedErr[i] = 0;
	}
	forcedErr[6] = 1;

	printf_s("Forced error: \n");
	print(&forcedErr, ENCODEDSIZE);



	//////////////////////////////////////////
	//Adding error
	//////////////////////////////////////////
	for (int i = 0; i < ENCODEDSIZE; i++)
	{
		encodedTest[i] = correctModulo(encodedTest[i] + forcedErr[i], 2);
	}

	printf_s("Error added to encoded: \n");
	print(&encodedTest, ENCODEDSIZE);



	//////////////////////////////////////////
	//Hamming decoding
	//////////////////////////////////////////
	int decodedTest[COMBINEDSIZE];
	hammingDecoding(&encodedTest, &decodedTest);



	//////////////////////////////////////////
	//Print resulting combined address and command
	//////////////////////////////////////////
	printf_s("Decoded: \n");
	print(&decodedTest, COMBINEDSIZE);
}


void randomBits(int* place, int size) //Randomize an int arrays input with 1s and 0s
{
	srand(time(NULL));

	for (int i = 0; i < size; i++)
	{
		place[i] = (rand() % 2);
	}
}


void print(int* place, int size) //Print all elements of an int array, given the size
{
	for (int i = 0; i < size; i++)
	{
		printf_s("%d", place[i]);
	}
	printf_s("\n");
}


void combineBitArrays(int* adr, int* com, int* combined) //Generating a new bitString by append one bitString to another
{

	for (int i = 0; i < ADDRESSSIZE; i++)
	{
		combined[i] = adr[i];
	}
	for (int j = 0; j < COMMANDSIZE; j++)
	{
		combined[j + ADDRESSSIZE] = com[j];
	}

}


int correctModulo(int x, int m) //Modulo in C is not entirely correct, this implementation fixes that
{
	return (x % m + m) % m;
}


void hammingEncoding(int* str, int* encoded) // (15,11) Hamming encoding
{

	//Original message
	for (int i = 0; i < COMBINEDSIZE; i++)
	{
		encoded[i] = str[i];
	}


	//Parity check bits
	int p1 = 0;
	int p2 = 0;
	int p3 = 0;
	int p4 = 0;

	//Parity check dependencies
	int checkP1[7] = { 1, 2, 4, 5, 7, 9, 11 };
	int checkP2[7] = { 1, 3, 4, 6, 7, 10, 11 };
	int checkP3[7] = { 2, 3, 4, 8, 9, 10, 11 };
	int checkP4[7] = { 5, 6, 7, 8, 9, 10, 11 };

	//Parity check p1
	for (int i = 0; i < 7; i++)
	{
		p1 += encoded[checkP1[i] - 1];
	}

	//Parity check p2
	for (int i = 0; i < 7; i++)
	{
		p2 += encoded[checkP2[i] - 1];
	}

	//Parity check p3
	for (int i = 0; i < 7; i++)
	{
		p3 += encoded[checkP3[i] - 1];
	}

	//Parity check p4
	for (int i = 0; i < 7; i++)
	{
		p4 += encoded[checkP4[i] - 1];
	}


	//Add the parity bits
	encoded[11] = correctModulo(p1, 2);
	encoded[12] = correctModulo(p2, 2);
	encoded[13] = correctModulo(p3, 2);
	encoded[14] = correctModulo(p4, 2);

}


void hammingDecoding(int* str, int* decoded) // (15,11) Hamming decoding
{

	//Original message, before error-correction
	for (int i = 0; i < COMBINEDSIZE; i++)
	{
		decoded[i] = str[i];
	}


	//Indexes as they match each checkSum.
	int check1[7] = { 1, 2, 4, 5, 7, 9, 11 };
	int check2[7] = { 1, 3, 4, 6, 7, 10, 11 };
	int check3[7] = { 2, 3, 4, 8, 9, 10, 11 };
	int check4[7] = { 5, 6, 7, 8, 9, 10, 11 };

	//The checkSums, initialized with their parity value
	int checkSum1 = str[11];
	int checkSum2 = str[12];
	int checkSum3 = str[13];
	int checkSum4 = str[14];


	//Adding up the checksums
	for (int i = 0; i < 7; i++)
	{
		checkSum1 += str[check1[i] - 1];
	}

	for (int i = 0; i < 7; i++)
	{
		checkSum2 += str[check2[i] - 1];
	}

	for (int i = 0; i < 7; i++)
	{
		checkSum3 += str[check3[i] - 1];
	}

	for (int i = 0; i < 7; i++)
	{
		checkSum4 += str[check4[i] - 1];
	}


	//Modulo 2 of checksums to get bool values, if all 0 then no errors, if all but one is 0, then error is in parity bits and we can ignore
	checkSum1 = correctModulo(checkSum1, 2);
	checkSum2 = correctModulo(checkSum2, 2);
	checkSum3 = correctModulo(checkSum3, 2);
	checkSum4 = correctModulo(checkSum4, 2);

	int errorCount = 0; // Error counter, may be removed

	//Array of errors found
	int error[COMBINEDSIZE] = { 0 };

	//Check for each bit in message, if an error, add to error array
	if (checkSum1 && checkSum2 && !checkSum3 && !checkSum4)	//m1
	{
		error[0] = 1;
		errorCount += 1;
	}
	if (checkSum1 && !checkSum2 && checkSum3 && !checkSum4)	//m2
	{
		error[1] = 1;
		errorCount += 1;
	}
	if (!checkSum1 && checkSum2 && checkSum3 && !checkSum4)	//m3
	{
		error[2] = 1;
		errorCount += 1;
	}
	if (checkSum1 && checkSum2 && checkSum3 && !checkSum4)	//m4
	{
		error[3] = 1;
		errorCount += 1;
	}
	if (checkSum1 && !checkSum2 && !checkSum3 && checkSum4)	//m5
	{
		error[4] = 1;
		errorCount += 1;
	}
	if (!checkSum1 && checkSum2 && !checkSum3 && checkSum4)	//m6
	{
		error[5] = 1;
		errorCount += 1;
	}
	if (checkSum1 && checkSum2 && !checkSum3 && checkSum4)	//m7
	{
		error[6] = 1;
		errorCount += 1;
	}
	if (!checkSum1 && !checkSum2 && checkSum3 && checkSum4)	//m8
	{
		error[7] = 1;
		errorCount += 1;
	}
	if (checkSum1 && !checkSum2 && checkSum3 && checkSum4)	//m9
	{
		error[8] = 1;
		errorCount += 1;
	}
	if (!checkSum1 && checkSum2 && checkSum3 && checkSum4)	//m10
	{
		error[9] = 1;
		errorCount += 1;
	}
	if (checkSum1 && checkSum2 && checkSum3 && checkSum4)	//m11
	{
		error[10] = 1;
		errorCount += 1;
	}


	//Add the error array to the decoded message, flipping any found error.
	for (int i = 0; i < 11; i++)
	{
		decoded[i] = correctModulo(decoded[i] + error[i], 2);
	}

}