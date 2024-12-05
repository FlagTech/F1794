#define BITS 10
#define BUZZER_PIN 22

void setup() {
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcAttachChannel(BUZZER_PIN, 2000, BITS, 0);  // 接腳, 頻率, 解析度, 通道
#else
  ledcSetup(0, 2000, BITS);   // PWM預設為20KHz，10位元解析度。
  ledcAttachPin(BUZZER_PIN, 0);
#endif
}

void loop() {
  ledcWriteNote(0, NOTE_E, 5);  // 通道0的頻率設成659Hz
  delay(500);
  ledcWriteNote(0, NOTE_A, 4);  // 通道0的頻率設成440Hz
  delay(500);
}
