SemaphoreHandle_t xMutex = xSemaphoreCreateMutex(); // 建立互斥旗號物件

void printJob(char *pcStr) {   // 接收一個字元指標（字串）參數
  xSemaphoreTake(xMutex, portMAX_DELAY);
  Serial.println(pcStr);
  xSemaphoreGive(xMutex);
}

void myTask(void *pvParam) {
  char *pcStr = (char *)pvParam;

  while (1) {
    printJob(pcStr);
    vTaskDelay(1);
  }
}

void setup() {
  Serial.begin(115200);

  xTaskCreate(myTask, "task 1", 1500, (void *)"++++++++++++++++++", 1, NULL);
  xTaskCreate(myTask, "task 2", 1500, (void *)"------------------", 1, NULL);
}

void loop() {}
