#pragma once                               // Mehrfacheinbindung verhindern
#include "A4988.h"                         // Bibliothek für A4988 Treiber

class Nema17 {                              // Basisklasse für Nema17‑Schrittmotoren
  protected:
    //Konstante Parameter
    const short int STEPS = 200;            // Vollschritte pro Umdrehung
    const short int MICROSTEPS = 16;        // Mikroschritte
    
    // Motoranschlüsse
    short int RPM;                          // Startdrehzahl
    short int dir;                          // Richtungspin
    short int step;                         // Steppin
    short int sleep;                        // Sleep-Pin
    short int ms1;                          // Microstepping-Pin 1
    short int ms2;                          // Microstepping-Pin 2
    short int ms3;                          // Microstepping-Pin 3

    // Button für Motorstopp
    short int stop;

  protected:  

    A4988 motor;  // Motorobjekt

  public:
    // Konstruktor: Pins und Parameter übernehmen
    Nema17(int RPM, const int dir, const int step, const int sleep, const int ms1, const int ms2, const int ms3, const int stop)
      : motor(this->STEPS, dir, step, sleep, ms1, ms2, ms3) {
      this->RPM = RPM;                      // Startgeschwindigkeit
      this->dir = dir;
      this->step = step;
      this->sleep = sleep;
      this->ms1 = ms1;
      this->ms2 = ms2;
      this->ms3 = ms3;
      this->stop = stop;
    }

    // Motor initialisieren
    virtual void begin() {
      this->motor.begin(this->RPM, this->MICROSTEPS); // Treiber starten
      this->motor.disable();                          // zunächst deaktivieren
      this->motor.setSpeedProfile(motor.CONSTANT_SPEED); // konstantes Tempo
    }

    // TODO: weitere gemeinsame Funktionen
  
};