/*!
 * @file QMC5883_compass.cpp
 * @brief The program shows how to realize the function compass.When the program runs, please spin QMC5883 freely to accomplish calibration.
 * @n 3-Axis Digital Compass IC
 *
 * @copyright	[DFRobot](http://www.dfrobot.com), 2017
 * @copyright	GNU Lesser General Public License
 *
 * @author [dexian.huang](952838602@qq.com)
 * @version  V1.0
 * @date  2017-7-3
 */

#include <Wire.h>
#include <DFRobot_QMC5883.h>

DFRobot_QMC5883 compass;

void setup()
{
  Serial.begin(115200);
  while (!compass.begin())
  {
    Serial.println("Could not find a valid 5883 sensor, check wiring!");
    delay(500);
  }

  if(compass.isHMC()){
    Serial.println("Initialize HMC5883");
    //compass.setRange(HMC5883L_RANGE_1_3GA);
    //compass.setMeasurementMode(HMC5883L_CONTINOUS);
    //compass.setDataRate(HMC5883L_DATARATE_15HZ);
    //compass.setSamples(HMC5883L_SAMPLES_8);
  }else if(compass.isQMC()){
    Serial.println("Initialize QMC5883");
    //compass.setRange(QMC5883_RANGE_2GA);
    //compass.setMeasurementMode(QMC5883_CONTINOUS); 
    //compass.setDataRate(QMC5883_DATARATE_50HZ);
    //compass.setSamples(QMC5883_SAMPLES_8);
  }else if(compass.isVCM()){
    Serial.println("Initialize VCM5883L");
    //compass.setMeasurementMode(VCM5883L_CONTINOUS); 
    //compass.setDataRate(VCM5883L_DATARATE_200HZ);
  }
  delay(1000);
}
void loop()
{
  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / PI);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / PI);
  compass.setDeclinationAngle(declinationAngle);
  Vector mag = compass.readRaw();
  compass.getHeadingDegrees();
  Serial.print("X:");
  Serial.print(mag.XAxis);
  Serial.print(" Y:");
  Serial.print(mag.YAxis);
  Serial.print(" Z:");
  Serial.println(mag.ZAxis);
  Serial.print("Degress = ");
  Serial.println(mag.HeadingDegress);
  delay(100);
}