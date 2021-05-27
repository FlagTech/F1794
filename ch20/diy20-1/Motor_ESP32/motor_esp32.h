#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

typedef enum
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  STOP
} Modes;

class Motor
{
  int8_t ENA, ENB, IN1, IN2, IN3, IN4;
  const int16_t PWM_FREQ = 2000;  // PWM頻率
  const int8_t PWM_CHANNEL_1 = 0; // PWM通道
  const int8_t PWM_CHANNEL_2 = 1; // PWM通道
  const int8_t BITS = 8;          // 8位元解析度
  int8_t dutyCycle = 0;           // 有效值：0~255
  Modes mode = STOP;              // 預設模式：停止

public:
  Motor(int8_t ENA, int8_t ENB, int8_t IN1, int8_t IN2, int8_t IN3, int8_t IN4);
  void drive(Modes mode, int pwmA = 0, int pwmB = 0);
};
#endif // MOTOR_H
