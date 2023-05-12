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

#include "MAX30100_BeatDetector.h"
#include "timer.h"
BeatDetectorState Beatstate;

float threshold;
float beatPeriod;
float lastMaxValue;
uint32_t tsLastBeat;
//BeatDetector::BeatDetector() :
//    state(BEATDETECTOR_STATE_INIT),
//    threshold(BEATDETECTOR_MIN_THRESHOLD),
//    beatPeriod(0),
//    lastMaxValue(0),
//    tsLastBeat(0)
//{
//}
void BeatDetector(){
    Beatstate=BEATDETECTOR_STATE_INIT;
    threshold=BEATDETECTOR_MIN_THRESHOLD;
    beatPeriod=0;
    lastMaxValue=0;
    tsLastBeat=0;

}

u8 addSample(float sample)//bool BeatDetector::addSample(float sample)
{
    return checkForBeat(sample);
}

float getRate()//float BeatDetector::getRate()
{
    if (beatPeriod != 0) {
        return 1 / beatPeriod * 1000 * 60;
    } else {
        return 0;
    }
}

float getCurrentThreshold()//float BeatDetector::getCurrentThreshold()
{
    return threshold;
}

u8 checkForBeat(float sample)//bool BeatDetector::checkForBeat(float sample)
{
//    bool beatDetected = false;
    u8 beatDetected = 0;
	  float delta;
    switch (Beatstate) {//switch (state) {
        case BEATDETECTOR_STATE_INIT:
            if (millis() > BEATDETECTOR_INIT_HOLDOFF) {
//                state = BEATDETECTOR_STATE_WAITING;
							Beatstate = BEATDETECTOR_STATE_WAITING;
            }
            break;

        case BEATDETECTOR_STATE_WAITING:
            if (sample > threshold) {
                threshold = min(sample, BEATDETECTOR_MAX_THRESHOLD);
//                state = BEATDETECTOR_STATE_FOLLOWING_SLOPE;
									Beatstate = BEATDETECTOR_STATE_FOLLOWING_SLOPE;
						}

            // Tracking lost, resetting
            if (millis() - tsLastBeat > BEATDETECTOR_INVALID_READOUT_DELAY) {
                beatPeriod = 0;
                lastMaxValue = 0;
            }

            decreaseThreshold();
            break;

        case BEATDETECTOR_STATE_FOLLOWING_SLOPE:
            if (sample < threshold) {
//                state = BEATDETECTOR_STATE_MAYBE_DETECTED;
									Beatstate = BEATDETECTOR_STATE_MAYBE_DETECTED;
						} else {
                threshold = min(sample, BEATDETECTOR_MAX_THRESHOLD);
            }
            break;

        case BEATDETECTOR_STATE_MAYBE_DETECTED:
            if (sample + BEATDETECTOR_STEP_RESILIENCY < threshold) {
                // Found a beat
//                beatDetected = true;
							beatDetected = 1;
                lastMaxValue = sample;
//                state = BEATDETECTOR_STATE_MASKING;
              Beatstate = BEATDETECTOR_STATE_MASKING;

							 delta = millis() - tsLastBeat;
							
                if (delta) {
                    beatPeriod = (float) BEATDETECTOR_BPFILTER_ALPHA * delta +
                            (1 - BEATDETECTOR_BPFILTER_ALPHA) * beatPeriod;
                }

                tsLastBeat = millis();
            } else {
//                state = BEATDETECTOR_STATE_FOLLOWING_SLOPE;
									Beatstate = BEATDETECTOR_STATE_FOLLOWING_SLOPE;
						}
            break;

        case BEATDETECTOR_STATE_MASKING:
            if (millis() - tsLastBeat > BEATDETECTOR_MASKING_HOLDOFF) {
//                state = BEATDETECTOR_STATE_WAITING;
							   Beatstate = BEATDETECTOR_STATE_WAITING;
            }
            decreaseThreshold();
            break;
    }

    return beatDetected;
}

void decreaseThreshold()//void BeatDetector::decreaseThreshold()
{
    // When a valid beat rate readout is present, target the
    if (lastMaxValue > 0 && beatPeriod > 0) {
        threshold -= lastMaxValue * (1 - BEATDETECTOR_THRESHOLD_FALLOFF_TARGET) /
                (beatPeriod / BEATDETECTOR_SAMPLES_PERIOD);
    } else {
        // Asymptotic decay
        threshold *= (float)BEATDETECTOR_THRESHOLD_DECAY_FACTOR;
    }

    if (threshold < BEATDETECTOR_MIN_THRESHOLD) {
        threshold = BEATDETECTOR_MIN_THRESHOLD;
    }
}

int min(int a ,int b){
  return (a>b)?b:a;
}

