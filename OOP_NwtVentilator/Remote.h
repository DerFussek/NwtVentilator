#pragma once
#include <IRremote.h>

enum Button { NONE, A, B, C, UP, DOWN };

class Remote {
  private:
    short int pin;
    IRrecv remote;
  
  public:
    Remote(const int pin) : remote(pin) {
      this->pin = pin;
    }

  public:
    void start() {
      remote.enableIRIN();
    }

    Button awaitInput(int pressDelayMs) {
      static long pressDelay = millis();

      if(remote.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) return;
      if(!remote.decode()) return;
      if(!(millis() - pressDelay) >= pressDelayMs) return;

      pressDelay = millis();

      unsigned long command = remote.decodedIRData.command;

      if(command == 0x45) {  //Taste "A"
        command = "";
        remote.resume();

        return A;
      } else if(command == 0x47) { //Taste "B"
        command = "";
        remote.resume();

        return B;
      } else if(command == 0x09) { //Taste "C"
        command = "";
        remote.resume();
        return C;
      } else if(command == 0x46) { //Taste "UP"
        command = "";
        remote.resume();

        return UP;
      } else if(command == 0x15) { //Taste "DOWN" 
        command = "";
        remote.resume();

        return DOWN;
      } 

      command = "";
      remote.resume();
      return NONE;
      
    }
};