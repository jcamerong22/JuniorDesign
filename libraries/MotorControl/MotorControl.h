#ifndef MotorControl_h
#define MotorControl_h

#include "Arduino.h"

struct Wheels {
    int leftF, leftB, rightF, rightB;
};

struct Speeds {
    int forward, backward, left, right, cw, ccw;
};

class MotorControl
{
	public:
		MotorControl(Wheels w, Speeds s);
		void halt();
		void forward();
		void backward();
		void left();
		void right();
		void cw();
		void ccw();
		
	private:
		struct Movement {
    		int w1, w2, w3, w4;
		};

		Wheels _w;
		Speeds _s;
		Movement _m;

		void driveMotor(int movement);
		void allPinsOpen();
};

#endif
