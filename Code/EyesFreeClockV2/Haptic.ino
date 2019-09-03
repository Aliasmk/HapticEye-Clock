void hapticOutput(int count,bool dp){
  int breakInto = 3;
  int shortHapticEffect = 7;
  int longHapticEffect = 13;

  if(count == 0){
    drv.setWaveform(0, longHapticEffect);
    drv.setWaveform(1,0);
    
    digitalWrite(DEBUG_LED, HIGH);
    drv.go();
    delay(ZEROBEEPTIME);
    digitalWrite(DEBUG_LED, LOW);
    delay(BEEPDELAY);
  }
    
  drv.setWaveform(0, shortHapticEffect);
  drv.setWaveform(1,0);
  
  for(int i = 1; i<=count; i++){
    digitalWrite(DEBUG_LED, HIGH);
    drv.go();
    delay(NONZEROBEEPTIME);
    digitalWrite(DEBUG_LED, LOW);
    delay(BEEPDELAY);
    if(i%breakInto == 0){
      delay(GROUPDELAYADD);
    }
  }
}
