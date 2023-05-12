#include <SD.h>
#include <LiquidCrystal.h>

#include "lrc.h"

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

byte data_dec[8][8];
byte data_copied[5];

bool fileready = false;

int frames = 0;
int lrcstate = 0;
unsigned long tstart;
File dataFile;

#define FILE_NAME "BA30.BIN"

void setup() {
	//Serial.begin(9600);
	//while (!Serial) ;
	lcd.begin(16, 2);
	
	for (uint8_t i = 0; i < 8; i++) {
		lcd.createChar(i, data_dec[i]);
	}
	lcd.clear();
	lcd.print("Initializing");
	lcd.setCursor(0, 1);
	lcd.print("SD card...");
	if (!SD.begin(10)) {
		lcd.clear();
		lcd.print("Card failed.");
		return;
	}
	dataFile = SD.open(FILE_NAME);
	lcd.clear();
	if (dataFile) {
		fileready = true;
	} else {
		lcd.print("error opening");
		lcd.setCursor(0, 1);
		lcd.print(FILE_NAME);
		return;
	}

	lcd.clear();
	lcd.setCursor(3, 0);
	lcd.print("Bad Apple");
	lcd.setCursor(3, 1);
	lcd.print("on LCD1602");
	delay(2000);
	
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.write(byte(0));
	lcd.write(byte(1));
	lcd.write(byte(2));
	lcd.write(byte(3));
	lcd.setCursor(0, 1);
	lcd.write(byte(4));
	lcd.write(byte(5));
	lcd.write(byte(6));
	lcd.write(byte(7));
	
	tstart = millis() + 100;
}

void loop() {
	if (fileready) {
		if (dataFile.available()) {
			//decompress
			for (uint8_t i = 0; i < 8; i++) {
				for (uint8_t j = 0; j < 5; j++) {
					data_copied[j] = dataFile.read();
				}
				data_dec[i][0] = data_copied[0];
				data_dec[i][1] = data_copied[0] >> 5 | data_copied[1] << 3;
				data_dec[i][2] = data_copied[1] >> 2;
				data_dec[i][3] = data_copied[1] >> 7 | data_copied[2] << 1;
				data_dec[i][4] = data_copied[2] >> 4 | data_copied[3] << 4;
				data_dec[i][5] = data_copied[3] >> 1;
				data_dec[i][6] = data_copied[3] >> 6 | data_copied[4] << 2;
				data_dec[i][7] = data_copied[4] >> 3;
				lcd.createChar(i, data_dec[i]);
			}
			
			if (frames == lrc[lrcstate].frame) {
				char strdata[12];
				uint8_t len;
				strcpy_P(strdata, lrc[lrcstate].l1);
				len = 11 - strlen(strdata);
				lcd.setCursor(5, 0);
				lcd.print(strdata);
				for (uint8_t i = 0; i < len; i++) {
					lcd.print(' ');
				}
				strcpy_P(strdata, lrc[lrcstate].l2);
				len = 11 - strlen(strdata);
				lcd.setCursor(5, 1);
				lcd.print(strdata);
				for (uint8_t i = 0; i < len; i++) {
					lcd.print(' ');
				}
				lrcstate++;
			}
			unsigned long tnext = ((unsigned long)frames * 100) / 3 + tstart;
			while(millis() < tnext);
			frames++;
		} else {
			dataFile.close();
			lcd.clear();
			lcd.setCursor(3, 0);
			lcd.print("Thanks for");
			lcd.setCursor(3, 1);
			lcd.print("watching!");
			fileready = false;
		}
	}
}

