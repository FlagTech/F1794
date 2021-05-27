#define LED_BUILTIN 5   // 請自行修改內建的LED接腳

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  int8_t hall = hallRead();   // 讀取並暫存霍爾感測值

  if (hall > 20) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (hall < -20) {
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(100);
}
