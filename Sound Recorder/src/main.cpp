#include <SPI.h>
#include <SD.h>

#define MIC_PIN A0

#define SIGNAL_LED 2

// SD Pins
#define CS_PIN 10
#define MOSI_PIN 11
#define MISO_PIN 12
#define SCK_PIN 13

byte audioData[1000];
size_t dataOffset = 0;
unsigned long currentTime;
unsigned long samplePeriod = 8000;
unsigned long previousSampleTime;

bool inErorrState = false;
File audioFile;

void createFile() {
    String potentialFileName = "test.txt";
    if ( !SD.exists(potentialFileName) ) {
        audioFile = SD.open( potentialFileName, FILE_WRITE );
        return;
    } else {
        SD.remove( potentialFileName );
        audioFile = SD.open( potentialFileName, FILE_WRITE );
        return;
    }
}

// Riff Header written according to http://soundfile.sapp.org/doc/WaveFormat/
void writeRiffHeader() { 
    audioFile.print( "RIFF" );          // Riff - Begin Chunk
    audioFile.print( "0000" );          // Chunk Size - needs to be set at the end of recording to Filesize - 8 Bytes
    audioFile.print( "WAVE" );          // WAVE - End Chunk
    audioFile.print( "fmt " );          // fmt - Begin Subchunk 1
    audioFile.print( (uint32_t) 16 );   // Subchunk 1 Size, constant
    audioFile.print( (uint16_t) 1 );    // AudioFormat, Linear quantization
    audioFile.print( (uint16_t) 1 );    // Channel count
    audioFile.print( (uint32_t) 8000 ); // Sample Rate 
    audioFile.print( (uint32_t) 8000 ); // Byte Rate
    audioFile.print( (uint16_t) 1 );    // Block Align
    audioFile.print( (uint16_t) 1 );    // BitsPerSample - End Subchunk 1
    audioFile.print( "data" );          // data - Begin Subchunk 2
    audioFile.print( "0000");           // Total Size of Data not including any of the header. Needs to be set to Filesize - 44 Bytes
}

void finalizeFile() {
    audioFile.seek( 4 ); // Goto Chunk Size
    audioFile.print( audioFile.size() - 8 );
    audioFile.seek( 40 );
    audioFile.print( audioFile.size() - 44 );
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

    currentTime = millis();

    if ( currentTime < (unsigned long) 60*1000 && previousSampleTime + samplePeriod <= currentTime ) {
        audioData[ dataOffset ] = analogRead( MIC_PIN );
        ++dataOffset;
        previousSampleTime = millis();
    }

    if ( dataOffset == 1000 ) {
        dataOffset = 0;
        audioFile.write( audioData, 1000 );
        audioFile.flush();
    }

    if ( currentTime > (unsigned long) 61*1000 ) {
        finalizeFile();
        digitalWrite( SIGNAL_LED, HIGH );
        while ( true );
    }

}