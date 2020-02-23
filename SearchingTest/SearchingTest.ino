#include <MotorControl.h>
#include <GroundSensor.h>
#include "Arduino.h"

volatile uint8_t state;
enum States_enum {OFF, ON, STOP, FORWARD, SEARCH};

volatile uint8_t cmd;
volatile uint8_t off;
enum Movements_enum {halt, forward, backward, left, right, rotate_cw, rotate_ccw, search};

/* Pin to turn on/off ground sensor */
const byte sensorLeds = 13;
/* Pin for analog input from ground sensor */
const byte analogPin = A0;

/* Range of colors are partitions of 1023 
const int B_LOW = 300;
const int B_HIGH = 500;
const int R_LOW = 500;
const int R_HIGH = 835;
*/
// Range of colors are partitions of 1023
// R = 5.6 kOhms
const int B_LOW = 400;
const int B_HIGH = 700;
const int R_LOW = 700;
const int R_HIGH = 1020;

const char RED_DO = halt;
const char BLUE_DO = forward;
const char BLACK_DO = search;
const char YELLOW_DO = search;

/* Set up the ground sensor */
Bounds bound = {B_LOW, B_HIGH, R_LOW, R_HIGH};
Instructions instr = {RED_DO, BLUE_DO, BLACK_DO, YELLOW_DO};
GroundSensor tapeSensor(sensorLeds, analogPin, bound, instr);

/* Pins for wheel control */
const byte WheelLeftF = 2;
const byte WheelLeftB = 3;
const byte WheelRightF = 4;
const byte WheelRightB = 5;

/* Duty Cycle constants */
const int MAX = 255;
const int TQUARTER = 191;
const int HALF = 127;
const int QUARTER = 64;
const int ZERO = 0;

/* Constant speed value*/
const int BEST= 45;

/* Acceleration time and speed*/
const int ACC_TIME = 100;
const int ACC_SPD  = QUARTER;

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
Accel  a = {ACC_TIME, ACC_SPD};
MotorControl motors(w, s, a);

/* Time of searching*/
const int search_time = 5;

void stateControl();
void bootSequence();
void detect();
void search_side2side();
void search_back();
void wait_and_check();

void setup() {
  Serial.begin(9600);
  Serial.println("Searching Test"); // so I can keep track of what is loaded
  Serial.println("Bot is OFF");
  state = OFF;
  cmd = BLACK_DO;
  off = 'n';
  Serial.println("Turn it on? Y/N");
}

void loop() {

  switch (state)
  {
    case OFF:
      /* Starting state, wait for user to turn on*/
      while (Serial.available()) {
        off = Serial.read();  //gets one byte from serial buffer
        if ((off == 'y')) {
          state = STOP;
          off = 'n';
          delay(2);
        }
      }
      motors.halt();
      digitalWrite(sensorLeds, HIGH);
      break;

    case STOP:
      motors.halt();
      tapeSensor.senseColor(&cmd);
      stateControl();
      break;

    case FORWARD:
      motors.forward();
      tapeSensor.senseColor(&cmd);
      stateControl();
      break;

    case SEARCH:
      tapeSensor.senseColor(&cmd);
      stateControl();
      detect();
      break;
  }
}

void stateControl()
{
  if (cmd == forward) {
    Serial.println("Going Forward");
    state = FORWARD;
  } else if (cmd == search) {
    Serial.println("Searching for path");
    state = SEARCH;
  } else if (cmd == halt) {
    state = STOP;
  }
  delay(2);  //slow loop to allow for change in state

  while (Serial.available()) {
    off = Serial.read();  //gets one byte from serial buffer
    if (off = 'y') {
      state = OFF;
      off = 'n';
      delay(2);
    }
    delay(2);
  }
}

void detect()
{
  motors.halt();
  delay(1000);
  search_side2side();
}

void wait_and_check()
{
    motors.halt();
    tapeSensor.senseColor(&cmd);
    stateControl();
    delay(1000);
}

void search_back()
{
    while (1) {
      motors.backward();
      delay(search_time * 100);
      wait_and_check();
      if (state != SEARCH) {
        return;
      }
    }
}

void search_side2side()
{
    motors.ccw();
    delay(search_time * 100);
    wait_and_check();
    if (state != SEARCH) {
      return;
    }
    motors.cw();
    delay((search_time) * 200);
    wait_and_check();
    if (state != SEARCH) {
      return;
    }
    motors.ccw();
    delay(search_time * 100);
    wait_and_check();
    if (state != SEARCH) {
      return;
    }
    search_back();
}
