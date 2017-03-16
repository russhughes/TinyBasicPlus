//
// ClassBot.h
//
//
// dependencies:
//		AccelStepper
//		MultiStepper
//		Servo
//		extEEPROM  		if using an external EEPROM for fonts
//

#ifndef ClassBotLib_h
#define ClassBotLib_h

#define EEEPROM_FONTS 1

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
#include <math.h>

#ifdef EEEPROM_FONTS 
#include <extEEPROM.h>
#define FONTS_OFFSET 0x10000
#else

// #define GOTHIC_ENGLISH_TRIPLEX_FONT
// #define GOTHIC_GERMAN_TRIPLEX_FONT
// #define GOTHIC_ITALIAN_TRIPLEX_FONT
// #define GREEK_COMPLEX_FONT
// #define GREEK_COMPLEX_SMALL_FONT
// #define GREEK_PLAIN_FONT
// #define GREEK_SIMPLEX_FONT
// #define ITALIC_COMPLEX_FONT
// #define ITALIC_COMPLEX_SMALL_FONT
// #define ITALIC_TRIPLEX_FONT
// #define CYRILLIC_COMPLEX_FONT
// #define ROMAN_COMPLEX_FONT
// #define ROMAN_COMPLEX_SMALL_FONT
// #define ROMAN_DUPLEX_FONT
// #define ROMAN_PLAIN_FONT
#define ROMAN_SIMPLEX_FONT
// #define ROMAN_TRIPLEX_FONT
// #define SCRIPT_COMPLEX_FONT
// #define SCRIPT_SIMPLEX_FONT

#include "fonts.h"

const char * const font[96] PROGMEM = {
                            char032,	char033,	char034,	char035,	char036,	char037,	char038,	char039,
    char040,	char041,	char042,	char043,	char044,	char045,	char046,	char047,    char048,	char049,
    char050,	char051,	char052,	char053,	char054,	char055,    char056,	char057,	char058,	char059,
    char060,	char061,	char062,	char063,    char064,	char065,	char066,	char067,	char068,	char069,
    char070,	char071,    char072,	char073,	char074,	char075,	char076,	char077,	char078,	char079,
    char080,	char081,	char082,	char083,	char084,	char085,	char086,	char087,    char088,	char089,
    char090,	char091,	char092,	char093,	char094,	char095,	char096,	char097,    char098,	char099,
    char100,	char101,	char102,	char103,	char104,	char105,	char106,	char107,    char108,	char109,
    char110,	char111,	char112,	char113,	char114,	char115,	char116,	char117,    char118,	char119,
    char120,	char121,	char122,	char123,	char124,	char125,	char126,	char127
};

#endif

#define MAX_SPEED 1000
#define LEFT 0
#define RIGHT 1

#define TWOPI 6.2831853
#define RAD2DEG 57.2957795

#define backlashComp 12
#define wheelDiameter 65				// mm (increase = spiral out)
#define wheelBase 112 					// mm (increase = spiral in)

static float wheelBPI = wheelBase * PI;
static int stepsPerRev = 4076;
static float stepsDist = stepsPerRev / (wheelDiameter * PI);

class ClassBot {
    public:
        ClassBot();

        void penDown();
        void penUp();
        void forward(long distance);
        void backward(long distance);
        void left(float degrees);
        void right(float degrees);
        void done();
        void moveTo(long x, long y, byte penUp);
        void face(long x, long y);
        void turnTo(float degrees);
        void character(char ch);
        void text(char *message);
        byte getPen();
        byte getScale();
        long getX();
        long getY();
        float getH();
        
        void setScale(byte s);
        
        void setFont(short int font);
    
        void setX(long x);
        void setY(long y);
        void setH(float h);
        void circle(long r);
        void ellipse(long xr, long yr);        

    private:
    	byte scale = 1;				// 1 unit == 1 mm 
        long botX = 0;				// at origin
        long botY = 0;				
        float botH = 0;				// facing up
        boolean penIsUp = true;
      
#ifdef EEEPROM_FONTS 
		extEEPROM	*fontEEPROM;
#endif
		short int	currentFont;
        AccelStepper *leftMotor;
        AccelStepper *rightMotor;
        MultiStepper motors;

        long steps[2] = { 0, 0 };
        byte lastPos[2] = { -1, -1 };
        Servo penServo;

        void setStep(int which,	long dist);
        float bearing(long a1, long a2, long b1, long b2);
};

#endif
