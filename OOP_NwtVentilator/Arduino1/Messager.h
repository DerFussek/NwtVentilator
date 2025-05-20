#pragma once
#include "HardwareSerial.h"
#include <stdint.h>
enum Modus {OFF = 0, ON = 1, MANUAL = 2};

class Sender {
  private:
    HardwareSerial &serialPort;

  public:
    Sender(HardwareSerial &port, unsigned long baud): serialPort(port) {
      serialPort.begin(baud);
    }
  
  void send(Modus m, uint8_t speed) {
    this->serialPort.write((uint8_t)m);
    this->serialPort.write(speed);
  }
};

class Receiver {
  private:
    HardwareSerial &serialPort;

  public:
    Receiver(HardwareSerial &port, unsigned long baud): serialPort(port) {
      serialPort.begin(baud);
    }
  
  bool available() {
    return serialPort.available() >=2;
  }

  void read(Modus &outMode, uint8_t &outSpeed) {
    if(Serial.available() > 0) {
      outMode  = static_cast<Modus>(serialPort.read());
      outSpeed = serialPort.read();
    }
  }
};

struct MessagerClass {
  Sender sender;
  Receiver receiver;

  MessagerClass()
    : sender(Serial2, 9600)
    , receiver(Serial2, 9600)
  {}
};


MessagerClass Messager;