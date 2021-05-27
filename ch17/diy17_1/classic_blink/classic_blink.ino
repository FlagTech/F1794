#define LED1 21  // 定義LED接腳
#define LED2 22  
unsigned long prevTime1 = 0;  // 紀錄前次時間
unsigned long prevTime2 = 0;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  unsigned long now = millis();  // 取得目前時間

  if (now - prevTime1 >= 250) {
    prevTime1 = now;
    digitalWrite(LED1, !digitalRead(LED1));
  }

  if (now - prevTime2 >= 500) {
    prevTime2 = now;
    digitalWrite(LED2, !digitalRead(LED2));
  }
}
