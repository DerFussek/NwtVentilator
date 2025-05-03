
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

//*****************Schrittmotoren*********************//
#include "Nema17.h"
//Schrittmotor 1 (Pinbelegungnen für den ersten Schrittmotor)
const short int s1_DIR = 2;
const short int s1_STEP = 3;
const short int s1_SLEEP = 4;
const short int s1_MS1 = 7;
const short int s1_MS2 = 6;
const short int s1_MS3 = 5; 
const short int s1_RPM = 80;

const short int s1_STOPP;

Nema17 stepper1(s1_RPM, s1_DIR, s1_STEP, s1_SLEEP, s1_MS1, s1_MS2, s1_MS3, s1_STOPP);

//Schrittmotor 2 (Pinbelegungnen für den zweiten Schrittmotor)
const short int s2_DIR = 2;
const short int s2_STEP = 3;
const short int s2_SLEEP = 4;
const short int s2_MS1 = 7;
const short int s2_MS2 = 6;
const short int s2_MS3 = 5; 
const short int s2_RPM = 80;

Nema17 stepper2(s2_RPM, s2_DIR, s2_STEP, s2_SLEEP, s2_MS1, s2_MS2, s2_MS3, -1);

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

void setup() {
  Serial.begin(115200);
  ampelDisplay.begin();
  remote.start();
  ledStrip.begin();
  //stepper1.begin();
  //stepper2.begin();
  //gsm.begin();

  display.begin();
}

int stufe = 0;
void loop() {
  
  Button b = remote.awaitInput(500);

  if(b == A) {
    ampelDisplay.Automatic();
  } else if(b == B) {
    ampelDisplay.Off();
  } else if(b == C) {
    ampelDisplay.Manual();
  } else if(b == UP) {
    ++stufe;
  } else if(b == DOWN) {
    --stufe;
  }

  ledStrip.Level(stufe, 0);

  display.updateDynamicUserInterface(1000, stufe);
}