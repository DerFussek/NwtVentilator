#pragma once
#include <Adafruit_NeoPixel.h>

class Ledstrip {
  private:
    short int pin;  //Pin an dem der Ledstrip angeschlossen ist
    short int numberLeds; //Anzahl an LEDS auf dem Strip
    
    Adafruit_NeoPixel strip; //objekt der Klasse Adafruit_NeoPixel einbinden

    byte numberOfLevels;
    byte stripColor[3] = {16, 16, 16};

  public:
    Ledstrip(const int pin, const int numberLeds, const byte numberOfLevels) : strip(numberLeds, pin, NEO_GRB + NEO_KHZ800) {
      this->pin = pin;
      this->numberLeds = numberLeds;
      this->numberOfLevels = numberOfLevels;
    }

    void begin() {
      this->strip.begin();
      this->strip.show();
    }

    void Level(int level, const int updateDelayMs) {
      static long updateDelay = millis();
      if(!(millis() - updateDelay >= updateDelayMs)) return;
      updateDelay = millis();


      this->strip.clear();
      int ledPerStage = this->numberLeds / this->numberOfLevels;

      for(int i = numberLeds; i >=  numberLeds - level * ledPerStage; i--) {
        this->strip.setPixelColor(i, strip.Color(stripColor[0], stripColor[1], stripColor[2]));
      }

      this->strip.show();
    }

    void setColor(byte r, byte g, byte b) {
      this->stripColor[0] = r;
      this->stripColor[1] = g;
      this->stripColor[2] = b;
    }
};