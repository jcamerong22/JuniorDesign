#include "Arduino.h"

const byte interruptPinButton = 2;

const byte ledPinRED = 4;
const byte ledPinGREEN = 5;
const byte ledPinBLUE = 6;
volatile int curr_speed;

String readString;

volatile uint8_t state;
enum States_enum {FORWARD, BACKWARD};

void myISR();
void ledOff(int pin);
void ledOn(int pin);
void speedControl();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(interruptPinButton, INPUT_PULLUP);  // RUN/STOP button
  pinMode(ledPinRED, OUTPUT);
  pinMode(ledPinGREEN, OUTPUT);
  pinMode(ledPinBLUE, OUTPUT);
  Serial.begin(9600);
  
  Serial.println("Driving motor using Serial Test"); // so I can keep track of what is loaded
  attachInterrupt(digitalPinToInterrupt(interruptPinButton), myISR, FALLING); 
  state = FORWARD;
}

void loop() {

  switch (state)
  {
    case FORWARD:
      ledOff(ledPinBLUE);
      speedControl();
      analogWrite(ledPinGREEN, curr_speed);
      break;

    case BACKWARD:
      ledOff(ledPinGREEN);
      speedControl();
      analogWrite(ledPinBLUE, curr_speed);
      break;  
  }
  
}

void myISR() {
  if(state == FORWARD){
    curr_speed = 0;
    state = BACKWARD;
    Serial.println("Moving BACKWARD"); 
  } else if(state == BACKWARD) {
    curr_speed = 0;
    state = FORWARD;
    Serial.println("Moving FORWARD"); 
  }
  delay(2);
  
}

void speedControl()
{
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
    Serial.println("Input is: ");  
    Serial.println(readString);  //so you can see the captured string
    curr_speed = readString.toInt();  //convert readString into a number

    readString=""; //empty for next input
  }
}

void ledOff(int pin)
{
    digitalWrite(pin, LOW);
}

void ledOn(int pin)
{
    digitalWrite(pin, HIGH);
}
 
