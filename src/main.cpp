#include <Arduino.h>
#include <string.h>
#include <softRTC.h>
#include <Utilities.h>

#define SECOND 1000
#define DIGIT_SWEEP 10 // may use s/th diff?
#define LOW_TRIGGER 200

#define NO_OF_SWITCHES 4

#define CATHODES_HOURS B00000110 // Hours
#define CATHODES_MINUTES B00001010 // Minutes
#define CATHODES_SECONDS B00001100 // Seconds

static const int cathode[] = {CATHODES_HOURS, CATHODES_MINUTES, CATHODES_SECONDS};
static const uint8_t buttons[] = {A0,A1,A2,A3,A4};

// Setup myRTC and its associated variables
softRTC myRTC;

uint8_t d, m, h, min, s, weekday;
uint16_t y;
bool pm = false, is12 = false;
uint8_t time[] = {h,m,d};

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
  anodesData = adjustAnodesByte(twoDigitsToBCD(value));
  anodesCarry = anodesData.carry;
/* needed to add the anodes carry bit or else that bit would never
be displayed */
/* Serial.println(value);
  Serial.println(anodesData.shifted);
  Serial.println(anodesCarry);
  Serial.println(cathode[digit]);
  Serial.println(cathode[digit]+anodesCarry);
  Serial.println("***************");
  */
  PORTB = cathode[digit]+anodesCarry;
  PORTD = anodesData.shifted;
}

/* I have no idea why these library functions don't work when I put
them in my library "Utilites.cpp / .h" So that is why I put them here
*/

void setupButtons() {
    for (int i=0; i<5; i++){
    pinMode(buttons[i], INPUT_PULLUP);
    }
}

void buttonPressed(int button){
  String number = String(button+1);
  String message = "Button " + number + " pressed!";
  Serial.println(message);
}

void checkButton(int buttonNumber) {
  if (analogRead(buttons[buttonNumber]) < LOW_TRIGGER) buttonPressed(buttonNumber); 
}

void checkButtons() {
  for (int i=0; i<5; i++) {
    checkButton(i);
  }  
}

void setup() {
// CONSOLE debug setup
  Serial.begin(9600);

// Setup Arduino nano digial pins (could do with a PORTB, PORTD command)
  for(int dPin = 0; dPin < 14; dPin++) {
    pinMode(dPin, OUTPUT);
  }

  setupButtons();

  Serial.println("Start up!");
  before = millis();
  now = millis();
  segment_time_before = millis();
  segment_time_now = millis();
  

/* Yank this out after including the analogButtons code in
the Utilities.cpp / *.h

Set up Arduino nano analog pins for the pushbuttons
  static const uint8_t button[] = {A0, A1, A2, A3}; // SW1A, SW2A, SW3A, SW4A
  for (int pin = 0; pin < NO_OF_SWITCHES; pin++) {
    pinMode(button[pin], INPUT_PULLUP);
  }
*/

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
  
  /*Serial.println(h);
  Serial.println(m);
  Serial.println(s);
  */

  // Seed our binaryClock with a starting time corresponding to
  // when this code was built. Use default values of 1-Mar-2028
  myRTC.write(1, 4, 2028, h, m, s, false, MODE_24H);

  /* Initialze the clock display based on above reading
     is there a flash memory that could be used on the Arduino nano once
     the clock has already been run? */
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
  updateTime();
  segment_time_now = millis();
  if ((segment_time_now-segment_time_before) >= segment_loop_time) {
    segment_time_before = segment_time_now; // start a new loop time
    testDigit(digit_select,time[digit_select]);
    digit_select++;
    if (digit_select > 2) digit_select=0;
  }

/* this will no longer work with the new millis bit
// some testing here
  for (int count=0; count < 25; count ++){
    testDigit(0, count);
    delay(SECOND);
  }
*/

// updateDigit(0); // testing
// digit_select = updateDigit(digit_select);

}