void setRTCTime(byte hour, byte minute, byte second){
  MCP7940.adjust(DateTime(2019, 1, 1, hour, minute, second));
}

void getRTCTime(byte& hour, byte& minute, byte& second){
  DateTime now = MCP7940.now();
  second = now.second();
  minute = now.minute();
  hour = now.hour();
}

void announceTime(bool silent){
  getRTCTime(rtcHour, rtcMinute, rtcSecond);

    int hourOneCount = rtcHour%10;
    int hourTenCount = (rtcHour-hourOneCount)/10;
    int minuteOneCount = rtcMinute%10;
    int minuteTenCount = (rtcMinute-minuteOneCount)/10;

    delay(400);

    if(silentMode){
      hapticOutput(hourTenCount, false);
      delay(BETWEENDELAY);
      hapticOutput(hourOneCount, false);
      delay(BETWEENDELAY);
      hapticOutput(minuteTenCount, true);
      delay(BETWEENDELAY);
      hapticOutput(minuteOneCount, true);
      clearInputRequest();
    } else {
      stopAudio();
      loadTrack(1);
      loadTrack(2);
      loadTrack(0);
      loadTrack(3);
      playAudio();
    }
}

void setTimeMode(){
  int timeSetStep = 0;
  
  int tempHourOne = rtcHour%10;
  int tempHourTen = (rtcHour-tempHourOne)/10;
  int tempMinOne = rtcMinute%10;
  int tempMinTen = (rtcMinute-tempMinOne)/10;
  
  //In general, pressing the main button will increment the number, and the mode button will increment to the next digit.
  while(timeSetMode){
    analogWrite(DEBUG_LED, 2);
    delay(2);
    digitalWrite(DEBUG_LED, LOW);
    delay(120);
    
    //0: hour 10s, 1: hour 1s, 2: minute 10s, 3: minute 1s, 4: the program sets the new time, plays it back, then returns.
    switch(timeSetStep){
      case 0:
        //Hour Tens
        if(buttonModeWait == true){
          if(++tempHourTen > 2)
            tempHourTen=0;
          buttonModeWait = false;
        }
        if(buttonSetWait == true){
          timeSetStep++;
          delay(100);
          buttonSetWait = false;
        }
      break;
      case 1:
        //Hour Ones
        if(buttonModeWait == true){
          int maxHourOne;
          if(tempHourTen == 2){
            maxHourOne = 3;
            tempHourOne = min(tempHourOne,3);
          }else{
            maxHourOne = 9;
          }
          
          if(++tempHourOne > maxHourOne)
            tempHourOne=0;
   
          buttonModeWait = false;
        }
        if(buttonSetWait == true){
          timeSetStep++;
          delay(100);
          buttonSetWait = false;
        }
      break;
      case 2:
        //Minute Tens
        if(buttonModeWait == true){
          if(++tempMinTen > 5)
            tempMinTen=0;
          buttonModeWait = false;
        }
        if(buttonSetWait == true){
          timeSetStep++;
          delay(100);
          buttonSetWait = false;
        }
      break;
      case 3:
        //Minute Ones
        if(buttonModeWait == true){
          if(++tempMinOne > 9)
            tempMinOne=0;
          buttonModeWait = false;
        }
        if(buttonSetWait == true){
          timeSetStep++;
          buttonSetWait = false;
        }
      break;
      case 4:   
        timeSetMode = false;
        setRTCTime(tempHourTen*10+tempHourOne, tempMinTen*10+tempMinOne, 0);
        delay(200);
        announceTime(silentMode);
    }
    
  }
}
