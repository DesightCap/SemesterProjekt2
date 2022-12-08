#include "Log.h"

Log::Log(string filename)
{
	filename_ = filename; // Husk .txt for tekstfil 
}

void Log::print() // Idskriver hele log
{
	string myReading;
	ifstream MyFile;
	MyFile.open(filename_);

	while (getline(MyFile, myReading))
	{
		cout << "Log indslag: " << myReading << endl;
	}

	MyFile.close();

}

void Log::addToLog(double temperature) // Tilf�jer l�sning til log
{

	time_t currentTime = time(NULL);
	struct tm localTime;

	time(&currentTime);
	//localTime = localtime(&currentTime); 
	errno_t errTme = localtime_s(&localTime, &currentTime);

	//cout << localTime->tm_mon; // Test time functions by printing to terminal
	if (errTme == 0)
	{
		ofstream MyFile;
		MyFile.open(filename_, ios_base::app);
		MyFile
			<< "Tidspunkt: " << setfill('0')		// udfyld med '0', s� fx. et sekund udskrives som 01 i stedet for 1
			<< setw(2) << localTime.tm_hour << ':'	// Skriver time som to tal til fil
			<< setw(2) << localTime.tm_min << ':'	// Skriver minut som to tal til fil
			<< setw(2) << localTime.tm_sec 			// Skriver time som to tal til fil
			<< " Dato: "
			<< localTime.tm_mday << "/" << localTime.tm_mon + 1 << "/" << localTime.tm_year + 1900
			<< " temperatureeratur: " << temperature
			<< endl;
		MyFile.close();
	}
	else
	{
		ofstream MyFile;
		MyFile.open(filename_, ios_base::app);
		MyFile << "Fejl i hent tid. " << endl;
		MyFile.close();
	}
}
