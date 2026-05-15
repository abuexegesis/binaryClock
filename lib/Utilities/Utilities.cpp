#include <Utilities.h>

// bring back later? #define LOW_TRIGGER 200
// bring back later .. static const uint8_t buttons[] = {A0,A1,A2,A3,A4};

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
like. Except the errors are NOT obvious at all
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