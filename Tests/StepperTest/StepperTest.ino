// Einfacher Testsketch für einen einzelnen Schrittmotor.
#include <Arduino.h>
#include "A4988.h"

// Motor-Konfiguration
#define MOTOR_STEPS 200       
#define MICROSTEPS 16         
#define RPM 120               

// Pins für A4988
#define DIR 2
#define STEP 3
#define SLEEP 4
#define MS1 7
#define MS2 6
#define MS3 5


A4988 stepper(MOTOR_STEPS, DIR, STEP, SLEEP, MS1, MS2, MS3);

void setup() {
  Serial.begin(115200);


  stepper.begin(RPM, MICROSTEPS);


  stepper.enable();


  stepper.setSpeedProfile(stepper.LINEAR_SPEED);
}

void loop() {
  Serial.println("Drehe nach rechts");
  stepper.rotate(360);      
  delay(1000);              

  Serial.println("Drehe nach links");
  stepper.rotate(-360);     
  delay(1000);              
}
