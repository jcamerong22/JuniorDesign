
const int analogIn = A1;
const int ledPin = 6;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Hall Sensor Test"); // so I can keep track of what is loaded 
  Serial.println("No Pedestrian");
}

void loop() {
  // put your main code here, to run repeatedly:
  int pedestrian = analogRead(analogIn);
  bool is_detected = (pedestrian >= 399);
  if (is_detected) {  
      digitalWrite(ledPin, HIGH);
  } else {
      digitalWrite(ledPin, LOW);
  }
}
