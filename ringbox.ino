#include "mpr121.h"
#include <Wire.h>
#include "touch_state_machine.h"

#define b(v) Serial.println( v ? "true" : "false" );
#define p(a) Serial.println(a);
#define q(a,b) Serial.println(a,b);

int irqpin = 10; // Digital 2
boolean touchStates[12]; //to keep track of the previous touch states
boolean tests_pass = true;

TouchStateMachine password(6);

void assert(bool test){
  if (test) {
    p('.');
  } else {
    tests_pass = false;
    p('#');
  }
}

void test() {
  // not passing initially
  assert( ! password.completed() );

  // null advances don't do anything
  password.advance_state(0x0);
  password.advance_state(0x0);
  assert( password.current_state() == 0 );

  // advancing state once, then resetting with an incorrect value
  password.advance_state(0x1);
  assert( password.current_state() == 1 );
  password.advance_state(0x2);
  assert( password.current_state() == 0 );

  // tapping the first number multiple times
  password.advance_state(0x1);
  assert( password.current_state() == 1 );
  password.advance_state(0x1);
  assert( password.current_state() == 1 );

  // first, second, third, first
  password.advance_state(0x1);
  assert( password.current_state() == 1 );
  assert( ! password.completed() );

  password.advance_state(0x0);
  assert( password.current_state() == 2 );
  assert( ! password.completed() );

  password.advance_state(0x2);
  assert( password.current_state() == 3 );
  assert( ! password.completed() );

  password.advance_state(0x0);
  assert( password.current_state() == 4 );
  assert( ! password.completed() );

  password.advance_state(0x1);
  assert( password.current_state() == 1 );
  assert( ! password.completed() );

  // correct password
  password.reset();
  assert( ! password.completed() );

  password.advance_state(0x1);
  password.advance_state(0x0);
  password.advance_state(0x2);
  password.advance_state(0x0);
  password.advance_state(0x4);
  password.advance_state(0x0);
  password.advance_state(0x8);
  assert( password.current_state() >= 6 );

  p( password.current_state() );
  assert( password.completed() );
}

void setup(){
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  password.set_state(0, 0x1);
  password.set_state(1, 0x0);
  password.set_state(2, 0x2);
  password.set_state(3, 0x0);

  password.set_state(4, 0x4);
  password.set_state(5, 0x0);
  password.set_state(6, 0x8);
  password.set_state(7, 0x0);

  test();

  // pinMode(irqpin, INPUT);
  // digitalWrite(irqpin, HIGH); //enable pullup resistor
  //
  // Serial.begin(9600);
  // Wire.begin();

  // mpr121_setup();
}

void loop(){
  digitalWrite(13, tests_pass);
  // if (millis() % 5000 == 0) {
  //   Serial.print( millis() );
  //   Serial.println("tick");
  // }
  // readTouchInputs();
}

void readTouchInputs(){
 if(!checkInterrupt()){
  
  //read the touch state from the MPR121
  Wire.requestFrom(0x5A,2); 
  
  byte LSB = Wire.read();
  byte MSB = Wire.read();
  
  unsigned short touched = ((MSB << 8) | LSB); //16bits that make up the touch states

  
  for (int i=0; i < 12; i++){ // Check what electrodes were pressed
   if(touched & (1<<i)){
   
    if(touchStates[i] == 0){
     //pin i was just touched
     Serial.print("pin ");
     Serial.print(i);
     Serial.println(" was just touched");
    
    }else if(touchStates[i] == 1){
     //pin i is still being touched
    } 
   
    touchStates[i] = 1;   
   }else{
    if(touchStates[i] == 1){
     Serial.print("pin ");
     Serial.print(i);
     Serial.println(" is no longer being touched");
     
     //pin i is no longer being touched
   }
    
    touchStates[i] = 0;
   }
  
  }
  
 }
}




void mpr121_setup(void){

 set_register(0x5A, ELE_CFG, 0x00); 
 
 // Section A - Controls filtering when data is > baseline.
 set_register(0x5A, MHD_R, 0x01);
 set_register(0x5A, NHD_R, 0x01);
 set_register(0x5A, NCL_R, 0x00);
 set_register(0x5A, FDL_R, 0x00);

 // Section B - Controls filtering when data is < baseline.
 set_register(0x5A, MHD_F, 0x01);
 set_register(0x5A, NHD_F, 0x01);
 set_register(0x5A, NCL_F, 0xFF);
 set_register(0x5A, FDL_F, 0x02);
 
 // Section C - Sets touch and release thresholds for each electrode
 set_register(0x5A, ELE0_T, TOU_THRESH);
 set_register(0x5A, ELE0_R, REL_THRESH);

 set_register(0x5A, ELE1_T, TOU_THRESH);
 set_register(0x5A, ELE1_R, REL_THRESH);
 
 set_register(0x5A, ELE2_T, TOU_THRESH);
 set_register(0x5A, ELE2_R, REL_THRESH);
 
 set_register(0x5A, ELE3_T, TOU_THRESH);
 set_register(0x5A, ELE3_R, REL_THRESH);
 
 set_register(0x5A, ELE4_T, TOU_THRESH);
 set_register(0x5A, ELE4_R, REL_THRESH);
 
 set_register(0x5A, ELE5_T, TOU_THRESH);
 set_register(0x5A, ELE5_R, REL_THRESH);
 
 set_register(0x5A, ELE6_T, TOU_THRESH);
 set_register(0x5A, ELE6_R, REL_THRESH);
 
 set_register(0x5A, ELE7_T, TOU_THRESH);
 set_register(0x5A, ELE7_R, REL_THRESH);
 
 set_register(0x5A, ELE8_T, TOU_THRESH);
 set_register(0x5A, ELE8_R, REL_THRESH);
 
 set_register(0x5A, ELE9_T, TOU_THRESH);
 set_register(0x5A, ELE9_R, REL_THRESH);
 
 set_register(0x5A, ELE10_T, TOU_THRESH);
 set_register(0x5A, ELE10_R, REL_THRESH);
 
 set_register(0x5A, ELE11_T, TOU_THRESH);
 set_register(0x5A, ELE11_R, REL_THRESH);
 
 // Section D
 // Set the Filter Configuration
 // Set ESI2
 set_register(0x5A, FIL_CFG, 0x04);
 
 // Section E
 // Electrode Configuration
 // Set ELE_CFG to 0x00 to return to standby mode
 set_register(0x5A, ELE_CFG, 0x0C); // Enables all 12 Electrodes
 
 
 // Section F
 // Enable Auto Config and auto Reconfig
 /*set_register(0x5A, ATO_CFG0, 0x0B);
 set_register(0x5A, ATO_CFGU, 0xC9); // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V  set_register(0x5A, ATO_CFGL, 0x82); // LSL = 0.65*USL = 0x82 @3.3V
 set_register(0x5A, ATO_CFGT, 0xB5);*/ // Target = 0.9*USL = 0xB5 @3.3V
 
 set_register(0x5A, ELE_CFG, 0x0C);
 
}


boolean checkInterrupt(void){
 return digitalRead(irqpin);
}


void set_register(int address, unsigned char r, unsigned char v){
  Wire.beginTransmission(address);
  Wire.write(r);
  Wire.write(v);
  Wire.endTransmission();
}
