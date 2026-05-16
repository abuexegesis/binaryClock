#ifndef UTILITIES_H
#define UTILITIES_H
#define ANODES_MASK_GENERAL B11111100
#define CATHODES_MASK_HOURS B11111100
#define CATHODES_MASK_GENERAL B1111110
#include <Arduino.h>
#include <string.h>

String testlib();

void syncToBuildTime();

byte digitToBCD(int number);

byte twoDigitsToBCD(int twodigits);

struct shiftAndCarry{
    byte shifted;
    byte carry;
};

struct currentTime{
    uint8_t h;
    uint8_t min;
    uint8_t s;
};

shiftAndCarry adjustAnodesByte(byte anodes);

void setupButtons();
void buttonPressed(int button);
void checkButton(int buttonNumber);
void checkButtons();

void syncToBuildTime();
currentTime updateTime();
void adjustTime(bool hh_mm, int delta);

#endif