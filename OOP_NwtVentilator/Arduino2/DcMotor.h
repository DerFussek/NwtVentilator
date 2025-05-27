#include "Arduino.h"
class DcMotor {
  private:
    short int in1;
    short int in2;
    short int in3;
    short int in4;
    short int speedPin;
  
  public:
    DcMotor(const int in1, const int in2, const int in3, const int in4, int speed) {
      this->in1 = in1;
      this->in2 = in2;
      this->in3 = in3;
      this->in4 = in4;
      this->speedPin = speed;
    }

    void begin() {
      pinMode(this->in1, OUTPUT);
      pinMode(this->in2, OUTPUT);
      pinMode(this->in3, OUTPUT);
      pinMode(this->in4, OUTPUT);

      pinMode(this->speedPin, OUTPUT);
    }

    void enable() {
      digitalWrite(this->in2, HIGH);
      digitalWrite(this->in4, HIGH);
    }

    void disable() {
      digitalWrite(this->in2, LOW);
      digitalWrite(this->in4, LOW);
    }

    void setSpeed(int speed) {
      analogWrite(this->speedPin, speed);
    }
};