// WriteLight library include file
#ifndef Rotary_h
#define Rotary_h

#include "Arduino.h"
#include "Metro.h"


// set these up to set the pins and interrupts
// you will also need to code for the variants if you use them
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	#define MEGA
//#else 
//	#define UNO  
#endif

// The Bourns Rotary Optical Encoder
// Black, Pin 1 -- Ground
// Yellow, Pin 3 -- Channel A
// White, Pin 4 -- VCC (+5V)
// Purple, Pin 5 -- Channel B


#ifdef MEGA
	const int MAX_ENCODER_COUNT = 3;
	enum interruptPins {
		pin1A = 2,   // Gray (from encoder)
		pin1B = 3,   // Blue (from encoder)
		pin2A = 21,  // Gray (from encoder)
		pin2B = 20,  // Blue (from encoder)
		pin3A = 19,  // Gray (from encoder)
		pin3B = 18}; // Blue (from encoder)
	
	const interruptPins interruptAPins[MAX_ENCODER_COUNT] = {pin1A, pin2A, pin3A};
	const interruptPins interruptBPins[MAX_ENCODER_COUNT] = {pin1B, pin2B, pin3B};
	const int interruptAs[] = {0,2,4};
	const int interruptBs[] = {1,3,5};
#endif




//const float ROTARY_INPUT_MAX = 1000;
//const float ROTARY_RANGE_MAX = 255;



class Rotary {
  public:
    Rotary(int inputMax, int rangeMax, bool fake);
    void setup(Rotary& me);
	void loop(void);
	bool changed(void);
	int value(void);

	void InterruptA(void);
	void InterruptB(void);
	
  private:
	const int _i; // encoder_index
	int _pinA,_pinB;
	bool _aSet,_bSet;
	int _encoderPos;
	int _lastPos;
	int _lastValue;
	bool _rotating;
	int _inputMax;
	int _rangeMax;
	bool _fake;
	
	static int installCount;
	static bool _interrupt_ok;
	
  public:
	static void interruptA1(void) {
		if(installCount >= 1 && encoders[0]) {
			encoders[0]->InterruptA();
		}
	};
	static void interruptB1(void){
		if(installCount >= 1 && encoders[0]) {
			encoders[0]->InterruptB();
		}
	};
	static void interruptA2(void){
		if(installCount >= 2  && encoders[1]) {
			encoders[1]->InterruptA();
		}
	};
	static void interruptB2(void){
		if(installCount >= 2 && encoders[1]) {
			encoders[1]->InterruptB();
		}
	};
	static void interruptA3(void){
		if(installCount >= 3 && encoders[2]) {
			encoders[2]->InterruptA();
		}
	};
	static void interruptB3(void){
		if(installCount >= 3 && encoders[2]) {
			encoders[2]->InterruptB();
		}
	};

	static Rotary* encoders[MAX_ENCODER_COUNT];

	static void (*A_interrupt_funs[MAX_ENCODER_COUNT])(void); 
	static void (*B_interrupt_funs[MAX_ENCODER_COUNT])(void);
	
};

#endif

