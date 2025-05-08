#include "A4988.h"
#include "LowerStepper.h"
#define MOTOR_STEPS 200
#define MICROSTEPS 16
#define RPM 20

#define DIR 2
#define STEP 3
#define SLEEP 4
#define MS1 5
#define MS2 6
#define MS3 8

#define STOPP 21  // INT4 — höchste Interrupt-Priorität auf Mega

LowerStepper stepper(MOTOR_STEPS, DIR, STEP, SLEEP, MS1, MS2, MS3, STOPP);




void setup() {
  Serial.begin(9600);
  pinMode(STOPP, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(STOPP), LowerStepper::stopInterrupt, FALLING);

  stepper.begin();

  
}

void loop() {
  //stepper.test();
  stepper.getStepper().rotate(90);
  delay(5000);
}
