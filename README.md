# binaryClock

## copyright

The hardware and schematic are copyright of Prof Martin Störzer of Hochschule Ravensbur-Weingarten University of Applied Sicences

## This code is MIT

Author Dawson Tennant
Initial commit date: 5-May-2026

## Description

This code is for David's Binary Clock soldering project.

## Hardware

See the picture of the hardware in docs folder.

### Inputs

* four pushbutton switches (no pull up resistor). When the pushbutton is closed, it connects to ground.
* A0 SW1a Hours+
* A1 SW2a Minutes+
* A0 SW3a Hours-
* A0 SW4a Minutes-
  **Function** to set the clock's time (Hours and Minutes via up / down switches)
  **Suggestions**
* also, need to debounce these switches through s/w
* short press move up or down one digit
* long press move up or down using a counter

### Outputs

20 LED's connected to their Anodes with a 220R resistor between D2-D8 which drives the Hours (Yellow), Minutes (Green), and Seconds (Red) displays and the units and tens of each of these parts of time.

* Seconds Units place D2 D3 D4 D5
* Seconds Tens place D6 D7 D8
* Minutes Units place D2 D3 D4 D5
* Minutes Tens place D6 D7 D8
* Hours Units place D2 D3 D4 D5
* Hours Tens place D6 D7
  Note, the Hours only needs 2 bits to go from 0-2
  The cathodes are connected as follows:
* D9 connects to the Seconds (Red) LEDs
* D10 connects to the Minutes (Green) LEDs
* D11 connects to the Hours (Yellow) LEDs

## Ideas

* look up a reasonable persistence value for strobing the Cathodes of the LEDs so that the display will appear to be stable and not flashing
* there may be a library that could be plied into service for:
- persistence of the multiplexed digits
- debouncing the switches
- clock module

## Hardware improvement ideas

* pull up resistors would make the input circuit less stable (unless there are already internal pull up resistors applied when these pins are programmed to be digital input pins)
* Addendum, you can set up analog pins to have a pullup resistor, so that when the buttons are not pressed, they will be close to 5 volts, and when they are pressed, they will "bounce" a bit and stabilize to 0 volts.
* use an external RTC to get better accuracy for the clock

## A note about git config

For this current repo, I used

```
git config user.name "Dawson Tennant"
git config user.email abu.exegesis@gmail.com
```

## Some basic useful git commands

* to add files or changes, type
  
  ```
  git add .
  ```
* to commit these files with a comment, type
  
  ```
  git commit -m 'Add pictures in docs folder'
  ```
* to know the status of your local git repo, type
  
  ```
  git status
  ```
* to clone a repo, type
  
  ```
  git clone <URL of the repo>
  ```
* to update the remote repo, say, github, type
  
  ```
  git push
  ```
* some other common task are to be found here as well as all over the internet:
  [common git tasks](https://git-scm.com/cheat-sheet))
* use gitk, or git-cola to see a graphical representation of the structure of your repo

## Notes about Platform.io
* you need to create a new platform.io project to get everything set up correctly

## Notes about wokwi diagrams
* you can draw the diagram.json using [wokwi diagram editor](https://wokwi.com/projects/463280990119898113). It is just json, so if you understand where the compents are placed and what they are you could just use a text editor.
* you need another file in the root of the project called wokwi.toml. This tells where the firmware.elf and firmware.hex files are located in your source code. It is typically in .pio/build/nonatmega328

## Software ideas
* direct port manipulation see [port manipulation](https://www.compilenrun.com/docs/iot/arduino/arduino-advanced-programming/arduino-direct-port-manipulation/)

These are the ports
* Port B (digital pins 8-13)
* Port C (analog input pins)
* Port D (digital pins 0-7)

Each port has three associated registers:

* DDRx (Data Direction Register): Configures pins as inputs or outputs
* PORTx (Port Register): Sets the output state of pins (HIGH or LOW)
* PINx (Pin Register): Reads the current state of input pins

Pin mapping
  Arduino Pin   Port   Port Pin
  ------------- ------ ----------
  0             D      PD0
  1             D      PD1
  2             D      PD2
  3             D      PD3
  4             D      PD4
  5             D      PD5
  6             D      PD6
  7             D      PD7
  8             B      PB0
  9             B      PB1
  10            B      PB2
  11            B      PB3
  12            B      PB4
  13            B      PB5
  A0            C      PC0
  A1            C      PC1
  A2            C      PC2
  A3            C      PC3
  A4            C      PC4
  A5            C      PC5

* Note in this mapping, that the HH does not have a 32
* Also, recall that HH MM SS cathodes are strobed. Only one is on at 
time. So, you need to always write the two Bytes each time you display
any of the "digits". For testing, start with just displaying the seconds.

  Here is the mapping for PORTS and LEDS in this project:

    Mapping   Function             Digital Pin   Port   Select bit is
  --------- ---------- --------- ------------- ------ ---------------
            Anode      Cathode                        
  HH                   \*        D11              B   0
  MM                   \*        D10              B   0
  SS                   \*        D9               B   0
  Ones 1    \*                   D8               B   1
  Ones 2    \*                   D7               D   1
  Ones 4    \*                   D6               D   1
  Ones 8    \*                   D5               D   1
  Tens 16   \*                   D4               D   1
  Tens 32   \*                   D3               D   1

## Math

Tens  Ones  Digit (integer) Comment
000   0000  00
001   0001  01
010   0010  22              For HH, the tens (only 2 bits)        
011   0011  33
100   0100  44
101   0101  55
000   0110  06
000   0111  07
000   1000  08
000   1001  09

## TODOs
* make SS display correctly and update
* ways to better use the loop without using "delay"? Maybe the timing will be more accurate then?
* make MM and HH display correctly
* sweep HH MM SS to make them all display correctly and update
* look into a buttons library for debounce, press, long_press
* make all buttons function
* refactor to make use of writing multiple digits at a time