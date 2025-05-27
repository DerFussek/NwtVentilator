void auswertenUndDrehen() {
  const int MAX_ENTFERNUNG = 150;
  long t_r0[6], t_r30[6];
  
    for(int i=0; i<6; i++) {
      long t_w[3];
      int n = 0;

      for(int n=0; n<3; n++) {
        t_w[n] = messung(sensoren[i].trig, sensoren[i].echo, MAX_ENTFERNUNG);
      }
      
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

      for(int n=0; n<3; n++) {
        t_w[n] = messung(sensoren[i].trig, sensoren[i].echo, MAX_ENTFERNUNG);
      }
      
      t_r30[i] = berechneMedian(t_w, 3);
      delay(10);
    }

    justiere30(t_r30);

    //Drehplatte um -30° drehen
    stepper.move(120);
    delay(100);
}

void justiere0(long t_r0[6]) {
  long delta[6];

  for(int i=0; i<6; i++) {
    delta[i] = r0[i] - t_r0[i];
    if(delta[i] < 20) delta[i] = 0;
  }

  int maxDelta = 0;
  int maxDeltaSensor = 0;
  for(int i=0; i<6; i++) {
    if(delta[i] > maxDelta) {
      maxDelta = delta[i];
      maxDeltaSensor = i;
    } else {
      continue;
    }
  }

  int ziel = maxDeltaSensor * 60;
  #ifdef DEV
  Serial.println("Ziel bei " + (String)ziel + "° ; Delta = " + (String) maxDelta);
  #endif
  movestepper(ziel);
}

void justiere30(long t_r30[6]) {
  long delta[6];

  for(int i=0; i<6; i++) {
    delta[i] = r30[i] - t_r30[i];
    if(delta[i] < 10) delta[i] = 0;
  }

  int maxDelta = 0;
  int maxDeltaSensor = 0;
  for(int i=0; i<6; i++) {
    if(delta[i] > maxDelta) {
      maxDelta = delta[i];
      maxDeltaSensor = i;
    } else {
      continue;
    }
  }

  int ziel = maxDeltaSensor * 60 + 30;
  #ifdef DEV
  Serial.println("Zeil bei " + (String)ziel + "° ; Delta = " + (String) maxDelta);
  #endif
  movestepper(ziel);
}

void movestepper(int ziel) {
  int delta = ziel - position;

  // Schrittmotor ansteuern
  stepper2.getStepper().enable();
  stepper2.getStepper().rotate(delta * -1);  // oder z. B. moveDegrees(delta); falls du so eine Methode hast

  // Position aktualisieren (Modulo 360, falls du willst)
  position = (position + delta);
  if (position < 0) position += 360;  // Immer positiv halten
}