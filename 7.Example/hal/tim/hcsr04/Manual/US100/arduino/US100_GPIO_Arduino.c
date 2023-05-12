unsigned int EchoPin = 2;           // connect Pin 2(Arduino digital io) to Echo/RX at US-100
unsigned int TrigPin = 3;           // connect Pin 3(Arduino digital io) to Trig/TX at US-100
unsigned long Time_Echo_us = 0;
unsigned long Len_mm  = 0;
void setup()
{  //Initialize
    Serial.begin(9600);                        //Serial: output result to Serial monitor
    pinMode(EchoPin, INPUT);                    //Set EchoPin as input, to receive measure result from US-100
    pinMode(TrigPin, OUTPUT);                   //Set TrigPin as output, used to send high pusle to trig measurement (>10us)
}

void loop()
{
    digitalWrite(TrigPin, HIGH);              //begin to send a high pulse, then US-100 begin to measure the distance
    delayMicroseconds(50);                    //set this high pulse width as 50us (>10us)
    digitalWrite(TrigPin, LOW);               //end this high pulse
    
    Time_Echo_us = pulseIn(EchoPin, HIGH);               //calculate the pulse width at EchoPin, 
    if((Time_Echo_us < 60000) && (Time_Echo_us > 1))     //a valid pulse width should be between (1, 60000).
    {
      Len_mm = (Time_Echo_us*34/100)/2;      //calculate the distance by pulse width, Len_mm = (Time_Echo_us * 0.34mm/us) / 2 (mm)
      Serial.print("Present Distance is: ");  //output result to Serial monitor
      Serial.print(Len_mm, DEC);            //output result to Serial monitor
      Serial.println("mm");                 //output result to Serial monitor
    }
    delay(1000);                            //take a measurement every second (1000ms)
}
