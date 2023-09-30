#include <Arduino.h>
#include <Adafruit_MPR121.h>
#include <Wire.h>

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// Wiring:
// SCL - D1
// SDA - D2
// IRQ and ADD can float
Adafruit_MPR121 cap = Adafruit_MPR121();

unsigned long lastTriggerTime = 0;


void triggerFunction0() { Serial.println("triggerFunction0"); }
void triggerFunction1() { Serial.println("triggerFunction1"); }
void triggerFunction2() { Serial.println("triggerFunction2"); }
void triggerFunction3() { Serial.println("triggerFunction3"); }
void triggerFunction4() { Serial.println("triggerFunction4"); }
void triggerFunction5() { Serial.println("triggerFunction5"); }
void triggerFunction6() { Serial.println("triggerFunction6"); }
void triggerFunction7() { Serial.println("triggerFunction7"); }
void triggerFunction8() { Serial.println("triggerFunction8"); }
void triggerFunction9() { Serial.println("triggerFunction9"); }


void (*triggerFunctions[10])() = {
  triggerFunction0,
  triggerFunction1,
  triggerFunction2,
  triggerFunction3,
  triggerFunction4,
  triggerFunction5,
  triggerFunction6,
  triggerFunction7,
  triggerFunction8,
  triggerFunction9
};

void setup() {
  Serial.begin(9600);
  
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  cap.writeRegister(MPR121_AUTOCONFIG0, 0b00001011);


  // correct values for Vdd = 3.3V
  cap.writeRegister(MPR121_UPLIMIT, 200);     // ((Vdd - 0.7)/Vdd) * 256
  cap.writeRegister(MPR121_TARGETLIMIT, 180); // UPLIMIT * 0.9
  cap.writeRegister(MPR121_LOWLIMIT, 130);    // UPLIMIT * 0.65
  Serial.println("MPR121 found!");
}

void loop() {
  // debugging info
  // Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
  Serial.print("BaselineDiff:");

  for (uint16_t i=0; i<10; i++) {
    uint16_t currentReading = cap.filteredData(i);
    uint16_t baselineReading = cap.baselineData(i);
    int16_t baselineDiff = baselineReading - currentReading;

     Serial.print("\t"); Serial.print(baselineDiff);
  }

  Serial.println();

  delay(100);
}