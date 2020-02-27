#include <MotorControl.h>
#include "Arduino.h"


volatile uint8_t state;
enum States_enum {FORWARD, BACKWARD};

void speedControl();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPinGREEN, OUTPUT);
  pinMode(ledPinBLUE, OUTPUT);
  Serial.begin(9600);
  
  Serial.println("Driving motor using Serial Test"); // so I can keep track of what is loaded
  Serial.println("Moving Forward");
  curr_speed = 0;
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

  if ((readString.length() >0)) {
    Serial.println("Input is:");  
    Serial.println(readString);  //so you can see the captured string
    
    curr_speed = readString.toInt();
    Serial.println("Speed:");
    Serial.println(curr_speed);

    readString = ""; //empty for next input
  }
}
 
