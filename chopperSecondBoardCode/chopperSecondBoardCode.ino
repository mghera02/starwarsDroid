////////////LIBRARIES////////////

//for sd card reader
#include "SD.h"
//for audio
#include "TMRpcm.h"
#include "SPI.h"
//ir
#include "IRremote.h"
//for temp and humidity
#include <dht.h>

////////////VARIABLES////////////
//for rgb led
#define BLUE 9 
#define GREEN 5
#define RED 6
#define delayTime 10 // fading time between colors
//for sd card reader
#define SD_ChipSelectPin 3
//for temp and humidity
#define DHT_SENSOR_TYPE DHT_TYPE_11
#define DHT11_PIN A1

//8 leds using shift register
int latchPin = A3;      // (11) ST_CP [RCK] on 74HC595
int clockPin = A4;      // (9) SH_CP [SCK] on 74HC595
int dataPin = A2;     // (12) DS [S1] on 74HC595
byte leds = 0;
//for keeping track of time for events
long lastMillis;
int rot=1;
//0 is no sound, 1 is only chopper sound, 2 is everything 
int speakerMode=0;
//Signal Pin of IR receiver
int receiver = 2;
//rgb led current colors
String choice="purpleRed";
//for dht
float temperature;
float humidity;

////////////DECLARE OBJECTS////////////
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
TMRpcm tmrpcm;               // create instance for music player
dht DHT;                     // create instance for dht

void setup()
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  tmrpcm.speakerPin=10;
  
  Serial.begin(9600);
  
  irrecv.enableIRIn();
  
  if(!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }

  //8 CHARACTER LIMIT FOR THE AUDIO FILE NAMES
  tmrpcm.setVolume(5);
  startUpEvent();
}

void loop() {
  //gets rid of high pitched sound from speaker when not playing
  digitalWrite(10, LOW);
  
  //for IR receiver
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  

  //changes rgb led colors
  fadeOut(choice);

  //for calling events every 2 mins
  if (millis() - lastMillis >= 2*60*1000UL) 
  {
   digitalWrite(10, HIGH);
   lastMillis = millis();  //get ready for the next iteration
   switch(rot) {
      case 1 :
        if(speakerMode>1){
          discoEvent(); 
        }
        break;
      case 2 :
        if(speakerMode>0){
          regEvent();
        }
        break;
      case 3 :
        if(speakerMode>0){
          reg2Event();
        }
        break;
      case 4 :
        if(speakerMode>0){
         reg3Event();
        }
        break;
      case 5 :
        if(speakerMode>0){
          reg4Event();
        }
        break;
     case 6 :
        if(speakerMode>1){
         kananEvent();
        }
        break;
     case 7 :
        if(speakerMode>1){
          alertEvent();
        }
        break;
     case 8 :
        if(speakerMode>0){
          woohooEvent();
        }
        break;
   }
   
   if(rot<8){
    rot++;
   }else{
    rot=1;
   }
  }

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  int chk = DHT.read11(DHT11_PIN);
  //Serial.print("Temperature = ");
  //Serial.println(DHT.temperature*9/5+32);
  //Serial.print("Humidity = ");
  //Serial.println(DHT.humidity);
  delay(500);
  if(DHT.temperature*9/5+32<70){
    choice="greenBlue";
  }else if(DHT.temperature*9/5+32<75){
    choice="blueRed";
  }else if(DHT.temperature*9/5+32<80){
    choice="redGreen";
  }else if(DHT.temperature*9/5+32<90){
    choice="purpleRed";
  }
}



////////////FUNCTIONS////////////

