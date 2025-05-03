#include "Arduino.h"
#pragma once

class AmpelDisplay {
  private:
    short int r;
    short int g;
    short int y;

  public: 
    AmpelDisplay(const int r, const int y, const int g) {
      this->r = r;
      this->g = g;
      this->y = y; 
    }

  public:
    void begin() {
      pinMode(this->r, OUTPUT);
      pinMode(this->g, OUTPUT);
      pinMode(this->y, OUTPUT);
    }

    void Automatic() {
      digitalWrite(this->r, LOW);
      digitalWrite(this->g, HIGH);
      digitalWrite(this->y, LOW);
    }

    void Off() {
      digitalWrite(this->r, HIGH);
      digitalWrite(this->g, LOW);
      digitalWrite(this->y, LOW);
    }

    void Manual() {
      digitalWrite(this->r, LOW);
      digitalWrite(this->g, LOW);
      digitalWrite(this->y, HIGH);
    }
};