#include "Arduino.h"

const byte WheelLeftF = 2;
const byte WheelLeftB = 3;
const byte WheelRightF = 4;
const byte WheelRightB = 5;

const byte sensorLeds = 6;

const byte analogPin = A0;

// Range of colors are partitions of 1023
const int B_LOW = 300;
const int B_HIGH = 500;
const int R_LOW = 500;
const int R_HIGH = 800;

const int MAX = 255;
const int TQUARTER = 191;
const int HALF = 127;
const int QUARTER = 64;
const int ZERO = 0;

/* Edit this for bot functionality
 * stop = 's'
 * forward = 'f'
 * backward = 'b'
 * right = 'r'
 * left  = 'l'
 **********************************/
 const char RED_DO = 's';
 const char BLUE_DO = 'f';
 const char BLACK_DO = 's';
 const char YELLOW_DO = 'r';

 const int FWD_SPD = QUARTER;
 const int BCK_SPD = QUARTER;
 const int LFT_SPD = QUARTER;
 const int RGT_SPD = QUARTER;
 const int RT_CW_SPD = QUARTER;
 const int RT_CCW_SPD = QUARTER;
/* *********************************/

String readString;

volatile uint8_t state;
enum States_enum {OFF, ON, STOP, FORWARD, BACKWARD, LEFT, RIGHT, CLOCKWISE, CCLOCKWISE};

void pinClose(byte pin);
void pinOpen(byte pin);
void allPinsOpen();
void stateControl(char c);
void senseTape();
void bootSequence();
void driveMotor(byte pinOpen1, byte pinOpen2, byte pinClose1, byte pinClose2);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(WheelLeftF, OUTPUT);
  pinMode(WheelLeftB, OUTPUT);
  pinMode(WheelRightF, OUTPUT);
  pinMode(WheelRightB, OUTPUT);
  pinMode(sensorLeds, OUTPUT);
  
  Serial.begin(9600);  
  Serial.println("Follow the Line Color"); // so I can keep track of what is loaded 
  Serial.println("OFF");
  state = OFF;
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    if (c = 'g') {
        state = ON;
    }
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
    readString=""; //empty for next input
    delay(2);  //slow down a bit so motors have time to get inputs
  }
  
  switch (state)
  {
    case OFF:
      /* Starting state, nothing should be on */
      allPinsOpen();
      break;

    case ON:
      bootSequence();
      senseTape();
    break;
    
    case STOP:
      senseTape();
      allPinsOpen();
      break;
    
    case FORWARD:
      senseTape();
      pinOpen(WheelLeftB);
      pinOpen(WheelRightB);
      analogWrite(WheelLeftF, QUARTER);
      analogWrite(WheelRightF, QUARTER);
      break;

    case BACKWARD:
      senseTape();
      pinOpen(WheelLeftF);
      pinOpen(WheelRightF);
      analogWrite(WheelLeftB, QUARTER);
      analogWrite(WheelRightB, QUARTER);
      break;

     case LEFT:
      senseTape();
      pinOpen(WheelLeftB);
      pinOpen(WheelRightB);
      analogWrite(WheelLeftF, ZERO);
      analogWrite(WheelRightF, QUARTER);
      break;

    case RIGHT:
      senseTape();
      pinOpen(WheelLeftB);
      pinOpen(WheelRightB);
      analogWrite(WheelLeftF, QUARTER);
      analogWrite(WheelRightF, ZERO);
      break;  

    case CLOCKWISE:
      senseTape();
      /*
      pinOpen(WheelLeftB);
      pinOpen(WheelRightF);
      analogWrite(WheelLeftF, QUARTER);
      analogWrite(WheelRightB, QUARTER);
      */
      driveMotor(WheelLeftB, WheelRightF, WheelLeftF, WheelRightB);
      break;

    case CCLOCKWISE:
      senseTape();
      /*
      pinOpen(WheelLeftF);
      pinOpen(WheelRightB);
      analogWrite(WheelLeftB, QUARTER);
      analogWrite(WheelRightF, QUARTER);
      */
      driveMotor(WheelLeftF, WheelRightB, WheelLeftB, WheelRightF);
      break;
  }
}

void driveMotor(byte pinOpen1, byte pinOpen2, byte pinClose1, byte pinClose2)
{
    pinOpen(pinOpen1);
    pinOpen(pinOpen2);
    
    switch (state)
    {
    case FORWARD:
      analogWrite(WheelLeftF, FWD_SPD);
      analogWrite(WheelRightF, FWD_SPD);
      break;

    case BACKWARD:
      analogWrite(WheelLeftB, BCK_SPD);
      analogWrite(WheelRightB, BCK_SPD);
      break;

     case LEFT:
      analogWrite(WheelLeftF, ZERO);
      analogWrite(WheelRightF, LFT_SPD);
      break;

    case RIGHT:
      analogWrite(WheelLeftF, RGT_SPD);
      analogWrite(WheelRightF, ZERO);
      break;  

    case CLOCKWISE:
      analogWrite(WheelLeftF, RT_CW_SPD);
      analogWrite(WheelRightB, RT_CW_SPD);
      break;

    case CCLOCKWISE:
      analogWrite(WheelLeftB, RT_CCW_SPD);
      analogWrite(WheelRightF, RT_CCW_SPD);
      break;
  }
}

void bootSequence()
{
      Serial.println("Bot is on");
      Serial.println("Moving in ");
      Serial.println("3");
      delay(1000);
      Serial.println("2");
      delay(1000);
      Serial.println("1");
      delay(1000);
      pinClose(sensorLeds);
}

void allPinsOpen()
{
      pinOpen(WheelLeftB);
      pinOpen(WheelRightB);
      pinOpen(WheelLeftF);
      pinOpen(WheelRightF);
      pinOpen(sensorLeds);
}

void senseTape()
{
        int voltage = analogRead(analogPin);
        bool is_black = voltage < B_LOW;
        bool is_blue = (voltage >= B_LOW) and (voltage <= B_HIGH);
        bool is_red = (voltage > R_LOW) and (voltage <= R_HIGH);
        bool is_yellow = voltage > R_HIGH;
        
        char command;       
        if (is_black) {
            Serial.println("Detected Black");
            command = BLACK_DO;
        } else if (is_blue) {
            Serial.println("Detected Blue");
            command = BLUE_DO;
        } else if (is_red) {
            Serial.println("Detected Red");
            command = RED_DO;
        } else if (is_yellow) {
            Serial.println("Detected Yellow");
            command = YELLOW_DO;
        } else {
            Serial.println("Invalid voltage detected! Check Code!");
            command = 's';
        }
  
        stateControl(command);
}

void stateControl(char c)
{
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
    } else if (c == 'd'){
        Serial.println("Turn 360 Clockwise");
        state = CLOCKWISE;
    } else if (c == 'a'){
        Serial.println("Turn 360 Counter Clockwise");
        state = CCLOCKWISE;
    } 
    
    delay(5);  //slow loop to allow for change in state
}

void pinOpen(byte pin)
{
    digitalWrite(pin, LOW);
}

void pinClose(byte pin)
{
    digitalWrite(pin, HIGH);
}
 
