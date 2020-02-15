#include <MotorControl.h>
#include "Arduino.h"

const byte WheelLeftF = 2;
const byte WheelLeftB = 3;
const byte WheelRightF = 4;
const byte WheelRightB = 5;
volatile int curr_speed;

const int MAX = 255;
const int TQUARTER = 191;
const int HALF = 127;
const int QUARTER = 64;
const int ZERO = 0;
String readString;

/* Control speed of each state*/
const int FWD_SPD = QUARTER;
const int BCK_SPD = QUARTER;
const int LFT_SPD = QUARTER;
const int RGT_SPD = QUARTER;
const int RT_CW_SPD = QUARTER;
const int RT_CCW_SPD = QUARTER;

/* Control the motor */
Wheels w = {WheelLeftF, WheelLeftB, WheelRightF, WheelRightB};
Speeds s = {FWD_SPD, BCK_SPD, LFT_SPD, RGT_SPD, RT_CW_SPD, RT_CCW_SPD};
MotorControl motors(w, s);

volatile uint8_t state;
enum States_enum {STOP, FORWARD, BACKWARD, LEFT, RIGHT, CIRCLE};

void stateControl();
void speedControl();

void setup() {
  Serial.begin(9600);  
  Serial.println("Driving motor motion Test"); // so I can keep track of what is loaded 
  Serial.println("Stop");
  state = STOP;
}

void loop() {

  switch (state)
  {
    case STOP:
      stateControl();
      motors.halt(); // Stop right away
      break;
    
    case FORWARD:
      stateControl();
      motors.forward();
      break;

    case BACKWARD:
      stateControl();
      motors.backward();
      break;

     case LEFT:
      stateControl();
      motors.left();
      break;

    case RIGHT:
      stateControl();
      motors.right();
      break;  

    case CIRCLE:
      stateControl();
      motors.cw();
      break;
  }
  
}

void stateControl()
{
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString

    if (c == 'f'){
        Serial.println("Going Forward");
        state = FORWARD;
    } else if (c == 'b'){
        Serial.println("Moving Backward");
        state = BACKWARD;
    }  else if (c == 'l'){
        Serial.println("Moving Left");
        state = LEFT;
    } else if (c == 'r'){
        Serial.println("Moving Right");
        state = RIGHT;
    }  else if (c == 's'){
        Serial.println("Stop");
        state = STOP;
    } else if (c == 'c'){
        Serial.println("Turn 360");
        state = CIRCLE;
    } 
    
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
    readString=""; //empty for next input
    
    delay(2);  //slow down a bit so motors have time to get inputs
  }
}
