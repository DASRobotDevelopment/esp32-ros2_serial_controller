/*
  ROS2-ESP32 Serial controller для четырехколесного робота.

  Назначение:
  - Прием команд по Serial от ROS2-узла / ПК и управление 4-мя моторами.
  - Поддержка прямой установки PWM, задания целевых RPM и настройки PID.
  - Отправка на хост текущих значений энкодеров, RPM и отладочной информации.

  Формат команд:
    e
      Запрос счетчиков энкодеров.
      Ответ:  e,<enc0>,<enc1>,<enc2>,<enc3>

    m,<pwm0>,<pwm1>,<pwm2>,<pwm3>
      Прямая установка PWM для каждого мотора (обход PID).

    v,<rpm0>,<rpm1>,<rpm2>,<rpm3>
      Задание целевых RPM для PID-контроллеров всех моторов.

    p,<kp>,<ki>,<kd>
      Установка PID-параметров (одинаковых) для всех моторов.

    t
      Тестовая / отладочная информация по каждому PID-контроллеру.

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

const int NUMBER_OF_WHEELS = 4;
const int MOTOR_PPR        = 988;
const int MOTOR_MAX_RPM    = 200;
const int PID_UPDATE_FREQ  = 50;

Motor         motors[NUMBER_OF_WHEELS];
MotorEncoder  encoders[NUMBER_OF_WHEELS];
PIDController pidControllers[NUMBER_OF_WHEELS];
float         targetRPM[NUMBER_OF_WHEELS];

char   cmdBuffer[64];
uint8_t cmdIndex = 0;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUMBER_OF_WHEELS; i++) {
    motors[i]   = Motor(motorPins[i][0], motorPins[i][1]);
    encoders[i] = MotorEncoder(encoderPins[i][0], encoderPins[i][1]);
    pidControllers[i] = PIDController(&motors[i], &encoders[i],
                                      MOTOR_PPR, MOTOR_MAX_RPM, PID_UPDATE_FREQ);
    motors[i].begin();
    encoders[i].begin();
  }
}

void loop() {
  for (int i = 0; i < NUMBER_OF_WHEELS; i++) {
    pidControllers[i].computePWM();
  }

  static unsigned long lastSerialCheck = 0;
  unsigned long now = millis();
  if (now - lastSerialCheck >= 10) {
    handleSerial();
    lastSerialCheck = now;
  }
}

void handleSerial() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c != '\n' && c != '\r') {
      if (cmdIndex < sizeof(cmdBuffer) - 1) {
        cmdBuffer[cmdIndex++] = c;
      }
      continue;
    }

    cmdBuffer[cmdIndex] = '\0';
    if (cmdIndex > 0) {
      processCommand(cmdBuffer);
    }
    cmdIndex = 0;
  }
}

void processCommand(const char* cmd) {
  char command = cmd[0];

  switch (command) {
    case 'e': sendEncoderData();        break;
    case 'm': parsePWM(cmd + 1);        break;
    case 'v': parseTargetRPM(cmd + 1);  break;
    case 'p': parsePID(cmd + 1);        break;
    case 't': sendTestInfo();           break;
    default:                             break;
  }
}

int parseIntList(const char* s, int* out, int maxCount) {
  int count = 0;
  while (*s && count < maxCount) {
    while (*s == ',' || *s == ' ') s++;
    if (!*s) break;
    int val;
    if (sscanf(s, "%d", &val) != 1) break;
    out[count++] = val;
    while (*s && *s != ',') s++;
  }
  return count;
}

int parseFloatList(const char* s, float* out, int maxCount) {
  int count = 0;
  while (*s && count < maxCount) {
    while (*s == ',' || *s == ' ') s++;
    if (!*s) break;
    float val;
    if (sscanf(s, "%f", &val) != 1) break;
    out[count++] = val;
    while (*s && *s != ',') s++;
  }
  return count;
}

// m,100,100,100,100
void parsePWM(const char* cmd) {
  int values[NUMBER_OF_WHEELS];
  int count = parseIntList(cmd + 1, values, NUMBER_OF_WHEELS);
  for (int i = 0; i < count; i++) {
    motors[i].setPWM(values[i]);
  }
}

// v,100,100,100,100
void parseTargetRPM(const char* cmd) {
  float values[NUMBER_OF_WHEELS];
  int count = parseFloatList(cmd + 1, values, NUMBER_OF_WHEELS);
  for (int i = 0; i < count; i++) {
    targetRPM[i] = values[i];
    pidControllers[i].setTargetRPM(values[i]);
  }
}

// p,1.2,0.1,0.05
void parsePID(const char* cmd) {
  float kp, ki, kd;
  if (sscanf(cmd + 1, ",%f,%f,%f", &kp, &ki, &kd) == 3) {
    for (int i = 0; i < NUMBER_OF_WHEELS; i++) {
      pidControllers[i].setPIDParameters(kp, ki, kd);
    }
  }
}

void sendEncoderData() {
  Serial.print('e');
  for (int i = 0; i < NUMBER_OF_WHEELS; i++) {
    Serial.print(',');
    Serial.print(encoders[i].getCount());
  }
  Serial.println();
}

void sendTestInfo() {
  for (int i = 0; i < NUMBER_OF_WHEELS; i++) {
    pidControllers[i].showTestInfo();
  }
}
