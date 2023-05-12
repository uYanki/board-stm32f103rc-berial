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

#include <Arduino.h>

#include "MAX30100_PulseOximeter.h"


PulseOximeter::PulseOximeter() :
    state(PULSEOXIMETER_STATE_INIT),
    tsFirstBeatDetected(0),
    tsLastBeatDetected(0),
    tsLastSample(0),
    tsLastBiasCheck(0),
    tsLastCurrentAdjustment(0),
    redLedPower((uint8_t)RED_LED_CURRENT_START),
    onBeatDetected(NULL)
{
}

void PulseOximeter::begin(PulseOximeterDebuggingMode debuggingMode_)
{
    debuggingMode = debuggingMode_;

    hrm.begin();
    hrm.setMode(MAX30100_MODE_SPO2_HR);
    hrm.setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT_START);

    irDCRemover = DCRemover(DC_REMOVER_ALPHA);
    redDCRemover = DCRemover(DC_REMOVER_ALPHA);

    state = PULSEOXIMETER_STATE_IDLE;
}

void PulseOximeter::update()
{
    checkSample();
    checkCurrentBias();
}

float PulseOximeter::getHeartRate()
{
    return beatDetector.getRate();
}

uint8_t PulseOximeter::getSpO2()
{
    return spO2calculator.getSpO2();
}

uint8_t PulseOximeter::getRedLedCurrentBias()
{
    return redLedPower;
}

void PulseOximeter::setOnBeatDetectedCallback(void (*cb)())
{
    onBeatDetected = cb;
}

void PulseOximeter::checkSample()
{
    if (millis() - tsLastSample > 1.0 / SAMPLING_FREQUENCY * 1000.0) {
        tsLastSample = millis();
        hrm.update();
        float irACValue = irDCRemover.step(hrm.rawIRValue);
        float redACValue = redDCRemover.step(hrm.rawRedValue);

        // The signal fed to the beat detector is mirrored since the cleanest monotonic spike is below zero
        float filteredPulseValue = lpf.step(-irACValue);
        bool beatDetected = beatDetector.addSample(filteredPulseValue);

        if (beatDetector.getRate() > 0) {
            state = PULSEOXIMETER_STATE_DETECTING;
            spO2calculator.update(irACValue, redACValue, beatDetected);
        } else if (state == PULSEOXIMETER_STATE_DETECTING) {
            state = PULSEOXIMETER_STATE_IDLE;
            spO2calculator.reset();
        }

        switch (debuggingMode) {
            case PULSEOXIMETER_DEBUGGINGMODE_RAW_VALUES:
                Serial.print("R:");
                Serial.print(hrm.rawIRValue);
                Serial.print(",");
                Serial.println(hrm.rawRedValue);
                break;

            case PULSEOXIMETER_DEBUGGINGMODE_AC_VALUES:
                Serial.print("R:");
                Serial.print(irACValue);
                Serial.print(",");
                Serial.println(redACValue);
                break;

            case PULSEOXIMETER_DEBUGGINGMODE_PULSEDETECT:
                Serial.print("R:");
                Serial.print(filteredPulseValue);
                Serial.print(",");
                Serial.println(beatDetector.getCurrentThreshold());
                break;

            default:
                break;
        }

        if (beatDetected && onBeatDetected) {
            onBeatDetected();
        }
    }
}

void PulseOximeter::checkCurrentBias()
{
    // Follower that adjusts the red led current in order to have comparable DC baselines between
    // red and IR leds. The numbers are really magic: the less possible to avoid oscillations
    if (millis() - tsLastBiasCheck > CURRENT_ADJUSTMENT_PERIOD_MS) {
        bool changed = false;
        if (irDCRemover.getDCW() - redDCRemover.getDCW() > 70000 && redLedPower < MAX30100_LED_CURR_50MA) {
            ++redLedPower;
            changed = true;
        } else if (redDCRemover.getDCW() - irDCRemover.getDCW() > 70000 && redLedPower > 0) {
            --redLedPower;
            changed = true;
        }

        if (changed) {
            hrm.setLedsCurrent(IR_LED_CURRENT, (LEDCurrent)redLedPower);
            tsLastCurrentAdjustment = millis();
        }

        tsLastBiasCheck = millis();
    }
}
