/***************************************************************************
  This is a library for the APDS9960 digital proximity, ambient light, RGB, and gesture sensor

  This sketch puts the sensor in color mode and reads the RGB and clear values.

  Designed specifically to work with the MH APDS9960 breakout
  ----> http://www.MH.com

  These sensors use I2C to communicate. The device's I2C address is 0x39

  MH invests time and resources providing this open source code,
  please support MH andopen-source hardware by purchasing products
  from MH!

  Written by Dean Miller for MH Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include "MH_APDS9960.h"
MH_APDS9960 apds;

void setup() {
  Serial.begin(115200);

  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");

  //enable color sensign mode
  apds.enableColor(true);
}

void loop() {
  //create some variables to store the color data in
  uint16_t r, g, b, c;
  
  //wait for color data to be ready
  while(!apds.colorDataReady()){
    delay(5);
  }

  //get the data and print the different channels
  apds.getColorData(&r, &g, &b, &c);
  Serial.print("red: ");
  Serial.print(r);
  
  Serial.print(" green: ");
  Serial.print(g);
  
  Serial.print(" blue: ");
  Serial.print(b);
  
  Serial.print(" clear: ");
  Serial.println(c);
  Serial.println();
  
  delay(500);
}