//EVENTS START
// Each event plays randomly and plays a sound and changes the shift register leds
// 0,1 is blue
// 2,3 is green
// 4,5 is yellow
// 6,7 is red
void startUpEvent(){
  tmrpcm.setVolume(5);
  //8 CHARACTER LIMIT FOR THE FILE NAMES
  tmrpcm.play("cs6.wav");
  //for 8 leds
  leds = 0;
  updateShiftRegister();
  int tDelay=100;
  delay(tDelay);
  for (int i = 8; i > -1; i--)
  {
    bitSet(leds, i);
    updateShiftRegister();
    delay(tDelay);
  }
  leds = 0;
  for (int i = -1; i < 8; i++)
  {
    bitSet(leds, i);
    updateShiftRegister();
    delay(tDelay);
  }
  delay(2500);
  leds = 0;
  updateShiftRegister();
}

void woohooEvent(){
  tmrpcm.setVolume(5);
  //8 CHARACTER LIMIT FOR THE FILE NAMES
  tmrpcm.play("cs.wav",7);
  //for 8 leds
  leds = 0;
  updateShiftRegister();
  int tDelay=70;
  delay(tDelay);
  for (int i = 8; i > 3; i--)
  {
    bitSet(leds, i);
    updateShiftRegister();
    delay(tDelay);
  }
  leds = 0;
  for (int i = 3; i < 8; i++)
  {
    bitSet(leds, i);
    updateShiftRegister();
    delay(tDelay);
  }
  delay(2500);
  tmrpcm.stopPlayback();
  leds = 0;
  updateShiftRegister();
}

void regEvent(){
  tmrpcm.setVolume(5);
  //8 CHARACTER LIMIT FOR THE FILE NAMES
  tmrpcm.play("cs.wav",0);
  //for 8 leds
  leds = 0;
  updateShiftRegister();
  int tDelay=150;
  delay(tDelay);
  for(int i = 0; i < 8; i++){
    if(i%2!=0){
      bitSet(leds, i);
      updateShiftRegister();
      delay(tDelay); 
    }
  }
  leds = 0;
  updateShiftRegister();
  for(int i = 8; i > 0; i--){
    if(i%2!=0){
      bitSet(leds, i);
      updateShiftRegister();
      delay(tDelay); 
    }
  }
  delay(1500);
  tmrpcm.stopPlayback();
  leds = 0;
  updateShiftRegister();
}

void reg2Event(){
  tmrpcm.setVolume(5);
  //8 CHARACTER LIMIT FOR THE FILE NAMES
  tmrpcm.play("cs.wav",3);
  //for 8 leds
  leds = 0;
  updateShiftRegister();
  int tDelay=150;
  delay(tDelay);
  for(int i = 0; i < 8; i++){
    if(i%2==0){
      bitSet(leds, i);
      updateShiftRegister();
      delay(tDelay); 
    }
  }
  leds = 0;
  updateShiftRegister();
  for(int i = 8; i >= 0; i--){
    if(i%2==0){
      bitSet(leds, i);
      updateShiftRegister();
      delay(tDelay); 
    }
  }
  delay(2000);
  tmrpcm.stopPlayback();
  leds = 0;
  updateShiftRegister();
}

void reg3Event(){
  tmrpcm.setVolume(5);
  //8 CHARACTER LIMIT FOR THE FILE NAMES
  tmrpcm.play("cs.wav",10);
  //for 8 leds
  leds = 0;
  updateShiftRegister();
  int tDelay=200;
  delay(tDelay);
  for(int i = 0; i < 15; i++){
    if(i%2==0){
      bitSet(leds, 3);
      updateShiftRegister();
      delay(tDelay);  
    }else{
      leds = 0;
      bitSet(leds, -1);
      updateShiftRegister();
      delay(tDelay);  
    }
  }
  delay(1000);
  tmrpcm.stopPlayback();
  leds = 0;
  updateShiftRegister();
}

void reg4Event(){
  tmrpcm.setVolume(5);
  //8 CHARACTER LIMIT FOR THE FILE NAMES
  tmrpcm.play("cs.wav",14);
  //for 8 leds
  leds = 0;
  updateShiftRegister();
  int tDelay=200;
  delay(tDelay);
  for(int i = 0; i < 15; i++){
    if(i%2==0){
      bitSet(leds, 1);
      bitSet(leds, 5);
      updateShiftRegister();
      delay(tDelay);  
    }else{
      leds = 0;
      bitSet(leds, -1);
      updateShiftRegister();
      delay(tDelay);  
    }
  }
  delay(2500);
  tmrpcm.stopPlayback();
  leds = 0;
  updateShiftRegister();
}

