/******************************************************************************
file name: KS1081EB.ino
Demo Program for KS1081 ECG Board (KS1081_EB)
******************************************************************************/
/******************************************************************************
Designed by Kngsense Electronics
Web: www.ks-chip.com
E-mail:support@ks-chip.com
Date:12/08/2021
Copyright © Kingsense® Electronics. All Rights Reserved. 
******************************************************************************/
/******************************************************************************
The KS1081 designed by Kingsense,is an integrated single-channel analog front-end chip 
for high performance low power ECG recording. It is widely used in the wearable/portable 
ECG devices and other weak signals acquisitions. 

This example shows a easy way to create a real-time ECG display by Processing. 
******************************************************************************/
/******************************************************************************
Resources:
This program requires a Processing sketch to view the data in real time.
Development environment specifics:
IDE: Arduino 1.8.19
Hardware: KS1081-EB & Arduino UNO 
Processing Version: 2.1.2
******************************************************************************/

void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(10, INPUT); // Setup for leads off detection LDF
}
void loop() {
  
  if((digitalRead(10) == 1)) {
    Serial.println('!');
  }
  else{
    // send the value of analog input 0:
      Serial.println(analogRead(A0));
  }
  //Wait for a bit to keep serial data from saturating
  delay(1);
}
