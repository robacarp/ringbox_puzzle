#include "Arduino.h"

#define MAX_STATES 10

class TouchStateMachine {
  unsigned short state;
  unsigned short * states[MAX_STATES];
  unsigned short pass_at;

  public:
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

void TouchStateMachine::advance_state( unsigned short new_state ) {
  if (*states[ state ] == new_state) {
    if ( state < MAX_STATES ) {
      state ++;
    }
  } else if ( *states[0] == new_state ) {
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
  Serial.print(" pass at: ");
  Serial.println(pass_at);
  Serial.print(" state : ");
  Serial.println(state);
  state >= pass_at;
}

unsigned short TouchStateMachine::get_state( unsigned short position ) {
  return * states[ position ];
}


void TouchStateMachine::set_state( unsigned short position, unsigned short value ) {
  states[ position ] = & value;
}
