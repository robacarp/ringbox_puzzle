#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#include "GPS.h"
#include "mpr121.h"
#include "touch_state_machine.h"

unsigned short irqpin = 10;
unsigned short gps_rx = 4;
unsigned short gps_tx = 3;

float destination_latitude = 0.0;
float destination_longitude = 0.0;

bool solved = false;
bool home = false;

TouchStateMachine password(4);
MPR121 touch(irqpin);
GPS gps(gps_rx, gps_tx, destination_latitude, destination_longitude);

void setup(){
  Serial.begin(115200);
  pinMode(13, OUTPUT);

  password.setup();

  gps.setup();
  touch.setup();

  test_lock();
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
    Serial.println("code entered");
    solved = true;
  }
}

void check_gps(){
  // gps.read();
  // gps.extract();

  // if ( ! gps.have_lock() ) {
  //   home = false;
  //   return;
  // }

  // Serial.println("lock found!");
  // gps.dump();
  home = true;
}

void open() {
  Serial.println("complete");
  digitalWrite(13, 1);
}







