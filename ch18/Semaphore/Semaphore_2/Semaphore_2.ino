SemaphoreHandle_t xSem = xSemaphoreCreateBinary();  // 建立2進位旗號

void task1(void *pvParam) {
  while (1) {
    xSemaphoreTake(xSem, portMAX_DELAY);  // 無限期等待取得旗號
    Serial.println("++++++++++++++++++");    // 取得旗號才會執行這一行
    xSemaphoreGive(xSem);    // 任務執行完畢，釋出旗號。
    vTaskDelay(1);
  }
}

void task2(void *pvParam) {
  while (1) {
    xSemaphoreTake(xSem, portMAX_DELAY);  // 無限期等待取得旗號
    Serial.println("------------------");          // 取得旗號才會執行這一行
    xSemaphoreGive(xSem);                // 任務執行完畢，釋出旗號。
    vTaskDelay(1);
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreate(task1, "task 1", 1500, NULL, 1, NULL);
  xTaskCreate(task2, "task 2", 1500, NULL, 1, NULL);
  // 釋出旗號，由任務調度器決定把旗號交給哪個任務。
  xSemaphoreGive(xSem);
}

void loop() {}
