#ifndef MOTORENCODER_H
#define MOTORENCODER_H

#include <Arduino.h>

class MotorEncoder {
public:
  MotorEncoder() : _pinA(0), _pinB(0) {} 
  MotorEncoder(int pinA, int pinB);
  ~MotorEncoder(); 

  void begin();
  long getCount();
  void resetCount();

private:
  bool _lastStateA;
  int _pinA, _pinB;
  volatile long _count;

  static void IRAM_ATTR onEncoderChangeA(void* arg);
};

#endif