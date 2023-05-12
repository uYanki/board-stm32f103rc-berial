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

//#include <Arduino.h>

#include "MAX30100_PulseOximeter.h"
#include "timer.h"
#include "stdlib.h"
#include "usart.h"

float onBeatDetected;
PulseOximeterState state;
PulseOximeterDebuggingMode debuggingMode;
uint32_t tsFirstBeatDetected;
uint32_t tsLastBeatDetected;
uint32_t tsLastSample;
uint32_t tsLastBiasCheck;
uint32_t tsLastCurrentAdjustment;
uint8_t redLedPower;
//    BeatDetector beatDetector;

DCRemover irDCRemover;
DCRemover redDCRemover;
//PulseOximeter::PulseOximeter() :
//    state(PULSEOXIMETER_STATE_INIT),
//    tsFirstBeatDetected(0),
//    tsLastBeatDetected(0),
//    tsLastSample(0),
//    tsLastBiasCheck(0),
//    tsLastCurrentAdjustment(0),
//    redLedPower((uint8_t)RED_LED_CURRENT_START),
//    onBeatDetected(NULL)
//{
//}
float onBeatDetected;
void PulseOximeter(){ 
    state=PULSEOXIMETER_STATE_INIT;
    tsFirstBeatDetected=0;
    tsLastBeatDetected=0;
    tsLastSample=0;
    tsLastBiasCheck=0;
    tsLastCurrentAdjustment=0;
    redLedPower=((uint8_t)RED_LED_CURRENT_START);
    onBeatDetected=NULL;
}

void begin()//void PulseOximeter::begin(PulseOximeterDebuggingMode debuggingMode_)
{
    debuggingMode = PULSEOXIMETER_DEBUGGINGMODE_NONE;

    max_begin();//hrm.begin();
    setMode(MAX30100_MODE_SPO2_HR);//hrm.setMode(MAX30100_MODE_SPO2_HR);
    setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT_START);//hrm.setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT_START);

//    irDCRemover = DCRemover(DC_REMOVER_ALPHA);
//    redDCRemover = DCRemover(DC_REMOVER_ALPHA);
	
			DCRemover1(DC_REMOVER_ALPHA,&irDCRemover.alpha,&irDCRemover.dcw);
			DCRemover1(DC_REMOVER_ALPHA,&redDCRemover.alpha,&redDCRemover.dcw);
    state = PULSEOXIMETER_STATE_IDLE;
}

void POupdate()//void PulseOximeter::update()
{
    checkSample();
    checkCurrentBias();
}

float getHeartRate()//float PulseOximeter::getHeartRate()
{
    return getRate();//beatDetector.getRate();
}

uint8_t POgetSpO2()//uint8_t PulseOximeter::getSpO2()
{
    return getSpO2();//spO2calculator.getSpO2();
}

uint8_t getRedLedCurrentBias()//uint8_t PulseOximeter::getRedLedCurrentBias()
{
    return redLedPower;
}

void setOnBeatDetectedCallback(float *cb)//void PulseOximeter::setOnBeatDetectedCallback(void (*cb)())
{
    onBeatDetected = *cb;
}

void checkSample()//void PulseOximeter::checkSample()
{
	      u8 beatDetected;
	      float filteredPulseValue;
	      float irACValue;
	      float redACValue;
    if (millis() - tsLastSample > 1.0 / SAMPLING_FREQUENCY * 1000.0) {
        tsLastSample = millis();
//        hrm.update();
			  update();
//        float irACValue = irDCRemover.step(hrm.rawIRValue);
//        float redACValue = redDCRemover.step(hrm.rawRedValue);
					irACValue = step(rawIRValue,&irDCRemover.alpha,&irDCRemover.dcw);
			    redACValue = step(rawRedValue,&redDCRemover.alpha,&redDCRemover.dcw);
        // The signal fed to the beat detector is mirrored since the cleanest monotonic spike is below zero
//        float filteredPulseValue = lpf.step(-irACValue);
			    filteredPulseValue = FBstep(-irACValue);
//        bool beatDetected = beatDetector.addSample(filteredPulseValue);
//        beatDetected = beatDetector.addSample(filteredPulseValue);
			  beatDetected = addSample(filteredPulseValue);
        if (getRate() > 0/*beatDetector.getRate() > 0*/) {
            state = PULSEOXIMETER_STATE_DETECTING;
            SPO2update(irACValue, redACValue, beatDetected);//spO2calculator.update(irACValue, redACValue, beatDetected);
        } else if (state == PULSEOXIMETER_STATE_DETECTING) {
            state = PULSEOXIMETER_STATE_IDLE;
//            spO2calculator.reset();
					    reset();
        }

        switch (debuggingMode) {
            case PULSEOXIMETER_DEBUGGINGMODE_RAW_VALUES:
//                Serial.print("R:");
//                Serial.print(hrm.rawIRValue);
//                Serial.print(",");
//                Serial.println(hrm.rawRedValue);
						    printf("R:");
                printf("%d",rawIRValue/*hrm.rawIRValue*/);
                printf(",");
                printf("%d\r\n",rawRedValue/*hrm.rawRedValue*/);
                break;

            case PULSEOXIMETER_DEBUGGINGMODE_AC_VALUES:
//                Serial.print("R:");
//                Serial.print(irACValue);
//                Serial.print(",");
//                Serial.println(redACValue);
						    printf("R:");
                printf("%lf",irACValue);
                printf(",");
                printf("%lf\r\n",redACValue);
                break;

            case PULSEOXIMETER_DEBUGGINGMODE_PULSEDETECT:
//                Serial.print("R:");
//                Serial.print(filteredPulseValue);
//                Serial.print(",");
//                Serial.println(beatDetector.getCurrentThreshold());
						    printf("R:");
                printf("%lf",filteredPulseValue);
                printf(",");
                printf("%lf\r\n",getCurrentThreshold()/*beatDetector.getCurrentThreshold()*/);
                break;

            default:
                break;
        }

        if (beatDetected && onBeatDetected) {
            onBeatDetected1();
        }
    }
}

void onBeatDetected1(void)
{
  
}
void checkCurrentBias()//void PulseOximeter::checkCurrentBias()
{
    // Follower that adjusts the red led current in order to have comparable DC baselines between
    // red and IR leds. The numbers are really magic: the less possible to avoid oscillations
    if (millis() - tsLastBiasCheck > CURRENT_ADJUSTMENT_PERIOD_MS) {
//        bool changed = false;
			  u8 changed = 0;
        if (getDCW(&irDCRemover.dcw)-getDCW(&redDCRemover.dcw)/*irDCRemover.getDCW() - redDCRemover.getDCW()*/ > 70000 && redLedPower < MAX30100_LED_CURR_50MA) {
            ++redLedPower;
//            changed = true;
					changed = 1;
        } else if (getDCW(&redDCRemover.dcw)/*redDCRemover.getDCW()*/ - getDCW(&irDCRemover.dcw)/*irDCRemover.getDCW()*/ > 70000 && redLedPower > 0) {
            --redLedPower;
//            changed = true;
					changed = 1;
        }

        if (changed) {
//            hrm.setLedsCurrent(IR_LED_CURRENT, (LEDCurrent)redLedPower);
				   	setLedsCurrent(IR_LED_CURRENT, (LEDCurrent)redLedPower);
            tsLastCurrentAdjustment = millis();
        }

        tsLastBiasCheck = millis();
    }
}
