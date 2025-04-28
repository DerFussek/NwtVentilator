#include "NwtVentilator.h"
#include "config.h"

NwtVentilator venti(TrafficPins);

void setup() {
  Serial.begin(9600);
  venti.setup();
}

void loop() {
  venti.update();
}
