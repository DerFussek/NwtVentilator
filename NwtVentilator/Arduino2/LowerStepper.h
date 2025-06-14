// Ableitung für den unteren Schrittmotor inklusive Referenzfahrt.
#pragma once                              // Mehrfacheinbindung verhindern
#include "Nema17.h"                       // Basisklasse für Nema17-Schrittmotor

volatile bool stopSignal = false;         // Flag wird im Interrupt gesetzt
volatile unsigned long lastInterruptTime = 0; // Zeitstempel des letzten Interrupts

class LowerStepper : public Nema17 {      // Spezieller Stepper mit Referenzfahrt
  public:
    static LowerStepper* instance;  // Zeiger auf aktuelles Objekt

    LowerStepper(int RPM, int dir, int step, int sleep, int ms1, int ms2, int ms3, int stop)
      : Nema17(RPM, dir, step, sleep, ms1, ms2, ms3, stop) {
      instance = this;                   // Objekt merken
    }

    // Interrupt-Routine: Stoppt den Motor und fährt ein Stück zurück
    /*
    Eine Interrupt-Service-Routine (ISR) wie stopInterrupt() kann nicht als reguläre Objektmethode aufgerufen werden, 
    weil attachInterrupt nur Zeiger auf Funktionen ohne impliziten this‑Zeiger akzeptiert. Durch das statische Attribut instance behält 
    die Klasse einen Verweis auf das aktuell erzeugte Objekt. Innerhalb der ISR kann so auf die Member (z.B. motor) zugegriffen werden, 
    obwohl die Funktion selbst statisch ist. Ohne diesen Zeiger wäre es in der ISR nicht möglich, auf das konkrete Motorobjekt zuzugreifen 
    und den Motor korrekt anzuhalten
    */
    static void stopInterrupt() {
      if (instance != nullptr) {          // nur wenn Objekt existiert
        instance->motor.disable();
        instance->motor.stop();            // sofort stoppen
        delay(500);
        instance->motor.setSpeedProfile(instance->motor.CONSTANT_SPEED);
        instance->motor.setRPM(20);       // langsam rückwärts
        instance->motor.enable();
        instance->motor.rotate(-25);      // kleines Stück zurück
        instance->motor.disable();
        stopSignal = true;                // Flag setzen
      }
    }

    // Referenzfahrt bis zum Endschalter
    void stepperToReferencePoint () {
      motor.setRPM(10);                   // langsam drehen
      motor.enable();
      motor.setSpeedProfile(motor.CONSTANT_SPEED);

      motor.rotate(99999);               // kontinuierlich rechts drehen

      while (stopSignal == false) {}     // warten bis Interrupt stopSignal setzt

      motor.stop();                      // außerhalb des Interrupts stoppen
      motor.disable();
      stopSignal = false;
      delay(300);                        // kurze Pause
    }

    // Initialisierung des Motors
    void begin() {
      motor.begin(RPM, MICROSTEPS);       // Bibliothek initialisieren

      stepperToReferencePoint();          // Nullpunkt suchen
      motor.setRPM(300);                  // Arbeitsgeschwindigkeit
      motor.setSpeedProfile(motor.CONSTANT_SPEED);
    }
    // einfache Bewegungsfunktion

    void move(int deg, int rpm=60) {
      motor.setSpeedProfile(motor.LINEAR_SPEED);
      motor.setRPM(rpm);
      motor.enable();
      motor.rotate(deg);
      motor.disable();
    }

    public:
    // Zugriff auf das Motorobjekt
    A4988 getStepper() {
      return this->motor;
    }
};
LowerStepper* LowerStepper::instance = nullptr;
