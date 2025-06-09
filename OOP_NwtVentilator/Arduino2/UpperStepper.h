// Spezialisierung des Nema17 für den oberen Drehteller.
#pragma once                              // nur einmal einbinden
#include "Nema17.h"                       // Basisklasse

class UpperStepper : public Nema17 {      // oberer Schrittmotor
  int position = 0;                       // gemerkte Position in Grad
  public:
    UpperStepper(int rpm, int dir, int step, int sleep, int ms1, int ms2, int ms3)
      : Nema17(rpm, dir, step, sleep, ms1, ms2, ms3, -1) {
      
    }

    // initialisieren
    void begin() {
      motor.setSpeedProfile(motor.CONSTANT_SPEED);
      motor.setRPM(10);

      motor.begin(RPM, MICROSTEPS);
      motor.disable();
    }

    // kleiner Testablauf
    void test() {
      delay(2500);
      motor.setRPM(25);
      motor.setSpeedProfile(motor.LINEAR_SPEED, 1000, 1000);
      motor.enable();
      motor.rotate(-90);
      
      delay(1000);
      motor.enable();
      motor.rotate(90);
      motor.disable();
      delay(2500);
    }

    // Motor auf absolute Position fahren
    void movestepper(int ziel) {
      int delta = ziel - position;

        motor.enable();
        motor.rotate(delta * -1);  // Bewegung ausführen

        position = (position + delta);     // neue Position merken
    }

        //Getter / Setter
    int getPosition() {
        return this->position;              // aktuellen Wert liefern
    }

    void setPosition(int pos) {
        this->position = pos;               // Position setzen
    }

    public:
      A4988 getStepper() {                  // Zugriff auf Treiber
        return this->motor;
      }
};

