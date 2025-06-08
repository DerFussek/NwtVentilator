
//*****************Ampelanzeige**********************//
#include "AmpelDisplay.h"                // Anzeige über drei LEDs
const int ampelRedPin = 45;               // Pin für Rot
const int ampelYellowPin = 41;            // Pin für Gelb
const int ampelGreenPin = 43;             // Pin für Grün

AmpelDisplay ampelDisplay(ampelRedPin, ampelYellowPin, ampelGreenPin); // Objekt anlegen

//*****************Fernbedinung**********************//
#include "Remote.h"                      // Infrarot-Fernbedienung
const int remotePIN = 13;                 // Pin für den IR-Empfänger

Remote remote(remotePIN);                 // Fernbedienungsobjekt

//*******************Ledstrip************************//
#include "Ledstrip.h"                    // RGB-LED-Strip
const int stripPin = 12;                  // Anschluss-Pin
const int stripLedNumber = 8;             // Anzahl LEDs
const int numberOfLevels = 4;             // Helligkeitsstufen

Ledstrip ledStrip(stripPin, stripLedNumber, numberOfLevels); // Strip-Objekt


//******************Andere*******************//
#include "Messager.h"                    // Serielle Kommunikation
Modus currentModus;                       // aktueller Betriebsmodus
Modus lastModus;                          // zuletzt gemeldeter Modus

void setup() {
  Serial.begin(9600);                     // USB‑Serielle Schnittstelle
  Serial2.begin(9600);                    // Verbindung zum zweiten Arduino

  ampelDisplay.begin();                   // Pins der Ampel konfigurieren
  remote.start();                         // IR-Empfänger starten
  
  ampelDisplay.Off();                     // Startzustand: aus
  
  ledStrip.begin();                       // LED-Strip initialisieren
  ledStrip.Level(2, 0);                   // Anfangshelligkeit setzen

}

uint8_t stufe = 0;                        // aktuelle Lüfterstufe
int lastStufe;                             // zuletzt gesendete Stufe
uint8_t pos = 0;                           // Position des oberen Motors

void loop() {
  Button b = remote.awaitInput(250);       // Eingabe von der Fernbedienung

  //if(b == A) Serial.println("A");
  //if(b == B) Serial.println("B");
  //if(b == C) Serial.println("C");
  //if(b == NONE) Serial.println("NONE");
  //if(b == UP) Serial.println("UP");
  //if(b == DOWN) Serial.println("DOWN");
  //if(b == LEFT) Serial.println("LEFT");
  //if(b == RIGHT) Serial.println("RIGHT");

  if(b == A) {                             // Taste A -> Automatik
    ampelDisplay.Automatic();              // grüne LED
    stufe = 1;                             // Mindeststufe
    currentModus = ON;                     // Modus setzen
  } else if(b == B) {
    ampelDisplay.Off();                    // rote LED
    stufe = 0;                             // alles aus
    currentModus = OFF;                    // Modus aus
  } else if(b == C) {
    ampelDisplay.Manual();                 // gelbe LED
    currentModus = MANUAL;                 // manueller Modus
  } else if(b == UP) {
    ++stufe;                               // Stufe erhöhen
    if(stufe >= 4) stufe = 4;              // Maximalwert begrenzen
  } else if(b == DOWN) {
    --stufe;                               // Stufe verringern
    if(stufe > 4) stufe = 0;               // Untergrenze prüfen
  } else if(b == RIGHT) {
    ++pos;                                 // Position nach rechts
    if(pos >= 36) pos = 36;                // Begrenzung
    Messager.send(currentModus, stufe, pos); // neue Position senden
  } else if(b == LEFT) {
    --pos;                                 // Position nach links
    if(pos  > 36) pos = 0;                 // Begrenzung
    Messager.send(currentModus, stufe, pos); // Position senden
  }
  
  // nur senden wenn sich etwas geändert hat
  if(stufe != lastStufe || currentModus != lastModus) {
    lastStufe  = stufe;                    // neue Werte merken
    lastModus  = currentModus;

    Messager.send(currentModus, stufe, pos); // Paket übertragen
  }
  ledStrip.Level(stufe, 250);             // Anzeige aktualisieren
  Serial.print(currentModus);              // Debug-Ausgabe
  Serial.print(";");
  Serial.print(stufe);
  Serial.print(";");
  Serial.print(pos);
  Serial.println(";");
}