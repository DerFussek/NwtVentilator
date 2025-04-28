#include "Remote.h"

const int PIN = 22;
Remote remote(PIN);

int red = 9;
int green = 10;
int yellow = 8;


void setup() {
  remote.setup();
}

void loop() {
  remote.update();
  remote.printPressedKey();

}
