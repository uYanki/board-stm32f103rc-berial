/***************************************************************************
  This is a library for the APDS9960 digital proximity, ambient light, RGB, and gesture sensor

  This sketch puts the sensor in proximity mode and enables the interrupt
  to fire when proximity goes over a set value

  Designed specifically to work with the MH APDS9960 breakout
  ----> http://www.MH.com/products/3595

  These sensors use I2C to communicate. The device's I2C address is 0x39

  MH invests time and resources providing this open source code,
  please support MH andopen-source hardware by purchasing products
  from MH!

  Written by Dean Miller for MH Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include "MH_APDS9960.h"

//the pin that the interrupt is attached to
#define INT_PIN 3

//create the APDS9960 object
MH_APDS9960 apds;

void setup() {
  Serial.begin(115200);
  pinMode(INT_PIN, INPUT_PULLUP);

  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");

  //enable proximity mode
  apds.enableProximity(true);

  //set the interrupt threshold to fire when proximity reading goes above 175
  apds.setProximityInterruptThreshold(0, 175);

  //enable the proximity interrupt
  apds.enableProximityInterrupt();
}

void loop() {

  //print the proximity reading when the interrupt pin goes low
  if(!digitalRead(INT_PIN)){
    Serial.println(apds.readProximity());

    //clear the interrupt
    apds.clearInterrupt();
  }
}