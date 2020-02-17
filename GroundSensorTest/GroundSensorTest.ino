#include <GroundSensor.h>
#include "Arduino.h"

const byte sensorleds = 13;
const byte analogPin = A0;

volatile uint8_t cmd;
enum Movements_enum {halt, forward, backward, left, right, rotate_cw, rotate_ccw, search}; 

// Range of colors are partitions of 1023
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
 const char RED_DO = right;
 const char BLUE_DO = forward;
 const char BLACK_DO = search;
 const char YELLOW_DO = halt;

Bounds bound = {B_LOW, B_HIGH, R_LOW, R_HIGH};
Instructions instr = {RED_DO, BLUE_DO, BLACK_DO, YELLOW_DO};
GroundSensor tapeSensor(sensorleds, analogPin, bound, instr);

void setup() {
  Serial.begin(9600);
  Serial.println("Ground Sensor Test"); // so I can keep track of what is loaded
  Serial.println("Trying to detect color");
  cmd = 'f';
}

void loop() {
    while (Serial.available()) {
      cmd = Serial.read();  //gets one byte from serial buffer
      if ((cmd == 'o')) {
            digitalWrite(sensorleds, HIGH);
      } else if ((cmd == 'f')) {
            digitalWrite(sensorleds, LOW);
      } 
    }   
    tapeSensor.senseColor(&cmd);
}
