// Funktionen zur Kalibrierung der Ultraschallsensoren.
// Median aus einem Array von long-Werten berechnen
long berechneMedian(const long werte[], int anzahl) {
  // 1) Werte kopieren
  long temp[anzahl];
  for (int i = 0; i < anzahl; i++) {
    temp[i] = werte[i];
  }

  // 2) Nach Größe sortieren
  for (int i = 0; i < anzahl - 1; i++) {
    int kleinster = i;
    for (int j = i + 1; j < anzahl; j++) {
      if (temp[j] < temp[kleinster]) {
        kleinster = j;
      }
    }
    long zw = temp[i];
    temp[i] = temp[kleinster];
    temp[kleinster] = zw;
  }

  // 3) Median ermitteln
  if (anzahl % 2 == 1) {
    // ungerade Anzahl —> mittleres Element
    return temp[anzahl / 2];
  } else {
    // gerade Anzahl —> Durchschnitt der beiden mittleren Werte
    int m = anzahl / 2;
    return (temp[m - 1] + temp[m]) / 2;
  }
}

/*
  1. Ultraschall Sensoren Messen 1x und werden in t_r0 gespeichert (bei 0° position des äußeren Zahnrads)
  2. Ultraschall Sensoren Messen 1x und werden in t_r30 gepsichert (bei 30° position des äußeren Zahnrads)
  3. Schritte 1 und 2 so oft, wie der Funktion über MESSANZAHL übergeben wurde, wiederholen
  4. Von allen Messreihen den Median ermitteln und in r0 und r30 speichern
*/
void kalibiere(const int MESSANZAHL) {
  const int MAX_ENTFERNUNG = 150;
  long t_r0[MESSANZAHL][6], t_r30[MESSANZAHL][6];
  
  
  for(int n = 0; n < MESSANZAHL; n++) { //Schritt eins und zwei so oft wiederholen wie Übergeben wurde (3. Schritt)
    
    //1. Schritt
    for(int i=0; i<6; i++) { //alle sechs Sensoren messen lassen und in t_r0 speichern lassen
      t_r0[n][i] = messung(sensoren[i].trig, sensoren[i].echo, MAX_ENTFERNUNG);
      delay(10);
    }

    //Drehplatte um +30° drehen
    stepper.move(-120);
    delay(100);
    
    //2. Schritt
    for(int i=0; i<6; i++) { //alle sechs Sensoren messen lassen und in t_r30 speichern lassen
      t_r30[n][i] = messung(sensoren[i].trig, sensoren[i].echo, MAX_ENTFERNUNG);
      delay(10);
    }

    //Drehplatte um -30° drehen
    stepper.move(120);
    delay(100);
  }

  //Wenn DEV definiert ist alle Messwerte ausgeben lassen
  #ifdef DEV
    Serial.println("Messwerte 0°: ");
    for(int i=0; i<6; i++ ){
      Serial.print("Sensor [" + (String)i + "] =");
      for(int n=0; n<MESSANZAHL; n++) {
        Serial.print(t_r0[n][i]);
        Serial.print(",");
      }
      Serial.println(";");
    }
    Serial.println("=================");
    
    Serial.println("Messwerte 30°: ");
    for(int i=0; i<6; i++ ){
      Serial.print("Sensor [" + (String)i + "] =");
      for(int n=0; n<MESSANZAHL; n++) {
        Serial.print(t_r30[n][i]);
        Serial.print(",");
      }
      Serial.println(";");
    }
  #endif


  //4. Schritt
  for(int i=0; i<6; i++) {
    // Temporäre Arrays zur Aufnahme aller Messwerte eines Sensors für beide Messwinkel (0° und 30°)
    long w0[MESSANZAHL];
    long w30[MESSANZAHL];
    
    for(int n=0; n<MESSANZAHL; n++) { 
      // Messwerte für Sensor i aus den Messreihen bei 0° und 30° in temporären Arrays speichern
      w0[n] = t_r0[n][i]; // Messwert n von Sensor i bei 0°
      w30[n] = t_r30[n][i]; // Messwert n von Sensor i bei 30°
    }

    // Median der Messwerte berechnen —> liefert den geeichten Referenzwert für Sensor i bei jeweiligem Winkel
    r0[i] = berechneMedian(w0, MESSANZAHL); //Referenzwert bei 0°
    r30[i] = berechneMedian(w30, MESSANZAHL); //Referenzwert bei 30°
  }

  //Wenn DEV definiert ist werden die geeichten Messwerte auf dem Seriellen Monitor ausgegeben
  #ifdef DEV
  Serial.println("==================");
  Serial.print("Mediane 0°: ");
  for(int i=0; i<6; i++) {
      Serial.print(r0[i]);
      Serial.print(", ");
  } 
  Serial.println("");
  Serial.println("===========================");

  Serial.print("Mediane 30°: ");
  for(int i=0; i<6; i++) {
      Serial.print(r30[i]);
      Serial.print(", ");
  } 
  Serial.println("");
  Serial.println("===========================");
  #endif
}