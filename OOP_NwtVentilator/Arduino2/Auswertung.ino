const int HYSTERESE = 15;
const int COUNTER = 1;
void auswertenUndDrehen() {
  const int MAX_ENTFERNUNG = 150;
  long t_r0[6], t_r30[6];
  
    for(int i=0; i<6; i++) {
      long t_w[3];
      int n = 0;

      Serial.print("Sensor [" + (String)i + "] = ");
      for(int n=0; n<3; n++) {
        t_w[n] = messung(sensoren[i].trig, sensoren[i].echo, MAX_ENTFERNUNG);
        Serial.print(t_w[n]);
        Serial.print(",");
      }
      Serial.println("");
      
      t_r0[i] = berechneMedian(t_w, 3);
      delay(10);
    }

    justiere0(t_r0);


    //Drehplatte um +30° drehen
    stepper.move(-120);
    delay(100);
    
    for(int i=0; i<6; i++) {
      long t_w[3];
      int n = 0;
     Serial.print("Sensor [" + (String)i+0.5 + "] = ");
      for(int n=0; n<3; n++) {
        t_w[n] = messung(sensoren[i].trig, sensoren[i].echo, MAX_ENTFERNUNG);
        Serial.print(t_w[n]);
        Serial.print(",");
      }
      Serial.println("");
      
      t_r30[i] = berechneMedian(t_w, 3);
      delay(10);
    }

    justiere30(t_r30);

    //Drehplatte um -30° drehen
    stepper.move(120);
    delay(100);
}

void justiere0(long t_r0[6]) {
  static long c_r0[6];
  long delta[6];
  
  for(int i=0; i<6; i++) {
    delta[i] = r0[i] - t_r0[i];
    if(delta[i] < HYSTERESE) delta[i] = 0;
  }

  int maxDelta = 0;
  int maxDeltaSensor = 0;
  for(int i=0; i<6; i++) {
    if(delta[i] > maxDelta) {
      maxDelta = delta[i];
      if(maxDelta != 0) maxDeltaSensor = i;
    } else {
      continue;
    }
  }

  if(maxDelta != 0) {
    c_r0[maxDeltaSensor]++;

    for(int i=0; i<6; i++) {
      if(i == maxDeltaSensor) continue;
      c_r0[i] = 0;
    }

    if(c_r0[maxDeltaSensor] >= COUNTER) {
      int ziel = maxDeltaSensor * 60;
      stepper2.movestepper(ziel);

      #ifdef DEV
      Serial.println("Ziel bei " + (String)ziel + "° ; Delta = " + (String) maxDelta);
      #endif
    }
  }
  
}

void justiere30(long t_r30[6]) {
  static long c_r30[6];
  long delta[6];

  for(int i=0; i<6; i++) {
    delta[i] = r30[i] - t_r30[i];
    if(delta[i] < HYSTERESE) delta[i] = 0;
  }

  int maxDelta = 0;
  int maxDeltaSensor = 0;
  for(int i=0; i<6; i++) {
    if(delta[i] > maxDelta) {
      maxDelta = delta[i];
      if(maxDelta != 0) maxDeltaSensor = i;
    } else {
      continue;
    }
  }
  if(maxDelta != 0) {
    c_r30[maxDeltaSensor]++;
    for(int i=0; i<6; i++) {
      if(i == maxDeltaSensor) continue;
      c_r30[i] = 0;
    }

    if(c_r30[maxDeltaSensor] >= COUNTER) {
      int ziel = maxDeltaSensor * 60 + 30;
      if(maxDeltaSensor == 5) ziel = -30;
      
      stepper2.movestepper(ziel);
      
      #ifdef DEV
      Serial.println("Ziel bei " + (String)ziel + "° ; Delta = " + (String) maxDelta);
      #endif
    }
  }
}

