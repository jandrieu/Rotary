#include "Rotary.h"
#include "Arduino.h"

Rotary::Rotary(int inputMax, int rangeMax, bool fake) : _inputMax(inputMax), _rangeMax(rangeMax), _fake(fake),_aSet(false),_bSet(false),_encoderPos(0),_lastPos(0),_rotating(false),_i(installCount++),
	_pinA(interruptAPins[_i]),_pinB(interruptBPins[_i]) {
}

void Rotary::setup(Rotary& me) {
	if(_fake)
		return;
		
	encoders[_i] = &me;
	pinMode(_pinA, INPUT); 
	pinMode(_pinB, INPUT); 
  
	attachInterrupt(interruptAs[_i], A_interrupt_funs[_i], CHANGE);
	attachInterrupt(interruptBs[_i], B_interrupt_funs[_i], CHANGE);
}


Metro fake_timer(250);

void Rotary::loop(void) {
	_interrupt_ok=true; // Free the flag for reading during the interrupt
	
	if(_fake && fake_timer.check()){
		_encoderPos++;
		_encoderPos = _encoderPos % _inputMax;
	}
		
}
bool Rotary::changed(void){
	return _encoderPos != _lastPos;
}

int Rotary::value(void){
	_lastPos  = _encoderPos;
	return map(_encoderPos,0,_inputMax,0,_rangeMax);
}

void Rotary::InterruptA(void){
  // debounce
  if ( !_interrupt_ok )
	delayMicroseconds (10);  // wait a little until the bouncing is done
  //_interrupt_ok = false; // don't process interrupts until we are reset by a loop
  
  // Test transition, did things really change? 
  if( digitalRead(_pinA) != _aSet ) {  // debounce once more
    _aSet = !_aSet;
    
    // adjust counter + if A leads B
    if ( _aSet && !_bSet ) {
      if(_encoderPos < _inputMax) {
          _encoderPos++;
      }  
    }
    _interrupt_ok = false;  // no more debouncing until loop() hits again
  }

}
void Rotary::InterruptB(void){
  // debounce
  if ( !_interrupt_ok ) 
	delayMicroseconds (10);  // wait a little until the bouncing is done
//	delay (1);  // wait a little until the bouncing is done

  // Test transition, did things really change? 
  if( digitalRead(_pinB) != _bSet ) {  // debounce once more
    _bSet = !_bSet;
    
    // adjust counter + if A leads B
    if ( !_aSet && _bSet ) {
      if(_encoderPos > 0) {
          _encoderPos--;
      }  
    }
  _interrupt_ok = false; // don't process interrupts until we are reset by a loop
   // _rotating = false;  // no more debouncing until loop() hits again
  }	
}	

int Rotary::installCount = 0;
void (*Rotary::A_interrupt_funs[MAX_ENCODER_COUNT])(void)  = {&Rotary::interruptA1, &Rotary::interruptA2, &Rotary::interruptA3};
void (*Rotary::B_interrupt_funs[MAX_ENCODER_COUNT])(void)  = {&Rotary::interruptB1, &Rotary::interruptB2, &Rotary::interruptB3};
Rotary::Rotary* Rotary::encoders[MAX_ENCODER_COUNT] = {NULL,NULL,NULL};
bool Rotary::_interrupt_ok = true;
