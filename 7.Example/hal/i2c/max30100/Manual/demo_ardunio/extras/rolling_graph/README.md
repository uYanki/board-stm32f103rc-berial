# Rolling graph

Graphs the filtered data from the sensor and overlays the beat detector stage, allowing
a visual inspection of the beat detection algorithm.

## Requirements

* Processing 3+ (http://www.processing.org)
* Flash the example MAX30100_Debug to the target microcontroller

## Usage

* Start processing and open the file rolling_graph.pde
* If under linux or windows, adjust the line in order to match the serial port the microcontroller is connected to (eg: COM2, /dev/ttyUSB0). Under OSX the port should be autodetected.
    final String serialPort = "/dev/ttyACM0";
* Run the sketch

## Interpreting the screen

Data coming from the sensor are plotted continuously. Legend for the time series:

* Black graph (ch 0): infrared channel response level (the beat detector uses the IR channel)
* Red graph (ch 1): beat detector threshold

On the top left corner the following realtime values are printed:

* channels 0 and 1 current, max and min values
* computed heart rate
* computed SpO2

A typical working session would show a fairly regular black graph. Each peak of the black graph
should be intercepted by the red graph. When this happens, the screen flashes red to signify
that the beat detector is counting beats.

If the red graph misses often one or more black peak, try to adjust the power of the IR LED.
