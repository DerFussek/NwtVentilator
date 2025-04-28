#pragma once
#include "Remote.h"
#include "DisplayManager.h"
#include "config.h"

class NwtVentilator {
  private:
    Remote remote;
    DisplayManager dsm;

    bool on = false;
    bool off = true;
    bool manual = false;

  public:
    NwtVentilator(int TrafficLightPINS[3]) : remote(remotePin), dsm(TrafficLightPINS) {
      
    }

  public:
    void setup() {
      this->remote.setup();
      
    }

    void update() {
      this->remote.update();

      if(remote.isKeyPressed("A")) {
        on = true;
        off = manual = false;
      }

      if(remote.isKeyPressed("B")) {
        off = true;
        on = manual = false;
      }

      if(remote.isKeyPressed("C")) {
        manual = true;
        on = off = false;
      }

      if(on) {
        dsm.trafficlight.on();
      } else if(off) {
        dsm.trafficlight.off();
      } else if(manual) {
        dsm.trafficlight.standbye();
      }
    }
};