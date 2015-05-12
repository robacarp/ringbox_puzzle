#include <Wire.h>
#include "mpr121.h"
#include "touch_state_machine.h"

#define ONE   0x1
#define TWO   0x2
#define THREE 0x4
#define FOUR  0x8
#define FIVE  0x10
#define SIX   0x20
#define SEVEN 0x30
#define EIGHT 0x40

int irqpin = 10;
bool solved = false;
bool home = false;

TouchStateMachine password(4);
MPR121 touch(irqpin);

void setup(){
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  touch.setup();

  password.set_state(0, ONE);
  password.set_state(1, TWO);
  password.set_state(2, THREE);
  password.set_state(3, FOUR);

  if ( ! test() ) {
    unsigned int mils;
    while( true ) {
      mils = millis() % 500;
      if (mils > 100 && mils < 150 ||
          mils > 200 && mils < 250
      ) {
        digitalWrite(13, true);
      } else {
        digitalWrite(13, false);
      }
    }

  } else {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(50);
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
  }
}

void loop(){
  if ( ! solved ) {
    check_puzzle();
  }

  if ( ! home ) {
    check_gps();
  }

  if ( solved && home ) {
    open();

    while (true);
  }
}

void check_puzzle(){
  if ( touch.read() ) {
    unsigned short buttons = touch.status();
    password.advance_state( buttons );
  }

  if (password.completed()) {
    solved = true;
  }
}

void check_gps(){
  home = true;
  return;
}


void open() {
  digitalWrite(13, 1);
}







