int Temperature45 = 0;

void setup() {           // connect RX (Pin 0 of Arduino digital IO) to Echo/Rx (US-100), TX (Pin 1 of Arduino digital IO) to Trig/Tx (US-100)
    Serial.begin(9600);  // set baudrate as 9600bps.
}

void loop() {
    Serial.flush();               // clear receive buffer of serial port
    Serial.write(0X50);           // trig US-100 begin to measure the temperature
    delay(500);                   // delay 500ms to wait result
    if (Serial.available() >= 1)  // when receive 1 bytes
    {
        Temperature45 = Serial.read();                     // Get the received byte (temperature)
        if ((Temperature45 > 1) && (Temperature45 < 130))  // the valid range of received data is (1, 130)
        {
            Temperature45 -= 45;                       // Real temperature = Received_Data - 45
            Serial.print("Present Temperature is: ");  // output result
            Serial.print(Temperature45, DEC);          // output result
            Serial.println(" degree centigrade.");     // output result
        }
    }
    delay(500);  // delay 500ms
}
