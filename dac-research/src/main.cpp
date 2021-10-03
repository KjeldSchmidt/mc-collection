#include <Arduino.h>
#include <i2s.h>
#include <i2s_reg.h>
#define write_sample(data) while (i2s_write_sample_nb(data)==0)

uint32_t fakePwm[] = {
		0,420980412,837906552,1246763195,1643612826,2024633568,2386155981,2724698408,3037000499,3320054616,3571134792,3787822988,3968032377,4110027446,4212440703,4274285854,4294967295,4274285854,4212440703,4110027446,3968032377,3787822988,3571134792,3320054616,3037000499,2724698408,2386155981,2024633568,1643612826,1246763195,837906552,420980412
};

uint32_t fakePwm2[] = {
		0, 837906552, 1643612826, 2386155981, 3037000499, 3571134792, 3968032377, 4212440703, 4294967295, 4212440703, 3968032377, 3571134792, 3037000499, 2386155981, 1643612826, 837906552
};

void outputDMA(void)
{
	i2s_begin();
	i2s_set_rate(48000); // Rate set to 50,000 lots of 32-bits per sec

	uint8_t state=0;
	for (int i=0; i<65536; i++) //(0 - (2^16-1))
	{

		state = (i >> 12)%2; // Toggle every 4096 double-words
		if (state)
			write_sample(fakePwm[i%32]);
		else
			write_sample(fakePwm2[i%16]);

		if ((i % 1000) == 0) yield(); // without this get WDT resets
	}
	i2s_end();
}

void setup()
{
	//Serial.begin(115200);

	pinMode(3, OUTPUT); // Override default Serial initiation
	digitalWrite(3,0); // Set pin low
}

void loop()
{
	static unsigned long last = millis();
	if (millis() - last > 1000) {
		outputDMA();
		last = millis();
	}
}