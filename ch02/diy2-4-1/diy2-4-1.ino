#define BITS 10
#define BUZZER_PIN 22  // 蜂鳴器接在腳22

void setup() {
  ledcSetup(0, 20000, BITS);   // PWM預設為20KHz，10位元解析度。
  ledcAttachPin(BUZZER_PIN, 0);
}

void loop() {
  ledcWriteTone(0, 659);  // 通道0的頻率設成659Hz
  delay(500);
  ledcWriteTone(0, 440);  // 通道0的頻率設成440Hz
  delay(500);
}
