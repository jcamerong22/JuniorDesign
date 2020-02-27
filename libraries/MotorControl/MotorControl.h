#ifndef MotorControl_h
#define MotorControl_h

#include "Arduino.h"

struct Wheels {
    int leftF, leftB, rightF, rightB;
};

struct Speeds {
    int forward, backward, left, right, cw, ccw;
};

struct Adjust {
    int left_adj, right_adj;
};

struct Accel {
    int accel_time, speed;
};


class MotorControl
{
    public:
        /* Set up Motor Control with default adjustment */
        MotorControl(Wheels w, Speeds s, Accel a);

        /* Set up Motor Control with user's adjustments */
        /* Adjust wheels' speed so bot run straighter*/
        MotorControl(Wheels w, Speeds s, Accel a, Adjust adjust);

        /* Movement functions*/
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
        Adjust _adjust;
        Movement _m;

        bool from_stop;
        Accel _a;

        void driveMotor(int movement);
        void accelerate();
        void allPinsOpen();
};

#endif
