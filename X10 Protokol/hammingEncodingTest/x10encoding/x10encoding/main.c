#include <time.h>
#include <stdlib.h>
#include <stdio.h>


#define STARTSIZE 4

int startBits[STARTSIZE] = { 1, 1, 1, 0 };

#define ADDRESSSIZE 3
#define COMMANDSIZE 8

#define MAXLENGHT 50

typedef struct BitString {
	int size;
	int bits[MAXLENGHT];
} bitString;


void randomBits(int*, int);
void print(int*, int);
bitString combineBitArrays(int*, int, int*, int);

int correctModulo(int, int);
bitString hammingEncoding(bitString*);
bitString hammingDecoding(bitString*);

void main()
{
	//////////////////////////////////////////
	//Address bits
	//////////////////////////////////////////
	bitString addressTest;
	addressTest.size = ADDRESSSIZE;

	//Test case address
	addressTest.bits[0] = 1;
	addressTest.bits[1] = 0;
	addressTest.bits[2] = 1;

	//Random bits, comment out for test case
	randomBits(&(addressTest.bits), addressTest.size);

	printf_s("Address: \n");
	print(&(addressTest.bits), addressTest.size);

	

	//////////////////////////////////////////
	//Command bits
	//////////////////////////////////////////
	bitString commandTest;
	commandTest.size = COMMANDSIZE;

	//Test case command
	commandTest.bits[0] = 1;
	commandTest.bits[1] = 0;
	commandTest.bits[2] = 0;
	commandTest.bits[3] = 0;
	commandTest.bits[4] = 1;
	commandTest.bits[5] = 1;
	commandTest.bits[6] = 0;
	commandTest.bits[7] = 1;

	//Random bits, comment out for test case
	randomBits(&(commandTest.bits), commandTest.size);

	printf_s("Command: \n");
	print(&(commandTest.bits), commandTest.size);



	//////////////////////////////////////////
	//Combine address and command strings
	//////////////////////////////////////////
	bitString combinedTest = combineBitArrays(&(addressTest.bits), addressTest.size, &(commandTest.bits), commandTest.size);
	printf_s("Combined: \n");
	print(&(combinedTest.bits), combinedTest.size);



	//////////////////////////////////////////
	//Encode the bits with Hamming coding
	//////////////////////////////////////////
	bitString encodedTest = hammingEncoding(&combinedTest);
	printf_s("Hamming encoded: \n");
	print(&(encodedTest.bits), encodedTest.size);



	//////////////////////////////////////////
	//Generating error for testing of decoding
	//////////////////////////////////////////
	bitString forcedErr;
	forcedErr.size = encodedTest.size;
	
	for (int i = 0; i < forcedErr.size; i++)
	{
		forcedErr.bits[i] = 0;
	}
	forcedErr.bits[6] = 1;

	printf_s("Forced error: \n");
	print(&(forcedErr.bits), forcedErr.size);



	//////////////////////////////////////////
	//Adding error
	//////////////////////////////////////////
	for (int i = 0; i < encodedTest.size; i++)
	{
		encodedTest.bits[i] = correctModulo(encodedTest.bits[i] + forcedErr.bits[i], 2);
	}

	printf_s("Error added to encoded: \n");
	print(&(encodedTest.bits), encodedTest.size);



	//////////////////////////////////////////
	//Hamming decoding
	//////////////////////////////////////////
	bitString decodedTest = hammingDecoding(&encodedTest);
	


	//////////////////////////////////////////
	//Print resulting combined address and command
	//////////////////////////////////////////
	printf_s("Decoded: \n");
	print(&(decodedTest.bits), decodedTest.size);
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


bitString combineBitArrays(int* bits1, int size1, int* bits2, int size2) //Generating a new bitString by append one bitString to another
{
	bitString combined;
	combined.size = size1 + size2;
	
	for (int i = 0; i < size1; i++)
	{
		combined.bits[i] = bits1[i];
	}
	for (int j = 0; j < size2; j++)
	{
		combined.bits[j + size1] = bits2[j];
	}

	return combined;
}

int correctModulo(int x, int m) //Modulo in C is not entirely correct, this implementation fixes that
{
	return (x % m + m) % m;
}


bitString hammingEncoding(bitString* str) // (15,11) Hamming encoding
{
	//bitString for encoded message
	bitString encoded;
	encoded.size = 15;

	//Original message
	for (int i = 0; i < str->size; i++)
	{
		encoded.bits[i] = str->bits[i];
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
		p1 += encoded.bits[checkP1[i] - 1];
	}

	//Parity check p2
	for (int i = 0; i < 7; i++)
	{
		p2 += encoded.bits[checkP2[i] - 1];
	}

	//Parity check p3
	for (int i = 0; i < 7; i++)
	{
		p3 += encoded.bits[checkP3[i] - 1];
	}

	//Parity check p4
	for (int i = 0; i < 7; i++)
	{
		p4 += encoded.bits[checkP4[i] - 1];
	}


	//Add the parity bits
	encoded.bits[11] = correctModulo(p1, 2);
	encoded.bits[12] = correctModulo(p2, 2);
	encoded.bits[13] = correctModulo(p3, 2);
	encoded.bits[14] = correctModulo(p4, 2);
		
	//Return the encoded message
	return encoded;
}


bitString hammingDecoding(bitString* str) // (15,11) Hamming decoding
{
	//bitString for output
	bitString decoded;
	decoded.size = 11;

	//Original message, before error-correction
	for (int i = 0; i < decoded.size; i++)
	{
		decoded.bits[i] = str->bits[i];
	}


	//Indexes as they match each checkSum.
	int check1[7] = { 1, 2, 4, 5, 7, 9, 11 };
	int check2[7] = { 1, 3, 4, 6, 7, 10, 11 };
	int check3[7] = { 2, 3, 4, 8, 9, 10, 11 };
	int check4[7] = { 5, 6, 7, 8, 9, 10, 11 };

	//The checkSums, initialized with their parity value
	int checkSum1 = str->bits[11];
	int checkSum2 = str->bits[12];
	int checkSum3 = str->bits[13];
	int checkSum4 = str->bits[14];


	//Adding up the checksums
	for (int i = 0; i < 7; i++)
	{
		checkSum1 += str->bits[check1[i] - 1];
	}

	for (int i = 0; i < 7; i++)
	{
		checkSum2 += str->bits[check2[i] - 1];
	}
		
	for (int i = 0; i < 7; i++)
	{
		checkSum3 += str->bits[check3[i] - 1];
	}
		
	for (int i = 0; i < 7; i++)
	{
		checkSum4 += str->bits[check4[i] - 1];
	}


	//Modulo 2 of checksums to get bool values, if all 0 then no errors, if all but one is 0, then error is in parity bits and we can ignore
	checkSum1 = correctModulo(checkSum1, 2);
	checkSum2 = correctModulo(checkSum2, 2);
	checkSum3 = correctModulo(checkSum3, 2);
	checkSum4 = correctModulo(checkSum4, 2);

	int errorCount = 0; // Error counter, may be removed

	//Array of errors found
	int error[11] = { 0 }; 

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
		decoded.bits[i] = correctModulo(decoded.bits[i] + error[i], 2);
	}

	//Return decoded and error corrected message
	return decoded;
}