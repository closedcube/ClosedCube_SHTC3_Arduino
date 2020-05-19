/*

Arduino library for Arduino library for Sensirion SHTC3 Digital Humidity and Temperature Sensor
version 2020.5.19

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

#include "ClosedCube_SHTC3.h"

ClosedCube::Sensor::SHTC3::SHTC3()
{
}

ClosedCube::Sensor::SHTC3::SHTC3(ClosedCube::Driver::I2CDevice device) : _sensor(device)
{
}

ClosedCube::Sensor::SHTC3::SHTC3(uint8_t address)
{
    _sensor.address(address);
}

void ClosedCube::Sensor::SHTC3::address(uint8_t address)
{
    _sensor.address(address);
}

void ClosedCube::Sensor::SHTC3::softReset()
{
    _sensor.writeWord(0x805D);
}

void ClosedCube::Sensor::SHTC3::sleep()
{
    _sensor.writeWord(0xB098);
    delay(15);
}

void ClosedCube::Sensor::SHTC3::wakeUp()
{
    _sensor.writeWord(0x3517);
    delay(15);
}

uint16_t ClosedCube::Sensor::SHTC3::readId()
{
    uint16_t result = 0x00;

    _sensor.writeWord(0xEFC8);
    delay(15);
    byte buf[3];

    if (_sensor.lastErrorCode() == 0)
    {
        _sensor.readBytes(buf, 3);
        if (calculateCrc(buf, 0, 2) == buf[2])
        {
            result = (buf[0] << 8) | buf[1];
        }
    }

    return result;
}

ClosedCube::Sensor::SHTC3_Result ClosedCube::Sensor::SHTC3::readTempAndHumidity()
{
    SHTC3_Result result;
    result.error = -1;

    _sensor.writeWord(0x7866, false);
    delay(20);

    uint8_t buf[6];
    if (_sensor.lastErrorCode() == 0)
    {
        _sensor.readBytes(buf, 6);

        if (calculateCrc(buf, 0, 2) == buf[2] && calculateCrc(buf, 3, 5) == buf[5])
        {
            result.t = calculateTemperature((buf[0] << 8) | buf[1]);
            result.rh = calculateHumidity((buf[3] << 8) | buf[5]);
            result.error = 0;
        }
    }

    return result;
}

float ClosedCube::Sensor::SHTC3::calculateTemperature(uint16_t rawValue)
{
    return 175.0f * (float)rawValue / 65535.0f - 45.0f;
}

float ClosedCube::Sensor::SHTC3::calculateHumidity(uint16_t rawValue)
{
    return 100.0f * (float)rawValue / 65535.0f;
}

uint8_t ClosedCube::Sensor::SHTC3::calculateCrc(uint8_t data[], short start, short end)
{
    uint8_t bit;
    uint8_t crc = 0xFF;
    short i = start;

    for (; i < end; i++)
    {
        crc ^= (data[i]);
        for (bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ 0x131;
            }
            else
            {
                crc = (crc << 1);
            }
        }
    }

    return crc;
}
