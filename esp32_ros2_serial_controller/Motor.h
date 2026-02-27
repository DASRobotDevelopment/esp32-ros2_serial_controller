#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
public:
  Motor() : _pinA(0), _pinB(0) {}
  Motor(int pinA, int pinB);

  void begin();
  void setPWM(int pwm);
  int getSpeed();

private:
  int _pinA, _pinB;
  int _pwm;
  int _direction;
};

#endif