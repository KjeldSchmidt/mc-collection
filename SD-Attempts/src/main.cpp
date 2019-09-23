#include <SPI.h>
#include <SD.h>

/*
*	Basic SD card usage. Connect pins as defined below (for Uno/Nano), add 5V and Ground.
*
*	Note that in the current version of the logging process, the log file is always appended, not created fresh.
*
*/

#define CS_PIN 10
#define MOSI_PIN 11
#define MISO_PIN 12
#define SCK_PIN 13

File logger;

void setup() {
	Serial.begin(9600);
	while( !Serial ) {}

	Serial.print("Initializing SD card");

	if ( !SD.begin( CS_PIN ) ) {
		Serial.println("Initialization failed!");
		return;
	}

	Serial.println("Initialization done!");

	if( SD.exists("logger.txt") ) {
		Serial.println("logger.txt exists - appending");
	} else {
		Serial.println("logger.txt doesn't exist yet - creating");
	}

	Serial.println("Creating/Opening");
	logger = SD.open("logger.txt", FILE_WRITE);

	if( SD.exists("logger.txt") ) {
		Serial.println("logger.txt exists");
	} else {
		Serial.println("logger.txt doesn't exist yet - oops");
	}
}

void loop() {
	bool print = true;
	if ( print ) {
		for ( char i = 99; i >= 0; --i) {
			logger.print(i, DEC);
			logger.print(" bytes of char in the index, ");
			logger.print(i, DEC);
			logger.print(" bytes of char - as of ");
			logger.println( millis() );
			delay( 30 );
		}

		Serial.println(" Counting done");
	}


	if ( Serial.available() > 0 ) {
		logger.close();
		print = false;
		Serial.println("Closed everything.");
		Serial.end();
	}
	
}