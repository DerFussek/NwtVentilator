#include "A4988.h"
#include "LowerStepper.h"
#define MOTOR_STEPS 200
#define MICROSTEPS 16
#define RPM 20

#define DIR 13
#define STEP 12
#define SLEEP 11
#define MS1 8
#define MS2 9
#define MS3 10

#define STOPP 21  // INT4 — höchste Interrupt-Priorität auf Mega

LowerStepper stepper(RPM, DIR, STEP, SLEEP, MS1, MS2, MS3, STOPP);

#include "UpperStepper.h"

#define upper_DIR 2
#define upper_STEP 3
#define upper_SLEEP 4
#define upper_MS3 7
#define upper_MS2 6
#define upper_MS1 5

//UpperStepper stepper2(RPM, upper_DIR, upper_STEP, upper_SLEEP, upper_MS1, upper_MS2, upper_MS3);

#include "GSM.h"
//GSM gsm(12, 11, 10, 9, 8);

void setup() {
  Serial.begin(9600);
  pinMode(STOPP, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(STOPP), LowerStepper::stopInterrupt, FALLING);

  //stepper2.begin();
  stepper.begin();
  //gsm.begin();

  
}

void loop() {
  //gsm.enable();
  //gsm.rotate(255, 0);
  //delay(5000);
  
  
  stepper.test2();
  //stepper.test();
  //stepper.getStepper().rotate(90);
  //delay(5000);
}
