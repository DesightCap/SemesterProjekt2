/*
 * HelperHeader.h
 *
 * Created: 09-12-2022 12:00:13
 *  Author: Useless
 */ 


#ifndef HELPERHEADER_H_
#define HELPERHEADER_H_

#define BUFFER_LENGTH 32
		  // == from twi libary ==
		  #ifndef TWI_BUFFER_LENGTH
		  #define TWI_BUFFER_LENGTH 32
		  #endif
		  
		  #define TWI_READY 0
		  #define TWI_MRX   1
		  #define TWI_MTX   2
		  #define TWI_SRX   3
		  #define TWI_STX   4
		  
		  static volatile uint8_t twi_state;
		  static volatile uint8_t twi_slarw;
		  static volatile uint8_t twi_sendStop;			// should the transaction end with a stop
		  static volatile uint8_t twi_inRepStart;			// in the middle of a repeated start
		  
		  static uint8_t twi_masterBuffer[TWI_BUFFER_LENGTH];
		  static volatile uint8_t twi_masterBufferIndex;
		  static volatile uint8_t twi_masterBufferLength;

		  static uint8_t twi_txBuffer[TWI_BUFFER_LENGTH];
		  static volatile uint8_t twi_txBufferIndex;
		  static volatile uint8_t twi_txBufferLength;

		 // static uint8_t twi_rxBuffer[TWI_BUFFER_LENGTH];
		  static volatile uint8_t twi_rxBufferIndex;

		  static volatile uint8_t twi_error;
		  
					  /*@{*/
			/** \ingroup util_twi
				\def TW_READ
				SLA+R address */
			#define TW_READ		1

			/** \ingroup util_twi
				\def TW_WRITE
				SLA+W address */
			#define TW_WRITE	0
			/*@}*/
			
			#define TWDR    _SFR_MEM8(0xBB)

			#define TWCR    _SFR_MEM8(0xBC)
			#define TWINT   7
			#define TWEA    6
			#define TWSTA   5
			#define TWSTO   4
			#define TWWC    3
			#define TWEN    2
			#define TWIE    0

			#define TWAMR   _SFR_MEM8(0xBD)
			#define TWAM6   7
			#define TWAM5   6
			#define TWAM4   5
			#define TWAM3   4
			#define TWAM2   3
			#define TWAM1   2
			#define TWAM0   1
			
			#define TW_MT_SLA_NACK		0x20
			#define TW_MT_DATA_NACK		0x30

		  // == / from twi libary ==
// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1


#include <inttypes.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>


uint8_t rxBuffer[BUFFER_LENGTH];
uint8_t rxBufferIndex = 0;
uint8_t rxBufferLength = 0;

uint8_t txAddress = 0;
uint8_t txBuffer[BUFFER_LENGTH];
uint8_t txBufferIndex = 0;
uint8_t txBufferLength = 0;

uint8_t transmitting = 0;
uint8_t m_u8I2CAddr;

  typedef enum  {temp_reg=0, config_reg, THyst_reg, TOS_reg } LM75Register;

  typedef union _CfgRegister
  {
	  struct
	  {
		  uint8_t shutdown:1;
		  uint8_t termostat_mode:1;
		  uint8_t termostat_output_polarity:1;
		  uint8_t termostat_fault_tolerance:2;
		  uint8_t resolution:2;
		  uint8_t reserved:1;
	  } mbits;

	  uint8_t mbyte;

  } CfgRegister;


// from twi.c

uint8_t twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop)
{
	uint8_t i;

	// ensure data will fit into buffer
	if(TWI_BUFFER_LENGTH < length){
		return 1;
	}

	// wait until twi is ready, become master transmitter
	while(TWI_READY != twi_state){
		continue;
	}
	twi_state = TWI_MTX;
	twi_sendStop = sendStop;
	// reset error state (0xFF.. no error occured)
	twi_error = 0xFF;

	// initialize buffer iteration vars
	twi_masterBufferIndex = 0;
	twi_masterBufferLength = length;
	
	// copy data to twi buffer
	for(i = 0; i < length; ++i){
		twi_masterBuffer[i] = data[i];
	}
	
	// build sla+w, slave device address + w bit
	twi_slarw = TW_WRITE;
	twi_slarw |= address << 1;
	
	// if we're in a repeated start, then we've already sent the START
	// in the ISR. Don't do it again.
	//
	if (1 == twi_inRepStart) {

		twi_inRepStart = 0;			// remember, we're dealing with an ASYNC ISR
		TWDR = twi_slarw;
		TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);	// enable INTs, but not START
	}
	else
	// send start condition
	TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE) | _BV(TWSTA);	// enable INTs

	// wait for write operation to complete
	while(wait && (TWI_MTX == twi_state)){
		continue;
	}
	
	if (twi_error == 0xFF)
	return 0;	// success
	else if (twi_error == TW_MT_SLA_NACK)
	return 2;	// error: address send, nack received
	else if (twi_error == TW_MT_DATA_NACK)
	return 3;	// error: data send, nack received
	else
	return 4;	// other twi error
}

