#define FRONT_LEFT_ENCODER_PIN_A 32 
#define FRONT_LEFT_ENCODER_PIN_B 33 

#define FRONT_RIGHT_ENCODER_PIN_A 5
#define FRONT_RIGHT_ENCODER_PIN_B 4 

#define REAR_LEFT_ENCODER_PIN_A 26
#define REAR_LEFT_ENCODER_PIN_B 25

#define REAR_RIGHT_ENCODER_PIN_A 18
#define REAR_RIGHT_ENCODER_PIN_B 19


#define FRONT_LEFT_MOTOR_PIN_A 27
#define FRONT_LEFT_MOTOR_PIN_B 23

#define FRONT_RIGHT_MOTOR_PIN_A 21
#define FRONT_RIGHT_MOTOR_PIN_B 22

#define REAR_LEFT_MOTOR_PIN_A 12
#define REAR_LEFT_MOTOR_PIN_B 13

#define REAR_RIGHT_MOTOR_PIN_A 16
#define REAR_RIGHT_MOTOR_PIN_B 17

int encoderPins[4][2] = {
    {FRONT_LEFT_ENCODER_PIN_A, FRONT_LEFT_ENCODER_PIN_B},
    {FRONT_RIGHT_ENCODER_PIN_A, FRONT_RIGHT_ENCODER_PIN_B},
    {REAR_LEFT_ENCODER_PIN_A, REAR_LEFT_ENCODER_PIN_B},
    {REAR_RIGHT_ENCODER_PIN_A, REAR_RIGHT_ENCODER_PIN_B}
};

int motorPins[4][2] = {
    {FRONT_LEFT_MOTOR_PIN_A, FRONT_LEFT_MOTOR_PIN_B},
    {FRONT_RIGHT_MOTOR_PIN_A, FRONT_RIGHT_MOTOR_PIN_B},
    {REAR_LEFT_MOTOR_PIN_A, REAR_LEFT_MOTOR_PIN_B},
    {REAR_RIGHT_MOTOR_PIN_A, REAR_RIGHT_MOTOR_PIN_B}
};

// ДВИГАТЕЛИ (все безопасные пины)
// #define FRONT_LEFT_MOTOR_PIN_A  27 /// 23 п
// #define FRONT_LEFT_MOTOR_PIN_B  13 /// 27 л
// #define FRONT_RIGHT_MOTOR_PIN_A 23 // 21 п
// #define FRONT_RIGHT_MOTOR_PIN_B 22 /// 22 п
// #define REAR_LEFT_MOTOR_PIN_A   16 // 15 п 
// #define REAR_LEFT_MOTOR_PIN_B   17 /// 13 л
// #define REAR_RIGHT_MOTOR_PIN_A  21 /// 16 п
// #define REAR_RIGHT_MOTOR_PIN_B  15 /// 17 п

// ЭНКОДЕРЫ (хорошие пины)
// #define FRONT_LEFT_ENCODER_PIN_A  32 /// 32 л
// #define FRONT_LEFT_ENCODER_PIN_B  33 /// 33 л
// #define FRONT_RIGHT_ENCODER_PIN_A 18 /// 5  п
// #define FRONT_RIGHT_ENCODER_PIN_B 19 /// 4  п
// #define REAR_LEFT_ENCODER_PIN_A   25 /// 26 л
// #define REAR_LEFT_ENCODER_PIN_B   26 /// 25 л
// #define REAR_RIGHT_ENCODER_PIN_A  5 /// 18 п
// #define REAR_RIGHT_ENCODER_PIN_B  4 /// 19 п

// СВОБОДНЫЕ ПИНЫ для датчиков:
// #define LINE_SENSOR_LEFT   34    // Только INPUT!
// #define LINE_SENSOR_RIGHT  35    // Только INPUT!
// #define IMU_SDA            21    // Если освободится
// #define IMU_SCL            22    // Если освободится

/*────────────────────────────────────────────────────────────┐
│                    ESP32 DevKit V1                          │
├─────────────────┬──────────────────┬────────────────────────┤
│ ДВИГАТЕЛИ (PWM) │ ШИМ + Направление│ GPIO25,26,27,33        │
├─────────────────┼──────────────────┼────────────────────────┤
│ A (Левый перед) │ PWM: GPIO25      │ DIR: GPIO32            │
│ B (Правый перед)│ PWM: GPIO26      │ DIR: GPIO33            │
│ C (Левый зад)   │ PWM: GPIO27      │ DIR: GPIO16            │
│ D (Правый зад)  │ PWM: GPIO17      │ DIR: GPIO18            │
├─────────────────┼──────────────────┼────────────────────────┤
│ ЭНКОДЕРЫ        │ A: GPIO19,21     │ B: GPIO22,23           │
│ Мотор A         │ A: GPIO19        │ B: GPIO22              │
│ Мотор B         │ A: GPIO21        │ B: GPIO23              │
│ Мотор C         │ A: GPIO5*        │ B: GPIO4*              │
│ Мотор D         │ A: GPIO2*        │ B: GPIO0* (осторожно)  │
├─────────────────┼──────────────────┼────────────────────────┤
│ ДАТЧИКИ         │                  │                        │
│ IMU (I2C)       │ SDA: GPIO21**    │ SCL: GPIO22**          │
│ Line sensors    │ GPIO34,35,36,39  │ Только ВХОД!           │
│ Bumpers         │ GPIO34-39        │ Только ВХОД!           │
└─────────────────┴──────────────────┴───────────────────────*/
