#include "HardwareSerial.h"
#include <stdint.h>
enum Modus {OFF = 0, ON = 1, MANUAL = 2};

class MessagerClass {
  private:
    HardwareSerial &serialPort;

  public:
    MessagerClass(HardwareSerial &port, unsigned long baud): serialPort(port) {
      serialPort.begin(baud);
    }

    void send(Modus m, uint8_t speed, uint8_t pos) {
      this->serialPort.write((uint8_t)m);
      this->serialPort.write(speed);
      this->serialPort.write(pos);
    }

    bool available() {
      return serialPort.available() >=3;
    }

    void read(Modus &outMode, uint8_t &outSpeed, uint8_t &outpos) {
      if(available()) {
        outMode  = static_cast<Modus>(serialPort.read());
        outSpeed = serialPort.read();
        outpos = serialPort.read();
      }
    }
};

MessagerClass Messager(Serial2, 9600);