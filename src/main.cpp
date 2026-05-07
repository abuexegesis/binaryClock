#include <Arduino.h>

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

// This next secion is ONLY for testing purposes
#define LED_TEST 0
#define SS_LEDS 1
#define MM_LEDS 2
#define HH_LEDS 3
#define LED_TEST_LOOPS 3

// Testing for output to console for DEBUG messages
int My_Int = 10;
unsigned long My_TimeStamp = 1416803716;
const char *My_CharArray = {"This is My_CharArray"};
float My_Float = 3.14159266759;

void setup() {
  // put your setup code here, to run once:
  // pinMode(5, OUTPUT); // Set pin D5 as an output
  for(int dPin = 0; dPin < 14; dPin++) {
    pinMode(dPin, OUTPUT);
  }

  static const uint8_t analog_switch[] = {SW1A, SW2A, SW3A, SW4A};
  for (int pin = 0; pin < NO_OF_SWITCHES; pin++) {
    pinMode(analog_switch[pin], INPUT_PULLUP);
  }

  // CONSOLE debug setup and test
  Serial.begin(9600);
  Serial.println("");
  Serial.println("BEGIN TEST");
  /*Serial.println(My_Int);
  Serial.println(My_TimeStamp);
  Serial.println(My_CharArray);
  Serial.print(My_Float, 5);  // prints to five places right of the decimal
  */
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
       delay(SECOND);
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
  for (int iterations = 0; iterations < LED_TEST_LOOPS; iterations++) {
    LEDs_on(section);
    delay(SECOND);
    LEDs_off(section);
    delay(HALF_SECOND);
  }
}

int pollSwitches() {
// I need a better way, as this was used 2x  
  // static const int analog_switch[] = {SW1A, SW2A, SW3A, SW4A};
  //int switchValue;
  
  /*for (int button = 0; button < NO_OF_SWITCHES; button++){
    switchValue = analogRead(analog_switch[button]);  
      if (switchValue < SWITCH_LOW) {
        Serial.println("This switch is on: " + button);
        delay(SECOND);
      }  
    }*/
  Serial.println("SW1A value is: ");
  Serial.println(analogRead(A0));
  Serial.println("SW2A value is: ");
  Serial.println(analogRead(A1));
  Serial.println("SW3A value is: ");
  Serial.println(analogRead(A2));
  Serial.println("SW4A value is: ");
  Serial.println(analogRead(A3));
  delay(2*SECOND);
  return 0;
}

void loop() {

  int action;
  action = pollSwitches();
  
 /* switch (action) {
    case 0:
      testLEDs ();
      break;
    case 1:
      testLEDsection(CATHODE_SS);
      break;
    case 2:
      testLEDsection(CATHODE_MM);
      break;
    case 3:
      testLEDsection(CATHODE_HH);
      break;
  }*/
  
  // Serial.println(analogRead(action));
  
}