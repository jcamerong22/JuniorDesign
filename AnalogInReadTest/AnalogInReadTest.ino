#include "Arduino.h"

const byte analogPin = A0;

// Need actual data for the value of voltage for each color
const int B_RANGE = 50;
const int R_RANGE = 100;
const int Y_RANGE = 150;

void setup() {
  Serial.begin(9600);
  Serial.println("AnalogIn Read Test"); // so I can keep track of what is loaded
}

void loop() {
    int voltage = analogRead(analogPin);
    Serial.println("Voltage Value: ");
    Serial.println(voltage);
    delay(1000);
}
