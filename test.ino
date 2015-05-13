#define b(v) Serial.println( v ? "true" : "false" );
#define p(a) Serial.print(a);
#define q(a,b) Serial.print(a,b);

boolean tests_pass = true;

void assert(bool test){
  if (test) {
    p('.');
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



void test_lock(){
  Serial.print("Self Testing");

  if ( ! test() ) {
    unsigned int mils;
    while( true ) {
      mils = millis() % 500;
      if (mils > 100 && mils < 150 ||
          mils > 200 && mils < 250
      ) {
        digitalWrite(13, true);
      } else {
        digitalWrite(13, false);
      }
    }

  } else {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(50);
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
  }

  Serial.println(": Pass\n");
}
