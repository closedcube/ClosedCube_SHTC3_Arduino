/*

Arduino library for Arduino library for Sensirion SHTC3 Digital Humidity and Temperature Sensor
version 2020.5.9

---

Copyright (c) 2020, ClosedCube
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation and/or 
   other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors 
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef CLOSEDCUBE_SHTC3_ARDUINO_H
#define CLOSEDCUBE_SHTC3_ARDUINO_H

#include "ClosedCube_I2C.h"

namespace ClosedCube
{

namespace Sensor
{

typedef enum
{
    SHTC3_NO_ERROR = 0,
    SHTC3_CRC_ERROR = -101,
    SHTC3_TIMEOUT_ERROR = -102,

    SHTC3_PARAM_WRONG_MODE = -501,
    SHTC3_WIRE_I2C_DATA_TOO_LOG = -10,
    SHTC3_WIRE_I2C_RECEIVED_NACK_ON_ADDRESS = -20,
    SHTC3_WIRE_I2C_RECEIVED_NACK_ON_DATA = -30,
    SHTC3_WIRE_I2C_UNKNOW_ERROR = -40
} SHTC3_ErrorCode;

struct SHTC3_Result
{
    float t;
    float rh;
    uint8_t error;
};

class SHTC3
{

public:
    SHTC3();
    SHTC3(uint8_t address);
    SHTC3(ClosedCube::Driver::I2CDevice device);

    void address(uint8_t address);
    void softReset();

    void sleep();
    void wakeUp();

    uint16_t readId();

    SHTC3_Result readTempAndHumidity();

private:
    ClosedCube::Driver::I2CDevice _sensor;

    uint8_t calculateCrc(uint8_t data[], short start, short end);

    float calculateHumidity(uint16_t rawValue);
    float calculateTemperature(uint16_t rawValue);
};
} // namespace Sensor
} // namespace ClosedCube

#endif
