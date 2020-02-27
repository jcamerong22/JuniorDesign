#ifndef GroundSensor_h
#define GroundSensor_h

#include "Arduino.h"

enum Color_enum{red, blue, black, yellow};

struct Bounds {
    int b_low, b_high, r_low, r_high;
};

struct Instructions {
    uint8_t red_do, blue_do, black_do, yellow_do;
};

class GroundSensor
{
  public:
    /* Set up the Ground Sensor with default transtion delay*/
    GroundSensor(int sensorPin, int analogPin, Bounds b, Instructions i);

    /* Set up the Ground Sensor with custom transition delay*/
    GroundSensor(int sensorPin, int analogPin, Bounds b, Instructions i, 
                  int transition_delay);

    /* Change the command based on color sensed*/
    void senseColor(volatile uint8_t *command);
  private:
    /* Pins for sensor and leds*/
    int _sensorPin, _analogPin;

    /* Bounds for colors' voltages*/
    Bounds _bound;

    /* Set of command based on color*/
    Instructions _inst;

    /* Detected voltage*/
    int _voltage;

    /* Current color*/
    int _current_color;

    /* How long to wait until we get a second reading in case of a 
     * transtion of color 
     */
    int _transition_delay;

    /* Helper functions*/
    bool is_red();
    bool is_blue();
    bool is_black();
    bool is_yellow();

    bool is_different_color(volatile uint8_t *command);
    void get_color();

    void transition();
};

#endif
