#include <Arduino.h>

#define OFF 0
#define STEADY 1
#define RUNNING 2
#define BLINKING 4

class LED {
  public:
    unsigned short pin;
    unsigned short duration;
    unsigned long started_at;
    unsigned short mode;

    LED(unsigned short pin)
      : pin(pin), started_at(0), mode(OFF) {
    }

    void setup() {
      pinMode(pin, OUTPUT);
    }

    void update() {
      if (mode == RUNNING) {
        run();
        return;
      } else if (mode == STEADY) {
        return;
      } else if (mode == BLINKING) {
        blink_more();
      }
    }



    void hit(unsigned short run_for) {
      started_at = millis();
      duration = run_for;
      digitalWrite(pin, 1);
      mode = RUNNING;
    }

    void run() {
      if (started_at > 0 && millis() > (started_at + duration) ) {
        digitalWrite(pin, 0);
        started_at = 0;
      }
    }


    void blink( unsigned short period ){
      if (mode == BLINKING && duration == period) return;

      mode = BLINKING;
      duration = period;
      started_at = millis();
      digitalWrite(pin, 1);
    }

    void blink_more() {
      if ((millis() - started_at) % duration > duration / 2) {
        digitalWrite(pin, 1);
      } else {
        digitalWrite(pin, 0);
      }
    }


    void on(){
      mode = STEADY;
      digitalWrite(pin, 1);
    }

    void off(){
      mode = OFF;
      digitalWrite(pin, 0);
    }
};
