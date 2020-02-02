#include "Arduino.h"

const byte WheelOneF = 2;
const byte WheelOneB = 3;
const byte WheelTwoF = 4;
const byte WheelTwoB = 5;
volatile int curr_speed;

String readString;

volatile uint8_t state;
enum States_enum {FORWARD, BACKWARD};

void pinClose(int pin);
void pinOpen(int pin);
void speedControl();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(WheelOneF, OUTPUT);
  pinMode(WheelOneB, OUTPUT);
  pinMode(WheelTwoF, OUTPUT);
  pinMode(WheelTwoB, OUTPUT);
  Serial.begin(9600);  
  Serial.println("Driving motor back and forth Test"); // so I can keep track of what is loaded 
  Serial.println("Moving Forward");
  state = FORWARD;
  curr_speed = 0;
}

void loop() {

  switch (state)
  {
    case FORWARD:
      speedControl();
      pinOpen(WheelOneB);
      pinOpen(WheelTwoB);
      analogWrite(WheelOneF, curr_speed);
      analogWrite(WheelTwoF, curr_speed);
      break;

    case BACKWARD:
      speedControl();
      pinOpen(WheelOneF);
      pinOpen(WheelTwoF);
      analogWrite(WheelOneB, curr_speed);
      analogWrite(WheelTwoB, curr_speed);
      break;  
  }
  
}

void speedControl()
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
    }    
    
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
    curr_speed = readString.toInt();
    Serial.println("Speed:");
    Serial.println(curr_speed);

    readString=""; //empty for next input
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

 
