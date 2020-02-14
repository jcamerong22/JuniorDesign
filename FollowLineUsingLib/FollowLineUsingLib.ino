#include <MotorControl.h>
#include <GroundSensor.h>
#include "Arduino.h"

volatile uint8_t state;
enum States_enum {OFF, ON, STOP, FORWARD, BACKWARD, LEFT, RIGHT, CLOCKWISE, CCLOCKWISE, SEARCH};

volatile uint8_t cmd;
enum Movements_enum {halt, forward, backward, left, right, rotate_cw, rotate_ccw, search}; 

/* Pin to turn on/off ground sensor */
const byte sensorLeds = 6;
/* Pin for analog input from ground sensor */
const byte analogPin = A0;

/* Range of colors are partitions of 1023 */
const int B_LOW = 300;
const int B_HIGH = 500;
const int R_LOW = 500;
const int R_HIGH = 800;

/* Edit this for bot functionality
 * stop = 's'
 * forward = 'f'
 * backward = 'b'
 * right = 'r'
 * left  = 'l'
 */
 const char RED_DO = right;
 const char BLUE_DO = forward;
 const char BLACK_DO = search;
 const char YELLOW_DO = halt;
 
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

void stateControl(uint8_t c);
void bootSequence();

void setup() {
  attachInterrupt(digitalPinToInterrupt(analogPin), detect, CHANGE);
  Serial.begin(9600);  
  Serial.println("Follow the Line Color"); // so I can keep track of what is loaded 
  Serial.println("Bot is OFF");
  state = OFF;
  Serial.println("Turn it on? Y/N");
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    if ((c = 'Y') || (c = 'y')) {
        state = ON;
    }
  }
  
  switch (state)
  {
    case OFF:
      /* Starting state, wait for user to turn on*/
      break;

    case ON:
      bootSequence();
    break;
    
    case STOP:
      motors.stop(); // Stop right away
      break;
    
    case FORWARD:
      stateControl(cmd);
      motors.forward();
      break;

    case BACKWARD:
      stateControl(cmd);
      motors.backward();
      break;

     case LEFT:
      stateControl(cmd);
      motors.left();
      break;

    case RIGHT:
      stateControl(cmd);
      motors.right();
      break;  

    case CLOCKWISE:
      stateControl(cmd);
      motors.cw();
      break;

    case CCLOCKWISE:
      stateControl(cmd);
      motors.ccw();
      break;

    case SEARCH:
      stateControl(cmd);
      motors.ccw();
      delay(1000);
      motors.cw();
      delay(3000);
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
      tapeSensor.senseColor(&cmd);
}

void stateControl(uint8_t c)
{
    if (c == forward){
        Serial.println("Going Forward");
        state = FORWARD;
    } else if (c == backward){
        Serial.println("Moving Backward");
        state = BACKWARD;
    }  else if (c == left){
        Serial.println("Moving Left");
        state = LEFT;
    } else if (c == right){
        Serial.println("Moving Right");
        state = RIGHT;
    }  else if (c == halt){
        Serial.println("Stop");
        state = STOP;
    } else if (c == rotate_cw){
        Serial.println("Turn 360 Clockwise");
        state = CLOCKWISE;
    } else if (c == rotate_ccw){
        Serial.println("Turn 360 Counter Clockwise");
        state = CCLOCKWISE;
    } else if (c == search){
        Serial.println("Searching for path");
        state = SEARCH;
    }
    
    delay(5);  //slow loop to allow for change in state
}

void detect()
{
  tapeSensor.senseColor(&cmd);
}
 
