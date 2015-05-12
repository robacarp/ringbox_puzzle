#define b(v) Serial.println( v ? "true" : "false" );
#define p(a) Serial.println(a);
#define q(a,b) Serial.println(a,b);

boolean tests_pass = true;

void assert(bool test){
  if (test) {
    // p('.');
  } else {
    tests_pass = false;
    p('#');
  }
}

bool test() {
  // not passing initially
  assert( ! password.completed() );

  // random advances don't do anything
  password.advance_state(0x8);
  password.advance_state(0x3);
  assert( password.current_state() == 0 );

  // advancing state once, then resetting with an incorrect value
  password.advance_state(0x1);
  assert( password.current_state() == 1 );
  password.advance_state(0x8);
  assert( password.current_state() == 0 );

  // the first number multiple times
  password.advance_state(0x1);
  password.advance_state(0x1);
  assert( password.current_state() == 1 );
  password.advance_state(0x1);
  assert( password.current_state() == 1 );

  // first, second, third, first
  password.advance_state(0x1);
  assert( password.current_state() == 1 );
  assert( ! password.completed() );

  password.advance_state(0x2);
  assert( password.current_state() == 2 );
  assert( ! password.completed() );

  password.advance_state(0x4);
  assert( password.current_state() == 3 );
  assert( ! password.completed() );

  password.advance_state(0x1);
  assert( password.current_state() == 1 );
  assert( ! password.completed() );

  // correct password
  password.reset();
  assert( ! password.completed() );

  password.advance_state(0x1);
  password.advance_state(0x2);
  password.advance_state(0x4);
  password.advance_state(0x8);
  assert( password.current_state() >= 4 );

  assert( password.completed() );

  password.reset();

  return tests_pass;
}
