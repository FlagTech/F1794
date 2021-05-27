typedef struct data {
  byte pin;  // 接腳
  int ms;    // 延遲時間
} LED;

void taskBlink(void *pvParam)  {
  LED *led = (LED *) pvParam;
  byte pin = led->pin;  // 取得接腳值
  int ms = led->ms;    // 取得延遲時間值
  
  pinMode(pin, OUTPUT);
  while (1) {
    digitalWrite(pin, !digitalRead(pin));
    vTaskDelay(pdMS_TO_TICKS(ms));
  }
}

void setup() {
  LED led1={21, 250};   // 定義結構變數
  LED led2={22, 500};
  
  xTaskCreate( taskBlink, "task1", 1000, (void *) &led1, 1 , NULL);
  xTaskCreate( taskBlink, "task2", 1000, (void *) &led2, 1 , NULL);
}

void loop() { }
