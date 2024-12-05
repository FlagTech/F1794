#define BITS 10
#define BUZZER_PIN 22  // 蜂鳴器接在腳22

void setup() {
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcAttachChannel(BUZZER_PIN, 2000, BITS, 0);  // 接腳, 頻率, 解析度, 通道
#else
  ledcSetup(0, 2000, BITS);   // PWM預設為20KHz，10位元解析度。
  ledcAttachPin(BUZZER_PIN, 0);
#endif
}

void loop() {
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcWriteTone(BUZZER_PIN, 659);  // 通道0的頻率設成659Hz
  delay(500);
  ledcWriteTone(BUZZER_PIN, 440);  // 通道0的頻率設成440Hz
  delay(500);
#else
  ledcWriteTone(0, 659);  // 通道0的頻率設成659Hz
  delay(500);
  ledcWriteTone(0, 440);  // 通道0的頻率設成440Hz
  delay(500);
#endif
}
