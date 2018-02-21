#include <SPI.h>
#include <SD.h>

// PHYSICAL CONSTANTS
#define MICROS_PER_SECOND 1000000


// PROGRAM PARAMETERS
#define AUDIO_BUFFER_SIZE 500
#define SAMPLE_FREQUENCY 8000


// PINS
#define MIC_PIN 0
#define SIGNAL_LED 2

// SD Pins
#define CS_PIN 10
#define MOSI_PIN 11
#define MISO_PIN 12
#define SCK_PIN 13

byte audioData[AUDIO_BUFFER_SIZE];
size_t dataOffset = 0;
unsigned long currentTime;
unsigned long samplePeriod = MICROS_PER_SECOND / SAMPLE_FREQUENCY;
unsigned long previousSampleTime;

bool inErorrState = false;
File audioFile;

void writeNumToFile( uint32_t num) {
    byte byte0 = num >> 24;
    byte byte1 = num >> 16;
    byte byte2 = num >> 8;
    byte byte3 = num >> 0;

    audioFile.write( byte3 );
    audioFile.write( byte2 );
    audioFile.write( byte1 );
    audioFile.write( byte0 );
}

void writeNumToFile( uint16_t num) {
    byte byte0 = num >> 8;
    byte byte1 = num >> 0;

    audioFile.write( byte1 );
    audioFile.write( byte0 );
}

void createFile() {
    String potentialFileName = "audio.wav";
    if ( !SD.exists("audio.wav") ) {
        audioFile = SD.open( "audio.wav", FILE_WRITE );
        return;
    } else {
        SD.remove( "audio.wav" );
        audioFile = SD.open( "audio.wav", FILE_WRITE );
        return;
    }
}

// Riff Header written according to http://soundfile.sapp.org/doc/WaveFormat/
void writeRiffHeader() { 
    audioFile.print( "RIFF" );          // Riff - Begin Chunk
    audioFile.print( "0000" );          // Chunk Size - needs to be set at the end of recording to Filesize - 8 Bytes
    audioFile.print( "WAVE" );          // WAVE - End Chunk
    audioFile.print( "fmt " );          // fmt - Begin Subchunk 1
    writeNumToFile( (uint32_t) 16 );    // Subchunk 1 Size, constant
    writeNumToFile( (uint16_t) 1 );     // AudioFormat, Linear quantization
    writeNumToFile( (uint16_t) 1 );     // Channel count
    writeNumToFile( (uint32_t) 8000 );  // Sample Rate 
    writeNumToFile( (uint32_t) 8000 );  // Byte Rate
    writeNumToFile( (uint16_t) 1 );     // Block Align
    writeNumToFile( (uint16_t) 1 );     // BitsPerSample - End Subchunk 1
    audioFile.print( "data" );          // data - Begin Subchunk 2
    audioFile.print( "0000");           // Total Size of Data not including any of the header. Needs to be set to Filesize - 44 Bytes
    audioFile.flush();
}

void finalizeFile() {
    audioFile.seek( 4 ); // Goto Chunk Size
    writeNumToFile( audioFile.size() - 8 );
    audioFile.seek( 40 );
    writeNumToFile( audioFile.size() - 44 );
    audioFile.close();
}

void initializeFile() {
    createFile();
    writeRiffHeader();
}

void setup() {
    pinMode( SIGNAL_LED, OUTPUT );
    if ( !SD.begin( CS_PIN ) ) {
        inErorrState = true;
        return;
    }
    initializeFile();
}

void loop() {
    if ( inErorrState ) {
        while ( true ) {
            digitalWrite( SIGNAL_LED, HIGH );
            delay(750);
            digitalWrite( SIGNAL_LED, LOW );
            delay(750);
        }
    }

    currentTime = micros();

    if ( currentTime < 60UL*1000000UL && previousSampleTime + samplePeriod <= currentTime ) {
        audioData[ dataOffset ] = analogRead( MIC_PIN ) - 127;
        ++dataOffset;
        previousSampleTime = currentTime;
    }

    if ( dataOffset == AUDIO_BUFFER_SIZE ) {
        dataOffset = 0;
        audioFile.write( audioData, AUDIO_BUFFER_SIZE );
        audioFile.flush();
    }

    if ( currentTime > 61UL * 1000000UL ) {
        finalizeFile();
        digitalWrite( SIGNAL_LED, HIGH );
        while ( true );
    }

}