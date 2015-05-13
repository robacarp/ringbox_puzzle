#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Arduino.h>

#define MAX_PARSE_TIME 50
#define REPORT_PARSE_TIME MAX_PARSE_TIME * 0.8

class GPS {
  unsigned short rx = 4;
  unsigned short tx = 3;

  bool new_data;

  TinyGPS * gps;
  SoftwareSerial * ss;

  public:
    float latitude, longitude;
    unsigned long age, satellite_count, precision, chars;
    unsigned short sentences, failed_checksum;

    GPS (unsigned short rx, unsigned short tx) : rx(rx), tx(tx), new_data(false) {}
    void setup();
    void read();
    void extract();
    void dump();
    bool have_lock();
};


void GPS::setup() {
  ss = new SoftwareSerial(rx, tx);
  gps = new TinyGPS;

  ss->begin(9600);
}

void GPS::read() {
  unsigned long start_time = millis();
  unsigned long run_time = 0;

  // spend less than 50ms pulling and parsing GPS data
  while (ss->available() && run_time < MAX_PARSE_TIME) {
    new_data = gps->encode( ss->read() );
    run_time = millis() - start_time;
  }

  if (run_time > REPORT_PARSE_TIME) {
    Serial.print("extracted data for: ");
    Serial.print(run_time);
    Serial.println("ms");
  }
}

void GPS::extract() {
  if (! new_data ) return;

  gps->f_get_position(& latitude, & longitude, &age);
  if (latitude == TinyGPS::GPS_INVALID_F_ANGLE)
    latitude = 0.0;
  if (longitude == TinyGPS::GPS_INVALID_F_ANGLE)
    longitude = 0.0;

  satellite_count = gps->satellites();
  if (satellite_count == TinyGPS::GPS_INVALID_SATELLITES)
    satellite_count = 0;

  precision = gps->hdop();
  if (precision == TinyGPS::GPS_INVALID_HDOP)
    precision = 0;

  gps->stats(& chars, & sentences, & failed_checksum);

  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
}

bool GPS::have_lock(){
  return latitude != 0.0 && longitude != 0.0 && precision > 0;
}

void GPS::dump() {
  Serial.print(" age:");
  Serial.print(age);
  Serial.print(" lat=");
  Serial.print(latitude, 8);
  Serial.print(" long=");
  Serial.print(longitude, 8);

  Serial.print(" precision=");
  Serial.print(precision);

  Serial.print(" chars=");
  Serial.print(chars);
  Serial.print(" sentences=");
  Serial.print(sentences);
  Serial.print(" csum err=");
  Serial.println(failed_checksum);
}
