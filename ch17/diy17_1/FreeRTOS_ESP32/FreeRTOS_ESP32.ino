#define LED1 21  // 定義LED接腳
#define LED2 22

void taskA(void *pvParam)  {  // 宣告任務A
  pinMode(LED1, OUTPUT);   // LED接腳設成「輸出」
  while (1) {
    digitalWrite(LED1, !digitalRead(LED1));  // 切換接腳的電位
    vTaskDelay( pdMS_TO_TICKS(250) );    // 延遲250ms
  }
}

void taskB(void *pvParam) {  // 宣告任務B
  pinMode(LED2, OUTPUT);
  while (1) {
    digitalWrite(LED2, !digitalRead(LED2));
    vTaskDelay(500 / portTICK_PERIOD_MS);  // 延遲500ms
  }
}

void setup() {
  xTaskCreate( taskA, "taskA", 1000, NULL, 1 , NULL);
  xTaskCreate( taskB, "taskB", 1000, NULL, 1 , NULL);
}

void loop() {  }
