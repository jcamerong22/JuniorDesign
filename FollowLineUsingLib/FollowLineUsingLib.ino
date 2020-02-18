#include <MotorControl.h>
#include <GroundSensor.h>
#include "Arduino.h"

volatile uint8_t state;
enum States_enum {OFF, ON, STOP, FORWARD, BACKWARD, LEFT, RIGHT, CLOCKWISE, CCLOCKWISE, SEARCH};

volatile uint8_t cmd;
volatile char off;
enum Movements_enum {halt, forward, backward, left, right, rotate_cw, rotate_ccw, search}; 

/* Pin to turn on/off ground sensor */
const byte sensorLeds = 13;
/* Pin for analog input from ground sensor */
const byte analogPin = A0;

/* Range of colors are partitions of 1023 */
const int B_LOW = 300;
const int B_HIGH = 500;
const int R_LOW = 500;
const int R_HIGH = 835;

/* Edit this for bot functionality
 * stop = 's'
 * forward = 'f'
 * backward = 'b'
 * right = 'r'
 * left  = 'l'
 */
 const char RED_DO = forward;
 const char BLUE_DO = forward;
 const char BLACK_DO = forward;
 const char YELLOW_DO = forward;
 
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

const int hallSensor = A1;
const int pedestrianLed = 6;

void stateControl(uint8_t c);
void bootSequence();
void detect();

void setup() {
  //attachInterrupt(digitalPinToInterrupt(sensorLeds), detect, CHANGE);
  pinMode(pedestrianLed, OUTPUT);
  Serial.begin(9600);  
  Serial.println("Follow the Line Color"); // so I can keep track of what is loaded 
  Serial.println("Bot is OFF");
  state = OFF;
  Serial.println("Turn it on? Y/N");
}

void loop() {
      
  switch (state)
  {
    case OFF:
      /* Starting state, wait for user to turn on*/
      while (Serial.available()) {
      off = Serial.read();  //gets one byte from serial buffer
      if ((off == 'Y') || (off == 'y')) {
            state = ON;
        }
      }     
      digitalWrite(sensorLeds, LOW);
      break;

    case ON:
      bootSequence();
      tapeSensor.senseColor(&cmd);
      stateControl();
      delay(100);
    break;
    
    case STOP:
      tapeSensor.senseColor(&cmd);
      stateControl();
      motors.halt();
      break;
    
    case FORWARD:
      tapeSensor.senseColor(&cmd);
      stateControl();
      motors.forward();
      break;

    case BACKWARD:
      tapeSensor.senseColor(&cmd);
      stateControl();
      motors.backward();
      break;

     case LEFT:
      tapeSensor.senseColor(&cmd);
      stateControl();
      motors.left();
      break;

    case RIGHT:
      tapeSensor.senseColor(&cmd);
      stateControl();
      motors.right();
      break;  

    case CLOCKWISE:
      tapeSensor.senseColor(&cmd);
      stateControl();
      motors.cw();
      break;

    case CCLOCKWISE:
      tapeSensor.senseColor(&cmd);
      stateControl();
      motors.ccw();
      break;

    case SEARCH:
      tapeSensor.senseColor(&cmd);
      stateControl();
      detect();
      break;
  }
}

void bootSequence()
{
      Serial.println("Bot is on");
      Serial.println("Moving in ");
      Serial.println("3");
      delay(1000);
      Serial.println("2");
      delay(1000);
      Serial.println("1");
      delay(1000);
      digitalWrite(sensorLeds, HIGH);
}

void stateControl()
{
    while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
        if (c) {
            state = OFF;
        }
    delay(5);
    }
  
    if (cmd == forward){
        Serial.println("Going Forward");
        state = FORWARD;
    } else if (cmd == backward){
        Serial.println("Moving Backward");
        state = BACKWARD;
    }  else if (cmd == left){
        Serial.println("Moving Left");
        state = LEFT;
    } else if (cmd == right){
        Serial.println("Moving Right");
        state = RIGHT;
    }  else if (cmd == halt){
        Serial.println("Stop");
        state = STOP;
    } else if (cmd == rotate_cw){
        Serial.println("Turn 360 Clockwise");
        state = CLOCKWISE;
    } else if (cmd == rotate_ccw){
        Serial.println("Turn 360 Counter Clockwise");
        state = CCLOCKWISE;
    } else if (cmd == search){
        Serial.println("Searching for path");
        state = SEARCH;
    }

    int person = analogRead(hallSensor);
    if (person >= 399) {
        Serial.println("Pedestrian detected");
        state = STOP;
        digitalWrite(pedestrianLed, HIGH);
    } else {
        digitalWrite(pedestrianLed, LOW);
    }
    
    delay(5);  //slow loop to allow for change in state
}

void detect()
{
      uint8_t old_cmd = cmd;
      motors.ccw();
      delay(200);
      motors.halt();
      tapeSensor.senseColor(&cmd);
      stateControl();
      delay(500);

      if (old_cmd == cmd) {
          motors.forward();
          delay(200);
          motors.halt();
          tapeSensor.senseColor(&cmd);
          stateControl();
          delay(500);
      }
      
      if (old_cmd == cmd) {
        motors.cw();
        delay(200);
        motors.halt();
        tapeSensor.senseColor(&cmd);
        stateControl();
        delay(500);
     }

     if (old_cmd == cmd) {
          motors.forward();
          delay(200);
          motors.halt();
          tapeSensor.senseColor(&cmd);
          stateControl();
          delay(500);
     }
     
     if (old_cmd == cmd) {
        motors.cw();
        delay(200);
        motors.halt();
        tapeSensor.senseColor(&cmd);
        stateControl();
        delay(500);
     }

     if (old_cmd == cmd) {
          motors.forward();
          delay(200);
          motors.halt();
          tapeSensor.senseColor(&cmd);
          stateControl();
          delay(500);
     }
}
 
