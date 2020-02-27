#include <MotorControl.h>
#include "Arduino.h"

const int MAX = 255;
const int TQUARTER = 191;
const int HALF = 127;
const int QUARTER = 64;
const int ZERO = 0;

/* Time of travel and constant speed value*/
const float distance = 5; // inches
const int BEST= 45;

/* Acceleration time and speed*/
const int ACC_TIME = 100;
const int ACC_SPD  = QUARTER;

const byte WheelLeftF = 2;
const byte WheelLeftB = 3;
const byte WheelRightF = 4;
const byte WheelRightB = 5;

/* Control speed of each state*/
const int FWD_SPD = BEST;
const int BCK_SPD = BEST+10;
const int LFT_SPD = BEST;
const int RGT_SPD = BEST;
const int RT_CW_SPD = BEST;
const int RT_CCW_SPD = BEST;

/* Control the motor */
Wheels w = {WheelLeftF, WheelLeftB, WheelRightF, WheelRightB};
Speeds s = {FWD_SPD, BCK_SPD, LFT_SPD, RGT_SPD, RT_CW_SPD, RT_CCW_SPD};
Accel  a = {ACC_TIME, ACC_SPD};
MotorControl motors(w, s, a);

volatile uint8_t state;
enum States_enum {STOP, FORWARD, BACKWARD};

void stateControl();

void setup() {
  Serial.begin(9600);  
  Serial.println("Distace Test"); // so I can keep track of what is loaded 
  Serial.println("Stop");
  state = STOP;
}

void loop() {

  int time_delay = distance * 1000;  
  switch (state)
  {
    case STOP:
      stateControl();
      motors.halt(); // Stop right away
      break;
    
    case FORWARD:
      stateControl();
      motors.forward();
      delay(time_delay);
      state = STOP;
      break;

    case BACKWARD:
      stateControl();
      motors.backward();
      delay(time_delay);
      state = STOP;
      break;
  }
  
}

void stateControl()
{
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer

    if (c == 'f'){
        Serial.println("Going Forward");
        state = FORWARD;
    } else if (c == 'b'){
        Serial.println("Moving Backward");
        state = BACKWARD;
    } else if (c == 's'){
        Serial.println("Stop");
        state = STOP;
    }
    
    delay(2);  //slow looping to allow buffer to fill with next character
  }
}
