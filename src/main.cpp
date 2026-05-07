#include <Arduino.h>

#define SECOND 1000

#define ANODE_UNITS_1 2
#define ANODE_UNITS_2 3
#define ANODE_UNITS_4 4
#define ANODE_UNITS_8 5
#define ANODE_TENS_1 6
#define ANODE_TENS_2 7
#define ANODE_TENS_4 8

#define CATHODE_SS 9
#define CATHODE_MM 10
#define CATHODE_HH 11

/* The idea here is for the cathode to be on as long as needed
but also to scan between cathodes fast enough that it appears
that all Hours Minutes Seconds LEDs are on all the time. This 
may take some time to get it right. I can as a first pass do
some googling to get some ballpark figures.
*/
#define cathodeNextScan 1000
#define cathodeScanWait 1000

// This next secion is ONLY for testing purposes
#define LEDtest 0
#define SSLEDs 1
#define MMLEDs 2
#define SSLEDs 3

void setup() {
  // put your setup code here, to run once:
  // pinMode(5, OUTPUT); // Set pin D5 as an output
  for(int dPin = 0; dPin < 14; dPin++) {
    pinMode(dPin, OUTPUT);
  }
}

void resetCathodes () {
  for(int cathode = 9; cathode < 12; cathode++){
    digitalWrite(cathode, HIGH);
  }
}

void resetAnodes () {
  for (int anode = 2; anode < 9; anode++){
    digitalWrite(anode, LOW);
  }
}

void selectCathode (int cathode) {
  resetCathodes();
  digitalWrite(cathode, LOW);
}

void selectAnode (int anode) {
  resetAnodes();
  digitalWrite(anode, HIGH);
}

// ONLY for testing purposes
void testLEDs () {
  for (int cathode = 9; cathode < 12; cathode++) {
    selectCathode(cathode);

    for (int anode = 2; anode < 9; anode++) {
      selectAnode(anode);
       delay(SECOND); // Wait for one second
    }
  }
}

void LEDs_on (int cathode) {
  selectCathode (cathode);
  for (int anode = 2; anode < 9; anode++){
    digitalWrite(anode, HIGH);
  }
}

void LEDs_off (int cathode) {
  selectCathode (cathode);
  for (int anode = 2; anode < 9; anode++){
    digitalWrite(anode, LOW);
  }
}

void testLEDsection (int section){
  LEDs_on(section);
  delay(SECOND);
  LEDs_off(section);
}

void loop() {
  testLEDs ();
  testLEDsection(CATHODE_SS);
  testLEDsection(CATHODE_MM);
  testLEDsection(CATHODE_HH);
}