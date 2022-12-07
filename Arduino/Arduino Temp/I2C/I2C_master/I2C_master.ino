// ================ I2C master ================
// from https://microcontrollerslab.com/lm75-temperature-sensor-module-pinout-interfacing-with-arduino/
#include <inttypes.h>
#include <Wire.h>
#include <lm75.h>

float HF(float _t){
  return (floor((_t*2)+0.5)/2);
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
    Serial.print(HF(Temperature.getTemp()));
    Serial.println(" oC");
    delay(2000);
}