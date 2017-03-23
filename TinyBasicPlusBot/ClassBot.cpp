

#ifdef EEEPROM_FONTS
#include <extEEPROM.h>
#endif

#include "ClassBot.h"

ClassBot::ClassBot() {

    leftMotor = new AccelStepper(AccelStepper::HALF4WIRE, 9, 7, 8, 6, true);  // d'oh! -- Classbot 1.0 board is backwards
    rightMotor = new AccelStepper(AccelStepper::HALF4WIRE, 2, 4, 3, 5, true); 

#ifdef EEEPROM_FONTS 
	fontEEPROM = new extEEPROM(kbits_1024, 1, 256, 0x50);
	fontEEPROM->begin(twiClock400kHz);
	currentFont = 9;
#endif

    pinMode(A0, OUTPUT);
    penServo.attach(A0);
    penUp();

    leftMotor->setMaxSpeed(MAX_SPEED);
    rightMotor->setMaxSpeed(MAX_SPEED);

    motors.addStepper(*leftMotor);
    motors.addStepper(*rightMotor);
}

void ClassBot::penDown() {
    penServo.write(60);
    penIsUp = false;
    delay(200);
}

void ClassBot::penUp() {
    penServo.write(150);
    penIsUp = true;
    delay(200);
}

void ClassBot::setStep(int which, long dist) {
    boolean pos = dist > 0;
    steps[which] = dist;

    if (lastPos[which] != -1) {
        if (pos & !lastPos[which]) {
            steps[which] += backlashComp;
        }
        if (!pos & lastPos[which]) {
            steps[which] -= backlashComp;
        }
    }
    lastPos[which] = pos;
}

void ClassBot::forward(long distance) {
    int s = int(distance * stepsDist);

    setStep(LEFT, -s);
    setStep(RIGHT, s);

    motors.moveTo(steps);
    motors.runSpeedToPosition();
    leftMotor->setCurrentPosition(0);
    rightMotor->setCurrentPosition(0);
}

void ClassBot::backward(long distance) {
    int s = int(distance * stepsDist);

    setStep(LEFT, s);
    setStep(RIGHT, -s);

    motors.moveTo(steps);
    motors.runSpeedToPosition();
    leftMotor->setCurrentPosition(0);
    rightMotor->setCurrentPosition(0);
}

void ClassBot::left(float degrees) {
    float distance = wheelBPI * (degrees / 360.0);
    int s = int(distance * stepsDist);

    setStep(LEFT, s);
    setStep(RIGHT, s);

    motors.moveTo(steps);
    motors.runSpeedToPosition();
    leftMotor->setCurrentPosition(0);
    rightMotor->setCurrentPosition(0);
}

void ClassBot::right(float degrees) {
    float distance = wheelBPI * (degrees / 360.0);
    int s = int(distance * stepsDist);

    setStep(LEFT, -s);
    setStep(RIGHT, -s);

    motors.moveTo(steps);
    motors.runSpeedToPosition();
    leftMotor->setCurrentPosition(0);
    rightMotor->setCurrentPosition(0);
}

void ClassBot::done() {
    penUp();
    for (int i = 2; i < 10; i++) {
        digitalWrite(i, LOW);
    }
	scale = 1;
    botX = 0;
    botY = 0;
    botH = 0;
}

float ClassBot::bearing(long a1, long a2, long b1, long b2) {
    float theta = atan2(b1 - a1, a2 - b2);
    if (theta < 0.0) {
        theta += TWOPI;
    }
    return theta * RAD2DEG;
}

void ClassBot::moveTo(long x, long y, byte penIsUp) {
    float b;
    float r;
    long dx, dy, dh;

    penUp();

    if ((x != botX) || (y != botY)) {
        b = bearing(botX, botY, x, y);

        dx = x - botX;
        dy = y - botY;

        r = sqrt((dx * dx) + (dy * dy));

        dh = botH - b;
        if (dh > 180) {
            dh = dh - 360;
        }

        if (dh < -180) {
            dh = dh + 360;
        }

        if (dh < 0) {
            right(abs(dh));
        }

        if (dh > 0) {
            left(dh);
        }

        if (!penIsUp) {
            penDown();
        }

        forward(r * scale);

        botX = x;
        botY = y;
        botH = b;
    }
}

