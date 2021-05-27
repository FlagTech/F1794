TaskHandle_t handleB;  // 任務2的參照
long counter = 0;      // 計數器

void taskA(void *pvParam)  {  // 宣告任務A
  while (1) {
    printf("任務A進行中\n");
    counter ++;
    if (counter == 8) {
      vTaskPrioritySet( handleB, 3);  // 調高優先權
    }
    yield();   // 讓出資源
  }
}

void taskB(void *pvParam) {  // 宣告任務B
  while (1) {
    printf("終於輪到任務B上場啦～\n");
    if (counter == 16) {
      vTaskDelete( handleB);       // 刪除“taskB”任務
    }
    yield();   // 讓出資源
  }
}

void setup() {
  xTaskCreate( taskA, "my task A", 1000, NULL, 2 , NULL);   // 高優先權
  xTaskCreate( taskB, "my task B", 1000, NULL, 1 , &handleB);
}

void loop() {  }
