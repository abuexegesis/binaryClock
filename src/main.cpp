#include <Arduino.h>

#define SECOND 1000
#define SWITCH_DEBOUNCE 50
#define SWITCH_LOW 200

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

#define SW1A A0
#define SW2A A1
#define SW3A A2
#define SW4A A3
#define NO_OF_SWITCHES 4
// does not seem to work > #define SWITCH(A0, A1, A2, A3)

void setup() {
  // put your setup code here, to run once:
  // pinMode(5, OUTPUT); // Set pin D5 as an output
  for(int dPin = 0; dPin < 14; dPin++) {
    pinMode(dPin, OUTPUT);
  }

// not sure this works ...
  static const uint8_t analog_switch[] = {SW1A, SW2A, SW3A, SW4A};
  for (int pin = 0; pin < NO_OF_SWITCHES; pin++) {
    pinMode(analog_switch[pin], INPUT_PULLUP);
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

int pollSwitches() {
/* Rough and ready way to check the pushbuttons 
I need a better way, as this was used 2x  
static const int analog_switch[] = {SW1A, SW2A, SW3A, SW4A}; */
  int switchValue;
  switchValue = 0;
  
  delay(SECOND);
  
  if (analogRead(A0) < SWITCH_LOW) switchValue = 1;
  if (analogRead(A1) < SWITCH_LOW) switchValue = 2;
  if (analogRead(A2) < SWITCH_LOW) switchValue = 3;
  if (analogRead(A3) < SWITCH_LOW) switchValue = 4;

  return switchValue;
}

void loop() {

  int action;
  action=0;
  action = pollSwitches();
  
/* Do something based on which pushbutton is pressed
  switch (action) {
    case 1:
      testLEDsection(CATHODE_SS);
      break;  
    case 2:
      testLEDsection(CATHODE_MM);
      break;
    case 3:
      testLEDsection(CATHODE_HH);
      break;
    case 4:
      testLEDs ();
      break;
  } */
  
}