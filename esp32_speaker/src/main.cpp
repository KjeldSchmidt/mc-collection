#include <Arduino.h>
#include <driver/dac.h>
// Pin to which an 8 Ohm speaker is connected (use a 150 - 220 Ohm resistor)
#define speakerPin DAC_CHANNEL_1


void setup() {
	// Set the speakerPin as an output
	dac_output_enable( speakerPin );

}

void loop() {
	dac_output_voltage( speakerPin, 127 + 127 * sin( millis() / 10 ));
}