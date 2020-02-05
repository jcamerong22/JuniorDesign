#include "Arduino.h"

const byte WheelLeftF = 2;
const byte WheelLeftB = 3;
const byte WheelRightF = 4;
const byte WheelRightB = 5;
volatile int curr_speed;

const int MAX = 255;
const int TQUARTER = 191;
const int HALF = 127;
const int QUARTER = 64;
const int ZERO = 0;

String readString;

volatile uint8_t state;
enum States_enum {STOP, FORWARD, BACKWARD, LEFT, RIGHT, CIRCLE};

void pinClose(int pin);
void pinOpen(int pin);
void stateControl();
void speedControl();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(WheelLeftF, OUTPUT);
  pinMode(WheelLeftB, OUTPUT);
  pinMode(WheelRightF, OUTPUT);
  pinMode(WheelRightB, OUTPUT);
  Serial.begin(9600);  
  Serial.println("Driving motor motion Test"); // so I can keep track of what is loaded 
  Serial.println("Stop");
  state = STOP;
}

void loop() {

  switch (state)
  {
    case STOP:
      speedControl();
      pinOpen(WheelLeftB);
      pinOpen(WheelRightB);
      pinOpen(WheelLeftF);
      pinOpen(WheelRightF);
      break;
    
    case FORWARD:
      speedControl();
      pinOpen(WheelLeftB);
      pinOpen(WheelRightB);
      analogWrite(WheelLeftF, HALF);
      analogWrite(WheelRightF, HALF);
      break;

    case BACKWARD:
      speedControl();
      pinOpen(WheelLeftF);
      pinOpen(WheelRightF);
      analogWrite(WheelLeftB, HALF);
      analogWrite(WheelRightB, HALF);
      break;

     case LEFT:
      speedControl();
      pinOpen(WheelLeftB);
      pinOpen(WheelRightB);
      analogWrite(WheelLeftF, ZERO);
      analogWrite(WheelRightF, TQUARTER);
      break;

    case RIGHT:
      speedControl();
      pinOpen(WheelLeftB);
      pinOpen(WheelRightB);
      analogWrite(WheelLeftF, TQUARTER);
      analogWrite(WheelRightF, ZERO);
      break;  

    case CIRCLE:
      speedControl();
      pinOpen(WheelLeftB);
      pinOpen(WheelRightF);
      analogWrite(WheelLeftF, QUARTER);
      analogWrite(WheelRightB, QUARTER);
      break; 
  }
  
}

void speedControl()
{
  stateControl();
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString 
    
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
    curr_speed = readString.toInt();
    Serial.println("Speed:");
    Serial.println(curr_speed);
    readString=""; //empty for next input
    
    delay(2);  //slow down a bit so motors have time to get inputs
  }
}

void stateControl()
{
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString

    if (c == 'f'){
        Serial.println("Going Forward");
        state = FORWARD;
    } else if (c == 'b'){
        Serial.println("Moving Backward");
        state = BACKWARD;
    }  else if (c == 'l'){
        Serial.println("Moving Left");
        state = LEFT;
    } else if (c == 'r'){
        Serial.println("Moving Right");
        state = RIGHT;
    }  else if (c == 's'){
        Serial.println("Stop");
        state = STOP;
    } else if (c == 'c'){
        Serial.println("Turn 360");
        state = CIRCLE;
    } 
    
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
    readString=""; //empty for next input
    
    delay(2);  //slow down a bit so motors have time to get inputs
  }
}

void pinOpen(int pin)
{
    digitalWrite(pin, LOW);
}

void pinClose(int pin)
{
    digitalWrite(pin, HIGH);
}

void blink (int pin, float hertz, int brightness) {
    unsigned int time;
    time = 1/hertz * 1000;

    if (state != STOP) {
      analogWrite(pin, brightness);   // turn the LED on (HIGH is the voltage level)
      delay(time);                       // wait for some second
      analogWrite(pin, 0);    // turn the LED off by making the voltage LOW
      delay(time); 
    }
}
 
