#include <Arduino.h>
#include <driver/dac.h>
// Pin to which an 8 Ohm speaker is connected (use a 150 - 220 Ohm resistor)
#define speakerPin DAC_CHANNEL_1


void setup() {
	// Set the speakerPin as an output
	Serial.begin( 115200 );
	dac_output_enable( speakerPin );

}

void loop() {
	Serial.println( "255" );
	dac_output_voltage( speakerPin, 255 );
	delay( 3000 );
	Serial.println( "128" );
	dac_output_voltage( speakerPin, 128 );
	delay( 3000 );
	Serial.println( "0" );
	dac_output_voltage( speakerPin, 0 );
	delay( 3000 );
}