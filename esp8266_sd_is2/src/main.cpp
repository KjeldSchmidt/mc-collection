#include <i2s.h>
#include <i2s_reg.h>

#define write_sample(data) while (i2s_write_sample_nb(data)==0)

void outputDMA(void)
{
    i2s_begin();
    i2s_set_rate(48000);

    uint8_t state=0;
    for (int i=0; i<48000;i++)
    {
        state = (i >> 12)%2; // Toggle every 4096 double-words
        if (state)
            write_sample(0xffff0000);
        else
            write_sample(0x0000ffff);

        if ((i % 1000) == 0) yield(); // without this get WDT resets
    }
    i2s_end();
}

void setup() {
	Serial.begin(115200);
	pinMode(3, OUTPUT); // Override default Serial initiation
	digitalWrite(3,0); // Set pin low
}

void loop() {
    static unsigned long last = millis();
    if (millis() - last > 1000) {
        outputDMA();
        last = millis();
    }
}

