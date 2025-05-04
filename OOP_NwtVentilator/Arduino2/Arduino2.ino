#include <Arduino.h>
#include "A4988.h"

#define MOTOR_STEPS 200
#define MICROSTEPS 16
#define RPM 20

#define DIR 3
#define STEP 4
#define SLEEP 5
#define MS1 6
#define MS2 7
#define MS3 8

#define STOPP 2  // INT4 — höchste Interrupt-Priorität auf Mega

A4988 stepper(MOTOR_STEPS, DIR, STEP, SLEEP, MS1, MS2, MS3);

volatile bool stopSignal = false;
volatile unsigned long lastInterruptTime = 0;

void stopInterrupt() {
  // nur Flag setzen (superkurz!)
  stepper.disable();
  delay(500);
  
  stepper.enable();
  stepper.rotate(-20);
  stepper.disable();

  stopSignal = true;
}

void stepperToReferencePoint () {
  stepper.enable();
  stepper.setSpeedProfile(stepper.CONSTANT_SPEED);

  stepper.rotate(99999);  // konstante Rechtsdrehung

  while (!stopSignal) {
    // leer lassen für schnellste Reaktion
  }

  // Stoppen — außerhalb des Interrupts
  stepper.stop();
  stepper.disable();
  stopSignal = false;
  delay(300);  // Beruhigungspause
}

void setup() {
  pinMode(STOPP, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(STOPP), stopInterrupt, FALLING);

  stepper.begin(RPM, MICROSTEPS);
  
  stepperToReferencePoint();
  stepper.setRPM(200);
  stepper.setSpeedProfile(stepper.LINEAR_SPEED);
}

void loop() {
  float steps = 240 / 3;
  for(int i = 0; i < 3; i++) {
    stepper.enable();
    stepper.rotate(-steps);
    stepper.disable();
    delay(150);
  } 

  for(int i = 0; i < 3; i++) {
    stepper.enable();
    stepper.rotate(steps);
    stepper.disable();
    delay(150);
  } 

  delay(150);
}
