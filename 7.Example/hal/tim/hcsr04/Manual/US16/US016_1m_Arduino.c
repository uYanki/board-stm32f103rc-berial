
unsigned int ADCValue;
void setup()
{
    Serial.begin(9600);
}

void loop()
{

    ADCValue = analogRead(0);
    Serial.print("Present Length is: ");
    Serial.print(ADCValue, DEC);
    Serial.println("mm");
    delay(1000);//delay 1S
}
