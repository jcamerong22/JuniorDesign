#include <MotorControl.h>
#include <GroundSensor.h>
#include "Arduino.h"

/* Duty Cycle constants */
const int MAX = 255;
const int TQUARTER = 191;
const int HALF = 127;
const int QUARTER = 64;
const int ZERO = 0;
const int BEST = 45; // Will need to change when weight of bot change

volatile uint8_t state;
enum States_enum {OFF, ON, STOP, FORWARD, BACKWARD, LEFT, RIGHT, CLOCKWISE, CCLOCKWISE};

volatile uint8_t cmd;
volatile uint8_t on;
enum Movements_enum {halt, forward, backward, left, right, rotate_cw, rotate_ccw, search};


/******************************* Configuration Section **********************************/

/* Pin to turn on/off ground sensor */
const byte sensorLeds = 13;
/* Pin for analog input from ground sensor */
const byte analogPin = A0;

/* Range of colors are partitions of 1023 */
/* R = 3.7 kOhms
const int B_LOW = 300;
const int B_HIGH = 500;
const int R_LOW = 500;
const int R_HIGH = 835;
*/
/* R = 5.6 kOhms */
const int B_LOW = 400;
const int B_HIGH = 700;
const int R_LOW = 700;
const int R_HIGH = 1020;

/* What bot should do when it sense a color*/
const char RED_DO = forward;
const char BLUE_DO = forward;
const char BLACK_DO = rotate_ccw;
const char YELLOW_DO = forward;

/* Pins for wheel control */
const byte WheelLeftF = 2;
const byte WheelLeftB = 3;
const byte WheelRightF = 4;
const byte WheelRightB = 5;

/* Control speed of each state*/
const int FWD_SPD = BEST;
const int BCK_SPD = BEST;
const int LFT_SPD = QUARTER;
const int RGT_SPD = QUARTER;
const int RT_CW_SPD = QUARTER;
const int RT_CCW_SPD = QUARTER;

/* Set transition delay*/
int transition_delay = 100;

/* Acceleration time and speed*/
const int ACC_TIME = 200;
const int ACC_SPD  = QUARTER;

/*****************************************************************************************/


/* Set up the ground sensor */
Bounds bound = {B_LOW, B_HIGH, R_LOW, R_HIGH};
Instructions instr = {RED_DO, BLUE_DO, BLACK_DO, YELLOW_DO};
GroundSensor tapeSensor(sensorLeds, analogPin, bound, instr, transition_delay);

/* Control the motor */
Wheels w = {WheelLeftF, WheelLeftB, WheelRightF, WheelRightB};
Speeds s = {FWD_SPD, BCK_SPD, LFT_SPD, RGT_SPD, RT_CW_SPD, RT_CCW_SPD};
Accel  a = {ACC_TIME, ACC_SPD};
MotorControl motors(w, s, a);

void stateControl();
void bootSequence();

void setup() {
  Serial.begin(9600);
  Serial.println("Follow the Line Color"); // so I can keep track of what is loaded
  Serial.println("Bot is OFF");
  state = OFF;
  on = 'n';
  cmd = BLACK_DO;
  Serial.println("Turn it on? Y/N");
}

void loop() {

  switch (state)
  {
    case OFF:
      /* Starting state, wait for user to turn on*/
      while (Serial.available()) {
        on = Serial.read();  //gets one byte from serial buffer
        if ((on == 'y')) {
          state = ON;
          on = 'n';
          delay(2);
          break;
        }
      }
      motors.halt();
      digitalWrite(sensorLeds, LOW);
      break;

    case ON:
      bootSequence();
      delay(500);
      tapeSensor.senseColor(&cmd);
      stateControl();
      break;

    case STOP:
      motors.halt();
      tapeSensor.senseColor(&cmd);
      stateControl();
      break;

    case FORWARD:
      if (cmd == backward) {
          motors.halt();
          delay(10000);
      }
      motors.forward();
      tapeSensor.senseColor(&cmd);
      stateControl();
      break;

    case BACKWARD:
      if (cmd == forward) {
          motors.halt();
          delay(10000);
      }
      motors.backward();
      tapeSensor.senseColor(&cmd);
      stateControl();
      break;

    case LEFT:
      motors.left();
      tapeSensor.senseColor(&cmd);
      stateControl();
      break;

    case RIGHT:
      motors.right();
      tapeSensor.senseColor(&cmd);
      stateControl();
      break;

    case CLOCKWISE:
      motors.cw();
      tapeSensor.senseColor(&cmd);
      stateControl();
      break;

    case CCLOCKWISE:
      motors.ccw();
      tapeSensor.senseColor(&cmd);
      stateControl();
      break;
  }
}

void bootSequence()
{
  Serial.println("Bot is on");
  digitalWrite(sensorLeds, HIGH);
  Serial.println("Ready!");
  delay(1000);
  Serial.println("Go!");
}

void stateControl()
{
  /* Change the state based on cmd*/
  if (cmd == forward) {
    //Serial.println("Going Forward");
    state = FORWARD;
  } else if (cmd == backward) {
    Serial.println("Moving Backward");
    state = BACKWARD;
  }  else if (cmd == left) {
    //Serial.println("Moving Left");
    state = LEFT;
  } else if (cmd == right) {
    //Serial.println("Moving Right");
    state = RIGHT;
  }  else if (cmd == halt) {
    //Serial.println("Stop");
    state = STOP;
  } else if (cmd == rotate_cw) {
    //Serial.println("Turn 360 Clockwise");
    state = CLOCKWISE;
  } else if (cmd == rotate_ccw) {
    //Serial.println("Turn 360 Counter Clockwise");
    state = CCLOCKWISE;
  }
  delay(2);  //slow loop to allow for change in state

  while (Serial.available()) {
     on = Serial.read();  //gets one byte from serial buffer
     if ((on == 'y')) {
        state = OFF;
        on = 'n';
        delay(2);
        break;
     }
  }
}
