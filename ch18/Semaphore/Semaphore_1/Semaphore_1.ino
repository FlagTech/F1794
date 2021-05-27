void task1(void *pvParam) {
  while (1) {
    Serial.println("++++++++++++++++++");
    vTaskDelay(1);
  }
}

void task2(void *pvParam) {
  while (1) {
    Serial.println("------------------");
    vTaskDelay(1);
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreate(task1, "Task 1", 1500, NULL, 1, NULL);
  xTaskCreate(task2, "Task 2", 1500, NULL, 1, NULL);
}

void loop() {}
