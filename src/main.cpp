#include <Arduino.h>
#include <string.h>
// #include <softRTC.h>
#include <Utilities.h>

#define SECOND 1000
#define DIGIT_SWEEP 10 // may use s/th diff?
//#define LOW_TRIGGER 200

#define NO_OF_SWITCHES 4

#define CATHODES_HOURS B00000110 // Hours
#define CATHODES_MINUTES B00001010 // Minutes
#define CATHODES_SECONDS B00001100 // Seconds

static const int cathode[] = {CATHODES_HOURS, CATHODES_MINUTES, CATHODES_SECONDS};
static const uint8_t buttons[] = {A0,A1,A2,A3,A4};

int digit_select = 0; // 0 => HH, 1 => MM, 2 => SS
const unsigned long button_loop_time = 150;  // interval at which to poll
const unsigned long segment_loop_time = 10; // may need to use micros() to get 1500 , or 1.5 ms
/* 200Hz refresh rate -> 1000/200
let's use 6 so it is 2 ms per digit 
for complete display refresh. Some research recommends the sweet
spot of 100 to 250 Hz refresh per digit */
unsigned long now; // where to put the current value of millis()
unsigned long before; // previous time millis() was called
unsigned long segment_time_now;
unsigned long segment_time_before;
bool newLoopTime = false;
int count;
uint8_t time[3];
currentTime updatedTime;

shiftAndCarry anodesData;
byte anodesCarry;

void testDigit(int digit, int value) {
  anodesData = adjustAnodesByte(twoDigitsToBCD(value));
  anodesCarry = anodesData.carry;
/* needed to add the anodes carry bit or else that bit would never
be displayed */
  PORTB = cathode[digit]+anodesCarry;
  PORTD = anodesData.shifted;
}

//************ SETUP *******************************************
void setup() {
// CONSOLE debug setup
  Serial.begin(9600);

// Setup Arduino nano digial pins (could do with a PORTB, PORTD command)
  for(int dPin = 0; dPin < 14; dPin++) {
    pinMode(dPin, OUTPUT);
  }

  setupButtons();

  Serial.println("Use compiled time to set up clock time!");
  before = millis();
  now = millis();
  segment_time_before = millis();
  segment_time_now = millis();

  void syncToBuildTime(); // Set myRTC as though it was the build time

}

void loop() {

  now = millis();
  if ((now-before) >= button_loop_time) {
//    Serial.println(". . . another loop passed by!");
    before = now; // start a new loop time
    checkButtons();
  }
 
// update digits loop
//d, m, y, h, min, s, pm, is12, weekday
  updatedTime = updateTime();
  
  segment_time_now = millis();
  time[0]=updatedTime.h;
  time[1]=updatedTime.min;
  time[2]=updatedTime.s;
  if ((segment_time_now-segment_time_before) >= segment_loop_time) {
    segment_time_before = segment_time_now; // start a new loop time
    testDigit(digit_select,time[digit_select]);
    digit_select++;
    if (digit_select > 2) digit_select=0;
  }

}