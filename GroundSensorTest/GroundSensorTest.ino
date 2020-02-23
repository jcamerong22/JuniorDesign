#include <GroundSensor.h>
#include "Arduino.h"

const byte sensorleds = 13;
const byte analogPin = A0;

volatile uint8_t cmd;
volatile uint8_t light;
enum Movements_enum {halt, forward, backward, left, right, rotate_cw, rotate_ccw, search};

// Range of colors are partitions of 1023
// R = 5.6 kOhms
const int B_LOW = 400;
const int B_HIGH = 700;
const int R_LOW = 700;
const int R_HIGH = 1020;

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
  light = 'f';
  cmd = BLACK_DO;
}

void loop() {
  while (Serial.available()) {
    light = Serial.read();  //gets one byte from serial buffer
    if ((light == 'o')) {
      digitalWrite(sensorleds, HIGH);
    } else if ((light == 'f')) {
      digitalWrite(sensorleds, LOW);
    }
  }
  tapeSensor.senseColor(&cmd);
}
