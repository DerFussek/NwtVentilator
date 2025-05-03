#pragma once
#include <Adafruit_NeoPixel.h>

class Ledstrip {
  private:
    short int pin;
    short int numberLeds;
    Adafruit_NeoPixel strip;
    byte numberOfLevels;
    byte stripColor[3] = {0, 0, 0};

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

      for(int i = 0; i < level * ledPerStage; i++) {
        this->strip.setPixelColor(i, ring.Color(stripColor[0], stripColor[1], stripColor[2]));
      }

      this->strip.show();
    }
};