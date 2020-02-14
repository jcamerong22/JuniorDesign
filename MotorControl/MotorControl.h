#ifndef MotorControl_h
#define MotorControl_h

#include "Arduino.h"
#include "FourPinControl.h"

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
		void stop();
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
		FourPinControl _pinControl;

		void driveMotor(int movement);
};

#endif
