#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Arduino.h>
#include <math.h>

#define MAX_PARSE_TIME 50
#define REPORT_PARSE_TIME MAX_PARSE_TIME * 0.8
#define EARTH_RADIUS 6371
#define TO_RAD (3.1415926536 / 180)

class GPS {
  unsigned short rx = 4;
  unsigned short tx = 3;

  bool new_data;

  TinyGPS * gps;
  SoftwareSerial * ss;

  public:
    double target_latitude, target_longitude;
    float latitude, longitude;
    double distance;
    unsigned long age, satellite_count, precision, chars;
    unsigned short sentences, failed_checksum;

    GPS (unsigned short rx, unsigned short tx)
      : rx(rx), tx(tx), new_data(false), distance(65537)
    {}

    static double coordinate_distance(double, double, double, double);

    void setup();
    void read();
    void extract();
    bool have_lock();
    double distance_to_target();
    bool at_target();
    void dump();
    void reset();
};


void GPS::setup() {
  ss = new SoftwareSerial(rx, tx);
  gps = new TinyGPS;

  ss->begin(9600);
}

void GPS::reset() {
    target_latitude = target_longitude = 0.0;
    latitude = longitude = 0.0;
    distance = 0.0;
    age = satellite_count = precision = chars = 0;
    sentences = failed_checksum = 0;
}

void GPS::read() {
  unsigned long start_time = millis();
  unsigned long run_time = 0;

  // spend less than 50ms pulling and parsing GPS data
  while (ss->available() && run_time < MAX_PARSE_TIME) {
    char c = ss->read();
    // Serial.write(c);
    new_data = gps->encode(c);
  }
  run_time = millis() - start_time;

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

  precision = gps->hdop() / 100;
  if (precision == TinyGPS::GPS_INVALID_HDOP)
    precision = 1000.0;

  gps->stats(& chars, & sentences, & failed_checksum);

  distance_to_target();

  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
}

bool GPS::have_lock(){
  return sentences > 0 && latitude != 0.0 && longitude != 0.0 && precision < 10;
}


// nicked from http://rosettacode.org/wiki/Haversine_formula#C
double GPS::coordinate_distance(double th1, double ph1, double th2, double ph2) {
  double dx, dy, dz;
  ph1 -= ph2;
  ph1 *= TO_RAD, th1 *= TO_RAD, th2 *= TO_RAD;

  dz = sin(th1) - sin(th2);
  dx = cos(ph1) * cos(th1) - cos(th2);
  dy = sin(ph1) * cos(th1);
  return asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * EARTH_RADIUS;
}


double GPS::distance_to_target(){
  if (! have_lock() ) return 65536;
  distance = coordinate_distance(latitude, longitude, target_latitude, target_longitude);
  return distance;
}

bool GPS::at_target() {
  if ( ! have_lock() )  {
    return false;
  }

  if ( precision > 10 ) {
    return false;
  }

  return distance < 1;
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
