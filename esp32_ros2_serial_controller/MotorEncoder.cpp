#include <Arduino.h>
#include "MotorEncoder.h"

MotorEncoder::MotorEncoder(int pinA_, int pinB_) : _pinA(pinA_), _pinB(pinB_), _count(0), _lastStateA(LOW) {}

MotorEncoder::~MotorEncoder() {
  detachInterrupt(digitalPinToInterrupt(_pinA)); // Отключаем прерывания
  detachInterrupt(digitalPinToInterrupt(_pinB));  
}

void MotorEncoder::begin() {
  pinMode(_pinA, INPUT_PULLUP); // Устанавливаем режим входа с подтягивающим резистором
  pinMode(_pinB, INPUT_PULLUP);

  // Присоединяем обработчики прерываний
  attachInterruptArg(digitalPinToInterrupt(_pinA), onEncoderChangeA, this, CHANGE);
}

long MotorEncoder::getCount() {
  return _count;
}

void MotorEncoder::resetCount() {
  noInterrupts(); // Блокируем прерывания временно
  _count = 0;     // Сбрасываем счётчик импульсов
  interrupts();   // Включаем прерывания обратно
}

void IRAM_ATTR MotorEncoder::onEncoderChangeA(void* arg) {
  MotorEncoder* self = (MotorEncoder*)arg;
  int stateA = digitalRead(self->_pinA);
  int stateB = digitalRead(self->_pinB);
  if (stateA != self->_lastStateA) {
    if (stateB != stateA) {
      self->_count++;
    } else {
      self->_count--;
    }
    self->_lastStateA = stateA;
  }
}