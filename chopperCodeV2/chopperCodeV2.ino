//www.elegoo.com
//2016.12.08
#include <Servo.h>
#include "IRremote.h"

int pos = 60;    // variable to store the servo position
int receiver = A5; // Signal Pin of IR receiver

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
Servo headMotor;  // create servo object to control a servo
Servo wheel1;
Servo wheel2;
int wheel1Speed=100;
int wheel2Speed=100;

void setup() {
  Serial.begin(9600);
  headMotor.attach(9);  // attaches the servo on pin 9 to the servo object
  
 
  //ir receiver
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn();
}

void loop() {
  //for IR receiver
  if (irrecv.decode(&results)) // have we received an IR signal?

  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
 // send data only when you receive data:

  wheel1.write(wheel1Speed);
  wheel2.write(wheel2Speed);
              
}

/*-----( Function )-----*/
void head(){
  for (pos = 60; pos <= 120; pos += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    headMotor.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 120; pos >= 0; pos -= 2) { // goes from 180 degrees to 0 degrees
    headMotor.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 0; pos <= 60; pos += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    headMotor.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}


void translateIR() // takes action based on IR code received

// describing Remote IR codes 

{

  switch(results.value)

  {
  case 0xFFA25D: Serial.println("POWER"); break;
  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
  case 0xFF629D: Serial.println("VOL+"); break;
  case 0xFF22DD: Serial.println("FAST BACK");    break;
  case 0xFF02FD: Serial.println("PAUSE");    break;
  case 0xFFC23D: Serial.println("FAST FORWARD");   break;
  case 0xFFE01F: Serial.println("DOWN");    break;
  case 0xFFA857: Serial.println("VOL-");    break;
  case 0xFF906F: Serial.println("UP");    break;
  case 0xFF9867: Serial.println("EQ");    break;
  case 0xFFB04F: Serial.println("ST/REPT");    break;
  case 0xFF6897:
    Serial.println("0");
    head();
    break;
  case 0xFF30CF:
    Serial.println("1");
    wheel1.attach(11);
    wheel1Speed=50;
    break;
  case 0xFF18E7:
    Serial.println("2");
    wheel1.detach();
    wheel1Speed=90;
    break;
  case 0xFF7A85: 
    Serial.println("3");
    wheel1.attach(11);
    wheel1Speed=125;
    break;
  case 0xFF10EF: 
    Serial.println("4");
    wheel2.attach(10);
    wheel2Speed=50;
    break;
  case 0xFF38C7: 
    Serial.println("5");
    wheel2.detach();
    wheel2Speed=90;
    break;
  case 0xFF5AA5: 
    Serial.println("6");
    wheel2.attach(10);
    wheel2Speed=125;
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


} //END translateIR
