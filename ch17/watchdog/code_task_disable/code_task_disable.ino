void taskA(void *pvParam)  {  // 宣告任務A
  while (1) {
    printf("任務A進行中\n");
  }
}

void taskB(void *pvParam) {  // 宣告任務B
  while (1) {
    printf("終於輪到任務B上場啦～\n");
  }
}

void setup() {
  disableCore0WDT();
  xTaskCreate( taskA, "my task A", 1000, NULL, 2 , NULL); // 高優先權
  xTaskCreate( taskB, "my task B", 1000, NULL, 1 , NULL);
}

void loop() {  }
