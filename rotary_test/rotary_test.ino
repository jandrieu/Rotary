/* interrupt routine for Rotary Encoders
   tested with Noble RE0124PVB 17.7FINB-24 http://www.nobleusa.com/pdf/xre.pdf - available at pollin.de
   and a few others, seems pretty universal

   The average rotary encoder has three pins, seen from front: A C B
   Clockwise rotation A(on)->B(on)->A(off)->B(off)
   CounterCW rotation B(on)->A(on)->B(off)->A(off)

   and may be a push switch with another two pins, pulled low at pin 8 in this case
   raf@synapps.de 20120107

*/

// usually the rotary encoders three pins have the ground pin in the middle
enum PinAssignments {
  encoderPinA = 2,   // rigth
  encoderPinB = 3,   // left
};

const float ROTARY_INPUT_MAX = 1000;
const float ROTARY_RANGE_MAX = 120;

volatile unsigned int encoderPos = 0;  // a counter for the dial
unsigned int lastReportedPos = 1;   // change management
static boolean rotating=false;      // debounce management

// interrupt service routine vars
boolean A_set = false;              
boolean B_set = false;


void setup() {

  pinMode(encoderPinA, INPUT); 
  pinMode(encoderPinB, INPUT); 
  pinMode(13, OUTPUT);
  
// encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);
// encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

  Serial.begin(9600);  // output
}

// main loop, work is done by interrupt service routines, this one only prints stuff
void loop() { 
  rotating = true;  // reset the debouncer

  if (lastReportedPos != encoderPos) {
    
    int encoder(map(encoderPos,0,ROTARY_INPUT_MAX,0,ROTARY_RANGE_MAX));

    int i;
    for(i=0; i<encoder; i++){
      Serial.print("#");
    }
    Serial.println("*");
  
//    Serial.println(encoderPos, DEC);
    lastReportedPos = encoderPos;
  }
}



// Interrupt on A changing state
void doEncoderA(){
  // debounce
  if ( rotating ) delay (1);  // wait a little until the bouncing is done

  // Test transition, did things really change? 
  if( digitalRead(encoderPinA) != A_set ) {  // debounce once more
    A_set = !A_set;
    
    // adjust counter + if A leads B
    if ( A_set && !B_set ) {
      if(encoderPos < ROTARY_INPUT_MAX) {
          encoderPos += 1;
      }  
      digitalWrite(13,HIGH );
    }
    rotating = false;  // no more debouncing until loop() hits again
  }
}


// Interrupt on B changing state, same as A above
void doEncoderB(){
  if ( rotating ) delay (1);
  if( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if( B_set && !A_set ) {
      digitalWrite(13,LOW);
      if(encoderPos > 0) {
        encoderPos -= 1;
      }
    }  

    rotating = false;
  }
}