void ClassBot::face(long x, long y) {
    float b;
    long dx, dy, dh;

    if ((x != botX) || (y != botY)) {
        b = bearing(botX, botY, x, y);

        dh = botH - b;
        if (dh > 180) {
            dh = dh - 360;
        }

        if (dh < -180) {
            dh = dh + 360;
        }

        if (dh < 0) {
            right(abs(dh));
        }

        if (dh > 0) {
            left(dh);
        }

        botH = b;
    }
}

void ClassBot::turnTo(float heading) {
    long dh;

    if (botH == heading) {
        dh = botH - heading;
        if (dh > 180) {
            dh = dh - 360;
        }

        if (dh < -180) {
            dh = dh + 360;
        }

        if (dh < 0) {
            right(abs(dh));
        }

        if (dh > 0) {
            left(dh);
        }
        botH = heading;
    }
}

#ifndef EEEPROM_FONTS 
void ClassBot::character(char ch) {
    char *vectors;
    byte c, length, width, vX, vY, pup = true;
    long x = botX, y = botY;

    penUp();
    if ((ch > 31) && (ch < 127)) {
        c = ch - 32;
        vectors = (char *) pgm_read_word(&font[c]);
        length = (char *) pgm_read_byte(vectors++);
        width = (char *) pgm_read_byte(vectors++);
        while (length--) {
            vX = (char *) pgm_read_byte(vectors++);
            vY = (char *) pgm_read_byte(vectors++);

            if ((vX == 255) && (vY == 255)) {
                pup = true;
                continue;
            }
            moveTo(x + vX, y - vY, pup);
            if (pup) {
                pup = false;
            }
        }
    }
    penUp();
    moveTo(x + width, y, true);

}

void ClassBot::text(char *message) {
   	char *s;
   	
    if (message) {
        for (s = message; *s; s++)
        	character(*s);
    }
}
#endif

void ClassBot::setFont(short int font) {
	currentFont = font;
}

#ifdef EEEPROM_FONTS 

//
// External EEPROM font data format
//
//	Font table
// 0000-0003 'FONT'
// 0004-003F Font Name, Zero padded
// 0040-00FF 16 Bit Table of offsets for the vector of each character 
//

void ClassBot::character(char ch) {
    word vectors = 0;
    byte c, length, width = 0, i, pup = true;
    char vX, vY;
    long x = botX, y = botY;
    long address;
	
    penUp();
    if ((ch > 31) && (ch < 127)) {
        c = ch - 32;

		address	= FONTS_OFFSET + (currentFont * 0x100) + 0x40 + (c*2);
		vectors	= fontEEPROM->read(address++);
		vectors+= (fontEEPROM->read(address) << 8);
		address = FONTS_OFFSET + vectors;
		length 	= fontEEPROM->read(address++);
        width 	= fontEEPROM->read(address++);
		
        while (length--) {
	    	vX = fontEEPROM->read(address++);
   	    	vY = fontEEPROM->read(address++);
	
	        if ((vX == -1) && (vY == -1)) {
                pup = true;
                continue;
            }
            
            moveTo(x + vX, y - vY, pup);
            if (pup) {
                pup = false;
            }
        }
    }
    penUp();
    moveTo(x + width, y, true);

}

void ClassBot::text(char *message) {
   	char *s;
   	
    if (message) {
        for (s = message; *s; s++)
        	character(*s);
    }
}

#endif

byte ClassBot::getPen() {
    return penIsUp;
}

byte ClassBot::getScale() {
    return scale;
}

long ClassBot::getX() {
    return botX;
}

long ClassBot::getY() {
    return botY;
}

float ClassBot::getH() {
    return botH;
}

void ClassBot::setScale(byte s) {
    scale = s;
}

void ClassBot::setX(long x) {
    botX = x;
}

void ClassBot::setY(long y) {
    botY = y;
}

void ClassBot::setH(float h) {
    botH = h;
}
void ClassBot::circle(long r) {
    ellipse(r, r);
}

void ClassBot::ellipse(long xr, long yr) {
    float step = 2 * PI / 20;
    float theta;
    int x, y, xx, yy, xxx = botX, yyy = botY, hhh = botH;

    for (theta = 0;  theta < 2 * PI;  theta += step) {
        x = xr * cos(theta);
        y = yr * sin(theta);
        if (theta == 0) {
            xx = x;
            yy = y;
            moveTo(x, y, 1);
        } else {
            moveTo(x, y, 0);
        }
    }
    moveTo(xx, yy, 0);
    moveTo(xxx, yyy, 1);
    turnTo(hhh);
}

