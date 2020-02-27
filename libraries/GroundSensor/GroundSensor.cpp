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
    _voltage = 0;
    _current_color = black;
    /* Default transtion delay is 500 ms*/
    _transition_delay = 500;
}

GroundSensor::GroundSensor(int sensorPin, int analogPin,
                           Bounds b, Instructions i, int transition_delay)
{
    pinMode(sensorPin, OUTPUT);
    _sensorPin = sensorPin;
    _analogPin = analogPin;
    _bound = b;
    _inst = i;
    _voltage = 0;
    _current_color = black;
    _transition_delay = transition_delay;
}

void GroundSensor::transition()
{
    /* We know that the command doesn't matches its color*/
    /* Wait for some time before getting another reading*/
    delay(_transition_delay);
    _voltage = analogRead(_analogPin);
    get_color();
}

bool GroundSensor::is_black()
{
    return (_voltage < _bound.b_low);
}

bool GroundSensor::is_blue()
{
    return (_voltage >= _bound.b_low) && (_voltage <= _bound.b_high);
}

bool GroundSensor::is_red()
{
    return (_voltage > _bound.r_low) && (_voltage <= _bound.r_high);
}

bool GroundSensor::is_yellow()
{
    return (_voltage > _bound.r_high);
}

bool GroundSensor::is_different_color(volatile uint8_t *command)
{
    /* Check if current command matches the color the sensor just received*/
    bool still_blue = (*command == _inst.blue_do) && (is_blue());
    bool still_red = (*command == _inst.red_do) && (is_red());
    bool still_yellow = (*command == _inst.yellow_do) && (is_yellow());
    bool still_black = (*command == _inst.black_do) && (is_black());

    bool is_different = !(still_blue || still_red || still_yellow 
                         || still_black);
                         
    return is_different;
}

void GroundSensor::get_color()
{
    if (is_red())
    {
        _current_color = red;
    }
    else if (is_blue())
    {
        _current_color = blue;
    }
    else if (is_black())
    {
        _current_color = black;
    }
    else if (is_yellow())
    {
        _current_color = yellow;
    }
}

void GroundSensor::senseColor(volatile uint8_t *command)
{
    _voltage = analogRead(_analogPin);
    if (is_different_color(command))
    {
        Serial.println("Different Color!");
        transition();
    }
    else
    {
        get_color();
    }

    if (_current_color == black)
    {
        Serial.println("Detected Black");
        *command = _inst.black_do;
    }
    else if (_current_color == blue)
    {
        Serial.println("Detected Blue");
        *command = _inst.blue_do;
    }
    else if (_current_color == red)
    {
        Serial.println("Detected Red");
        *command = _inst.red_do;
    }
    else if (_current_color == yellow)
    {
        Serial.println("Detected Yellow");
        *command = _inst.yellow_do;
    }
    else
    {
        /* Voltage spikes, don't want these*/
        Serial.println("Invalid _voltage detected! Check Code!");
        *command = 0;
    }
    // Print out voltage for calibration
    //Serial.println(_voltage);
}
