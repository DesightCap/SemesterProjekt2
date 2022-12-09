/*
 Copyright © 2014 José Luis Zabalza  License LGPLv3+: GNU
 LGPL version 3 or later <http://www.gnu.org/copyleft/lgpl.html>.
 This is free software: you are free to change and redistribute it.
 There is NO WARRANTY, to the extent permitted by law.
*/


#include <Wire.h>
#include "lm75.h"
#include <Arduino.h>


//-------------------------------------------------------------------------------
TempI2C_LM75::TempI2C_LM75( uint8_t i2c_addr, Resolution resolution)
{
    Wire.begin();

    m_u8I2CAddr = i2c_addr;
}

//-------------------------------------------------------------------------------
float TempI2C_LM75::getTemp()
{
    union //The purpose of union is to save memory by using the same memory region for storing different objects at different times.
    { //meaning the the value for tempX == tempS;
        unsigned short tempX; // == for referance, an is a 16 bits of ranges [0, 65,535]
        short tempS; //== for referance, a short int or short is equal to 16 and indicates a range of [-32,767; 32,767]. ==
        //           //== this needs clarafication: signed 16 =>                                                         ==
        //             == 0b01111111 11111111 equals  32,767 int                                                         ==
        //             == 0b00111111 11111111 equals  16,383 int                                                         == 
        //             == 0b00011111 11111111 equals   8,191 int                                                         == 
        //             == 0b00001111 11111111 equals   4,095 int                                                         == 
        //             == 0b10000000 00000000 equals -32,768 int                                                         == 
        //             == 0b11000000 00000000 equals -16,384 int                                                         == 
        //             == 0b11100000 00000000 equals - 8,192 int                                                         == 
        //             == 0b11110000 00000000 equals - 4,095 int                                                         ==
    } temperature;

    temperature.tempX = getReg(temp_reg);
    return (temperature.tempS / 256.0F);
    //
}

//-------------------------------------------------------------------------------
unsigned TempI2C_LM75::getReg(LM75Register reg)
{
    unsigned Result = 0xFFFF; // meaning Result == 65534;

#ifdef DEBUG
    Serial.print("getReg"); Serial.println(uint8_t(reg),HEX);
#endif
    // == m_u8I2CAddr is a pravit uint8_t object. why it is named so is beyond me ==.
    Wire.beginTransmission(m_u8I2CAddr);
    Wire.write(reg); // pointer reg
    Wire.endTransmission();

    uint8_t c;  

    Wire.requestFrom(m_u8I2CAddr, uint8_t(2));
    if(Wire.available())
    {
        c = Wire.read(); // == the unit_8 c object is assigned to what the Wire.read, reads ==
        Result = c;      // == assigns Result to c ==
        if(reg != config_reg)
        {
            Result = (unsigned(c))<<8; //== bitShiftes (unsiged converted c) 8 bits to the left ==
            if(Wire.available())       //== for referance, Result is a 32bits                   ==
            {
                c = Wire.read(); //== Wire.read() returns the next byte received so c is assigned to whatever bytes read ==
                //                 == for Referance, a byte is equel to 8 bits.                                          ==
                Result |= (unsigned(c)); //== here the result is OR-assigend to C (unsigend converted) ==
                //                       //== to be clear: C is 0bxxxxxxxx bits. the convertion changes==
                //                         == It to: 0b00000000 00000000
                //                                     00000000 xxxxxxxx or 0xFFFF FFFF                ==
            }
            else
            {
#ifdef DEBUG
                Serial.println("Error ");
#endif
                Result = 0xFFFF;
            }
        }
    }
#ifdef DEBUG
    else
        Serial.println("Error");
#endif


    return(Result);
}
//-------------------------------------------------------------------------------
void TempI2C_LM75::setReg(LM75Register reg, unsigned newValue)
{
#ifdef DEBUG
    Serial.print("setReg"); Serial.println(uint8_t(reg),HEX);
#endif
    Wire.beginTransmission(m_u8I2CAddr);
    Wire.write(reg); // pointer reg
    if(reg != config_reg)
    {
        Wire.write((newValue>>8) & 0xFF);
    }
    Wire.write(newValue & 0xFF);

    Wire.endTransmission();
}

//-------------------------------------------------------------------------------
void TempI2C_LM75::setTHyst(float newTHyst)
{
    setReg(THyst_reg,int(newTHyst * 256));
}

//-------------------------------------------------------------------------------
void TempI2C_LM75::setTOS(float newTOS)
{
    setReg(TOS_reg,int(newTOS * 256) );
}

//-------------------------------------------------------------------------------
float TempI2C_LM75::getTHyst(void)
{
    return(int(getReg(THyst_reg)) / 256.0F);
}

//-------------------------------------------------------------------------------
float TempI2C_LM75::getTOS(void)
{
    return(int(getReg(TOS_reg)) / 256.0F);
}

//-------------------------------------------------------------------------------
TempI2C_LM75::TermostatMode TempI2C_LM75::getTermostatMode()
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);

    return(TermostatMode(regv.mbits.termostat_mode));
}

//-------------------------------------------------------------------------------
void TempI2C_LM75::setTermostatMode(TempI2C_LM75::TermostatMode newMode)
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);
    regv.mbits.termostat_mode = newMode;

    setReg(config_reg,unsigned(regv.mbyte));
}

//-------------------------------------------------------------------------------
TempI2C_LM75::TermostatFaultTolerance TempI2C_LM75::getTermostatFaultTolerance()
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);

    return(TermostatFaultTolerance(regv.mbits.termostat_fault_tolerance));
}

//-------------------------------------------------------------------------------
void TempI2C_LM75::setTermostatFaultTolerance(TermostatFaultTolerance newFaultTolerance)
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);
    regv.mbits.termostat_fault_tolerance = newFaultTolerance;

    setReg(config_reg,unsigned(regv.mbyte));
}

//-------------------------------------------------------------------------------
TempI2C_LM75::Resolution TempI2C_LM75::getResolution()
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);

    return(Resolution(regv.mbits.resolution));
}
//-------------------------------------------------------------------------------
void TempI2C_LM75::setResolution(Resolution newResolution)
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);
    regv.mbits.resolution = newResolution;

    setReg(config_reg,unsigned(regv.mbyte));
}

//-------------------------------------------------------------------------------
TempI2C_LM75::OSPolarity TempI2C_LM75::getOSPolarity()
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);

    return(OSPolarity(regv.mbits.termostat_output_polarity));
}

//-------------------------------------------------------------------------------
void TempI2C_LM75::setOSPolarity(OSPolarity newOSPolarity)
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);
    regv.mbits.termostat_output_polarity = newOSPolarity;

    setReg(config_reg,unsigned(regv.mbyte));
}

