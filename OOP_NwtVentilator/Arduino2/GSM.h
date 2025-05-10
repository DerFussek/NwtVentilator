#pragma once
#include <Arduino.h>

class GSM {
  private:
    short int ENABLE;
    short int IN1;
    short int IN2;
    short int IN3;
    short int IN4;

    bool isEnabled = false;
    uint8_t speed = 0;

  public:
    GSM(const int ENABLE, const int IN1, const int IN2, const int IN3, const int IN4) {
      this->ENABLE = ENABLE;
      this->IN1 = IN1;
      this->IN1 = IN2;
      this->IN1 = IN3;
      this->IN1 = IN3;
    }

    void begin() {
      pinMode(ENABLE, OUTPUT);
      pinMode(IN1, OUTPUT);
      pinMode(IN2, OUTPUT);
      pinMode(IN3, OUTPUT);
      pinMode(IN4, OUTPUT);
    }

    void enable() {
      this->isEnabled = true;
    }

    void disable() {
      this->isEnabled = false;
    }

    void rotate(uint8_t speed, bool dir) {
      //if(!isEnabled) return;

      if(dir) {
        Serial.println("Direction 1");
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENABLE, speed);
      } else if(!dir) {
        Serial.println("Direction 2");
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENABLE, speed);
      } 
    }
};