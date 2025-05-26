#pragma once
#include "Nema17.h"

volatile bool stopSignal = false;
volatile unsigned long lastInterruptTime = 0;

class LowerStepper : public Nema17 {
  public:
    static LowerStepper* instance;  // Zeiger auf aktives Objekt

    LowerStepper(int RPM, int dir, int step, int sleep, int ms1, int ms2, int ms3, int stop)
      : Nema17(RPM, dir, step, sleep, ms1, ms2, ms3, stop) {
      instance = this;  // beim Erstellen merken
    }

    static void stopInterrupt() {
      if (instance != nullptr) {
        instance->motor.disable();
        instance->motor.stop();  // Zugriff auf das Objekt
        delay(500);
        instance->motor.setSpeedProfile(instance->motor.CONSTANT_SPEED);
        instance->motor.setRPM(20);
        instance->motor.enable();
        instance->motor.rotate(-25);
        instance->motor.disable();
        stopSignal = true;
      }
    }

    void stepperToReferencePoint () {
      motor.setRPM(10);
      motor.enable();
      motor.setSpeedProfile(motor.CONSTANT_SPEED);

      motor.rotate(99999);  // konstante Rechtsdrehung

      while (stopSignal == false) {}

      // Stoppen — außerhalb des Interrupts
      motor.stop();
      motor.disable();
      stopSignal = false;
      delay(300);  // Beruhigungspause
    }

    void begin() {
      motor.begin(RPM, MICROSTEPS);
  
      stepperToReferencePoint();
      motor.setRPM(300);
      motor.setSpeedProfile(motor.CONSTANT_SPEED);
    }

    void test() {
      float steps = 240 / 1;
      motor.setSpeedProfile(motor.LINEAR_SPEED);
      Serial.println("3x hin und her");
      for(int j = 20; j < 140; j+=40) {
        motor.setRPM(j);
        for(int i = 0; i < 1; i++) {
          motor.enable();
          motor.rotate(-steps);
          motor.disable();
          delay(j);
        } 

        for(int i = 0; i < 1; i++) {
          motor.enable();
          motor.rotate(steps);
          motor.disable();
          delay(j);
        } 
      }
      motor.stop();
      delay(1000);
      Serial.println("Reference Punkt");
      stepperToReferencePoint();
      delay(5000);
      Serial.println("360bewegung 20rpm");
      motor.setSpeedProfile(motor.LINEAR_SPEED);
      motor.enable();
      motor.setRPM(20);
      motor.rotate(-500);
      delay(500);
      motor.setRPM(90);
      motor.rotate(500);
      motor.stop();
      motor.disable();
      delay(2500);
      while(digitalRead(this->stop)) {}
      while(!digitalRead(this->stop)) {}
    }

    void test2() {
      float steps = 120; //(360/6)/2 * 4
       
      motor.setSpeedProfile(motor.LINEAR_SPEED);
      Serial.println("3x hin und her");
        motor.setRPM(69);
        for(int i = 0; i < 1; i++) {
          motor.enable();
          motor.rotate(-steps);
          motor.disable();
          delay(250);
        } 

        for(int i = 0; i < 1; i++) {
          motor.enable();
          motor.rotate(steps);
          motor.disable();
          delay(250);
        } 
      
      motor.stop();
      motor.disable();
      delay(500);
    }

    void move(int deg) {
      motor.setSpeedProfile(motor.LINEAR_SPEED);
      motor.setRPM(60);
      motor.enable();
      motor.rotate(deg);
      motor.disable();    
    }

    public:
    A4988 getStepper() {
      return this->motor;
    }
};
LowerStepper* LowerStepper::instance = nullptr;
