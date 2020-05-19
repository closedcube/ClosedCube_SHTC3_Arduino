/*

Example: shtc3demo

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

#define UART_BAUD 9600
#define SHTC3_I2C_ADDRESS 0x70

ClosedCube::Sensor::SHTC3 shtc3;
ClosedCube::Sensor::SHTC3_Result result;

void setup()
{
    Serial.begin(UART_BAUD);
    Wire.begin();

    Serial.println("ClosedCube SHTC3 Arduino Demo ");
    shtc3.address(SHTC3_I2C_ADDRESS); // I2C Address: 0x70

    shtc3.wakeUp();

    Serial.print("Serial # 0x");
    Serial.println(shtc3.readId(), HEX);
}

void loop()
{
    result = shtc3.readTempAndHumidity();

    if (result.error == 0)
    {
        Serial.print("T=");
        Serial.print(result.t);
        Serial.print("C, RH=");
        Serial.print(result.rh);
        Serial.println("%");
    }
    else
    {
        Serial.print(": [ERROR] Code #");
        Serial.println(result.error);
    }

    shtc3.sleep();
    delay(1000); // 1 second delay
    shtc3.wakeUp();
}

