// Median aus einem Array von long-Werten berechnen
long berechneMedian(const long werte[], int anzahl) {
  // 1) Werte kopieren
  long temp[anzahl];
  for (int i = 0; i < anzahl; i++) {
    temp[i] = werte[i];
  }

  // 2) Selection Sort
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
    // gerade Anzahl —> Durchschnitt der beiden mittleren Werte (integer division)
    int m = anzahl / 2;
    return (temp[m - 1] + temp[m]) / 2;
  }
}


void kalibiere(const int MESSANZAHL) {
  const int MAX_ENTFERNUNG = 150;
  long t_r0[MESSANZAHL][6], t_r30[MESSANZAHL][6];
  
  for(int n = 0; n < MESSANZAHL; n++) {
    for(int i=0; i<6; i++) {
      long var;
      var = messung(sensoren[i].trig, sensoren[i].echo, MAX_ENTFERNUNG);

      t_r0[n][i] = var;

      delay(10);
    }

    //Drehplatte um +30° drehen
    stepper.move(-120);
    delay(100);
    
    for(int i=0; i<6; i++) {
      t_r30[n][i] = messung(sensoren[i].trig, sensoren[i].echo, MAX_ENTFERNUNG);
      delay(10);
    }

    //Drehplatte um -30° drehen
    stepper.move(120);
    delay(100);
  }

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

  for(int i=0; i<6; i++) {
    long w0[MESSANZAHL];
    long w30[MESSANZAHL];
    for(int n=0; n<MESSANZAHL; n++) {
      w0[n] = t_r0[n][i];
      w30[n] = t_r30[n][i];
    }
    r0[i] = berechneMedian(w0, MESSANZAHL);
    r30[i] = berechneMedian(w30, MESSANZAHL);
  }

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