//www.elegoo.com
//2016.12.08
#include <Servo.h>
#include "IRremote.h"

int pos = 60;    // variable to store the servo position
int receiver = A5; // Signal Pin of IR receiver
const int trigPin = A1; // variable for ultrasonic sensor
const int echoPin = A0; // variable for ultrasonic sensor

unsigned long timeNow=0;

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
Servo headMotor;  // create servo object to control a servo
Servo wheel1;
Servo wheel2;
Servo armMotor;
int wheel1Speed = 100;
int wheel2Speed = 100;
long duration;
int distance;
bool autMode = false;
bool moving = false;
int backUpNum = 0;

// Variables for movement during disco event
bool danceToggle = false;
bool firstDance = true;
long beginningDance;
int dancePhase = 1;
int danceCounter = 0;

// Variables for surveillance mode
bool surveillanceMode = false;
bool finishedMovingHead = true;

void setup() {
  Serial.begin(9600);
  headMotor.attach(9);  // attaches the servo on pin 9 to the servo object
  armMotor.attach(3);
  armMotor.write(180);
  armMotor.detach();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  //ir receiver
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn();
  
  head(0);
}

void loop() {   
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance = duration * 0.034 / 2;
  
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance);
  //delay(100);

  if(surveillanceMode){
    if(finishedMovingHead){
      head(1);
    }
  }

  // Do motion twenty times if button has been pressed to move during disco event
  if(danceToggle && danceCounter < 20){
    if(firstDance){
      firstDance = false;
      beginningDance = millis();
      dancePhase = 1;
      danceCounter++;
    }
    if(millis() - beginningDance >= 1000 && millis() - beginningDance < 3000){
      if(dancePhase == 1){
        backwardsAll();
        dancePhase = 2; 
      }
    }else if(millis() - beginningDance >= 3000 && millis() - beginningDance < 5000){
      if(dancePhase == 2){
        forwardAll();
        dancePhase = 3;
      }
    }else if(millis() - beginningDance >= 5000 && millis() - beginningDance <= 7000){
      stopAll();
      if(dancePhase == 3){
        head(0);
        dancePhase = 4;
      }
    }else if(millis() - beginningDance > 7000 && dancePhase == 4){
      dancePhase = 1;
      firstDance = true;
    }
  }else{
    danceToggle = false;
  }

 if(distance > 40 && distance < 2000 && autMode){
    //go forward
    forwardAll();
  }else if(!(distance > 40 && distance < 2000) && autMode){
    stopAll();
    if(backUpNum == 0){
      wheel1.attach(11);
      wheel1Speed = 50;
      wheel2.attach(10);
      wheel2Speed = 125;
      backUpNum++;
    }else{
      stopAll();
      wheel1.attach(11);
      wheel1Speed = 125;
      backUpNum++;
      if(backUpNum > 5){
        backUpNum = 0;
      }
    }
  }else if((!autMode || distance > 2000) && !moving){
    //stop
    stopAll();
  }
  
  //for IR receiver
  if(irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
  // send data only when you receive data:
  wheel1.write(wheel1Speed);
  wheel2.write(wheel2Speed);
}

/*-----( Function )-----*/
void head(bool surveillance){
  for (pos = 60; pos <= 120; pos += 1) { // goes from 60 degrees to 120 degrees
    // in steps of 1 degree
    headMotor.write(pos);            // tell servo to go to position in variable 'pos'
    delay(15);                       // wait for the servo to reach the position
  }
  for (pos = 120; pos >= 0; pos -= 1) { // goes from 120 degrees to 0 degrees
    headMotor.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // wait for the servo to reach the position
  }
  for (pos = 0; pos <= 60; pos += 1) { // goes from 0 degrees to 60 degrees
    headMotor.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // wait for the servo to reach the position
  }
}

void arm(){
  armMotor.attach(3);
  for (pos = 180; pos >= 100; pos -= 2) { // goes from 180 degrees to 100 degrees
    armMotor.write(pos);              // tell servo to go to position in variable 'pos'
    delay(25);                       // waits 25ms for the servo to reach the position
  }
  delay(2000);
  for (pos = 130; pos <= 100; pos += 2) { // goes from 130 degrees to 100 degrees
    armMotor.write(pos);              // tell servo to go to position in variable 'pos'
    delay(25);                       // waits 15ms for the servo to reach the position
  }
  armMotor.detach();
}

void forwardAll(){
  wheel1.attach(11);
  wheel1Speed = 125;
  wheel2.attach(10);
  wheel2Speed = 50;
  moving = true;
}

void stopAll(){
  wheel2.detach();
  wheel2Speed = 90;
  wheel1.detach();
  wheel1Speed = 90;
  moving = false;
}

void backwardsAll(){
  wheel1.attach(11);
  wheel1Speed = 50;
  wheel2.attach(10);
  wheel2Speed = 125;
  moving = true;
}

void translateIR() // takes action based on IR code received
// describing Remote IR codes 
{
  switch(results.value)
  {
  case 0xFFA25D:
    Serial.println("POWER"); 
    if(surveillanceMode){
      surveillanceMode = false;
    }else{
      surveillanceMode = true;
    }
    break;
  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
  case 0xFF629D: Serial.println("VOL+"); break;
  case 0xFF22DD: Serial.println("FAST BACK");    break;
  case 0xFF02FD:
    Serial.println("PAUSE");
    danceToggle = true;
    danceCounter = 0;
    break;
  case 0xFFC23D: Serial.println("FAST FORWARD");   break;
  case 0xFFE01F: Serial.println("DOWN");    break;
  case 0xFFA857: Serial.println("VOL-");    break;
  case 0xFF906F: Serial.println("UP");    break;
  case 0xFF9867:
    Serial.println("EQ");
    arm();
    break;
  case 0xFFB04F:
    Serial.println("ST/REPT");
 
    break;
  case 0xFF6897:
    //Move head around 120 deg and end in center
    Serial.println("0");
    head(0);
    break;
  case 0xFF30CF:
    //Forward Left
    Serial.println("1");
    wheel1.attach(11);
    wheel1Speed = 125;
    moving = true;
    break;
  case 0xFF18E7:
    //Forward All
    Serial.println("2");
    forwardAll();
    break;
  case 0xFF7A85: 
    //Forward Right 
    Serial.println("3");
    wheel2.attach(10);
    wheel2Speed = 50;
    moving = true;
    break;
  case 0xFF10EF: 
    //toggle autMode
    Serial.println("4");
    if(autMode){
      autMode = false;
    }else{
      autMode = true;
    }
    break;
  case 0xFF38C7: 
    //Stop all
    Serial.println("5");
    stopAll();
    break;
  case 0xFF5AA5: 
    break;
  case 0xFF42BD: 
    //Backwards Left
    Serial.println("7");
    wheel1.attach(11);
    wheel1Speed = 50;
    moving = true;
    break;
  case 0xFF4AB5:
    //Backwards All
    Serial.println("8");
    backwardsAll();
    break;
  case 0xFF52AD:
    //Backwards Right
    Serial.println("9");
    wheel2.attach(10);
    wheel2Speed = 125;
    moving = true;
    break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");break;  

  default: 
    Serial.println(" other button : ");
    Serial.println(results.value);
  }// End Case
  delay(500); // Do not get immediate repeat
}
