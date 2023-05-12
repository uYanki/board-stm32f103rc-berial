//以15毫秒一步的速度转动
#include <Servo.h>
Servo servo1; // 建立Servo实例
Servo servo2;
const int servo1Pin = 8;  // servo1 接 Pin 8
const int servo2Pin = 9;  // servo2 接 Pin 9
const int every1 = 15;
const int every2 = 15;
int servo1Target = 0;
int servo2Target = 0;

int servo1Dir = 10;
int servo2Dir = 1;

int posOfServo1 = 90;
int posOfServo2 = 90;
void setup() {
  servo1.attach(servo1Pin, 500, 2500); // pin8,
  servo2.attach(servo2Pin, 500, 2500); // pin9
  Serial.begin(9600);
  servo1.write(posOfServo1);
  servo2.write(posOfServo2);
  Serial.println("2 Servo test");
  delay(500);
}
void loop( ) {
  checkServo1();
  checkServo2();

  if (posOfServo1 > 179)
    servo1Target = 0;
  if (posOfServo1 < 1 )
    servo1Target = 180;

  if (posOfServo2 > 179)
    servo2Target = 0;
  if (posOfServo2 < 1 )
    servo2Target = 180;
}



void checkServo1( ) {
  static uint32_t Timer;
  if (Timer > millis())
    return;
  Timer = millis() + every1;
  if (servo1Target > posOfServo1) {
    posOfServo1 += servo1Dir;
  } else if (servo1Target == posOfServo1) {
    return;
  } else {
    posOfServo1 += -servo1Dir;
  }
  servo1.write(posOfServo1);
}
void checkServo2( ) {
  static uint32_t Timer;
  if (Timer > millis())
    return;
  Timer = millis() + every2;
  if (servo2Target > posOfServo2) {
    posOfServo2 += servo2Dir;
  } else if (servo2Target == posOfServo2) {
    return;
  } else {
    posOfServo2 += -servo2Dir;
  }
  servo2.write(posOfServo2);
}

void checkServo3(){
  //依Servo1，servo2，以同样方法添加更多舵机
}

