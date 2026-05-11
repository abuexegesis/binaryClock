#include <Arduino.h>
#include <string.h>
#include <softRTC.h>
#include <Utilities.h>

#define SECOND 1000
#define DIGIT_SWEEP 10
#define SWITCH_LOW 200
#define SW1A A0
#define SW2A A1
#define SW3A A2
#define SW4A A3
#define NO_OF_SWITCHES 4
// does not seem to work > #define SWITCH(A0, A1, A2, A3)
#define CATHODES_HOURS B00111100
#define CATHODES_MINUTES B00111010
#define CATHODES_SECONDS B00110110

// declare myRTC for this project
softRTC myRTC;
// RTC variables
uint8_t d, m, h, min, s, weekday;
uint16_t y;
bool pm, is12;

shiftAndCarry anodesData;
byte anodesCarry;   

void updateTime(){
  myRTC.read(d, m, y, h, min, s, pm, is12, weekday);
}

void setup() {
  // CONSOLE debug setup and test
  Serial.begin(9600);

  for(int dPin = 0; dPin < 14; dPin++) {
    pinMode(dPin, OUTPUT);
  }

  static const uint8_t analog_switch[] = {SW1A, SW2A, SW3A, SW4A};
  for (int pin = 0; pin < NO_OF_SWITCHES; pin++) {
    pinMode(analog_switch[pin], INPUT_PULLUP);
  }
  
  myRTC.write(8, 5, 2026, 12, 54, 45, false, MODE_24H);

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

// Refactor this as a loop
  
  anodesData = adjustAnodesByte (twoDigitsToBCD(s));
  anodesCarry = anodesData.carry;
  PORTB = CATHODES_HOURS || anodesCarry;
  PORTD = anodesData.shifted;
  delay(DIGIT_SWEEP);
  
  anodesData = adjustAnodesByte (twoDigitsToBCD(m));
  anodesCarry = anodesData.carry;
  PORTB = CATHODES_MINUTES  || anodesCarry;
  PORTD = anodesData.shifted;
  delay(DIGIT_SWEEP);
  
  anodesData = adjustAnodesByte (twoDigitsToBCD(h));
  anodesCarry = anodesData.carry;
  PORTB = CATHODES_SECONDS ;
  PORTD = anodesData.shifted || anodesCarry;
  delay(DIGIT_SWEEP);

}