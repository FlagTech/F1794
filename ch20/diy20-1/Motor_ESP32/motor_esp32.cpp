#include "motor_esp32.h"

Motor::Motor(int8_t ENA, int8_t ENB, int8_t IN1, int8_t IN2, int8_t IN3, int8_t IN4)
{
  this->ENA = ENA;
  this->ENB = ENB;
  this->IN1 = IN1;
  this->IN2 = IN2;
  this->IN3 = IN3;
  this->IN4 = IN4;

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  // 設置PWM輸出
  ledcSetup(PWM_CHANNEL_1, PWM_FREQ, BITS);
  ledcSetup(PWM_CHANNEL_2, PWM_FREQ, BITS);
  ledcAttachPin(ENA, PWM_CHANNEL_1);
  ledcAttachPin(ENB, PWM_CHANNEL_2);
  ledcWrite(PWM_CHANNEL_1, dutyCycle);
  ledcWrite(PWM_CHANNEL_2, dutyCycle);
}

void Motor::drive(Modes mode, int pwmA, int pwmB)
{
  // 把PWM參數值限制在0~255範圍
  int8_t _pwmA = constrain(pwmA, 0, 255);
  int8_t _pwmB = constrain(pwmB, 0, 255);

  // 如果模式跟之前不同，先暫停馬達…
  if (this->mode != mode)
  {
    this->mode = mode; // 更新模式值
    // 停止馬達
    ledcWrite(PWM_CHANNEL_1, 0);
    ledcWrite(PWM_CHANNEL_2, 0);
    delay(200); // 暫停0.2 秒
  }

  switch (mode)
  {
  case FORWARD: // 前進
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    break;
  case BACKWARD: // 倒退
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    break;
  case LEFT: // 左轉
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    break;
  case RIGHT: // 右轉
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    break;
  case STOP: // 停止
  default:
    _pwmA = 0;
    _pwmB = 0;
    break;
  }
  // 驅動馬達
  ledcWrite(PWM_CHANNEL_1, _pwmA);
  ledcWrite(PWM_CHANNEL_2, _pwmB);
}
