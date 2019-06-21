//New Timer Button Poll Interrupt
ISR(TIMER1_COMPA_vect){
   //Polls every 20ms for new input on button pins
   checkInputWait = true;
   ioCheckCounter++;

   //Every 100th check, also check for IR
   if(ioCheckCounter > 100){
      ioCheckCounter = 0;
      //checkIRWait = true;
   }
}

void checkInput(){
   //Check Buttons 
   //TODO speed up using port addressing rather than digitalRead
   bool stateButtonMode = digitalRead(MODE_BUTTON);
   bool stateButtonSet = digitalRead(SET_BUTTON);
   //compare with last seen state. Do not trigger again until at least one interrupt 
   //is recorded with the buttons down.
   if(stateButtonMode != buttonModeLast && stateButtonMode == LOW)
      buttonModeWait = true;

   if(stateButtonSet != buttonSetLast&& stateButtonSet == LOW)
      buttonSetWait = true;

   buttonModeLast = stateButtonMode;
   buttonSetLast = stateButtonSet;

  //Check IR Sensor
  if(checkIRWait == true){
    //bool initialIRSensorState = digitalRead(IR_DETECT);
    //delay(5);
    digitalWrite(IR_LED, HIGH);
    delay(5);
    bool stateIRSensor = digitalRead(IR_DETECT);
    delay(5);
    digitalWrite(IR_LED, LOW);

    if(stateIRSensor != irSensorLast && stateIRSensor == LOW){
      //User has just covered IR Sensor
      irTimeMillis = millis();
    } else if(stateIRSensor == irSensorLast && stateIRSensor == LOW){
      //User still has their hand on the IR sensor
      long irTimeElapsed = millis() - irTimeMillis;
      if(irTimeElapsed > IRTIMELONG){
        longWaveWait = true;
      }
    } else if(stateIRSensor != irSensorLast && stateIRSensor == HIGH){
      //User has removed their hand from the sensor
      long irTimeElapsed = millis() - irTimeMillis;
      if(irTimeElapsed > IRTIMESHORT){
        shortWaveWait = true;
      }
    }  
    
    checkIRWait = false;
  }
  
  checkInputWait = false;
}

void clearInputRequest(){
  buttonModeWait = false;
  buttonSetWait = false;
  longWaveWait = false;
  shortWaveWait = false;
}
