// Hacking tool using ATTiny85 for windows computers
#include "DigiKeyboard.h"
void setup() {
  #define KEY_TAB 43
  #define KEY_ARROW_RIGHT 0x4F
  #define KEY_ARROW_DOWN 0x51
  #define KEY_SPACE 0x2C 
}

void loop() {
  // Opens settings and makes cursor large, inverts the colors, and makes the sound at the maximum volume
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(300);
  DigiKeyboard.print("ms-settings:home");
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_ARROW_RIGHT);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_ARROW_DOWN);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_ARROW_DOWN);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(300);
  for(int i=0;i<16;i++){
    DigiKeyboard.sendKeyStroke(KEY_ARROW_RIGHT);
  }
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(200);
  for(int i=0;i<8;i++){
    DigiKeyboard.sendKeyStroke(KEY_TAB);
  }
  for(int i=0;i<5;i++){
    DigiKeyboard.sendKeyStroke(KEY_ARROW_DOWN);
  }
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.sendKeyStroke(KEY_SPACE);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_SPACE);   
  DigiKeyboard.delay(300);
  for(int i=0;i<10;i++){
    DigiKeyboard.sendKeyStroke(KEY_TAB);
  }
  for(int i=0;i<8;i++){
    DigiKeyboard.sendKeyStroke(KEY_ARROW_DOWN);
  }
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(300);
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(300);
  for(int i=0;i<100;i++){
    DigiKeyboard.sendKeyStroke(KEY_ARROW_RIGHT);
  }
  
  
  // Play video and fake update
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(600);
  DigiKeyboard.print("https://youtu.be/xR-FkYEl_4Y?t=22");
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(500);
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(300);
  DigiKeyboard.print("http://fakeupdate.net/win10ue");
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(2000);
  DigiKeyboard.sendKeyStroke(KEY_F11);
  while (true)
  {
      digitalWrite(0, HIGH);
      delay(1000);
      digitalWrite(0, LOW);
      delay(1000);
  }
}
 
