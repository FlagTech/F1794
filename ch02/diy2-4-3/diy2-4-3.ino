#define BITS 10
#define BUZZER_PIN 22  // 蜂鳴器接在腳22

int interval = 500;
int tones[2] = {650, 400};
byte toneSize = sizeof(tones) / sizeof(int);

void alarmSnd() {
  static uint8_t i = 0;   // 音調陣列索引，從0開始。
  ledcWriteTone(0, tones[i]);
  delay(interval);
  if (++i % toneSize == 0) {  // 若取到最後一個陣列元素…
    i = 0;               // 下次從第0個元素開始
  }
}

void setup() {
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcAttachChannel(BUZZER_PIN, 2000, BITS, 0);  // 接腳, 頻率, 解析度, 通道
#else
  ledcSetup(0, 2000, BITS);   // PWM預設為20KHz，10位元解析度。
  ledcAttachPin(BUZZER_PIN, 0);
#endif
}

void loop() {
  alarmSnd();
}
