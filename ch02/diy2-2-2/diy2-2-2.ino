// ESP32 Arduino開發環境3.x版不再支援霍爾感測器

#define LED_BUILTIN 5   // 請自行修改內建的LED接腳

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
#if ESP_ARDUINO_VERSION >=  ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  printf("不支援霍爾感測器。\n");
#else
  int8_t hall = hallRead();   // 讀取並暫存霍爾感測值

  if (hall > 20) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (hall < -20) {
    digitalWrite(LED_BUILTIN, LOW);
  }
#endif
  delay(100);
}
