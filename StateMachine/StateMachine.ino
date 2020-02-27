#include "Arduino.h"

const byte interruptPinButton = 2;
const byte interruptPinOnOff = 3;

const byte ledPinRED = 4;
const byte ledPinGREEN = 5;
const byte ledPinBLUE = 6;
const byte analogPinPot1 = A0;
const byte analogPinPot2 = A1;


volatile uint8_t state;
enum States_enum {OFF, ON, RUN};

void blink(int pin, float hertz, int brightness);
void myISR1();
void myISR2();
void fade(int pin, int timeToOff);
int calcFadeTime();
int calcBrightness();

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(interruptPinButton, INPUT_PULLUP);  // RUN/STOP button
  pinMode(interruptPinOnOff, INPUT_PULLUP);  // ON/OFF switch
  pinMode(ledPinRED, OUTPUT);
  pinMode(ledPinGREEN, OUTPUT);
  pinMode(ledPinBLUE, OUTPUT);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(interruptPinButton), myISR1, FALLING); 
  attachInterrupt(digitalPinToInterrupt(interruptPinOnOff), myISR2, FALLING);
  
  state = OFF;
}

void loop() {

  switch (state)
  {
    case OFF:
      digitalWrite(4 , HIGH);    // turn on RED LED
      //Serial.println("I'm Off"); // send a message if in OFF state
      break;

    case ON:
      digitalWrite(ledPinRED, LOW);                    
      blink(5, 10, calcBrightness());             // Blink at 10 Hz
      //Serial.println("I'm On"); // send a message if in ON state
      break;

      
    case RUN:
      fade(ledPinBLUE, calcFadeTime());        // fade over pot set time
      //Serial.println("I'm Running"); // send a message if in RUN state
      break;
  }
  
}

void blink (int pin, float hertz, int brightness) {
    unsigned int time;
    time = 1/hertz * 1000;

    if (state != OFF) {
      analogWrite(pin, brightness);   // turn the LED on (HIGH is the voltage level)
      delay(time);                       // wait for some second
      analogWrite(pin, 0);    // turn the LED off by making the voltage LOW
      delay(time); 
    }
}

void myISR1() {
  Serial.println("I'm Interrupting Button");
  if(state == ON){
    state = RUN;
  } else if(state == RUN) {
    state = ON;
  }
}

void myISR2() {
  Serial.println("I'm Interrupting On/Off");
  if(state == OFF) {
    state = ON;
  } else {
    state = OFF;
  }
}

void fade(int pin, int timeToOff) {
  analogWrite(pin,255);
  for (int i = 0; i < 51; i++){
    analogWrite(pin, 255-i*5);
    delay(timeToOff/51);
    }
  analogWrite(pin, LOW);
}

int calcFadeTime(){
  int voltage = analogRead(analogPinPot1);

  float timeMs = voltage / 350.00 * 3000 + 500;

  return timeMs;
 }

 int calcBrightness(){
  int voltage = analogRead(analogPinPot2);
  float brightness = voltage / 615.00 * 255;
  return round(brightness);
 }
