#include "Arduino.h"

#define ONE   0x1
#define TWO   0x2
#define THREE 0x4
#define FOUR  0x8
#define FIVE  0x10
#define SIX   0x20
#define SEVEN 0x30
#define EIGHT 0x40

#define MAX_STATES 10

class TouchStateMachine {
  unsigned short state;
  unsigned short states[MAX_STATES];
  unsigned short pass_at;

  public:
    void setup();
    void advance_state(unsigned short new_state);
    void reset();
    bool completed();
    unsigned short current_state();
    unsigned short get_state( unsigned short position );
    void set_state( unsigned short position, unsigned short value );

    TouchStateMachine(unsigned short _pass_at) {
      state = 0;
      pass_at = _pass_at;
    }
};

void TouchStateMachine::setup(){
  set_state(0, ONE);
  set_state(1, TWO);
  set_state(2, THREE);
  set_state(3, FOUR);
}

void TouchStateMachine::advance_state( unsigned short new_state ) {
  if (new_state == 0x0) return;

  if (states[ state ] == new_state) {
    if ( state < MAX_STATES ) {
      state ++;
    }
  } else if ( states[0] == new_state ) {
    state = 1;
  } else {
    state = 0;
  }
}

void TouchStateMachine::reset(){
  state = 0;
}

unsigned short TouchStateMachine::current_state(){
  return state;
}

bool TouchStateMachine::completed() {
  return state >= pass_at;
}

unsigned short TouchStateMachine::get_state( unsigned short position ) {
  return states[ position ];
}


void TouchStateMachine::set_state( unsigned short position, unsigned short value ) {
  states[ position ] = value;
}
