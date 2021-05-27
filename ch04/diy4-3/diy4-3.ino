portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
volatile bool state = 0;
hw_timer_t * timer;   // 宣告硬體計時器物件指標

void IRAM_ATTR onTimer() {  // 計時器中斷服務常式
  portENTER_CRITICAL(&mux);
  state = !state;
  digitalWrite(LED_BUILTIN, state);
  portEXIT_CRITICAL(&mux);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // 使用硬體計時器0建立1微秒週期的計時器。
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void loop() { }
