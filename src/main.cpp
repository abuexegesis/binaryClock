#include <Arduino.h>
#include <string.h>
#include <softRTC.h>
#include <Utilities.h>

#define SECOND 1000
#define DIGIT_SWEEP 10 // may use s/th diff?
#define SWITCH_LOW 200

#define NO_OF_SWITCHES 4

#define CATHODES_HOURS B00000110 // Hours
#define CATHODES_MINUTES B00001010 // Minutes
#define CATHODES_SECONDS B00001100 // Seconds

static const int cathode[] = {CATHODES_HOURS, CATHODES_MINUTES, CATHODES_SECONDS};

// Setup myRTC and its associated variables
softRTC myRTC;

uint8_t d, m, h, min, s, weekday;
uint16_t y;
bool pm = false, is12 = false;
uint8_t time[] = {h,m,d};

int digit_select = 0; // 0 => HH, 1 => MM, 2 => SS
const unsigned long loop_time = 500;  // interval at which to poll
const unsigned long digit_refresh_time = 10; // may need to use micros() to get 1500 , or 1.5 ms
/* 200Hz refresh rate -> 1000/200
let's use 6 so it is 2 ms per digit 
for complete display refresh. Some research recommends the sweet
spot of 100 to 250 Hz refresh per digit */
unsigned long now; // where to put the current value of millis()
unsigned long before; // previous time millis() was called
unsigned long segment_time_now;
unsigned long segment_time_before;

shiftAndCarry anodesData;
byte anodesCarry;

/* Use the following strings from compile time to set
a reasonable starting point for the binaryClock */
String time_compiled = __TIME__;

void updateTime(){
  myRTC.read(d, m, y, h, min, s, pm, is12, weekday);
  time[0]=h;
  time[1]=m;
  time[2]=s;
}

void testDigit(int digit, int value) {
  anodesData = adjustAnodesByte (twoDigitsToBCD(value));
//void testDigit(int digit){ // 0-> HH, 1-> MM, 2-> SS
//  anodesData = adjustAnodesByte (twoDigitsToBCD(time[digit]));
  anodesCarry = anodesData.carry;
  PORTB = cathode[digit]; 
  PORTD = anodesData.shifted;
}

void setup() {
// CONSOLE debug setup
  Serial.begin(9600);

// Setup Arduino nano digial pins (could do with a PORTB, PORTD command)
  for(int dPin = 0; dPin < 14; dPin++) {
    pinMode(dPin, OUTPUT);
  }

// Set up Arduino nano analog pins for the pushbuttons
  static const uint8_t button[] = {A0, A1, A2, A3}; // SW1A, SW2A, SW3A, SW4A
  for (int pin = 0; pin < NO_OF_SWITCHES; pin++) {
    pinMode(button[pin], INPUT_PULLUP);
  }

/* this wonky temp_time is used because the compiler refuses
to do this:
h=time_compiled.substring(0,2).toInt();
*/
  String temp_time=time_compiled.substring(0,2);
  h = temp_time.toInt();
  temp_time=time_compiled.substring(3,5);
  m = temp_time.toInt();
  // using ...substring(6) because it works, (6,7) only give one digit, (6,8) gives 2
  temp_time=time_compiled.substring(6);
  s = temp_time.toInt();
  
  Serial.println(h);
  Serial.println(m);
  Serial.println(s);

  // Seed our binaryClock with a starting time corresponding to
  // when this code was built. Use default values of 1-Mar-2028
  myRTC.write(1, 4, 2028, h, m, s, false, MODE_24H);

  /* Initialze the clock display based on above reading
     is there a flash memory that could be used on the Arduino nano once
     the clock has already been run? */
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

updateTime();
//d, m, y, h, min, s, pm, is12, weekday

// some testing here
for (int count=0; count < 25; count ++){
  testDigit(0, count);
  delay(SECOND);
}

// updateDigit(0); // testing
// digit_select = updateDigit(digit_select);

}