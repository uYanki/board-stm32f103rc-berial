#include "DFRobot_Display_Clock.h"

DFRobot_Display_Clock::DFRobot_Display_Clock(DFRobot_Display* pDisplay)
{
  _pDisplay = pDisplay;
  _hourWidth = 1;
  _minWidth = 1;
  _secWidth = 1;
}

void DFRobot_Display_Clock::setClockDial(int16_t x, int16_t y, uint16_t r)
{
  _x = x;
  _y = y;
  _r = r;
}

void DFRobot_Display_Clock::setPointerColor(uint16_t hc, uint16_t mc, uint16_t sc)
{
  _hourColor = hc;
  _minColor = mc;
  _secColor = sc;
}

void DFRobot_Display_Clock::setPointerLength(uint16_t hl, uint16_t ml, uint16_t sl)
{
  _hourLen = hl;
  _minLen = ml;
  _secLen = sl;
}

void DFRobot_Display_Clock::setDialColor(uint16_t border, uint16_t background)
{
  _background = background;
  _borderColor = border;
}

void DFRobot_Display_Clock::setPointerWidth(uint16_t hw, uint16_t mw, uint16_t sw)
{
  _hourWidth = hw;
  _minWidth = mw;
  _secWidth = sw;
}

void DFRobot_Display_Clock::showDial()
{
  uint16_t      lastLineWidth = _pDisplay->getLineWidth();
  _pDisplay->setLineWidth(1);

  _pDisplay->fillCircle(_x, _y, _r, _background);
  _pDisplay->drawCircle(_x, _y, _r, _borderColor);
  _pDisplay->drawVLine(_x, _y - _r, _r / 8, _borderColor);
  _pDisplay->drawHLine(_x + _r, _y, - _r / 8, _borderColor);
  _pDisplay->drawVLine(_x, _y + _r, - _r / 8, _borderColor);
  _pDisplay->drawHLine(_x - _r, _y, _r / 8, _borderColor);

  int16_t       x, y;
  calcPointerPosition(5, _r, &x, &y);
  _pDisplay->drawLine(x * 7 / 8, y * 7 / 8, x, y, _borderColor);
  calcPointerPosition(10, _r, &x, &y);
  _pDisplay->drawLine(x * 7 / 8, y * 7 / 8, x, y, _borderColor);
  calcPointerPosition(20, _r, &x, &y);
  _pDisplay->drawLine(x * 7 / 8, y * 7 / 8, x, y, _borderColor);
  calcPointerPosition(25, _r, &x, &y);
  _pDisplay->drawLine(x * 7 / 8, y * 7 / 8, x, y, _borderColor);
  calcPointerPosition(35, _r, &x, &y);
  _pDisplay->drawLine(x * 7 / 8, y * 7 / 8, x, y, _borderColor);
  calcPointerPosition(40, _r, &x, &y);
  _pDisplay->drawLine(x * 7 / 8, y * 7 / 8, x, y, _borderColor);
  calcPointerPosition(50, _r, &x, &y);
  _pDisplay->drawLine(x * 7 / 8, y * 7 / 8, x, y, _borderColor);
  calcPointerPosition(55, _r, &x, &y);
  _pDisplay->drawLine(x * 7 / 8, y * 7 / 8, x, y, _borderColor);
  
  _pDisplay->setLineWidth(lastLineWidth);
}

void DFRobot_Display_Clock::updateClock(uint8_t h, uint8_t m, uint8_t s)
{
  uint16_t        w = _pDisplay->getLineWidth();

  _pDisplay->setLineWidth(_hourWidth);
  _pDisplay->drawLine(_x, _y, _lastHourX, _lastHourY, _background);
  calcPointerPositionHour(h, m, _hourLen, &_lastHourX, &_lastHourY);
  _pDisplay->drawLine(_x, _y, _lastHourX, _lastHourY, _hourColor);
  _DEBUG_CODE(
    Serial.println();
    Serial.print("hour pointer ");
    Serial.print(_x);
    Serial.print(" ");
    Serial.print(_y);
    Serial.print(" ");
    Serial.print(_lastHourX);
    Serial.print(" ");
    Serial.print(_lastHourY);
    Serial.println(" ");
  );

  _pDisplay->setLineWidth(_minWidth);
  _pDisplay->drawLine(_x, _y, _lastMinX, _lastMinY, _background);
  calcPointerPosition(m, _minLen, &_lastMinX, &_lastMinY);
  _pDisplay->drawLine(_x, _y, _lastMinX, _lastMinY, _minColor);

  _pDisplay->setLineWidth(_secWidth);
  _pDisplay->drawLine(_x, _y, _lastSecX, _lastSecY, _background);
  calcPointerPosition(s, _secLen, &_lastSecX, &_lastSecY);
  _pDisplay->drawLine(_x, _y, _lastSecX, _lastSecY, _secColor);

  _pDisplay->setLineWidth(w);
}
