#include "Arduino.h"
#include "MotorControl.h"

/* Duty Cycle constants */
const int MAX = 255;
const int TQUARTER = 191;
const int HALF = 127;
const int QUARTER = 64;
const int ZERO = 0;

enum Movements_enum {FORWARD, BACK, LEFT, RIGHT, CW, CCW}; 

MotorControl::MotorControl(Wheels w, Speeds s, Accel a)
{
    pinMode(w.leftF, OUTPUT);
    pinMode(w.leftB, OUTPUT);
    pinMode(w.rightF, OUTPUT);
    pinMode(w.rightB, OUTPUT);
    _w = w;
    _s = s;
    _a = a;
    _adjust = {0, 0};
    from_stop = true;
}

MotorControl::MotorControl(Wheels w, Speeds s, Accel a, Adjust adjust)
{
    pinMode(w.leftF, OUTPUT);
    pinMode(w.leftB, OUTPUT);
    pinMode(w.rightF, OUTPUT);
    pinMode(w.rightB, OUTPUT);
    _w = w;
    _s = s;
    _a = a;
    _adjust = adjust; 
    from_stop = true;
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
    from_stop = true;
}

void MotorControl::forward()
{
    _m = {_w.leftB, _w.rightB, _w.leftF + _adjust.left_adj,
          _w.rightF + _adjust.right_adj};
    driveMotor(FORWARD);
}

void MotorControl::backward()
{
    _m  = {_w.leftF, _w.rightF, _w.leftB + _adjust.left_adj, 
    _w.rightB + _adjust.right_adj};
    driveMotor(BACK);
}

void MotorControl::left()
{
    _m  = {_w.leftB, _w.rightB, _w.leftF + _adjust.left_adj, 
    _w.rightF + _adjust.right_adj};
    driveMotor(LEFT);
}
void MotorControl::right()
{
    _m  = {_w.leftB, _w.rightB, _w.leftF + _adjust.left_adj, 
    _w.rightF + _adjust.right_adj};
    driveMotor(RIGHT);
}

void MotorControl::cw()
{
    _m = {_w.leftB, _w.rightF, _w.leftF + _adjust.left_adj, 
    _w.rightB + _adjust.right_adj};
    driveMotor(CW);
}

void MotorControl::ccw()
{
    _m  = {_w.leftB, _w.rightF, _w.leftB + _adjust.left_adj,
     _w.rightF + _adjust.right_adj};
    driveMotor(CCW);
}

void MotorControl::accelerate()
{
    if (from_stop == true) {
          Serial.println("Accelerate!");
          digitalWrite(_m.w1, LOW);
          digitalWrite(_m.w2, LOW);
          analogWrite(_m.w3, _a.speed);
          analogWrite(_m.w4, _a.speed);
          delay(_a.accel_time);
          from_stop = false;
    }
}

void MotorControl::driveMotor(int movement)
{
    digitalWrite(_m.w1, LOW);
    digitalWrite(_m.w2, LOW);
    
    switch (movement)
    {
    case FORWARD:
      accelerate();
      analogWrite(_m.w3, _s.forward);
      analogWrite(_m.w4, _s.forward);
      break;

    case BACK:
      accelerate();
      analogWrite(_m.w3, _s.backward);
      analogWrite(_m.w4, _s.left);
      break;

     case LEFT:
      accelerate();
      analogWrite(_m.w3, ZERO);
      analogWrite(_m.w4, _s.left);
      break;

    case RIGHT:
      accelerate();
      analogWrite(_m.w3, _s.right);
      analogWrite(_m.w4, ZERO);
      break;  

    case CW:
      accelerate();
      analogWrite(_m.w3, _s.cw);
      analogWrite(_m.w4, _s.cw);
      break;

    case CCW:
      accelerate();
      analogWrite(_m.w3, _s.ccw);
      analogWrite(_m.w4, _s.ccw);
      break;
  }
}
