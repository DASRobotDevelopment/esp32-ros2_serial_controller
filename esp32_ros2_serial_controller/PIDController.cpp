#include "PIDController.h"

PIDController::PIDController(Motor* motor, MotorEncoder* encoder, int motorPPR, int motorMaxRPM, int frequency, float kp, float ki, float kd) : 
_motor(motor), _encoder(encoder), _motorPPR(motorPPR), _motorMaxRPM(motorMaxRPM), _frequency(frequency), _Kp(kp), _Ki(ki), _Kd(kd), _errorSum(0), _lastError(0), _targetRPM(0), _lastTime(0) {}

void PIDController::setTargetRPM(int rpm) {
  _targetRPM = constrain(rpm, -90, 90);
  if (rpm == 0) {
    PIDController::stopMotor(); 
    return ;
  }
  _targetRPM = rpm;
  _isMoving = true;
}

void PIDController::computePWM() {
  if(!_isMoving) return;

  unsigned long nowTime = millis();
  unsigned long deltaTime = nowTime - _lastTime;
  if (deltaTime < (unsigned long)(1000 / _frequency)) return;

  float deltaTimeSec = deltaTime / 1000.0f;
  
  _lastTime = nowTime;

  long encoderCount = _encoder->getCount();
  long deltaEncoderCount = encoderCount - _lastEncoderCount;
  _lastEncoderCount = encoderCount;

  float currentRPM = (deltaEncoderCount / (float)_motorPPR) * (60.0f / deltaTimeSec);
  float error = _targetRPM - currentRPM;
  
  _errorSum += error * deltaTimeSec;
  _errorSum = constrain(_errorSum, -100.0f, 100.0f);
  
  static float filtered_deriv = 0;
  float raw_deriv = (error - _lastError) / deltaTimeSec;
  filtered_deriv = 0.7f * raw_deriv + 0.3f * filtered_deriv;
  
  _lastError = error;

  float output = _Kp * error + _Ki * _errorSum + _Kd * filtered_deriv;
  float limitedOutput = constrain(output, -255, 255);
  _motor->setPWM(limitedOutput);

  // TEST INFO
  // Serial.println();
  // Serial.print("PID info:"); 
  // Serial.print(" Kp: ");  
  // Serial.print(_Kp); 
  // Serial.print(" Ki: ");  
  // Serial.print(_Ki); 
  // Serial.print(" Kd: ");  
  // Serial.print(_Kd);

  // Serial.print(" EncoderCount: ");
  // Serial.print(encoderCount);

  // Serial.print(" TargetRPM: ");
  // Serial.print(_targetRPM);

  // Serial.print(" CurrentRPM: ");
  // Serial.print(currentRPM);

  // Serial.print(" DeltaEncoderCount: ");
  // Serial.print(deltaEncoderCount);

  // Serial.print(" Error: ");
  // Serial.print(error);
  
  // Serial.print(" Output: ");
  // Serial.print(limitedOutput);
  // Serial.println();
}

void PIDController::stopMotor() {
  _targetRPM = 0;
  _motor->setPWM(0);
  _isMoving = false;
  PIDController::reset();
}

void PIDController::reset() {
    _errorSum = 0;
    _lastError = 0;
    _lastEncoderCount = _encoder->getCount();
}

void PIDController::setPIDParameters(float kp, float ki, float kd){
  _Kp = kp;
  _Ki = ki;
  _Kd = kd;
}

void PIDController::showTestInfo(){
  _testInfo = !_testInfo;
}
