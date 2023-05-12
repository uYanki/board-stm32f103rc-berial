#include <Wire.h>
#include <LIS3DSH.h>

LIS3DSH accel;

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  accel.enableDefault();
}

void loop() 
{
  int16_t x, y, z;
  int8_t temperature;

  accel.readAccel(&x, &y, &z);
  accel.readTemperature(&temperature);

  Serial.print("Accel ");
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(" Z: ");
  Serial.print(z);
  Serial.print(" T: ");
  Serial.println(temperature);

  delay(100);
}
