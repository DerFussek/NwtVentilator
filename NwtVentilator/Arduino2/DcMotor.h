// Einfache Klasse zur Ansteuerung des Gleichstrom-Gebläsemotors.
#include "Arduino.h"               // Grundlegende Arduino-Funktionen
class DcMotor {                     // Klasse zur Ansteuerung eines DC-Motors
  private:
    short int in1;                  // Richtungspin 1
    short int in2;                  // Richtungspin 2
    short int in3;                  // weiteres Steuersignal 1
    short int in4;                  // weiteres Steuersignal 2
    short int speedPin;             // PWM-Pin für die Geschwindigkeit
  
  public:
    // Pins über den Konstruktor festlegen
    DcMotor(const int in1, const int in2, const int in3, const int in4, int speed) {
      this->in1 = in1;             // Pins speichern
      this->in2 = in2;
      this->in3 = in3;
      this->in4 = in4;
      this->speedPin = speed;      // PWM-Pin speichern
    }

    // Pins als Ausgänge konfigurieren
    void begin() {
      pinMode(this->in1, OUTPUT);
      pinMode(this->in2, OUTPUT);
      pinMode(this->in3, OUTPUT);
      pinMode(this->in4, OUTPUT);

      pinMode(this->speedPin, OUTPUT);    // PWM-Pin
    }

    // Motor einschalten
    void enable() {
      digitalWrite(this->in2, HIGH);      // Richtung frei
      digitalWrite(this->in4, HIGH);      // Richtung frei
    }

    // Motor ausschalten
    void disable() {
      digitalWrite(this->in2, LOW);       // abschalten
      digitalWrite(this->in4, LOW);       // abschalten
    }

    // Geschwindigkeit über PWM setzen
    void setSpeed(int speed) {
      analogWrite(this->speedPin, speed); // PWM ausgeben
    }
};
