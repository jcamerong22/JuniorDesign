#include <MotorControl.h>
#include "Arduino.h"

const int hallSensor = 7;
const int sensorLed = 12;

void brakeISR();

void setup() {
  pinMode(sensorLed, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(hallSensor), brakeISR, HIGH);
  Serial.begin(9600);  
  Serial.println("Hall Sensor Test"); // so I can keep track of what is loaded 
}

void loop() {
    digitalWrite(sensorLed, LOW);
}

void brakeISR()
{
    Serial.println("Pedestrian Detected");
    digitalWrite(sensorLed, HIGH);
    state = STOP;
}
