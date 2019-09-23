#include <MIDI.h>

// Simple tutorial on how to receive and send MIDI messages.
// Here, when receiving any message on channel 4, the Arduino
// will blink a led and play back a note for 1 second.

MIDI_CREATE_DEFAULT_INSTANCE();

#define ONBOARD_LED 13

unsigned char numberOfCurrentlyPlayingNotes = 0;

void MyHandleNoteOn( byte Channel, byte pitch, byte velocity ) {
    ++numberOfCurrentlyPlayingNotes;
    digitalWrite( ONBOARD_LED, HIGH );
}

void MyHandleNoteOff( byte Channel, byte pitch, byte velocity ) {
    --numberOfCurrentlyPlayingNotes;
    if ( numberOfCurrentlyPlayingNotes == 0 ) {
        digitalWrite( ONBOARD_LED, LOW );
    }
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

