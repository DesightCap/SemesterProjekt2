#include <iostream>
#include <string>

#include "UART.h"

using namespace std;


// Man burde kunne cleare Rx bufferen via CancelIo(handleToCOM)



UART::UART(char* portName)
{

	errors = 0;
	status = { 0 };
	connected = false;

	// Opretter og åbner COM I/O device. Retunere et handle til COM device
	handleToCOM = CreateFileA(static_cast<LPCSTR>(portName), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// Fejlhåndtering // DWORD bruger WIN32 API funktioner til at få den sidste fejl i en interger værdi.
	DWORD errMsg = GetLastError();

	// Fejlkode 2 i DWORD er ERROR_FILE_NOT_FOUND. 
	// Fejlkode 5 i DWORD er ERROR_ACCESS_DENIED.
	// Det er de eneste vi er interesseret i. 
	if (errMsg == 2)
	{
		cout << "Tilslut kommunikations enhed. " << endl;
	}
	else if (errMsg == 5)
	{
		cout << "Kommunikations enheden er allerde i brug. " << endl;
	}
	else if (errMsg == 0)
	{
		// DCB er "Device Control Block", en stuktur til at holde COM port indstillinger 
		DCB dcbSerialParameters = { 0 };

		if (!GetCommState(handleToCOM, &dcbSerialParameters))
		{
			cout << "Det lykkedes ikke at få de nuværende seriel parametre. " << endl;
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
				cout << "Advarsel: Seriel port parametres kunne ikke sættes. " << endl;
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
	if (connected == true)
	{
		connected = false;
		CloseHandle(handleToCOM);
	}
}

int UART::initUART(char* port_)
{
	// Det ser ud til at settings bliver sat i constructor, så jeg tvivler på at denne er nødvendig



	return 0;
}

int UART::getTemp(char* buffer, unsigned int buf_size)
{
	DWORD bytesRead;
	unsigned int toRead = 0;

	ClearCommError(handleToCOM, &errors, &status);

	// Vi finder ud af hvor meget skal indlæses i ReadFile() metoden efterfølgende
	// cbInQue er antallet af bytes modtaget i seriel port, men endnu ikke læst af ReadFile operationen,
	// Forbered til at læse alle tilgængelige bytes, uden af overskride det forspurgte antal bytes. 
	if (status.cbInQue > 0)
	{
		if (status.cbInQue > buf_size)
		{
			toRead = buf_size;
		}
		else toRead = status.cbInQue;
	}

	// Indlæs antal ("toRead") bytes ind i "buffer" og retuner antallet af bytes der er blevet læst
	if (ReadFile(handleToCOM, buffer, toRead, &bytesRead, NULL))
	{
		return bytesRead;
	}
	
	return 0;
}

void UART::sendOp()
{

}

void UART::sendNed()
{
}


bool UART::send(char* buffer, unsigned int buf_size)
{
	DWORD bytesWritten  = 0;
	unsigned int toWrite = OUTPUT_DATA_BYTES;

	bool Status = WriteFile(handleToCOM, buffer, toWrite, &bytesWritten, NULL);
	
	

	return Status;
}

char UART::getChar()
{
	return 0;
}

bool UART::isConnected()
{
	return connected;
}
