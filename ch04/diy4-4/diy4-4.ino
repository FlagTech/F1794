#define LED1 19        // 外接LED的接腳

volatile bool state0 = 0;   // 紀錄內建LED的狀態
volatile bool state1 = 0;   // 紀錄外接LED的狀態
volatile byte counter = 0;  // 計數器

portMUX_TYPE mux0 = portMUX_INITIALIZER_UNLOCKED; // 資源鎖0
portMUX_TYPE mux1 = portMUX_INITIALIZER_UNLOCKED; // 資源鎖1

hw_timer_t * timer0;   // 計時器物件0
hw_timer_t * timer1;

void IRAM_ATTR onTimer0() {
  portENTER_CRITICAL(&mux0);
  state0 = !state0;
  digitalWrite(LED_BUILTIN, state0);
  portEXIT_CRITICAL(&mux0);
}

void IRAM_ATTR onTimer1() {
  portENTER_CRITICAL(&mux1);
  state1 = !state1;
  digitalWrite(LED1, state1);
  if (++counter == 10) {       // 先累加計數值再跟10比較
    if (timer1 != NULL) {
      timerAlarmDisable(timer1);
      timerDetachInterrupt(timer1); 
      timerEnd(timer1);
      timer1 = NULL;
    }
  }
  portEXIT_CRITICAL(&mux1);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED1, OUTPUT);


  timer0 = timerBegin(0, 80, true);  // 設置計時器0
  timer1 = timerBegin(1, 80, true);  // 設置計時器1

  timerAttachInterrupt(timer0, &onTimer0, true);
  timerAttachInterrupt(timer1, &onTimer1, true);

  timerAlarmWrite(timer0, 1000000, true);   // 1000ms（1秒）
  timerAlarmWrite(timer1, 500000, true);    // 500ms（0.5秒）

  timerAlarmEnable(timer0);  // 啟動計時器
  timerAlarmEnable(timer1);
}

void loop() { }
