#include "Motor.h"
#include <Arduino.h>

Motor::Motor(int pinA, int pinB) : _pinA(pinA), _pinB(pinB), _pwm(0), _direction(0) {}

void Motor::begin() {
  pinMode(_pinA, OUTPUT);
  pinMode(_pinB, OUTPUT);
  analogWrite(_pinA, LOW);
  analogWrite(_pinB, LOW);
}

void Motor::setPWM(int pwm) {
  

  _direction = (pwm >= 0) ? 1 : 0;
  _pwm = constrain(abs(pwm), 0, 255);
  
  if (_pwm == 0) {
    analogWrite(_pinA, LOW);
    analogWrite(_pinB, LOW);
  } else if (_direction == 1) {
    analogWrite(_pinA, _pwm);
    analogWrite(_pinB, LOW);
  } else {
    analogWrite(_pinA, LOW);
    analogWrite(_pinB, _pwm);
  }
}

int Motor::getSpeed() {
  return _pwm;
}