#pragma once
#include "Nema17.h"

class UpperStepper : public Nema17 {
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

    public:
    A4988 getStepper() {
      return this->motor;
    }
};

