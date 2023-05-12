#include <Arduino.h>
#include <SoftwareSerial.h>
#include "qrencode.h"
extern SoftwareSerial thermalPrinter;

void printQR(String message)
{
    // new line
    thermalPrinter.print("\n");
    thermalPrinter.print("\n");

    // zero line spacing
    thermalPrinter.write(27);
    thermalPrinter.write(51);
    thermalPrinter.write((byte)0);

    // center-align
    thermalPrinter.write(27);
    thermalPrinter.write(97);
    thermalPrinter.write(49);

    // create QR code
    message.toCharArray((char*)strinbuf, 47);
    qrencode();

    // print QR Code
    for (byte x = 0; x < WD; x += 2) {
        for (byte y = 0; y < WD; y++) {
            if (QRBIT(x, y) && QRBIT((x + 1), y)) { thermalPrinter.write(219); }    // black square on top of black square
            if (!QRBIT(x, y) && QRBIT((x + 1), y)) { thermalPrinter.write(220); }   // white square on top of black square
            if (QRBIT(x, y) && !QRBIT((x + 1), y)) { thermalPrinter.write(223); }   // black square on top of white square
            if (!QRBIT(x, y) && !QRBIT((x + 1), y)) { thermalPrinter.print(" "); }  // white square on top of white square
        }
        thermalPrinter.write(10);  // next line
    }

    // default line spacing
    thermalPrinter.write(27);
    thermalPrinter.write(50);

    // default-align (left)
    thermalPrinter.write(27);
    thermalPrinter.write(97);
    thermalPrinter.write(48);

    // new line
    thermalPrinter.print("\n");
    thermalPrinter.print("\n");
}