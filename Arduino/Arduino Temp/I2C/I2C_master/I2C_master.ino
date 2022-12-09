// ================ I2C master ================
// from https://microcontrollerslab.com/lm75-temperature-sensor-module-pinout-interfacing-with-arduino/
#include <inttypes.h>
#include <Wire.h>
#include <lm75.h>
#include <avr/io.h>
#include "../../../x10/x10.h"
 #include "utility/twi.h"

float HF(float _t){
  return (floor((_t*2)+0.5)/2);
}

byte sixBitsSend(double inputTemp_)
{
  byte Rval = 0b0000000;
  int Temp = inputTemp_;

  if((inputTemp_ - Temp) > 0)
  {
    Rval |= 0b00000001;
  }
  Rval |= (byte(Temp)<<1);
  return Rval;
}


TempI2C_LM75 Temperature = TempI2C_LM75(0x48,   TempI2C_LM75::nine_bits);
//                                      ^           ^              
//                                      |           |
//                                      Under the constructer for TempI2C this value
//                                        is 72 in HEX. but it is also the I2C_addr.
//                                                  |
//                                                  this is surpossed to be the 
//                                                    resolution enum.
 
void setup()
{
  	DDRA = 0xFF;
  //Wire.beginTransmission(0b01001111);
  Serial.begin(9600);
  Serial.println("Start");
  Serial.print("Actual temp ");
  Serial.print(Temperature.getTemp());
  Serial.println(" oC");
  delay(2000);
  
}
 
void loop()
{
    uint16_t helperfuntion = (sixBitsSend(HF(Temperature.getTemp())));
    Serial.print(HF(Temperature.getTemp()));
    Serial.print(" ");
    Serial.print(helperfuntion,BIN);
    Serial.println(" oC");
    uint16_t address = 3;
    uint16_t combined_ = 0;  
    uint16_t encoded_ = 0;  
    uint32_t datapakke_ = 0;  
    volatile int counter_ = 100;  
    sendx10(&address, &helperfuntion, &combined_, &encoded_, &datapakke_, &counter_);
    delay(1000);
}