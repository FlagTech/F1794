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
  ledcSetup(0, 2000, BITS);
  ledcAttachPin(BUZZER_PIN, 0);
}

void loop() {
  alarmSnd();
}
