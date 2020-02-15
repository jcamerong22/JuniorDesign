#include "Arduino.h"
#include "MotorControl.h"

/* Duty Cycle constants */
const int MAX = 255;
const int TQUARTER = 191;
const int HALF = 127;
const int QUARTER = 64;
const int ZERO = 0;

enum Movements_enum {FORWARD, BACK, LEFT, RIGHT, CW, CCW}; 


MotorControl::MotorControl(Wheels w, Speeds s)
{
    pinMode(w.leftF, OUTPUT);
    pinMode(w.leftB, OUTPUT);
    pinMode(w.rightF, OUTPUT);
    pinMode(w.rightB, OUTPUT);
    _w = w;
    _s = s;
}

void MotorControl::allPinsOpen()
{
    digitalWrite(_w.leftF, LOW);
    digitalWrite(_w.leftB, LOW);
    digitalWrite(_w.rightF, LOW);
    digitalWrite(_w.rightB, LOW);
}

void MotorControl::halt()
{
    allPinsOpen();
}

void MotorControl::forward()
{
    _m = {_w.leftB, _w.rightB, _w.leftF, _w.rightF};
    driveMotor(FORWARD);
}

void MotorControl::backward()
{
    _m  = {_w.leftF, _w.rightF, _w.leftB, _w.rightB};
    driveMotor(BACK);
}

void MotorControl::left()
{
    _m  = {_w.leftB, _w.rightB, _w.leftF, _w.rightF};
    driveMotor(LEFT);
}
void MotorControl::right()
{
    _m  = {_w.leftB, _w.rightB, _w.leftF, _w.rightF};
    driveMotor(RIGHT);
}

void MotorControl::cw()
{
    _m = {_w.leftB, _w.rightF, _w.leftF, _w.rightB};
    driveMotor(CW);
}

void MotorControl::ccw()
{
    _m  = {_w.leftB, _w.rightF, _w.leftB, _w.rightF};
    driveMotor(CCW);
}

void MotorControl::driveMotor(int movement)
{
    digitalWrite(_m.w1, LOW);
    digitalWrite(_m.w2, LOW);
    
    switch (movement)
    {
    case FORWARD:
      analogWrite(_m.w3, _s.forward);
      analogWrite(_m.w4, _s.forward);
      break;

    case BACK:
      analogWrite(_m.w3, _s.backward);
      analogWrite(_m.w4, _s.left);
      break;

     case LEFT:
      analogWrite(_m.w3, ZERO);
      analogWrite(_m.w4, _s.left);
      break;

    case RIGHT:
      analogWrite(_m.w3, _s.right);
      analogWrite(_m.w4, ZERO);
      break;  

    case CW:
      analogWrite(_m.w3, _s.cw);
      analogWrite(_m.w4, _s.cw);
      break;

    case CCW:
      analogWrite(_m.w3, _s.ccw);
      analogWrite(_m.w4, _s.ccw);
      break;
  }
}
