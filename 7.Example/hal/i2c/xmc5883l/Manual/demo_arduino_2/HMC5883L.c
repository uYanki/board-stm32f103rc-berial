// I found this code somewhere. Can't remember where (sorry for not acknowledging the contributor )

#define HMC5883_WriteAddress          0x1E  //  i.e 0x3C >> 1
#define HMC5883_ModeRegisterAddress   0x02
#define HMC5883_ContinuousModeCommand 0x00
#define HMC5883_DataOutputXMSBAddress 0x03

int regb     = 0x01;
int regbdata = 0x40;

int outputData[6];

void setup()
{
    Serial.begin(9600);
    Wire.begin();  // Initiate the Wire library and join the I2C bus as a master
}

void loop()
{
    int    i, x, y, z;
    double angle;

    Wire.beginTransmission(HMC5883_WriteAddress);
    Wire.send(regb);
    Wire.send(regbdata);
    Wire.endTransmission();

    delay(1000);
    Wire.beginTransmission(HMC5883_WriteAddress);  // Initiate a transmission with HMC5883 (Write address).
    Wire.send(HMC5883_ModeRegisterAddress);        // Place the Mode Register Address in send-buffer.
    Wire.send(HMC5883_ContinuousModeCommand);      // Place the command for Continuous operation Mode in send-buffer.
    Wire.endTransmission();                        // Send the send-buffer to HMC5883 and end the I2C transmission.
    delay(100);

    Wire.beginTransmission(HMC5883_WriteAddress);  // Initiate a transmission with HMC5883 (Write address).
    Wire.requestFrom(HMC5883_WriteAddress, 6);     // Request 6 bytes of data from the address specified.

    delay(500);

    // Read the value of magnetic components X,Y and Z

    if (6 <= Wire.available())  // If the number of bytes available for reading be <=6.
    {
        for (i = 0; i < 6; i++) {
            outputData = Wire.receive();  // Store the data in outputData buffer
        }
    }

    x = outputData[0] << 8 | outputData[1];  // Combine MSB and LSB of X Data output register
    z = outputData[2] << 8 | outputData[3];  // Combine MSB and LSB of Z Data output register
    y = outputData[4] << 8 | outputData[5];  // Combine MSB and LSB of Y Data output register

    angle = atan2((double)y, (double)x) * (180 / 3.14159265) + 180;  // angle in degrees

    /*

  Refer the following application note for heading calculation.
  http://www.ssec.honeywell.com/magnetic/datasheets/lowcost.pdf
  ----------------------------------------------------------------------------------------
  atan2(y, x) is the angle in radians between the positive x-axis of a plane and the point
  given by the coordinates (x, y) on it.
  ----------------------------------------------------------------------------------------

  This sketch does not utilize the magnetic component Z as tilt compensation can not be done without an Accelerometer

  ----------------->y
  |
  |
  |
  |
  |
  |
 \/
  x



     N
 NW  |  NE
     |
W----------E
     |
 SW  |  SE
     S

 */

    // Print the approximate direction

    Serial.print("You are heading ");
    if ((angle < 22.5) || (angle > 337.5))
        Serial.print("South");
    if ((angle > 22.5) && (angle < 67.5))
        Serial.print("South-West");
    if ((angle > 67.5) && (angle < 112.5))
        Serial.print("West");
    if ((angle > 112.5) && (angle < 157.5))
        Serial.print("North-West");
    if ((angle > 157.5) && (angle < 202.5))
        Serial.print("North");
    if ((angle > 202.5) && (angle < 247.5))
        Serial.print("NorthEast");
    if ((angle > 247.5) && (angle < 292.5))
        Serial.print("East");
    if ((angle > 292.5) && (angle < 337.5))
        Serial.print("SouthEast");

    Serial.print(": Angle between X-axis and the South direction ");
    if ((0 < angle) && (angle < 180)) {
        angle = angle;
    } else {
        angle = 360 - angle;
    }
    Serial.print(angle, 2);
    Serial.println(" Deg");
    delay(100);
}
