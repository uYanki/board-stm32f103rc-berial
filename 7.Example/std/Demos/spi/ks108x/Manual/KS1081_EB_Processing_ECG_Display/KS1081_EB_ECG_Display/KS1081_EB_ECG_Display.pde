/******************************************************************************
file name: KS1081_EB_ECG_Display.ino
Demo Program for KS1081 ECG Board (KS1081-EB).
******************************************************************************/

/******************************************************************************
Designed by Kngsense Electronics
Web: www.ks-chip.com
E-mail:support@ks-chip.com
Date:12/08/2021
Copyright © Kingsense® Electronics. All Rights Reserved. 
******************************************************************************/

/******************************************************************************
The KS1081 designed by Kingsense®, is an integrated single-channel analog front-end chip for high performance low power ECG recording.  
It is widely used in the wearable/portable ECG devices and other weak signals acquisitions. This example shows a easy way to create an real-time
ECG display by Processing. 
******************************************************************************/

/******************************************************************************Resources:
This program requires a Processing sketch to view the data in real time.

Development environment specifics:
  IDE: Arduino 1.8.19
  Hardware: KS1081-EB & Arduino UNO 
  Processing Version: 2.1.2
******************************************************************************/


import processing.serial.*;

Serial myPort;        // The serial port
int xPos = 1;         // horizontal position of the graph
float height_old = 0;
float height_new = 0;
float inByte = 0;


void setup () {
  // set the window size:
  size(1200, 500);        
  // List all the available serial ports
  println(Serial.list());
  
  // Open whatever port is the one you're using and revise the serial port name
  myPort = new Serial(this, "COM8", 115200);
  
  // don't generate a serialEvent() unless you get a newline character:
  myPort.bufferUntil('\n');
  // set inital background:
  background(0xff);
}


void draw () {
  // everything happens in the serialEvent()
}

void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');

  if (inString != null) {
    // trim off any whitespace:
    inString = trim(inString);

    // If leads off detection is true notify with blue line
    if (inString.equals("!")) { 
      stroke(0, 0, 0xff); //Set stroke to blue ( R, G, B)
      inByte = 512;  // middle of the ADC range (Flat Line)
    }
    // If the data is good let it through
    else {
      stroke(0xff, 0, 0); //Set stroke to red ( R, G, B)
      inByte = float(inString); 
     }
     
     //Map and draw the line for new data point
//     inByte = map(inByte, 0, 1023, 0, height);
     inByte = map(inByte, 0, 4096, 0, height);
//     inByte = map(inByte, 0, 1024, 0, height);
     height_new = height - inByte; 
     line(xPos - 1, height_old, xPos, height_new);
     height_old = height_new;
    
      // at the edge of the screen, go back to the beginning:
      if (xPos >= width) {
        xPos = 0;
        background(0xff);
      } 
      else {
        // increment the horizontal position:
        xPos++;
      }
    
  }
}

