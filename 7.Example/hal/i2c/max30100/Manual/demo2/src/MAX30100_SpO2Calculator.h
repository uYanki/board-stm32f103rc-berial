/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAX30100_SPO2CALCULATOR_H
#define MAX30100_SPO2CALCULATOR_H

#include <stdint.h>

#define CALCULATE_EVERY_N_BEATS         3

class SpO2Calculator {
public:
    SpO2Calculator();

    void update(float irACValue, float redACValue, bool beatDetected);
    void reset();
    uint8_t getSpO2();

private:
    static const uint8_t spO2LUT[43];

    float irACValueSqSum;
    float redACValueSqSum;
    uint8_t beatsDetectedNum;
    uint32_t samplesRecorded;
    uint8_t spO2;
};

#endif
