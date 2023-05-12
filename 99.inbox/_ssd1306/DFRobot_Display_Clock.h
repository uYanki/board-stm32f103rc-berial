#ifndef DFRobot_DISPLAY_CLOCK_H
#define DFRobot_DISPLAY_CLOCK_H

#include "DFRobot_Display.h"
#include "math.h"

#ifndef PI
#define PI 3.1415926
#endif
#define ANG2DEG(x) (x / 180.0f * PI)

class DFRobot_Display_Clock {
public:
    DFRobot_Display_Clock(DFRobot_Display* pDisplay);

    void setPointerLength(uint16_t hl, uint16_t ml, uint16_t sl);
    void setPointerColor(uint16_t hc, uint16_t mc, uint16_t sc);
    void setPointerWidth(uint16_t hw, uint16_t mw, uint16_t sw);
    void setClockDial(int16_t x, int16_t y, uint16_t r);
    void setDialColor(uint16_t border, uint16_t background);
    void showDial();
    void updateClock(uint8_t h, uint8_t m, uint8_t s);

private:
    DFRobot_Display* _pDisplay;
    int16_t          _x, _y, _r;
    int16_t          _hourLen, _minLen, _secLen;
    uint16_t         _hourWidth, _minWidth, _secWidth;
    uint16_t         _hourColor, _minColor, _secColor;
    int16_t          _lastHourX, _lastHourY, _lastMinX, _lastMinY, _lastSecX, _lastSecY;
    uint16_t         _background, _borderColor;

    void calcPointerPosition(uint8_t t, uint16_t len, int16_t* pX, int16_t* pY)
    {
        if (t > 59)
            t = 59;
        double angle    = (t % 15) / 15.0f * 90.0f;
        double nearLine = cos(ANG2DEG(angle)) * (double)len;
        double opLine   = sqrt(((double)len * (double)len) - (nearLine * nearLine));
        _DEBUG_CODE(
            Serial.println();
            Serial.print("calcPointerPosition ");
            Serial.print(len);
            Serial.print(" ");
            Serial.print(angle);
            Serial.print(" ");
            Serial.print((int16_t)nearLine);
            Serial.print(" ");
            Serial.println((int16_t)opLine););
        switch (t / 15) {
            case 0: {
                if (t == 0) {
                    *pX = _x;
                    *pY = _y - len;
                } else {
                    *pX = _x + (int16_t)opLine;
                    *pY = _y - (int16_t)nearLine;
                }
            } break;
            case 1: {
                if (t == 15) {
                    *pX = _x + len;
                    *pY = _y;
                } else {
                    *pX = _x + (int16_t)nearLine;
                    *pY = _y + (int16_t)opLine;
                }
            } break;
            case 2: {
                if (t == 30) {
                    *pX = _x;
                    *pY = _y + len;
                } else {
                    *pX = _x - (int16_t)opLine;
                    *pY = _y + (int16_t)nearLine;
                }
            } break;
            case 3: {
                if (t == 45) {
                    *pX = _x - len;
                    *pY = _y;
                } else {
                    *pX = _x - (int16_t)nearLine;
                    *pY = _y - (int16_t)opLine;
                }
            } break;
        }
    }

    void calcPointerPositionHour(uint8_t th, uint8_t tm, uint16_t len, int16_t* pX, int16_t* pY)
    {
        if (th > 23)
            th = 23;
        th %= 12;
        uint16_t thm      = (uint16_t)th * 60 + tm;
        double   angle    = (thm % 180) / 180.0f * 90.0f;
        double   nearLine = cos(ANG2DEG(angle)) * (double)len;
        double   opLine   = sqrt(((double)len * (double)len) - (nearLine * nearLine));
        switch (thm / 180) {
            case 0: {
                if (thm == 0) {
                    *pX = _x;
                    *pY = _y - len;
                } else {
                    *pX = _x + (int16_t)opLine;
                    *pY = _y - (int16_t)nearLine;
                }
            } break;
            case 1: {
                if (thm == 180) {
                    *pX = _x + len;
                    *pY = _y;
                } else {
                    *pX = _x + (int16_t)nearLine;
                    *pY = _y + (int16_t)opLine;
                }
            } break;
            case 2: {
                if (thm == 360) {
                    *pX = _x;
                    *pY = _y + len;
                } else {
                    *pX = _x - (int16_t)opLine;
                    *pY = _y + (int16_t)nearLine;
                }
            } break;
            case 3: {
                if (thm == 540) {
                    *pX = _x - len;
                    *pY = _y;
                } else {
                    *pX = _x - (int16_t)nearLine;
                    *pY = _y - (int16_t)opLine;
                }
            } break;
        }
    }
};

#endif
