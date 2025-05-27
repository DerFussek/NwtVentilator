#pragma once
#include "Nema17.h"

class UpperStepper : public Nema17 {
  int position = 0;
  public:
    UpperStepper(int rpm, int dir, int step, int sleep, int ms1, int ms2, int ms3)
      : Nema17(rpm, dir, step, sleep, ms1, ms2, ms3, -1) {
      
    }

    void begin() {
      motor.setSpeedProfile(motor.CONSTANT_SPEED);
      motor.setRPM(10);
      
      motor.begin(RPM, MICROSTEPS);
      motor.disable();
    }

    void test() {
      delay(2500);
      motor.setRPM(25);
      motor.setSpeedProfile(motor.LINEAR_SPEED, 1000, 1000);
      motor.enable();
      motor.rotate(-90);
      
      delay(1000);
      motor.enable();
      motor.rotate(90);
      motor.disable();
      delay(2500);
    }

    void movestepper(int ziel) {
      int delta = ziel - position;

      // Schrittmotor ansteuern
      motor.enable();
      motor.rotate(delta * -1);  // oder z. B. moveDegrees(delta); falls du so eine Methode hast

      // Position aktualisieren (Modulo 360, falls du willst)
      position = (position + delta);  
    }

        //Getter / Setter
    int getPosition() {
      return this->position;
    }

    void setPosition(int pos) {
      this->position = pos;
    }

    public:
    A4988 getStepper() {
      return this->motor;
    }
};

