//#define bank0
//#define bank1
//#define bank2
#define bank3

#include "fonts.h"

#define EEPROM_OFFSET 0x10000
#include <Wire.h>
#include <extEEPROM.h>

extEEPROM *eep; 
unsigned char buffer[16]= {0};
unsigned char buffer2[16] = {0};
long address;
int status;

void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);
	Serial.println("Beginning...");
	pinMode(A4, INPUT_PULLUP);
	pinMode(A5, INPUT_PULLUP);
	pinMode(A1, INPUT_PULLUP);
	
	Serial.println("Waiting to program...");
	while (digitalRead(A1) == HIGH)
		delay(250);

	eep = new extEEPROM(kbits_1024, 1, 256, 0x50);
	if (eep)
	{
		status = eep->begin();      //go fast!
		if (status == 0) 
		{
			Serial.println("Programming...");
			address = EEPROM_OFFSET+EEPROM_BEGIN;
			
			for (long i = 0; i < sizeof(font); i += 16) {
				memcpy_P(buffer, font+i, 16);
				Serial.println(address, 16);
				eep->write(address, buffer, 16);
				address += 16;
			}

			Serial.println("Verifying...");
			address = EEPROM_OFFSET+EEPROM_BEGIN;
			for (long i = 0; i < sizeof(font); i += 16) {
				Serial.println(address, 16);
				memcpy_P(buffer2, font+i, 16);
				eep->read(address, buffer, 16);
				for (int j = 0; j < 16; j++ ) {
					if (buffer2[j] != buffer[j]) {
						Serial.print("Verify error, expected: ");
						Serial.print(buffer2[j], 16);
						Serial.print(" but got: ");
						Serial.println(buffer[j], 16);
					}
				}
				address += 16;
			} 
		} else {
			Serial.print("begin failed with ");
			Serial.println(status);
		}
	} else {
		Serial.println("eeprom init failed,");
	}
	
}

void loop() {
  // put your main code here, to run repeatedly:

}
