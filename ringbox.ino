#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>


#include "GPS.h"
#include "mpr121.h"
#include "touch_state_machine.h"
#include "buzz.h"
#include "leds.h"

#define IRQPIN 10
#define GPS_RX 4
#define GPS_TX 3
#define BUZZ_PIN 6
#define LATCH_PIN 9
#define LED_1 2
#define LED_2 5


// apartment
#define APARTMENT_LATITUDE 39.998897
#define APARTMENT_LONGITUDE -105.230499

// flagstaff mtn
#define FLAGSTAFF_LATITUDE 40.00644302
#define FLAGSTAFF_LONGITUDE -105.29473114

double destination_latitude = APARTMENT_LATITUDE;
double destination_longitude = APARTMENT_LONGITUDE;

bool solved = false;
bool home = false;
bool opened = false;

TouchStateMachine password(4);
MPR121 touch(IRQPIN);
GPS gps(GPS_RX, GPS_TX);
Buzz buzzer(BUZZ_PIN);

LED led_one(LED_1);
LED led_two(LED_2);

void setup(){
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  buzzer.setup();
  led_one.setup();
  led_two.setup();
  password.setup();
  gps.setup();
  touch.setup();

  test_lock();

  gps.target_latitude = destination_latitude;
  gps.target_longitude = destination_longitude;

  led_one.on();
  led_two.on();
}

void loop(){
  buzzer.update();
  led_one.update();
  led_two.update();

  check_puzzle();

  if ( ! home ) {
    check_gps();
  }
  // heartbeat flash
  //  unsigned long mils = millis() % 500;
  //  if (mils > 100 && mils < 150 ||
  //      mils > 200 && mils < 250
  //  ) {
  //    digitalWrite(13, true);
  //  } else {
  //    digitalWrite(13, false);
  //  }

  if ( solved && home && ! opened ) {
    open();
  }
}

void check_puzzle(){
  if ( touch.read() ) {
    unsigned short buttons = touch.status();

    if ( ! solved ) {
      if (buttons > 0) {
        buzzer.buzz(100);
      }

      password.advance_state( buttons );
    }

    if ( solved && buttons == 0x20 ) {
      open();
    }
  }

  if (password.completed()) {
    solved = true;
    led_two.blink( 200 );
  }
}

void check_gps(){
  gps.read();
  gps.extract();

  if (! gps.have_lock()) return;

  gps.distance_to_target();

  home = gps.at_target();

  if (home) {
    led_one.blink( 200 );
  } else {
    led_one.blink(2000);
  }

  // gps.dump();
}

void open() {
  opened = true;

  digitalWrite(13, 1);

  digitalWrite(LATCH_PIN, 1);
  delay(100);
  digitalWrite(LATCH_PIN, 0);
  delay(100);

  digitalWrite(LATCH_PIN, 1);
  delay(100);
  digitalWrite(LATCH_PIN, 0);
  delay(100);
}
