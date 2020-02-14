#ifndef GroundSensor_h
#define GroundSensor_h

#include "Arduino.h"

struct Bounds {
    int b_low, b_high, r_low, r_high;
};

struct Instructions {
    uint8_t red_do, blue_do, black_do, yellow_do;
};

class GroundSensor
{
  public:
    GroundSensor(int sensorPin, int analogPin, Bounds b, Instructions i);
    void senseColor(volatile uint8_t *command);
  private:
    int _sensorPin, _analogPin;
    Bounds _bound;
    Instructions _inst;
};

#endif
