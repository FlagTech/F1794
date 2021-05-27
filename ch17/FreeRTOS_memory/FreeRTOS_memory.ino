#define LED1 21  // 定義LED接腳
#define LED2 22

TaskHandle_t handleA;     // 任務A的參照
uint16_t stackSize = 1000;   // 分配給任務的記憶體大小

void taskA(void *pvParam)  {
  pinMode(LED1, OUTPUT);
  while (1) {
    digitalWrite(LED1, !digitalRead(LED1));
    vTaskDelay( pdMS_TO_TICKS(250) );
  }
}

void taskB(void *pvParam) {
  pinMode(LED2, OUTPUT);
  while (1) {
    digitalWrite(LED2, !digitalRead(LED2));
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


void setup() {
  Serial.begin(115200);
  Serial.printf("堆積區大小：%u位元組\n", ESP.getHeapSize());
  Serial.printf("建立任務前的堆積：%u位元組\n", ESP.getFreeHeap());
  xTaskCreate( taskA, "task A", stackSize, NULL, 1 , &handleA);
  xTaskCreate( taskB, "task B", stackSize, NULL, 1 , NULL);
  Serial.printf("建立任務後的堆積：%u位元組\n", ESP.getFreeHeap());
}

void loop() {
  UBaseType_t m = stackSize - uxTaskGetStackHighWaterMark(handleA);
  Serial.printf("任務A佔用 %u位元組\n", m);
  vTaskDelay(1000);
}
