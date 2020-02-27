byte duty_cycle = 0;

String readString;

void setup(){
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);

  /* Phase Correct PWM */
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
  TCCR2B = _BV(CS22); 

  //TCCR1A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
  //TCCR1B = _BV(CS22); 
  
  /*
  TCCR3A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM20);
  TCCR1B = _BV(WGM22) | _BV(CS22);
  */
  
  /*
  TCCR3A = (0<<COM2A1)|(1<<COM2A0)|(0<<COM2B1)|(1<<COM2B0)| 0        | 0        |(1<<WGM21)|(0<<WGM20);
  TCCR1B = (0<<FOC2A )|(1<<FOC2B )| 0         | 0         |(0<<WGM22)|(0<<CS22 )|(1<<CS21 )|(0<<CS20 );
  */
}

void loop(){
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
    duty_cycle = readString.toInt();
    
    Serial.println("Duty_Cycle A:");
    Serial.println(duty_cycle);
    Serial.println("Duty_Cycle B:");
    Serial.println(duty_cycle);

    OCR2A = duty_cycle;
    OCR2B = duty_cycle;
    readString=""; //empty for next input
  }
  analogWrite(10, OCR2A);
  analogWrite(9, OCR2B);
  delay(2);
}