void kananEvent(){
  tmrpcm.setVolume(5);
  //8 CHARACTER LIMIT FOR THE FILE NAMES
  tmrpcm.play("kan.wav");
  //for 8 leds
  leds = 0;
  updateShiftRegister();
  int tDelay=200;
  delay(tDelay);
  bitSet(leds, 0);
  bitSet(leds, 1);
  updateShiftRegister();
  delay(380000);
  leds = 0;
  updateShiftRegister();
}

void alertEvent(){
  tmrpcm.setVolume(5);
  //8 CHARACTER LIMIT FOR THE FILE NAMES
  tmrpcm.play("mp2.wav");
  //for 8 leds
  leds = 0;
  updateShiftRegister();
  int tDelay=200;
  delay(tDelay);
  for(int i = 0; i < 90; i++){
    if(i%2==0){
      bitSet(leds, 6);
      updateShiftRegister();
      delay(tDelay);  
    }else{
      leds = 0;
      updateShiftRegister();
      bitSet(leds, 7);
      updateShiftRegister();
      delay(tDelay); 
      leds = 0;
      updateShiftRegister(); 
    }
  }
  fadeOut("redGreen");
  delay(20000);
  leds = 0;
  updateShiftRegister();
}

void discoEvent(){
  tmrpcm.setVolume(5);
  //8 CHARACTER LIMIT FOR THE FILE NAMES
  tmrpcm.play("disco.wav");
  //for 8 leds
  leds = 0;
  updateShiftRegister();
  int tDelay=200;
  delay(tDelay);
  int randNumber;
  for(int i = 0; i < 900; i++){
    randNumber = random(8);
    if(i%2==0){
      bitSet(leds, randNumber);
      updateShiftRegister();
      delay(tDelay);  
    }else{
      leds = 0;
      updateShiftRegister();
      bitSet(leds, randNumber);
      updateShiftRegister();
      delay(tDelay); 
      leds = 0;
      updateShiftRegister(); 
    }
  }
  delay(290000);
  leds = 0;
  updateShiftRegister();
}

//EVENTS END

void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}

