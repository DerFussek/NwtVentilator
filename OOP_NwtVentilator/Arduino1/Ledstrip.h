#pragma once                                 // Mehrfacheinbindung verhindern
#include <Adafruit_NeoPixel.h>               // Bibliothek für den LED‑Strip

class Ledstrip {                            // Klasse zur Steuerung eines NeoPixel‑Strips
  private:
    short int pin;                          // Pin an dem der Strip angeschlossen ist
    short int numberLeds;                   // Anzahl der LEDs auf dem Strip

    Adafruit_NeoPixel strip;                // NeoPixel Objekt für die Ansteuerung

    byte numberOfLevels;                    // Anzahl der Helligkeitsstufen
    byte stripColor[3] = {16, 16, 16};      // Aktuelle Farbe des Streifens

  public:
    // Konstruktor übernimmt Pin, LED‑Anzahl und Levelanzahl
    Ledstrip(const int pin, const int numberLeds, const byte numberOfLevels) : strip(numberLeds, pin, NEO_GRB + NEO_KHZ800) {
      this->pin = pin;                      // Merken des Anschluss-Pins
      this->numberLeds = numberLeds;        // Anzahl der LEDs speichern
      this->numberOfLevels = numberOfLevels;// Anzahl der Helligkeitsstufen merken
    }

    // Strip initialisieren
    void begin() {
      this->strip.begin();                  // Bibliothek starten
      this->strip.show();                   // LEDs ausschalten
    }

    // Level auf dem Strip anzeigen
    void Level(int level, const int updateDelayMs) {
      static long updateDelay = millis();         // Zeitstempel für Updates
      if(!(millis() - updateDelay >= updateDelayMs)) return; // nur alle X ms
      updateDelay = millis();

      this->strip.clear();                        // alle LEDs ausschalten
      int ledPerStage = this->numberLeds / this->numberOfLevels; // LEDs pro Stufe

      for(int i = numberLeds; i >=  numberLeds - level * ledPerStage; i--) { // gewünschte Anzahl einschalten
        this->strip.setPixelColor(i, strip.Color(stripColor[0], stripColor[1], stripColor[2]));
      }

      this->strip.show();                         // Änderungen anzeigen
    }

    // neue Farbe setzen
    void setColor(byte r, byte g, byte b) {
      this->stripColor[0] = r;             // Rotanteil speichern
      this->stripColor[1] = g;             // Grünanteil speichern
      this->stripColor[2] = b;             // Blauanteil speichern
    }
};
