#include <iostream>
#include <string>
#include <string.h>
#include "UART.h"

#define deviceInUse 5
#define notConnected 2
#define noError 0

using namespace std;


// Man burde kunne cleare Rx bufferen via CancelIo(handleToCOM)



UART::UART(char* portName)
{


	errors = 0;
	status = { 0 };
	connected = false;

	// Opretter og �bner COM I/O device. Retunere et handle til COM device
	handleToCOM = CreateFileA(static_cast<LPCSTR>(portName), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// Fejlh�ndtering // DWORD bruger WIN32 API funktioner til at f� den sidste fejl i en interger v�rdi.
	DWORD errMsg = GetLastError();
	
	// Fejlkode 2 i DWORD er ERROR_FILE_NOT_FOUND. 
	// Fejlkode 5 i DWORD er ERROR_ACCESS_DENIED.
	// Det er de eneste vi er interesseret i. 
	if (errMsg == notConnected)
	{
		cout << "Tilslut kommunikations enhed. " << endl;
	}
	else if (errMsg == deviceInUse)
	{
		cout << "Kommunikations enheden er allerde i brug. " << endl;
	}
	else if (errMsg == noError)
	{
		// DCB er "Device Control Block", en stuktur til at holde COM port indstillinger 
		DCB dcbSerialParameters = { 0 };

		if (!GetCommState(handleToCOM, &dcbSerialParameters))
		{
			cout << "Det lykkedes ikke at f� de nuv�rende seriel parametre. " << endl;
		}
		else
		{
			dcbSerialParameters.BaudRate = CBR_9600;
			dcbSerialParameters.ByteSize = 8;
			dcbSerialParameters.StopBits = ONESTOPBIT;
			dcbSerialParameters.Parity = NOPARITY;
			dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(handleToCOM, &dcbSerialParameters))
			{
				cout << "Advarsel: Seriel port parametres kunne ikke s�ttes. " << endl;
			}
			else
			{
				connected = true;
				PurgeComm(handleToCOM, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(ARDUINO_WAIT_TIME);
			}
		}
	}
}

UART::~UART()
{
	if (connected)
	{
		connected = false;
		CloseHandle(handleToCOM);
	}
}



int UART::getTemperature(char* buffer, unsigned int buf_size)
{
	DWORD bytesRead;
	unsigned int toRead = 0;

	ClearCommError(handleToCOM, &errors, &status);

	// Vi finder ud af hvor meget skal indl�ses i ReadFile() metoden efterf�lgende
	// cbInQue er antallet af bytes modtaget i seriel port, men endnu ikke l�st af ReadFile operationen,
	// Forbered til at l�se alle tilg�ngelige bytes, uden af overskride det forspurgte antal bytes. 
	if (status.cbInQue > 0)
	{
		if (status.cbInQue >= buf_size)
		{
			toRead = buf_size;
		}
		else
		{
			toRead = status.cbInQue;
		}
	}

	// Indl�s antal ("toRead") bytes ind i "buffer" og retuner antallet af bytes der er blevet l�st
	if (ReadFile(handleToCOM, buffer, toRead, &bytesRead, NULL))
	{
		CancelIo(handleToCOM);
		return bytesRead;
	}

	return 0;
}

void UART::sendUp()
{
	char up = 'u';
	send(&up, 1);
}

void UART::sendDown()
{
	char down = 'd';
	send(&down, 1);
}


bool UART::send(char* buffer, unsigned int buf_size)
{
	DWORD bytesWritten  = 0;
	unsigned int toWrite = OUTPUT_DATA_BYTES;

	


	if (!WriteFile(handleToCOM, buffer, toWrite, &bytesWritten, NULL))
	{
		ClearCommError(handleToCOM, &errors, &status);
		return false;
	}

	return true;

}


bool UART::isConnected()
{
	return connected;
}