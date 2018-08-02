#define BUZZPIN 4           //the buzzer (or vibrator mosfet)
#define BUTTONPIN 2         //the main button
#define MODEBUTTONPIN 3     //the mode button

#define SRLATCH 8
#define SRCLK 12
#define SRDATA 11

#define BUTTONTIME 200      //delay between allowed button pressess. TODO: detect rollover

//Beep timings
#define BETWEENDELAY 800    //time to wait between tens and ones etc.
#define NONZEROBEEPTIME 50  //the time to beep for a non-zero number
#define ZEROBEEPTIME 300    //the time to beep for a zero
#define BEEPDELAY 200        //the spacing between beeps of the same group
#define GROUPDELAYADD 40    //the amount of delay between groupings (ie ... ... .. to make 8)

#define enableSerial 0

#include <DS3231.h>
#include <Wire.h>

DS3231 Clock;

bool H12 = false; //24 Hour Mode
byte rtcHour, rtcMinute, rtcSecond;

volatile int lastPress;             //time pulled from millis() of the last button press
volatile bool button1Wait = false;  //triggered from interrupt if main button is pressed
volatile bool button2Wait = false;  //triggered from interrupt if mode button is pressed

bool timeSetMode = false;           //this flag will keep the program in time set mode.

void setup() {
  pinMode(BUZZPIN, OUTPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);
  pinMode(MODEBUTTONPIN, INPUT_PULLUP);
  pinMode(SRLATCH, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(SRDATA, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTONPIN), button1Down, FALLING);
  attachInterrupt(digitalPinToInterrupt(MODEBUTTONPIN), button2Down, FALLING);

  Wire.begin();
  Serial.begin(9600);
  
  Clock.turnOffAlarm(1); //no alarms used, don't want interrupts.
  Clock.turnOffAlarm(2);
  Clock.enableOscillator(true, false, 0); //turn on 1Hz oscillator pin on non-battery mode
  
  //Test All LED Digits
  for(int i = 0; i<=9; i++){
    ledDisplay(i,false);
    delay(50);
  }

  ledClear();
  
  digitalWrite(BUZZPIN, HIGH);
  delay(50);
  digitalWrite(BUZZPIN, LOW);
  delay(100);
  digitalWrite(BUZZPIN, HIGH);
  delay(50);
  digitalWrite(BUZZPIN, LOW);
}

void loop() {
  //if button 1 is pressed, beep the time.
  if(button1Wait){
    beepTime();
    ledClear();
    button1Wait = false;
  }

  //if button 2 is pressed, enter time set mode.
  if(button2Wait){
    timeSetMode = true;
    button2Wait = false;
    setTimeMode();
  }
}

