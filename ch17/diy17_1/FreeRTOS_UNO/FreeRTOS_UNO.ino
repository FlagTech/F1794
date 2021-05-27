#include <Arduino_FreeRTOS.h>

void taskA(void *pvParam)  {
  pinMode(8, OUTPUT);
  while (1) {
    digitalWrite(8, !digitalRead(8));
    vTaskDelay( 250 / portTICK_PERIOD_MS );
  }
}

void taskB(void *pvParam) {
  pinMode(13, OUTPUT);
  while (1) {
    digitalWrite(13, !digitalRead(13));
    vTaskDelay( 500 / portTICK_PERIOD_MS );
  }
}

void setup() {
  xTaskCreate( taskA, "task A", 100, NULL, 1 , NULL);
  xTaskCreate( taskB, "task B", 100, NULL, 1 , NULL);

  vTaskStartScheduler();   // 啟動任務調度器
}

void loop() { }
