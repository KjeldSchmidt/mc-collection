/*
*  This will take MIDI input and control a NeoLED Strip based on that.
*
*
*/

#include <MIDI.h>
#include <NeoPixelBus.h>


#define LED_STRIP_PIN 12
#define NUMBER_OF_LEDS 88
#define ONBOARD_LED 13


MIDI_CREATE_DEFAULT_INSTANCE();
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(NUMBER_OF_LEDS, LED_STRIP_PIN);


RgbColor blue(2, 1, 4);
RgbColor black(0, 0, 0);

void MyHandleNoteOn( byte Channel, byte pitch, byte velocity ) {
    byte ledToTurnOn = pitch - 21;
    strip.SetPixelColor(ledToTurnOn, blue);
    strip.Show();
}

void MyHandleNoteOff( byte Channel, byte pitch, byte velocity ) {
    byte ledToTurnOn = pitch - 21;
    strip.SetPixelColor(ledToTurnOn, black);
    strip.Show();
}

void setup()
{
    pinMode(ONBOARD_LED, OUTPUT);
    MIDI.begin(MIDI_CHANNEL_OMNI);                      // Launch MIDI and listen to all channels
    MIDI.setHandleNoteOn(MyHandleNoteOn);
    MIDI.setHandleNoteOff(MyHandleNoteOff);
}

void loop()
{
    MIDI.read();
}

