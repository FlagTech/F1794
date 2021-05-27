byte LED1 = 21;
byte LED2 = 22;

void taskBlink(void* pvParam)  {
  byte pin = *(byte *) pvParam;  // 將參數值轉型成「位元組」

  pinMode(pin, OUTPUT);
  while (1) {
    digitalWrite(pin, !digitalRead(pin));
    vTaskDelay(pdMS_TO_TICKS(500));  // 延遲500ms
  }
}

void setup() {
  xTaskCreate( taskBlink, "task1", 1000, (void *) &LED1, 1 , NULL);
  xTaskCreate( taskBlink, "task2", 1000, (void *) &LED2, 1 , NULL);
}
void loop() { }
