
//*****************Ampelanzeige**********************//
#include "AmpelDisplay.h"
const int ampelRedPin = 11;
const int ampelYellowPin = 10;
const int ampelGreenPin = 9;

AmpelDisplay ampelDisplay(ampelRedPin, ampelYellowPin, ampelGreenPin);

//*****************Fernbedinung**********************//
#include "Remote.h"
const int remotePIN = 13;

Remote remote(remotePIN);

//*******************Ledstrip************************//
#include "Ledstrip.h"
const int stripPin = 12;
const int stripLedNumber = 8;
const int numberOfLevels = 4;

Ledstrip ledStrip(stripPin, stripLedNumber, numberOfLevels);


//******************Andere*******************//
#include "Messager.h"
Modus currentModus;
Modus lastModus;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  ampelDisplay.begin();
  remote.start();
  ampelDisplay.Off();
  ledStrip.begin();
  ledStrip.Level(2, 0);

}

uint8_t stufe = 0;
int lastStufe;
uint8_t pos = 0;

void loop() {
  
  Button b = remote.awaitInput(250);

  //if(b == A) Serial.println("A");
  //if(b == B) Serial.println("B");
  //if(b == C) Serial.println("C");
  //if(b == NONE) Serial.println("NONE");
  //if(b == UP) Serial.println("UP");
  //if(b == DOWN) Serial.println("DOWN");
  //if(b == LEFT) Serial.println("LEFT");
  //if(b == RIGHT) Serial.println("RIGHT");

  if(b == A) {
    ampelDisplay.Automatic();
    stufe = 1;
    currentModus = ON;
  } else if(b == B) {
    ampelDisplay.Off();
    stufe = 0;
    currentModus = OFF;
  } else if(b == C) {
    ampelDisplay.Manual();
    currentModus = MANUAL;
  } else if(b == UP) {
    ++stufe;
    if(stufe >= 4) stufe = 4;
  } else if(b == DOWN) {
    --stufe;
    if(stufe > 4) stufe = 0;
  } else if(b == RIGHT) {
    ++pos;
    if(pos >= 36) pos = 36;
    Messager.sender.send(currentModus, stufe, pos);
  } else if(b == LEFT) {
    --pos;
    if(pos  > 36) pos = 0;
    Messager.sender.send(currentModus, stufe, pos);
  }

  //
  if(stufe != lastStufe || currentModus != lastModus) {
    lastStufe  = stufe;
    lastModus  = currentModus;
    ledStrip.Level(stufe, 0);
    Messager.sender.send(currentModus, stufe, pos);
  }

  Serial.print(currentModus);
  Serial.print(";");
  Serial.print(stufe);
  Serial.print(";");
  Serial.print(pos);
  Serial.println(";");
}