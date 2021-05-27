#define BITS 10   // PWM輸出的解析度
#define ALARM_PERIOD 10*1000  // 警報持續時間（毫秒數）
#define BUZZER_PIN 12  // 蜂鳴器接腳
#define INT_PIN 24     // 中斷腳（接PIR或微波感測器的輸出）

// 警報聲相關
int interval = 500;
int tones[2] = {650, 400};
byte toneSize = sizeof(tones) / sizeof(int);
unsigned long prevTime = 0;

unsigned long alarmTime = 0;  // 發出警報的時間
volatile byte alarming = false;  // 是否觸發警報
bool issued = false;          // 是否已發出警報

void alarmSound() {
  static int i = 0;

  if (millis() - prevTime > interval) {
    prevTime = millis();  // 儲存前次時間

    ledcWriteTone(0, tones[i]);  // 通道0的頻率設成659Hz
    if (++i % toneSize == 0) {
      i = 0;
    }
  }
}

void IRAM_ATTR ISR() {  // 中斷服務常式
  alarming = true;
}

void setup() {
  ledcSetup(0, 2000, BITS);      // PWM預設為20KHz，10位元解析度。
  ledcAttachPin(BUZZER_PIN, 0);
  pinMode(INT_PIN, INPUT);     // 接PIR或微波感測模組的訊號輸出
  attachInterrupt(INT_PIN, ISR, RISING); 
}

void loop() {  
  if (alarming) {    // 若中斷發生，觸發警報了…
    if (!issued) {
      issued = true;
      alarmTime = millis();  // 紀錄目前時間
    }

    alarmSound();

    if (millis() - alarmTime > ALARM_PERIOD) {
      alarming = false;   // 解除警報
      issued = false;
      ledcWriteTone(0, 0);
    }
  }
}
