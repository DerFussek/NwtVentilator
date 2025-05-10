#pragma once
#include "A4988.h"

class Nema17 {
  protected:
    const short int STEPS = 200;     
    const short int MICROSTEPS = 16;
    
    short int RPM;
    short int dir;
    short int step;
    short int sleep;
    short int ms1;
    short int ms2;
    short int ms3;

    short int stop;

  protected:  
    A4988 motor;

    short int position;

  public:
    Nema17(int RPM, const int dir, const int step, const int sleep, const int ms1, const int ms2, const int ms3, const int stop) 
      : motor(this->STEPS, dir, step, sleep, ms1, ms2, ms3) {
      this->RPM = RPM;
      this->dir = dir;
      this->step = step;
      this->sleep = sleep;
      this->ms1 = ms1;
      this->ms2 = ms2;
      this->ms3 = ms3;
      this->stop = stop;
    }

    virtual void begin() {
      this->motor.begin(this->RPM, this->MICROSTEPS);
      this->motor.disable();
      this->motor.setSpeedProfile(motor.CONSTANT_SPEED);
    }

    //TODO
    
    //Getter / Setter
    int getPosition() {
      return this->position;
    }

    void setPosition(int pos) {
      this->position = pos;
    }
};