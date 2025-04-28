#include "Arduino.h"
class DisplayManager {
  
  private:
    struct TrafficLight {
      const int RED;
      const int YELLOW;
      const int GREEN;

      // Konstruktor
      TrafficLight(int r, int y, int g)
        : RED(r), YELLOW(y), GREEN(g) {}

      // Methode: Nur Grün an
      void on() const {
        digitalWrite(RED, LOW);
        digitalWrite(YELLOW, LOW);
        digitalWrite(GREEN, HIGH);
      }

      // Methode: Nur Rot an
      void off() const {
        digitalWrite(RED, HIGH);
        digitalWrite(YELLOW, LOW);
        digitalWrite(GREEN, LOW);
      }

      // Methode: Nur Gelb an (Standby)
      void standbye() const {
        digitalWrite(RED, LOW);
        digitalWrite(YELLOW, HIGH);
        digitalWrite(GREEN, LOW);
      }
    };

  public:
    TrafficLight trafficlight;
  
  public:
    
    DisplayManager(int TrafficLightPINS[3])
      : trafficlight(TrafficLightPINS[0], TrafficLightPINS[1], TrafficLightPINS[2])
    {
      pinMode(trafficlight.RED, OUTPUT);
      pinMode(trafficlight.YELLOW, OUTPUT);
      pinMode(trafficlight.GREEN, OUTPUT);
    }

  public:
   
    void setup() {
      
    
    }

};