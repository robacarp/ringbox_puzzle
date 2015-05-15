#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#include "GPS.h"
#include "mpr121.h"
#include "touch_state_machine.h"
#include "buzz.h"

#define IRQPIN 10
#define GPS_RX 4
#define GPS_TX 3
#define BUZZ_PIN 6
#define LATCH_PIN 9
#define LED_1 2
#define LED_2 5


double destination_latitude = 39.998897;
double destination_longitude = -105.230499;

bool solved = false;
bool home = false;
unsigned long start = 0;

unsigned long dump_time = 0;

TouchStateMachine password(4);
MPR121 touch(IRQPIN);
GPS gps(GPS_RX, GPS_TX, destination_latitude, destination_longitude);
Buzz buzzer(BUZZ_PIN);

void setup(){
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);


  password.setup();

  gps.setup();
  touch.setup();

  test_lock();
}

void loop(){
//  touch.read();
//
//  unsigned short wanted = 0x20;
//  if ( touch.status() == wanted) {
//    if (start == 0) {
//      start = millis();
//      Serial.println( start );
//    }
//  } else {
//    start = 0;
//  }
//
//  if (start > 0 && millis() - start > 1000) {
//    open();
//    forever();
//  }

  if ( ! solved ) {
    check_puzzle();
  }

  if ( ! home ) {
    check_gps();
  } else {
    unsigned long mils = millis() % 500;
    if (mils > 100 && mils < 150 ||
        mils > 200 && mils < 250
    ) {
      digitalWrite(13, true);
    } else {
      digitalWrite(13, false);
    }
  }

  if ( solved && home ) {
    open();

    while (true);
  }

  // buzzer.beep();
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
  gps.read();
  gps.extract();

  home = gps.at_target();

  if (home) {
    Serial.println("home found!");
    Serial.println(millis() / 1000);
  }

  if (millis() - dump_time > 1000) {
    dump_time = millis();
    Serial.print("\n\n");
    Serial.print("distance: ");
    Serial.println(gps.distance, 6);
    Serial.print("precision: ");
    Serial.println(gps.precision);
    Serial.print("satellite_count: ");
    Serial.println(gps.satellite_count);
    Serial.print("latitude: ");
    Serial.println(gps.latitude, 6);
    Serial.print("longitude: ");
    Serial.println(gps.longitude, 6);
    Serial.print("\n\n");
  }
  // gps.dump();
}

void open() {
  Serial.println("complete");
  digitalWrite(13, 1);

  digitalWrite(LATCH_PIN, 1);
  delay(50);
  digitalWrite(LATCH_PIN, 0);
  delay(50);
  digitalWrite(LATCH_PIN, 1);
  delay(50);
  digitalWrite(LATCH_PIN, 0);
  delay(50);
}

void forever() {
  while( true );
}