uint8_t twi_readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop)
{
	uint8_t i;

	// ensure data will fit into buffer
	if(TWI_BUFFER_LENGTH < length){
		return 0;
	}

	// wait until twi is ready, become master receiver
	while(TWI_READY != twi_state){
		continue;
	}
	twi_state = TWI_MRX;
	twi_sendStop = sendStop;
	// reset error state (0xFF.. no error occured)
	twi_error = 0xFF;

	// initialize buffer iteration vars
	twi_masterBufferIndex = 0;
	twi_masterBufferLength = length-1;  // This is not intuitive, read on...
	// On receive, the previously configured ACK/NACK setting is transmitted in
	// response to the received byte before the interrupt is signalled.
	// Therefor we must actually set NACK when the _next_ to last byte is
	// received, causing that NACK to be sent in response to receiving the last
	// expected byte of data.

	// build sla+w, slave device address + w bit
	twi_slarw = TW_READ;
	twi_slarw |= address << 1;

	if (1 == twi_inRepStart) {
		
		twi_inRepStart = 1;			// remember, we're dealing with an ASYNC ISR
		TWDR = twi_slarw;
		TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);	// enable INTs, but not START
	}
	else
	// send start condition
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);

	// wait for read operation to complete
	while(TWI_MRX == twi_state){
		continue;
	}

	if (twi_masterBufferIndex < length)
	length = twi_masterBufferIndex;

	// copy twi buffer to data
	for(i = 0; i < length; ++i){
		data[i] = twi_masterBuffer[i];
	}
	
	return length;
}

uint8_t twi_transmit(const uint8_t* data, uint8_t length)
{
	uint8_t i;

	// ensure data will fit into buffer
	if(TWI_BUFFER_LENGTH < length){
		return 1;
	}
	
	// ensure we are currently a slave transmitter
	if(TWI_STX != twi_state){
		return 2;
	}
	
	// set length and copy data into tx buffer
	twi_txBufferLength = length;
	for(i = 0; i < length; ++i){
		twi_txBuffer[i] = data[i];
	}
	
	return 0;
}


int available(void)
{
	return rxBufferLength - rxBufferIndex;
}
int read(void)
{
	int value = -1;
	
	// get each successive byte on each call
	if(rxBufferIndex < rxBufferLength){
		value = rxBuffer[rxBufferIndex];
		++rxBufferIndex;
	}

	return value;
}
void beginTransmission(uint8_t address)
{
	// indicate that we are transmitting
	transmitting = 1;
	// set address of targeted slave
	txAddress = address;
	// reset tx buffer iterator vars
	txBufferIndex = 0;
	txBufferLength = 0;
}
uint8_t endTransmission2(uint8_t sendStop)
{
	// transmit buffer (blocking)
	uint8_t ret = twi_writeTo(txAddress, txBuffer, txBufferLength, 1, sendStop);
	// reset tx buffer iterator vars
	txBufferIndex = 0;
	txBufferLength = 0;
	// indicate that we are done transmitting
	transmitting = 0;
	return ret;
}
uint8_t endTransmission(void)
{
	return endTransmission2(1); // == changed from true to 1; ==
}
unsigned int write(uint8_t data)
	{
		if(transmitting){
			if(txBufferLength >= BUFFER_LENGTH){
				// threw error
				return 0;
			}
			// put byte in tx buffer
			txBuffer[txBufferIndex] = data;
			++txBufferIndex;
			// update amount in buffer
			txBufferLength = txBufferIndex;
			}else{
			// in slave send mode
			// reply to master
			twi_transmit(&data, 1);
		}
		return 1;
	}
uint8_t requestFrom3(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
	{
		if (isize > 0) {
			// send internal address; this mode allows sending a repeated start to access
			// some devices' internal registers. This function is executed by the hardware
			// TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

			beginTransmission(address);

			// the maximum size of internal address is 3 bytes
			if (isize > 3){
				isize = 3;
			}

			// write internal register address - most significant byte first
			while (isize-- > 0)
			write((uint8_t)(iaddress >> (isize*8)));
			endTransmission2(0); //== changed from false to 0; ==
		}

		// clamp to buffer length
		if(quantity > BUFFER_LENGTH){
			quantity = BUFFER_LENGTH;
		}
		// perform blocking read into buffer
		uint8_t read = twi_readFrom(address, rxBuffer, quantity, sendStop);
		// set rx buffer iterator vars
		rxBufferIndex = 0;
		rxBufferLength = read;

		return read;
	}
uint8_t requestFrom2(uint8_t address, uint8_t quantity, uint8_t sendStop) {
		return requestFrom3((uint8_t)address, 
						   (uint8_t)quantity, 
						   (uint32_t)0, 
						   (uint8_t)0, 
						   (uint8_t)sendStop);
	}
uint8_t requestFrom1(uint8_t address, uint8_t quantity)
	{
		return requestFrom2((uint8_t)address, (uint8_t)quantity, (uint8_t)1); // == changed fom true to 1 ==
	}

LM75Register getReg(LM75Register reg)
	{
		unsigned Result = 0xFFFF; // meaning Result == 65534;
		// == m_u8I2CAddr is a pravit uint8_t object. why it is named so is beyond me ==.
		beginTransmission(m_u8I2CAddr);
		write(reg); // pointer reg
		endTransmission();
		uint8_t c;
		requestFrom1(m_u8I2CAddr, (2));
		if(available())
		{
			c = read();
			Result = c;
			if(reg != config_reg)
			{
				//== added to help ==
				unsigned HELPc = c;
				//== /added to help ==
			  //Result = ( unsigned(c) )<<8;
				Result = ( HELPc )<<8;
				if(available())
				{
					c = read();
					//== added to help ==
					unsigned HELPc = c;
					//== /added to help ==
				  //Result |= ( unsigned(c) );
				    Result |= ( HELPc );
				}
				else
				{
					Result = 0xFFFF;
				}
			}
		}
		return(Result);
	}

float getTemp()
{
	union
	{
		unsigned short tempX;
		short tempS;

	} temperature;

	temperature.tempX = getReg(temp_reg);
	return (temperature.tempS / 256.0F);
}









#endif /* HELPERHEADER_H_ */




