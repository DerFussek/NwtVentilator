#pragma once
#include <IRremote.h>

class Remote {
  private:
    struct KeyMapping {
      unsigned long code;
      const char* name;
    };

    const KeyMapping keys[18] = {
      {0x45, "A"},
      {0x47, "B"},
      {0x09, "C"},
      {0x46, "UP"},
      {0x15, "DOWN"},
      {0x44, "LEFT"},
      {0x43, "RIGHT"},
      {0x40, "ENTER"},
      {0x07, "NUMBER[0]"},
      {0x16, "NUMBER[1]"},
      {0x19, "NUMBER[2]"},
      {0x0D, "NUMBER[3]"},
      {0x0C, "NUMBER[4]"},
      {0x18, "NUMBER[5]"},
      {0x5E, "NUMBER[6]"},
      {0x08, "NUMBER[7]"},
      {0x1C, "NUMBER[8]"},
      {0x5A, "NUMBER[9]"}
    };

    IRrecv receiver;
    unsigned long command;
    const char* pressedKeyName;

  public:
    Remote(int Pin) : receiver(Pin), pressedKeyName(nullptr) {}

    void setup() {
      receiver.enableIRIn();
    }

    void update() {
      static long pressDelay = millis();
       // Immer zurücksetzen
      pressedKeyName = nullptr;  
      if(!(millis() - pressDelay >= 150)) return; 
        
      if (!receiver.decode()) return;

      this->command = receiver.decodedIRData.command;

      // Taste suchen
      for (int i = 0; i < 18; i++) {
        if (keys[i].code == this->command) {
          pressedKeyName = keys[i].name;
          break;
        }
      }
      receiver.resume();
      pressDelay = millis();
    }

    void printPressedKey() {
      if (pressedKeyName != nullptr) {
        Serial.println(pressedKeyName);
      }
    }

    bool isKeyPressed(String key) {
      if (pressedKeyName == nullptr) return false; // keine Taste gedrückt
      return key.equals(String(pressedKeyName));
    }
};
