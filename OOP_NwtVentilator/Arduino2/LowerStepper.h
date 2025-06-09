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

    // Testprogramm für Bewegungen
    void test() {
      float steps = 240 / 1;               // Schrittanzahl
      motor.setSpeedProfile(motor.LINEAR_SPEED);
      Serial.println("3x hin und her");
      for(int j = 20; j < 140; j+=40) {
        motor.setRPM(j);
        for(int i = 0; i < 1; i++) {
          motor.enable();
          motor.rotate(-steps);
          motor.disable();
          delay(j);
        } 

        for(int i = 0; i < 1; i++) {
          motor.enable();
          motor.rotate(steps);
          motor.disable();
          delay(j);
        } 
      }
      motor.stop();
      delay(1000);
      Serial.println("Reference Punkt");
      stepperToReferencePoint();
      delay(5000);
      Serial.println("360bewegung 20rpm");
      motor.setSpeedProfile(motor.LINEAR_SPEED);
      motor.enable();
      motor.setRPM(20);
      motor.rotate(-500);
      delay(500);
      motor.setRPM(90);
      motor.rotate(500);
      motor.stop();
      motor.disable();
      delay(2500);
      while(digitalRead(this->stop)) {}     // warten bis Taster losgelassen
      while(!digitalRead(this->stop)) {}    // und erneut gedrückt wurde
    }

    // zweites Testprogramm
    void test2() {
      float steps = 120; //(360/6)/2 * 4
       
      motor.setSpeedProfile(motor.LINEAR_SPEED);
      Serial.println("3x hin und her");
        motor.setRPM(69);
        for(int i = 0; i < 1; i++) {
          motor.enable();
          motor.rotate(-steps);
          motor.disable();
          delay(250);
        } 

        for(int i = 0; i < 1; i++) {
          motor.enable();
          motor.rotate(steps);
          motor.disable();
          delay(250);
        } 
      
      motor.stop();
      motor.disable();
      delay(500);
    }

    // einfache Bewegungsfunktion
    void move(int deg) {
      motor.setSpeedProfile(motor.LINEAR_SPEED);
      motor.setRPM(60);
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
