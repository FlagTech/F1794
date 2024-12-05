// ESP32 Arduino開發環境3.x版不再支援霍爾感測器

void setup() { }

void loop() {
#if ESP_ARDUINO_VERSION >=  ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  printf("不支援霍爾感測器。\n");
#else
  printf("%d\n", hallRead());
#endif
  delay(100);
}
