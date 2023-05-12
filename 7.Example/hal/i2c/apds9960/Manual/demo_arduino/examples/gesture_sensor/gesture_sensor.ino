/***************************************************************************
  This is a library for the APDS9960 digital proximity, ambient light, RGB, and gesture sensor

  This sketch puts the sensor in gesture mode and decodes gestures.
  To use this, first put your hand close to the sensor to enable gesture mode.
  Then move your hand about 6" from the sensor in the up -> down, down -> up, 
  left -> right, or right -> left direction.

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
MH_APDS9960 apds;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  
  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");

  //gesture mode will be entered once proximity mode senses something close
  apds.enableProximity(true);
  apds.enableGesture(true);
}

// the loop function runs over and over again forever
void loop() {
  //read a gesture from the device
    uint8_t gesture = apds.readGesture();
    if(gesture == APDS9960_DOWN) Serial.println("v");
    if(gesture == APDS9960_UP) Serial.println("^");
    if(gesture == APDS9960_LEFT) Serial.println("<");
    if(gesture == APDS9960_RIGHT) Serial.println(">");
}