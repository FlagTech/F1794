#define BITS 10
#define BUZZER_PIN 22

void setup() {
  ledcSetup(0, 20000, BITS);
  ledcAttachPin(BUZZER_PIN, 0);
}

void loop() {
  ledcWriteNote(0, NOTE_E, 5);  // 通道0的頻率設成659Hz
  delay(500);
  ledcWriteNote(0, NOTE_A, 4);  // 通道0的頻率設成440Hz
  delay(500);
}
