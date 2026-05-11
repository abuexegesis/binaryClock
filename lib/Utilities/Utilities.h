#ifndef UTILITIES_H
#define UTILITIES_H
#define ANODES_MASK_GENERAL B11111100
#define CATHODES_MASK_HOURS B11111100
#define CATHODES_MASK_GENERAL B1111110
#include <Arduino.h>
#include <string.h>

String testlib();

byte digitToBCD(int number);

byte twoDigitsToBCD(int twodigits);

/*word buildDisplayOut(byte displayMask, int displaySegment);
String testlib();*/

#endif