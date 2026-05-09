#include <Arduino.h>
#include <string.h>
#include <softRTC.h>
#include <Utilities.h>

// macro idea from https://stackoverflow.com/questions/29983943/converting-int-to-binary-in-c-arduino
#define BIT(n,i) (n>>i&1)
#define SECOND 1000
#define HALF_SECOND 500
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

// declare myRTC for this project
softRTC myRTC;
// RTC variables
uint8_t d, m, h, min, s, weekday;
uint16_t y;
bool pm, is12;
char binaryOut[16];
// = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // used with sprintf and Serial.println() to ouptut messages to Console

byte intTOByte(int n){
  int tens;
  int ones;
  byte b;
  // should never be the case, but ...
  if (n > 59){
    n = 0;
  }
  tens = n/10;
  ones = n - 10*tens;
  b = 10*tens + ones;
  return b;
}

void updateTimeDisplay(){
  myRTC.read(d, m, y, h, min, s, pm, is12, weekday);

  Serial.println("The time is now:");
  Serial.println(h);
  Serial.println(min);
  Serial.println(s);
  Serial.println("");
  Serial.println("Int to hex value of seconds is: ");
  Serial.println(intTOByte(s));
}

void updateTimeSegment(int segment, int timeValue){
/*  segment HH=0 (D11); MM=1 (D10); SS=3 (D9)
    Port B (digital pins 8-13)
    Port D (digital pins 0-7)
           |--CATHODES-|------ANODES----------|
           |           |---TENS----|---ONES---|                     
    D13 D12 D11 D10 D9  D8 | D7 D6 D5 D4 D3 D2 D1 D0
      x   x SSC MMC HHC 64 | 32 16  8  4  2  1  x  x   - x= not connected
      1   1   1   1   0  0    1  1  1  1  1  1  0  0   - mask for HH cathode, only 2 bits for tens
      1   1   1   0   1  1    1  1  1  1  1  1  0  0   - mask for MM cathode, 3 bits for tens
      1   1   0   1   1  1    1  1  1  1  1  1  0  0   - mask for SS cathode, 3 bits for tens


ANODES_MASK_GENERAL B11111100
CATHODES_MASK_HOURS B11111100
CATHODES_MASK_GENERAL B11111110
*/
}     

void updateTime(){
  myRTC.read(d, m, y, h, min, s, pm, is12, weekday);
}

void setup() {
  // CONSOLE debug setup and test
  Serial.begin(9600);

  for(int dPin = 0; dPin < 14; dPin++) {
    pinMode(dPin, OUTPUT);
  }

// not sure this works ...
  static const uint8_t analog_switch[] = {SW1A, SW2A, SW3A, SW4A};
  for (int pin = 0; pin < NO_OF_SWITCHES; pin++) {
    pinMode(analog_switch[pin], INPUT_PULLUP);
  }

  // write(day, month, year, hour, minute, second, isPM, is12HMode)
  // Example: set 10:15:30 AM, 21 March 2025 in 24-hour mode
  // Use today 12:54:45 AM, 8 April 2026 in 24-hour mode
  myRTC.write(8, 5, 2026, 12, 54, 45, false, MODE_24H);

  /* Initialze the clock display based on above reading
     is there a flash memory that could be used on the Arduino nano once
     the clock has already been run? */

  // updateTimeDisplay();

}

/* Placeholder for thought about changing the display
 void changeTime(int hourDelta, int minuteDelta, int secondDelta) {
  readRTC();
  myRTC.write(day, month, year, hour+hourDelta, minute+minuteDelta,
  second+secondDelta, false, MODE_24H)
}*/

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

/*  int action;
  action=0;
  action = pollSwitches();
*/
  
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
/*  for (int timeValue = 0; timeValue < 60; timeValue++) {
    sprintf(printBuffer, "%d in binary is %d%d%d %d%d%d%d", timeValue, BIT(timeValue, 6), BIT(timeValue, 5), BIT(timeValue, 4), BIT(timeValue, 3),BIT(timeValue, 2), BIT(timeValue, 1), BIT(timeValue, 0));
    Serial.println(printBuffer);

    Serial.println("");
    delay(HALF_SECOND);
  }
  // updateTimeDisplay();*/
  word testOut = buildDisplayOut(B11110000, 2);
  Serial.println(testOut);
  Serial.println(testOut, BIN);

  Serial.println(testlib());
  Serial.println("Testing out building again");
  delay(SECOND);

}