#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#include "Motor.h"
#include "MotorEncoder.h"

class PIDController {
  public:
  PIDController() {};
  PIDController(Motor* motor, MotorEncoder* encoder, int motorPPR, int motorMaxRPM, int  frequency, float kp = 4.0, float ki = 1.5, float kd = 0.01);
  void setTargetRPM(int rpm);
  void computePWM();
  void stopMotor();
  void reset();
  void showTestInfo();
  void setPIDParameters(float kp, float ki, float kd);
  
  private:
  // Ссылки на объекты управления
  Motor* _motor;
  MotorEncoder* _encoder;
  // Параметры для ПИД регулятора
  int _frequency;
  float _Kp, _Ki,_Kd;
  long _lastEncoderCount;
  unsigned long _lastTime;
  float _targetRPM, _errorSum, _lastError;
  // Параметры мотора
  int _motorPPR;
  int _motorMaxRPM;
  // Сервисные параметры
  bool _isMoving;
  bool _testInfo;
};

#endif