// For constantly changing the shift register and RGB LED random colors based on the temp
void fadeOut(String c)
{
  int randNumber;
  randNumber = random(4);
  Serial.print(randNumber);
  leds = 0;
  updateShiftRegister();
  #define delayTime 7.5 // fading time between colors
  if(c == "redGreen"){
    switch(randNumber){
        case 0:
          bitSet(leds, 2);
          break;
        case 1:
          bitSet(leds, 3);
          break;
        case 2:
          bitSet(leds, 6);
          break;
        case 3:
          bitSet(leds, 7);
          break;
    }
    updateShiftRegister();
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
      // fades out red bring green full when i=255
      for (int i = 0; i < 510; i += 1)
    {
      if(i<255){
        analogWrite(RED, 255-i);
        analogWrite(GREEN, i);
        delay(delayTime);
      }else{
        analogWrite(RED, i-254);
        analogWrite(GREEN, 255-i+255);
        delay(delayTime);
      }
    }
  }
  else if(c=="greenBlue"){
    switch(randNumber){
        case 0:
          bitSet(leds, 0);
          break;
        case 1:
          bitSet(leds, 1);
          break;
        case 2:
          bitSet(leds, 2);
          break;
        case 3:
          bitSet(leds, 3);
          break;
      }
      updateShiftRegister();
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
    // fades out green bring blue full when i=255
    for (int i = 0; i < 510; i += 1)
    {
      if(i<255){
        analogWrite(GREEN, 255-i);
        analogWrite(BLUE, i);
        delay(delayTime);
      }else{
        analogWrite(GREEN, i-254);
        analogWrite(BLUE, 255-i+255);
        delay(delayTime);
      }
    }
  }  
  else if(c=="blueRed"){
    switch(randNumber){
        case 0:
          bitSet(leds, 0);
          break;
        case 1:
          bitSet(leds, 1);
          break;
        case 2:
          bitSet(leds, 6);
          break;
        case 3:
          bitSet(leds, 7);
          break;
      }
      updateShiftRegister();
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
    // fades out blue bring red full when i=255
    for (int i = 0; i < 510; i += 1)
    {
      if(i<255){
        analogWrite(BLUE, 255-i);
        analogWrite(RED, i);
        delay(delayTime);
      }else{
        analogWrite(BLUE, i-254);
        analogWrite(RED, 255-i+255);
        delay(delayTime);
      }
    }
  }
  else if(c=="purpleRed"){
    switch(randNumber){
        case 0:
          bitSet(leds, 0);
          break;
        case 1:
          bitSet(leds, 1);
          break;
        case 2:
          bitSet(leds, 6);
          break;
        case 3:
          bitSet(leds, 7);
          break;
      }
      updateShiftRegister();
    //Serial.print("here");
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
    // fades out red bring purple full when i=255
    for (int i = 0; i < 320; i += 1)
    {
      if(i<127){
        analogWrite(RED, 254-i);
        analogWrite(BLUE, i);
        delay(delayTime);
      }else if(i<254){
        analogWrite(BLUE, 254-i);
        analogWrite(RED, i);
        delay(delayTime);
      }else{
        analogWrite(RED, 254);
      }
    }
  }
}

void translateIR() // takes action based on IR code received
// describing Remote IR codes 
{

  switch(results.value)

  {
  case 0xFFA25D: Serial.println("POWER"); break;
  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
  case 0xFF629D:
    Serial.println("VOL+");
    //increments speaker mode
    int currMode;
    if(speakerMode<2){
      speakerMode++;
    }else{
      speakerMode=0;
    }
    // displays speaker mode on shift register leds
    switch(speakerMode) {
      case 0 :
        currMode=7;
        break;
      case 1 :
        currMode=4;
        break;
      case 2 :
        currMode=2;
        break;
      default:
        break;
    }
    leds = 0;
    updateShiftRegister();
    bitSet(leds, currMode);
    updateShiftRegister();
    delay(1500);
    leds = 0;
    updateShiftRegister();
    break;
  case 0xFF22DD: Serial.println("FAST BACK");    break;
  case 0xFF02FD: Serial.println("PAUSE");    break;
  case 0xFFC23D:
    Serial.println("FAST FORWARD");
    discoEvent();
    break;
  case 0xFFE01F: Serial.println("DOWN");    break;
  case 0xFFA857: Serial.println("VOL-");    break;
  case 0xFF906F: Serial.println("UP");    break;
  case 0xFF9867: Serial.println("EQ");    break;
  case 0xFFB04F:
    Serial.println("ST/REPT");
    alertEvent();
    break;
  case 0xFF6897:
    Serial.println("0");
    
    break;
  case 0xFF30CF:
    Serial.println("1");
    
    break;
  case 0xFF18E7:
    Serial.println("2");
    
    break;
  case 0xFF7A85: 
    Serial.println("3");
    
    break;
  case 0xFF10EF: 
    Serial.println("4");
    
    break;
  case 0xFF38C7: 
    Serial.println("5");
    
    break;
  case 0xFF5AA5: 
    Serial.println("6");

    break;
  case 0xFF42BD: 
    Serial.println("7");
    
    break;
  case 0xFF4AB5:
    Serial.println("8");
    
    break;
  case 0xFF52AD:
    Serial.println("9");
    
    break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");break;  

  default: 
    Serial.println(" other button : ");
    Serial.println(results.value);

  }// End Case

  delay(500); // Do not get immediate repeat

}
