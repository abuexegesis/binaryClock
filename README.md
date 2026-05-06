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