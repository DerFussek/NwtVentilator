
/*
  // Routinen zur Auswertung der Ultraschallsensoren
  1. Messen bei 0° Drehung des Zahnrads (Jeder Sensor misst 3x)
  2. Auswerten und Oberen Schrittmotor ausrichten
  3. Messen bei 30 Drehung des Zahnrads [kleines Zahnrad muss sich 120° drehen]
  4. Ausweten und Oberen Schrittmotor ausrichten
*/
// und zur automatischen Ausrichtung des oberen Motors.
const int HYSTERESE = 15;         // Toleranzschwelle bei den Sensoren
const int COUNTER = 1;            // wie oft ein Delta überschritten werden muss
void auswertenUndDrehen() {       // Messwerte ermitteln und Motor steuern
  const int MAX_ENTFERNUNG = 150; // maximale Distanz für Ultraschallsensoren
  long t_r0[6], t_r30[6];         // Arrays für Messreihen
  
    for(int i=0; i<6; i++) {                // alle Sensoren durchgehen
      long t_w[3];                          // drei Messungen puffern

      for(int n=0; n<3; n++) {
        t_w[n] = messung(sensoren[i].trig, sensoren[i].echo, MAX_ENTFERNUNG);
      }
      
      t_r0[i] = berechneMedian(t_w, 3);    // Median bilden
      delay(10);
    }

    justiere0(t_r0);  //Auswerten und Oberen Schrittmotor ausrichten

    //Drehplatte um +30° drehen
    stepper.move(-120);                     // Drehteller weiterdrehen
    delay(100);
    
    for(int i=0; i<6; i++) {                // erneute Messrunde
      long t_w[3];

      for(int n=0; n<3; n++) {
        t_w[n] = messung(sensoren[i].trig, sensoren[i].echo, MAX_ENTFERNUNG);
      }
      
      t_r30[i] = berechneMedian(t_w, 3);   // Median speichern
      delay(10);
    }

    justiere30(t_r30); //Auswerten und Oberen Schrittmotor ausrichten

    //Drehplatte um -30° drehen
    stepper.move(120);
    delay(100);
}

void justiere0(long t_r0[6]) {           // Ausrichtung bei 0° Messung
  static long c_r0[6];                   // Zähler für jeden Sensor
  long delta[6];                         // Abweichungen
  
  for(int i=0; i<6; i++) {                // Differenz berechnen
    delta[i] = r0[i] - t_r0[i];
    if(delta[i] < HYSTERESE) delta[i] = 0; // kleine Werte ignorieren
  }

  int maxDelta = 0;                       // größtes Delta
  int maxDeltaSensor = 0;                 // zugehöriger Sensor
  for(int i=0; i<6; i++) {  //Größtes Delta berechnen
    if(delta[i] > maxDelta) {
      maxDelta = delta[i];
      if(maxDelta != 0) maxDeltaSensor = i;
    } else {
      continue;
    }
  }

  if(maxDelta != 0) {                     // wenn eine Abweichung vorliegt
    c_r0[maxDeltaSensor]++;               // Zähler erhöhen

    for(int i=0; i<6; i++) {
      if(i == maxDeltaSensor) continue;   // nur für diesen Sensor zählen
      c_r0[i] = 0;
    }

    if(c_r0[maxDeltaSensor] >= COUNTER) {
      int ziel = maxDeltaSensor * 60;     // Zielposition berechnen
      stepper2.movestepper(ziel);         // Schrittmotor bewegen

      #ifdef DEV
      Serial.println("Ziel bei " + (String)ziel + "° ; Delta = " + (String) maxDelta);
      #endif
    }
  }
  
}

void justiere30(long t_r30[6]) {          // Ausrichtung bei 30° Messung
  static long c_r30[6];                   // Zähler je Sensor
  long delta[6];                          // Abweichungen

  for(int i=0; i<6; i++) {                // Abweichung berechnen
    delta[i] = r30[i] - t_r30[i];
    if(delta[i] < HYSTERESE) delta[i] = 0; // kleine Werte ignorieren
  }

  int maxDelta = 0;                       // größtes Delta
  int maxDeltaSensor = 0;                 // zugehöriger Sensor
  for(int i=0; i<6; i++) {
    if(delta[i] > maxDelta) {
      maxDelta = delta[i];
      if(maxDelta != 0) maxDeltaSensor = i;
    } else {
      continue;
    }
  }
  if(maxDelta != 0) {                     // bei Abweichung Zähler erhöhen
    c_r30[maxDeltaSensor]++;
    for(int i=0; i<6; i++) {
      if(i == maxDeltaSensor) continue;   // nur für diesen Sensor zählen
      c_r30[i] = 0;
    }

    if(c_r30[maxDeltaSensor] >= COUNTER) {
      int ziel = maxDeltaSensor * 60 + 30; // Zielposition bestimmen
      if(maxDeltaSensor == 5) ziel = -30;  // Sonderfall für den größten
      
      stepper2.movestepper(ziel); //Oberen Schrittmotor zur Zielposition fahren
      
      #ifdef DEV
      Serial.println("Ziel bei " + (String)ziel + "° ; Delta = " + (String) maxDelta);
      #endif
    }
  }
}

