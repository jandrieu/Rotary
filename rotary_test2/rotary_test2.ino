#include <Metro.h>

#include <rotary.h>

const int INPUT_MAX(200);
const int RANGE_MAX(255);
const int DUMP_WIDTH(30);

// the order selects the pin assignment
Rotary redRotary(INPUT_MAX, RANGE_MAX);    // pins 2,3   (A,B) encoder: A is yellow, B is purple
Rotary greenRotary(INPUT_MAX, RANGE_MAX);  // pins 21,20 (A,B) A is white, B is green
Rotary blueRotary(INPUT_MAX, RANGE_MAX);   // pins 19,18 (A,B)

// black ground
// gray channel A
// red vcc
// blue channel B

Metro dump_timer(200);

void dump(char label, int value);
void dumpState();

void setup() {
  redRotary.setup(redRotary);
  greenRotary.setup(greenRotary);
  blueRotary.setup(blueRotary);
  
  Serial.begin(115200);  // output
}



// main loop, work is done by interrupt service routines, this one only prints stuff
void loop() {
  redRotary.loop();
  blueRotary.loop();
  greenRotary.loop();

  if(dump_timer.check()){
    dump('R', redRotary.value());
    dump('G', greenRotary.value());
    dump('B', blueRotary.value());
    Serial.print("\n");
  }  
//  Serial.println(redKnob, DEC);
  
}

void dump(char label, int value) {
  
  Serial.print("  ");
  Serial.print(label);
  Serial.print(":");
  if(value < 100) Serial.print(" ");
  if(value < 10) Serial.print(" ");
  Serial.print(value, DEC);

  int display(map(value,0,RANGE_MAX,0,DUMP_WIDTH)); 
  
  int i;
  for(i=0;i<DUMP_WIDTH;i++){
    if(display > i)
      Serial.print('.');
    else
      Serial.print(' ');
  }
}  

  
  


