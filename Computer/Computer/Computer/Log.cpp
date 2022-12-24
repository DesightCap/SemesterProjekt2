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

void Log::addToLog(double temperature) // Tilføjer læsning til log
{
	time_t currentTime = time(NULL);
	struct tm localTime;

	time(&currentTime);
	errno_t errTme = localtime_s(&localTime, &currentTime);
	// Hvis errTme == 0 er det lykkedes at finde tiden. Ellers er der sket en fejl.
	if (errTme == 0)
	{
		ofstream MyFile;
		MyFile.open(filename_, ios_base::app);
		MyFile
			<< "Tidspunkt: " << setfill('0')		// udfyld med '0', så fx. et sekund udskrives som 01 i stedet for 1
			<< setw(2) << localTime.tm_hour << ':'	// Skriver time som to tal til fil
			<< setw(2) << localTime.tm_min << ':'	// Skriver minut som to tal til fil
			<< setw(2) << localTime.tm_sec 			// Skriver time som to tal til fil
			<< " Dato: "
			<< localTime.tm_mday << "/" << localTime.tm_mon + 1 << "/" << localTime.tm_year + 1900 // "dd/mm/åå" // måned [0..11] && år er år siden 1900
			<< " temperatur: " << temperature
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
