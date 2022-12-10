/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>

float fetchData(uint8_t adress);

int main(void)
{
    CyGlobalIntEnable; // Enable global interrupts.
    
	
	I2C_LM75_Start(); // Klargøre I2C 
	// Fra datablad: 
	// You must set up the I2C Slave buffers before this function call to avoid reading or writing partial data while the buffers are setting up.
	
	UART_TERMINAL_Start(); // Klargøre UART
	
	float temp = 0; // Variable til opbevaring af beregnet temperatur data
	
	
	char buffer[256]; // Chart array til udskrivning af temperatur data
	uint8_t status = 0; // Til tjek om I2C er klar
	

	/*
	Adresse opsætning: 
	Adressens første fire bit er faste
	De sidste fire bit er hhv. 3 bit indstillet på LM75 kredsløb og et bit som fortæller om du skriver eller læser
	1001xxxy
	x = valgt via den røde kasse på LM75
	y = 1 for læs || 0 for skriv
	y sættes i funktionskald.
	Eksempel: I2C_LM75_MasterSendStart(0x48, 1) 
	*/
	
	uint8_t slaveAddr1 = 0x48; // 1 == on; 2 == on; 3 == 0n -> 0b01001000 
	//uint8_t slaveAddr2 = 0x4F; // 1 == off; 2 == off; 3 == 0ff -> 0b01001111

	
    for(;;)
    {
		// Hent temperatur data fra parameter adresse
		temp = fetchData(slaveAddr1);
		
		uint8_t whole = (int)temp;
		uint8_t decimal = temp - whole;
		
		whole = whole << 1;
		if (decimal)
			whole++;
		
		
		//snprintf(buffer, sizeof(buffer), "%.1f\r\n", whole);
		//UART_TERMINAL_PutString((char)whole);
		UART_TERMINAL_PutChar((char)whole);
		CyDelay(2000);
		/*
		// Klargøre buffer til udskrive med adresse på I2C enhed og float temperatur
		snprintf(buffer, sizeof(buffer), "Enhed med adresse %u har temperatur: %.1f \r\n", slaveAddr1, dec);
		UART_TERMINAL_PutString(buffer); 

		// Hent temperatur data fra parameter adresse
		dec = fetchData(slaveAddr2);
		// Klargøre buffer til udskrive med adresse på I2C enhed og temperatur float
		snprintf(buffer, sizeof(buffer), "Enhed med adresse %u har temperatur: %.1f \r\n", slaveAddr2, dec);
		UART_TERMINAL_PutString(buffer);
		
		UART_TERMINAL_PutString("\r\n");
		*/
		// Forsinkelse mellem data hentning og udskrift
		CyDelay(2000);
    }
}

// Henter temperatur data fra LM75, omregner anden komplement til float værdi og behandler data og returnere float med beregnet data
float fetchData(uint8_t adress)
{
	uint8_t recievedValue1 = 0;
	uint8_t recievedValue2 = 0;
	// starter dataoverførsel og sikre at der ikke er fejl 
	uint8_t status = I2C_LM75_MasterSendStart(adress, 1);
	if (I2C_LM75_MSTR_NO_ERROR == status)
		{
			// Første modtagede byte gemmes i recievedValue1 og anden i recievedValue2
			// Rå data
		    recievedValue1 = I2C_LM75_MasterReadByte(I2C_LM75_ACK_DATA); // Indeholder temperatur heltals data 
			recievedValue2 = I2C_LM75_MasterReadByte(I2C_LM75_NAK_DATA); // Indeholder temperatur komma
			I2C_LM75_MasterSendStop(); // Dataoverførsel slut
			
			// Vi indsætter vores først modtagede byte i dec variablen
			float dec = recievedValue1; // Til behandling af data
			
			// Vi undersøger om recievedValue1 (som er to's komplements binær) er et negativt tal
			if (recievedValue1 >> 7)  
			{
				// recievedValue1 omregnes fra to-komplement til decimal og gemmes i variablen dec
			    dec = (~recievedValue1+1); // omregn 2-komp -> decimal
				
			}
			
			// Herunder One-line databeregning - Potentilt mere effektiv (dekompiler for at teste effektivitet)
			//float combinedValue = ((((~recievedValue1+1) << 8) + (recievedValue2)) >> 7)/2; 
			
			// vi er intresseret i om MSB i recievedValue2 er forskellig fra 0
			if ((recievedValue2>>7) != 0) 
			{
			    // Hvis MSB i recievedValue2 byte er forskellig fra 0 tilføjes en halv grad til float variablen dec. 
				dec += 0.5;
			}
			//  Data beregning slut
			return dec;
			
		}
		else
		{
			I2C_LM75_MasterSendStop();
			return 0;
		}
}

/*
// Funktion fungere ikke og er derfor fjernet. Implementeret direkte i main. 
// Udskriver "Enh". Kan evt. fixes ved at passe sizeof(buffer) i stedet for at gøre det i snprintf
void printData(uint8_t adress, float* decPtr, char* buffer)
{
	snprintf(&buffer, sizeof(buffer), "Enhed med adresse %u har temperatur: %.1f \r\n", adress, *decPtr);
	UART_TERMINAL_PutString(&buffer);
}
*/
/*TO DO*/
/*
Hvad er LM75's adresse? - ox48 - Implementeret
	1001xxxy
	x = den røde kasse på LM75
	y = 1 for læs || 0 for skriv
Hvordan virker I2C? - Traelst! - Implementeret
	1: I2C_Start()
	2: Hit ud af at refere til slave, ack og nack, andre ting som skal sendes?
	3: I2C_MasterReadByte() eller I2C_MasterReadBuf() (<- ???)
Hvordan virker buffer? (Nemmeste måde at kommunikere?) - Just barely - Implementeret
	buffer er et char array
Klip og klister 2 * 8 bit fra LM75 - Implementeret (Tjek op på udregning - 2 steder med + 0.5)
Bereng 2nd complement - Implementeret
Beregn temperatur - Implementeret
Float to string (Kan nok ikke typecaste) - snprintf er bedre end det herunder - Implementeret
	læs op på gcvt(): https://www.geeksforgeeks.org/gcvt-convert-float-value-string-c/ 
	gcvt(float værdi, int antalTegnIFloat, char * buffer)
		float value : It is the float or double value.
		int ndigits : It is number of digits.
		char * buf : It is character pointer, in this 
		variable string converted value will be copied.
Kald flere temp enheder
Flå kode over i funktioner
*/
/* [] END OF FILE */
