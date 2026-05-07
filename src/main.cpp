#include <Arduino.h>

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

void loop() {

  for (int cathode = 9; cathode < 12; cathode++) {
    selectCathode(cathode);

    for (int anode = 2; anode < 9; anode++) {
      selectAnode(anode);
       delay(1000); // Wait for one second
    }
  }

}