#include <Servo.h>

#define SERVO1 34
#define SERVO2 35
#define MOTOR  36

Servo servo1;
Servo servo2;

int controlFlag = 0;

void systemStart() {
  int i;
  controlFlag = 1;

  for(i = 180; i >= 0; i--) {
    servo1.write(i);
    servo2.write(i);
    delay(10);
  }
  digitalWrite(MOTOR, HIGH);
}

void systemStop() {
  int i;
  controlFlag = 0;
  digitalWrite(MOTOR, LOW);

  for(i = 0; i <= 180; i++) {
    servo1.write(i);
    servo2.write(i);
    delay(10);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(MOTOR, OUTPUT);
  servo1.attach(SERVO1);
  servo2.attach(SERVO2);
  servo1.write(180);
  servo2.write(180);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial1.available()) {
    int data = Serial1.read();
    if(data == 1) {
      if(controlFlag == 0) {
        Serial.println("Receive from Master: START");
        systemStart();
      } else {
        Serial.println("Already started");
      }
    } else if(data == 2) {
      if(controlFlag == 1) {
        Serial.println("Receive from Master: STOP");
        systemStop();
      } else {
        Serial.println("Already stopped");
      }
    }
  }
}