void setRTCTime(byte hour, byte minute, byte second){
  Clock.setClockMode(H12);
  rtcHour = hour;
  rtcMinute = minute;
  rtcSecond = second;

  Serial.print("Time: ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

  Clock.setHour(hour);
  Clock.setMinute(minute);
  Clock.setSecond(second);
}

//Only interested in the hours, minutes, and seconds 
void readRTCTime(byte& hour, byte& minute, byte& second){
  bool bH12, bPM;
  
  second = Clock.getSecond();
  minute = Clock.getMinute();
  hour = Clock.getHour(bH12,bPM);
  Serial.print("Time: ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.print(second);
  if(bH12){
      if(bPM)
        Serial.println(" PM");
      else
        Serial.println(" AM");
  } else {
    Serial.println();
  }
}

//Interrupt Function for the pushbutton
void button1Down(){
  if(millis()-lastPress > BUTTONTIME){
    lastPress = millis();
    button1Wait = true;
  }
}

void button2Down(){
  if(millis()-lastPress > BUTTONTIME){
    lastPress = millis();
    button2Wait = true;
  }
}

void clearButtonRequest(){
  lastPress = 0;
  button1Wait = false;
  button2Wait = false;
}

void buzzCount(int count,bool dp){
  int breakInto = 3;
  ledDisplay(count,dp);

  if(count == 0){
    digitalWrite(BUZZPIN, HIGH);
    delay(ZEROBEEPTIME);
    digitalWrite(BUZZPIN, LOW);
    delay(BEEPDELAY);
  }
    
  for(int i = 1; i<=count; i++){
    digitalWrite(BUZZPIN, HIGH);
    delay(NONZEROBEEPTIME);
    digitalWrite(BUZZPIN, LOW);
    delay(BEEPDELAY);
    if(i%breakInto == 0){
      delay(GROUPDELAYADD);
    }
  }
}

void setTimeMode(){
  int timeSetStep = 0;
  
  int tempHourOne = rtcHour%10;
  int tempHourTen = (rtcHour-tempHourOne)/10;
  int tempMinOne = rtcMinute%10;
  int tempMinTen = (rtcMinute-tempMinOne)/10;
  
  //In general, pressing the main button will increment the number, and the mode button will increment to the next digit.
  ledDisplay(tempHourTen, false);
  while(timeSetMode){
    digitalWrite(BUZZPIN, HIGH);
    delay(2);
    digitalWrite(BUZZPIN, LOW);
    delay(120);
    
    //0: hour 10s, 1: hour 1s, 2: minute 10s, 3: minute 1s, 4: the program sets the new time, plays it back, then returns.
    switch(timeSetStep){
      case 0:
        //Hour Tens
        if(button1Wait == true){
          if(++tempHourTen > 2)
            tempHourTen=0;
          ledDisplay(tempHourTen, false);
          button1Wait = false;
        }
        if(button2Wait == true){
          timeSetStep++;
          ledClear();
          delay(100);
          ledDisplay(tempHourOne, false);
          button2Wait = false;
        }
      break;
      case 1:
        //Hour Ones
        if(button1Wait == true){
          int maxHourOne;
          if(tempHourTen == 2){
            maxHourOne = 3;
            tempHourOne = min(tempHourOne,3);
          }else{
            maxHourOne = 9;
          }
          
          if(++tempHourOne > maxHourOne)
            tempHourOne=0;
   
          ledDisplay(tempHourOne, false);
          button1Wait = false;
        }
        if(button2Wait == true){
          timeSetStep++;
          ledClear();
          delay(100);
          ledDisplay(tempMinTen, true);
          button2Wait = false;
        }
      break;
      case 2:
        //Minute Tens
        if(button1Wait == true){
          if(++tempMinTen > 5)
            tempMinTen=0;
          ledDisplay(tempMinTen, true);
          button1Wait = false;
        }
        if(button2Wait == true){
          timeSetStep++;
          ledClear();
          delay(100);
          ledDisplay(tempMinOne, true);
          button2Wait = false;
        }
      break;
      case 3:
        //Minute Ones
        if(button1Wait == true){
          if(++tempMinOne > 9)
            tempMinOne=0;
          ledDisplay(tempMinOne, true);
          button1Wait = false;
        }
        if(button2Wait == true){
          ledClear();
          timeSetStep++;
          button2Wait = false;
        }
      break;
      case 4: 
        
        timeSetMode = false;
        setRTCTime(tempHourTen*10+tempHourOne, tempMinTen*10+tempMinOne, 0);
        delay(200);
        beepTime();
        ledClear();
    }
    
  }
  clearButtonRequest();
}

void ledDisplay(int number, bool dp){
  byte led[] = {B11111100,B01100000,B11011010,B11110010,B01100110,B10110110,B10111110,B11100000,B11111110,B11110110};
  digitalWrite(SRLATCH,LOW);
  shiftOut(SRDATA, SRCLK, LSBFIRST, led[number]|dp);  
  digitalWrite(SRLATCH,HIGH);
}

void ledClear(){
  digitalWrite(SRLATCH,LOW);
  shiftOut(SRDATA, SRCLK, LSBFIRST, 0);  
  digitalWrite(SRLATCH,HIGH);
}

void beepTime(){
  readRTCTime(rtcHour, rtcMinute, rtcSecond);

    int hourOneCount = rtcHour%10;
    int hourTenCount = (rtcHour-hourOneCount)/10;
    int minuteOneCount = rtcMinute%10;
    int minuteTenCount = (rtcMinute-minuteOneCount)/10;

    buzzCount(hourTenCount, false);
    delay(BETWEENDELAY);
    buzzCount(hourOneCount, false);
    delay(BETWEENDELAY);
    buzzCount(minuteTenCount, true);
    delay(BETWEENDELAY);
    buzzCount(minuteOneCount, true);
    clearButtonRequest();
}

