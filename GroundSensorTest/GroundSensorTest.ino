#include "Arduino.h"

const byte ledRED = 2;
const byte ledBLUE = 3;
const byte ledYELLOW = 4;
const byte ledBLACK = 5;

const byte sensorledRED = 6;
const byte sensorledBLUE = 7;

const byte analogPin = A0;

// Range of colors are partitions of 1023
const int B_LOW = 300;
const int B_HIGH = 500;
const int R_LOW = 500;
const int R_HIGH = 800;

String readString;

volatile uint8_t state;
enum States_enum {OFF, ON};

void pinOn(int pin);
void allOff();
void pinsOff(int pin, int pin2, int pin3);
void stateControl();
void senseTape();

void setup() {
  pinMode(ledRED, OUTPUT);
  pinMode(ledBLUE, OUTPUT);
  pinMode(ledYELLOW, OUTPUT);
  pinMode(ledBLACK, OUTPUT);

  pinMode(sensorledRED, OUTPUT);
  pinMode(sensorledBLUE, OUTPUT);

  Serial.begin(9600);
  Serial.println("Ground Sensor Test"); // so I can keep track of what is loaded
  Serial.println("Off"); 
  state = OFF;
}

void loop() {

  switch (state)
  {
    case OFF:
      stateControl();
      allOff();
      break;

    case ON:
      stateControl();
      break;
  }
  
}

void senseTape()
{
        int voltage = analogRead(analogPin);
        bool is_black = voltage < B_LOW;
        bool is_blue = (voltage >= B_LOW) and (voltage <= B_HIGH);
        bool is_red = (voltage > R_LOW) and (voltage <= R_HIGH);
        bool is_yellow = voltage > R_HIGH;
        
        if (is_black) {
            pinsOff(ledBLUE, ledRED, ledYELLOW);
            pinOn(ledBLACK);
        } else if (is_blue) {
            pinsOff(ledBLACK, ledRED, ledYELLOW);
            pinOn(ledBLUE);
        } else if (is_red) {
            pinsOff(ledBLUE, ledBLACK, ledYELLOW);
            pinOn(ledRED);
        } else if (is_yellow) {
            pinsOff(ledBLUE, ledRED, ledBLACK);
            pinOn(ledYELLOW);
        } else {
            Serial.println("Invalid voltage detected! Check Code!");
        }
}

void stateControl()
{
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString

    if (c == 'l'){
        Serial.println("Both LEDs on");
        state = ON;
    } else if (c == 'o'){
        Serial.println("Off");
        state = OFF;
    } 
    
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
    readString=""; //empty for next input   
    delay(2);  //slow down a bit so motors have time to get inputs
  }
}

void pinOn(int pin)
{
    digitalWrite(pin, HIGH);
}

void allOff()
{
    pinsOff(ledRED,ledBLUE,ledYELLOW);
    pinsOff(ledBLACK,sensorledRED,sensorledBLUE);
}

void pinsOff(int pin1, int pin2, int pin3)
{
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, LOW);
}
