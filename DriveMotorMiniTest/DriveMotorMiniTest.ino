#include "Arduino.h"

String readString;
const byte ledPinBLUE = 6;

void setup() {
  Serial.begin(9600);
  Serial.println("Driving led using Serial Test"); // so I can keep track of what is loaded
  pinMode(ledPinBLUE, OUTPUT);
}

void loop() {
  int n;
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
    Serial.println("Input is: ");  
    Serial.println(readString);  //so you can see the captured string
    n = readString.toInt();  //convert readString into a number

    readString=""; //empty for next input
  }

  analogWrite(ledPinBLUE, n);
}
