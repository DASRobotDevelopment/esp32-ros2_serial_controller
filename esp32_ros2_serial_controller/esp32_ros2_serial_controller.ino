/*
  ROS2-ESP32 Serial controller для четырехколесного робота.

  Назначение:
  - Прием команд по Serial от ROS2-узла / ПК и управление моторами.
  - Поддержка прямой установки PWM, задания целевых RPM и настройки PID.
  - Отправка на хост текущих значений энкодеров.

  Формат команд:
    e
      Запрос счетчиков энкодеров.
      Ответ:  e,<enc0>,<enc1>,<enc2>,<enc3>

    m,<+000>,<+000>,<+000>,<+000>
      Важно! Команда должна быть строго заданного формата!
      Прямая установка PWM для каждого мотора (обход PID).

    v,+010,+010,-010,+050
      Важно! Команда должна быть строго заданного формата!
      Задание целевых RPM для PID-контроллеров всех моторов.

    p,<0.00>,<0.00>,<0.00>
      Важно! Команда должна быть строго заданного формата!
      Установка PID-параметров (одинаковых) для всех моторов.

  Параметры оборудования задаются через:
    NUMBER_OF_WHEELS – количество колес/мотор-энкодерных пар,
    MOTOR_PPR        – импульсов на оборот энкодера,
    MOTOR_MAX_RPM    – максимальные обороты мотора,
    PID_UPDATE_FREQ  – частота обновления PID (Гц).

  Код спроектирован так, чтобы:
  - легко менять NUMBER_OF_WHEELS (масштабируемый парсинг списков),
  - минимизировать нагрузку на MCU при обработке строк,
  - упростить интеграцию с ROS2 через простой текстовый протокол.
*/

#include "Pins.h"
#include "Motor.h"
#include "MotorEncoder.h"
#include "PIDController.h"

// Параметры, которые нужно задать пользователю.
const int MOTOR_PPR        = 988;
const int MOTOR_MAX_RPM    = 200;
const int PID_UPDATE_FREQ  = 50;
const int NUMBER_OF_WHEELS = 4;

// Переменные для работы основных функций.
float targetRPMOfWheels[NUMBER_OF_WHEELS];
float targetPWMOfWheels[NUMBER_OF_WHEELS];
int separators[6][2] = {
  {2, 6},
  {7, 11},
  {12, 16},
  {17, 21},
  {22, 26},
  {27, 31} 
};

char inputChar;
String inputString;

// Инициализация элементов базы робота.
Motor motors[NUMBER_OF_WHEELS];
MotorEncoder encoders[NUMBER_OF_WHEELS];
PIDController pidControllers[NUMBER_OF_WHEELS];


void setup() {
  Serial.begin(115200);          

  for (int i = 0; i < NUMBER_OF_WHEELS; i++) {
    motors[i] = Motor(motorPins[i][0], motorPins[i][1]);
    motors[i].begin();
    encoders[i] = MotorEncoder(encoderPins[i][0], encoderPins[i][1]);
    encoders[i].begin();
    pidControllers[i] = PIDController(&motors[i], &encoders[i], MOTOR_PPR, MOTOR_MAX_RPM, PID_UPDATE_FREQ);
  }

}

void loop() {

  for(int i = 0; i < NUMBER_OF_WHEELS; i++){
    pidControllers[i].computePWM();
  } 

}

// Работа с последовательным портом //
void serialEvent() {

  while (Serial.available()) {
    inputChar = Serial.read();
    inputString += inputChar;
    if (inputChar == '\n') {
      processCommand();
      inputString = ""; 
    }
  }

}

void processCommand() {

  if (inputString.startsWith("e")) sendEncoderData();
  if (inputString.startsWith("v")) parseTargetRPM();
  if (inputString.startsWith("m")) parseTargetPWM();
  if (inputString.startsWith("p")) parseTargetPID();

}

void sendEncoderData() {
  Serial.print("e,");
  for(int i = 0; i < NUMBER_OF_WHEELS; i++){
    if(i == NUMBER_OF_WHEELS - 1){
      Serial.println(encoders[i].getCount());
    }
    else{
      Serial.print(encoders[i].getCount());
      Serial.print(",");
    }
  }
}

void parseTargetRPM() {
  for(int i = 0; i < NUMBER_OF_WHEELS; i++){
    targetRPMOfWheels[i] = inputString.substring(separators[i][0], separators[i][1]).toFloat();
    pidControllers[i].setTargetRPM(targetRPMOfWheels[i]);
    
    // TEST INFO
    // Serial.print("Set RPM");
    // Serial.println(targetRPMOfWheels[i]);
  }  
}

void parseTargetPWM() {
    for(int i = 0; i < NUMBER_OF_WHEELS; i++){
    targetPWMOfWheels[i] = inputString.substring(separators[i][0], separators[i][1]).toFloat();
    motors[i].setPWM(targetPWMOfWheels[i]);
    
    // TEST INFO
    // Serial.print("Set PWM");
    // Serial.println(targetPWMOfWheels[i]);
  }  
}

void parseTargetPID() {
    float kp = inputString.substring(2, 6).toFloat();
    float ki = inputString.substring(7, 11).toFloat();
    float kd = inputString.substring(12, 16).toFloat();
    for(int i = 0; i < NUMBER_OF_WHEELS; i++){
      pidControllers[i].setPIDParameters(kp, ki, kd);
    }   
    
    // TEST INFO
    // Serial.print("PID:");
    // Serial.print("kp - ");
    // Serial.print(kp);
    // Serial.print("ki - ");
    // Serial.print(ki);
    // Serial.print("kd - ");
    // Serial.println(kd); 
}