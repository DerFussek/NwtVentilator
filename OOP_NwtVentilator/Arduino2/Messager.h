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
  
  void send(Modus m, uint8_t speed, uint8_t pos) {
    this->serialPort.write((uint8_t)m);
    this->serialPort.write(speed);
    this->serialPort.write(pos);
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
    return serialPort.available() >=3;
  }

  void read(Modus &outMode, uint8_t &outSpeed, uint8_t &outpos) {
    if(serialPort.available() >= 3) {
      outMode  = static_cast<Modus>(serialPort.read());
      outSpeed = serialPort.read();
      outpos = serialPort.read();
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