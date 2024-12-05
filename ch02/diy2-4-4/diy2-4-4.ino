#define BITS 10
#define BUZZER_PIN 22  // 蜂鳴器接在腳22

typedef struct data {
  uint16_t pitch;    // 音高頻率
  uint16_t interval; // 間隔時間
} note;  // 自訂類型

note tones[] = {   // 宣告儲存「音調」結構體的陣列
  {650, 500},
  {400, 500}
};

byte toneSize = sizeof(tones) / sizeof(note);

void alarmSnd() {
  static uint8_t i = 0;
  ledcWriteTone(0, tones[i].pitch);
  delay(tones[i].interval);
  if (++i % toneSize == 0) {
    i = 0;
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
