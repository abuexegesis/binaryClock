#include <Arduino.h>
#include <Utilities.h>
#include <softRTC.h>

#define LOW_TRIGGER 200
static const uint8_t buttons[] = {A0,A1,A2,A3,A4};

// Setup myRTC and its associated variables

uint8_t d, m, h, min, s, weekday;
uint16_t y;
bool pm = false, is12 = false;
uint8_t time[] = {h,min,s};
softRTC myRTC;

void syncToBuildTime(){
/* Use the following strings from compile time to set
a reasonable starting point for the binaryClock */
  String time_compiled = __TIME__;

/* TEMPORARY => This wonky temp_time is used because of type mismatches that
the compiler whines about, so do this:
h=time_compiled.substring(0,2).toInt();*/

  String temp_time=time_compiled.substring(0,2);
  h = temp_time.toInt();
  temp_time=time_compiled.substring(3,5);
  m = temp_time.toInt();
// using ...substring(6) because it works, (6,7) only give one digit, (6,8) gives 2
  temp_time=time_compiled.substring(6);
  s = temp_time.toInt();

// Seed our binaryClock with a starting time corresponding to
// when this code was built. Use default values of 1-Mar-2028
  myRTC.write(1, 4, 2028, h, m, s, false, MODE_24H);

/* Initialze the clock display based on above reading
   is there a flash memory that could be used on the Arduino nano once
   the clock has already been run? 
*/
}

currentTime updateTime(){
  myRTC.read(d, m, y, h, min, s, pm, is12, weekday);
  currentTime time;
  /*time[0]=h;
  time[1]=min;
  time[2]=s;*/
  time.h = h;
  time.min = min;
  time.s = s;
  return time;
}

/* hh_mm = false -> change MM; true -> change HH settings
delta is how much to change it by (+/- delta)

TEST: simply display two different numbers for HH MM and
test the four buttons out, then return the code to displaying
the clock on the LEDs
*/

void adjustTime(bool hh_mm, int delta){

  updateTime();

  if (hh_mm = false){
    m = m+delta;
  } else {
    h = h+delta;
  }

  myRTC.write(1, 4, 2028, h, m, s, false, MODE_24H);
}

word buildDisplayOut(byte displayMask, int displaySegment) {
    word displayOut;
    displayOut = CATHODES_MASK_GENERAL*256 + ANODES_MASK_GENERAL;
    
    return displayOut;
} 

String testlib(){
    String message = "Test out the library";
    return message;
}

byte digitToBCD(int number) {
    byte bcdEncodedNumber;
    if ((number > 9) || (number < 0)) number = 0;  // one digit safety

    switch (number) {
    case 9:
        bcdEncodedNumber = B00001001;
        break;
    case 8:
        bcdEncodedNumber = B00001000;
        break;
    case 7:
        bcdEncodedNumber = B0000111;
        break;
    case 6:
        bcdEncodedNumber = B00000110;
        break;
    case 5:
        bcdEncodedNumber = B00000101;
        break;
    case 4:
        bcdEncodedNumber = B00000100;
        break;
    case 3:
        bcdEncodedNumber = B00000011;
        break;
    case 2:
        bcdEncodedNumber = B00000010;
        break;
    case 1:
        bcdEncodedNumber = B00000001;
        break;
    case 0:
        bcdEncodedNumber = B00000000;    
    }
    return bcdEncodedNumber;
}

byte twoDigitsToBCD(int twodigits){
    byte bcdEncodedNumber;
    int tens;
    int ones;
    if ((twodigits > 99) || (twodigits < 0)) twodigits = 0;  // two digit safety

    ones = twodigits % 10;
    tens = (twodigits - ones) / 10;

    bcdEncodedNumber = 16*tens + ones;
    return bcdEncodedNumber;

}

shiftAndCarry adjustAnodesByte(byte anodes){
  shiftAndCarry result;
  result.shifted = (byte)(anodes << 2);
  // Serial.println("From shift and carry function");
/* Note the carry is not the most significant bit, that would be
if you are only shifting left one time. We are shifting left 2x! */
  //Serial.println(anodes);  
  result.carry = B01000000 & anodes; // not &&, that is two booleans
  //Serial.println(result.carry);
  if (result.carry == B01000000) {
       result.carry = B00000001;
  }
  else {
    result.carry = B00000000; 
  }
  return result;
}

/* maybe the recompile after getting rid of some .pio files to
force a recompile of the library now I get linking errors it looks
like. Except the errors are NOT obvious at all */
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

/*===============================================================

*/

// None of the day month year even matters, see main.cpp!
String timestamp = __TIMESTAMP__;
  // write(day, month, year, hour, minute, second, isPM, is12HMode)
  // Example: set 10:15:30 AM, 21 March 2025 in 24-hour mode
  // Use today 12:54:45 AM, 8 April 2026 in 24-hour mode


  /* TIMESTAMP yeilds: Mon May 11 16:57:55 2026
     tens digit        000000000011111111112222
     ones digit        012345678901234567890123
  */

 // int timeIDX[14] {0, 3, 4, 6, 8, 9, 11, 12, 14, 15, 17, 18, 20, 23};

 /*  convertTimestamp (String[24]) {}   => return a structure / define the structure
  weekday = atoi(timestamp.substring(0,3));
  day = dayNameToInt (weekday);
  dayNameToInt(String dayName) {
    int day
    switch (dayName) {
    case "Mon":
      day = 1;
      break;
    case "Tue":
      day = 2;
      break;
    case "Wed":
      day = 3;
      break;
    case "Thu":
      day = 4;
      break;
    case "Fri":
      day = 5;
      break;
    case "Sat":
      day = 6;
      break;
    case "Sun":
      day = 7;
    }
    return day;
  }
  String mmm = timestamp.substring(4,3);
  m = monthStringToInt (mmm);
  d = atoi(timestamp.substring(8,2));
  h = atoi(timestamp.substring(11,2));
  if (h > 12) {
    h = h - 12;
  }
  min = atoi(timestamp.substring(14,2));
  s = atoi(timestamp.substring(17,2));
  y = atoi(timestamp.substring(17,2));
*/

/* Misc working utilities from Main.cpp, will need Utilities.h definitions if used here
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

*/