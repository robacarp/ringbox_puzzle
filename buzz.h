#include "tones.h"

class Buzz {
  unsigned short pin;
  unsigned int freq;


  public:
    unsigned long started_at;
    unsigned short duration;

    Buzz(unsigned short pin) : pin(pin), started_at(0), freq(NOTE(C, 6)) {
    }

    void setup() {
      pinMode(pin, OUTPUT);
    }

    void update(){
      if (millis() > (started_at + duration)) {
        noTone(pin);
        started_at = 0;
      }
    }

    void buzz(unsigned short run_for){
      started_at = millis();
      duration = run_for;
      tone(pin, freq);
    }
};
