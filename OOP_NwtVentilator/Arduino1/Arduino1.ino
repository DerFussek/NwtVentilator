
//*****************Ampelanzeige**********************//
#include "AmpelDisplay.h"
const int ampelRedPin = 9;
const int ampelYellowPin = 10;
const int ampelGreenPin = 11;

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

//***************Gleichschrittmotor*******************//
#include "DcMotor.h"
const short int in1;
const short int in2;
const short int in3;
const short int in4;
const short int speed;

DcMotor gsm(in1, in2, in3, in4, speed);

//***************Ultraschallsensoren*******************//
//TODO

//***************TFT-Display*******************//
#include "UserInterface.h"
#define TFT_CS    25
#define TFT_RST   23
#define TFT_DC    27
#define TFT_MOSI  29
#define TFT_SCLK  31
UserInterface display(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//******************Andere*******************//
#include "Messager.h"
Modus currentModus;
Modus lastModus;

void setup() {
  Serial.begin(9600);
  ampelDisplay.begin();
  remote.start();
  
  ledStrip.begin();
  display.begin();
}

int stufe = 0;
int lastStufe;

void loop() {
  
  Button b = remote.awaitInput(250);

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
  } else if(b == DOWN) {
    --stufe;
  }

  if (stufe != lastStufe || currentModus != lastModus) {
    lastStufe  = stufe;
    lastModus  = currentModus;
    ledStrip.Level(stufe, 0);
    Messager.sender.send(currentModus, stufe);
  }
  
  display.updateDynamicUserInterface(1000, stufe);
}