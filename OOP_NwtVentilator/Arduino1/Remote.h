#pragma once
#include <IRremote.h>

enum Button { NONE, A, B, C, UP, DOWN, RIGHT, LEFT };

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
      remote.enableIRIn();
    }

    Button awaitInput(int pressDelayMs) {
      static long pressDelay = millis();

      if (!remote.decode()) return NONE; 

      if (remote.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
          remote.resume();
          return NONE;
      }
      
      if (millis() - pressDelay < pressDelayMs) return NONE;

      pressDelay = millis();                          
      unsigned long command = remote.decodedIRData.command; 

      
      remote.resume();

      switch (command) {
        case 0x45: return A;   // Taste "A"
        case 0x47: return B;   // Taste "B"
        case 0x09: return C;   // Taste "C"
        case 0x46: return UP;  // Taste "UP"
        case 0x15: return DOWN;
        case 0x43: return RIGHT;
        case 0x44: return LEFT;// Taste "DOWN"
        default:    return NONE;
      }
    }

};