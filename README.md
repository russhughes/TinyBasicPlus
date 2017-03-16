
TinyBasic Plus ClassBot Edition
===============================

A modified version of BlueLlama's TinyBasicPlus with additional commands control a small 3D printed drawing robot.  
It uses a custom Arduino Shield to drive a pair of 28BYJ-48 stepper motors using two ULN2003 driver IC's.  The shield 
also has room for a SPI Serial Ram Chip and a I2C Serial EEPROM.  This version of TinyBasicPlus is configured to use
the first half of a 1MBit Serial EEPROM as a virtual disk for saving and loading programs and the second half to store a 
collection of 11 [Hershey vector fonts](https://en.wikipedia.org/wiki/Hershey_fonts).

Requires these Arduino Libraries:
- https://github.com/greiman/RamDisk
- https://github.com/JChristensen/extEEPROM
- http://www.airspayce.com/mikem/arduino/AccelStepper/


TinyBasic Plus
==============

A C implementation of Tiny Basic, with a focus on support for
Arduino.  It was originally written by Gordon Brandly in the form
of "68000 Tiny Basic", and then ported to C by Michael Field as
"Arduino Basic", though still called "Tiny Basic" in the source
files.

TinyBasic Plus is an extension and modification upon the original
"Tiny Basic" by adding support for a few more devices, configurable
at build time.  It is designed for use on the Arduino, although
builds will soon be easily possible for other platforms through
command line makefiles.  Provided is a makefile that builds for 
unix-ey type OSes.  It has only been tested for Darwin (OS X).

Features added include support for fileio (SD Library), autorunning
a program from the SD card, smaller footprint (PROGMEM), support
for pin data IO, and support for the on-chip EEProm storage for your
program. 

# Full list of supported statements and functions

## System
- BYE		- *exits Basic, soft reboot on Arduino*
- END 		- *stops execution from the program, also "STOP"*
- MEM		- *displays memory usage statistics*
- NEW		- *clears the current program*
- RUN		- *executes the current program*

## File IO/SD Card
- FILES		- 	*lists the files on the SD card*
- LOAD filename.bas	- *loads a file from the SD card*
- CHAIN filename.bas - *equivalent of: new, load filename.bas, run*
- SAVE filename.bas	- *saves the current program to the SD card, overwriting*

## EEProm - nonvolatile on-chip storage
- EFORMAT	- clears the EEProm memory
- ELOAD		- load the program in from EEProm
- ESAVE		- save the current program to the EEProm
- ELIST		- print out the contents of EEProm
- ECHAIN	- load the program from EEProm and run it

## IO, Documentation
- PEEK( address )	- *set a value in memory* (unimplemented)
- POKE			- *get a value in memory* (unimplemented)
- PRINT expression	- *print out the expression, also "?"*
- REM stuff		- *remark/comment, also "'"*

## Expressions, Math
- A=V, LET A=V	- *assign value to a variable*
- +, -, \*, / - *Math*
- <,<=,=,<>,!=,>=,> - *Comparisons*
- ABS( expression )  - *returns the absolute value of the expression*
- RSEED( v ) - *sets the random seed to v*
- RND( m ) - *returns a random number from 0 to m*

## Control
- IF expression statement - *perform statement if expression is true*
- FOR variable = start TO end	- *start for block*
- FOR variable = start TO end STEP value - *start for block with step*
- NEXT - *end of for block*
- GOTO linenumber - *continue execution at this line number*
- GOSUB linenumber - *call a subroutine at this line number*
- RETURN	- *return from a subroutine*

## Pin IO 
- DELAY	timems*- wait (in milliseconds)*
- DWRITE pin,value - *set pin with a value (HIGH,HI,LOW,LO)*
- AWRITE pin,value - *set pin with analog value (pwm) 0..255*
- DREAD( pin ) - *get the value of the pin* 
- AREAD( analogPin ) - *get the value of the analog pin*

NOTE: "PINMODE" command removed as of version 0.11

## Sound - Piezo wired with red/+ on pin 5 and black/- to ground
- TONE freq,timems - play "freq" for "timems" milleseconds (1000 = 1 second)
- TONEW freq,timems - same as above, but also waits for it to finish
- NOTONE - stop playback of all playing tones

NOTE: TONE commands are by default disabled

## External EEPROM File IO
- DEL -  removes a file from the external EEPROM
- FILES - lists the files on the external EEPROM
- LOAD filename.bas - loads a file from the external EEPROM
- CHAIN filename.bas - equivalent of: new, load filename.bas, run
- SAVE filename.bas - saves the current program to the external EEPROM

## ROBOT SPECIFIC COMMANDS
- FWD n : move forward n mm's, negative values move backward
- BACK n : move backward n mm's, negative values move forward
- LEFT n : turn n degrees to the left, negative values move right
- RIGHT n : turn n degrees to the right, negative values move left
- PENUP : raise pen
- PENDOWN : lower pen
- MOVE x, y : Moves in a straight line to a point x, y
- RMOVE x, y : Moves in a straight line to a point x, y units away 
- DRAW x, y : Moves in a straight line point x, y drawing as it goes regardless of the current pen status
- RDRAW x, y : Moves in a straight line  x, y units away drawing as it goes regardless of the current pen status
- SETXY x, y : Sets the current location to x,y without moving 
- DIR n : Turns to face the heading n in degree's
- FACE x, y : Turn to face the point x,y.
- CIRCLE n : Draws a circle of the given radius, centered on the current location.
- ELLIPSE xradius, yradius: Draws an ellipse of the given Xradius and Yradius, centered on the current location.
- WRITE expression : Writes the value of the expression starting at the current location  
- DONE Turns off motors, lifts pen and resets location to 0,0 with heading 0
- SCALE x Sets the scale (size) of the WRITE command.
- USE n: Use font n when WRITEing.

## ROBOT SPECIFIC FUNCTIONS:
- XLOC(): returns the current location on the x axis
- YLOC(): returns the current location on the y axis
- HEAD(): returns the current heading 
- PEN(): returns the up/down status of the pen

# Example programs

Here are a few example programs to get you started...

## Blink

hook up an LED between pin 3 and ground

	10 FOR A=0 TO 10
	20 DWRITE 3, HIGH
	30 DELAY 250
	40 DWRITE 3, LOW
	50 DELAY 250
	60 NEXT A

## Fade

hook up an LED between pin 3 and ground

	10 FOR A=0 TO 10
	20 FOR B=0 TO 255
	30 AWRITE 3, B
	40 DELAY 10
	50 NEXT B
	60 FOR B=255 TO 0 STEP -1
	70 AWRITE 3, B
	80 DELAY 10
	90 NEXT B
	100 NEXT A

## LED KNOB

hook up a potentiometeter between analog 2 and ground, led from digital 3 and ground.  If knob is at 0, it stops

	10 A = AREAD( 2 )
	20 PRINT A
	30 B = A / 4
	40 AWRITE 3, B
	50 IF A == 0 GOTO 100
	60 GOTO 10
	100 PRINT "Done."

## ECHAIN example

Write a small program, store it in EEPROM.  We'll show that variables don't
get erased when chaining happens

	EFORMAT
	10 A = A + 2
	20 PRINT A
	30 PRINT "From eeprom!"
	40 IF A = 12 GOTO 100
	50 PRINT "Shouldn't be here."
	60 END
	100 PRINT "Hi!"

Then store it in EEProm

	ESAVE

Now, create a new program in main memory and run it

	NEW
	10 A = 10
	20 PRINT A
	30 PRINT "From RAM!"
	40 ECHAIN

List both, and run

	ELIST
	LIST
	RUN
	

# Device Support
## Current
- Arduino - ATMega 168 (~100 bytes available)
- Arduino - ATMega 368 (~1100 bytes available)
- SD cards (via SD Library, for FILES, LOAD, SAVE commands, uses 9k of ROM)
- EEProm (via EEProm Library, uses 500 bytes of ROM)
- Serial IO - command console

## Future
- PS2 Keyboard for standalone use (maybe)
- Graphics support via common function names and ANSI/ReGIS escape codes


# Known Quirks and Limitations
- If LOAD or SAVE are called, FILES fails subsequent listings
- SD cards are not hot-swappable. A reset is required between swaps.


# Authors and Contributors

- Tiny Basic 68k - Gordon Brandly [Project Page](http://members.shaw.ca/gbrandly/68ktinyb.html)
- Arduino Basic / Tiny Basic C - Michael Field [Project Page](http://hamsterworks.co.nz/mediawiki/index.php/Arduino_Basic)
- Tiny Basic Plus - Scott Lawrence <yorgle@gmail.com> [Github Page](http://github.com/BleuLlama/TinyBasicPlus]

- Jurg Wullschleger - Fix for unary operations and whitespace in expressions

# Links
- [Arduino Microcontroller](http://arduino.cc)

# MIT License

Copyright (c) 2012-2013

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
