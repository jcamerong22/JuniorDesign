#include "Arduino.h"
#include "GroundSensor.h"

GroundSensor::GroundSensor(int sensorPin, int analogPin, 
                            Bounds b, Instructions i)
{
  pinMode(sensorPin, OUTPUT);
  _sensorPin = sensorPin;
  _analogPin = analogPin;
  _bound = b;
  _inst = i;
}

void GroundSensor::senseColor(volatile uint8_t *command)
{
        int voltage = analogRead(_analogPin);
        bool is_black = voltage < _bound.b_low;
        bool is_blue = (voltage >= _bound.b_low) and (voltage <= _bound.b_high);
        bool is_red = (voltage > _bound.r_low) and (voltage <= _bound.r_high);
        bool is_yellow = voltage > _bound.r_high;
          
        if (is_black) {
            Serial.println("Detected Black");
            *command = _inst.black_do;
        } else if (is_blue) {
            Serial.println("Detected Blue");
            *command = _inst.blue_do;
        } else if (is_red) {
            Serial.println("Detected Red");
            *command = _inst.red_do;
        } else if (is_yellow) {
            Serial.println("Detected Yellow");
            *command = _inst.yellow_do;
        } else {
            Serial.println("Invalid voltage detected! Check Code!");
            *command = 's';
        }
}
