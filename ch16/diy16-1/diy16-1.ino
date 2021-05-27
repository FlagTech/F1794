const byte CLK_PIN = 19;   // CLK接腳
const byte DT_PIN = 21;    // DT接腳
int counter = 0;           // 計數值
bool now = 0;         // 暫存CLK腳的目前值
bool prev = 0;         // 暫存CLK腳的前次值

void setup() {
  Serial.begin(115200);
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);

  prev = digitalRead(CLK_PIN);      // 讀取CLK腳的值
}
void loop() {
  now = digitalRead(CLK_PIN);

  if (now != prev) {
    if (digitalRead(DT_PIN) != now) {
      counter ++;
    } else {
      counter --;
    }
    Serial.printf("計數：%d\n", counter);
  }
  prev = now;
}
