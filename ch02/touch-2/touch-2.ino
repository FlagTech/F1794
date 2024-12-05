#define LED_BUILTIN 5    // 請自行修改內建的LED接腳
const byte touchPin = 15;   // 觸控腳
const int threshold = 20;    // 觸控臨界值
int touchValue;

void setup(){
  pinMode (LED_BUILTIN, OUTPUT);
}

void loop(){
  touchValue = touchRead(touchPin);  // 讀取觸控腳的值

  if (touchValue < threshold) {
    digitalWrite(LED_BUILTIN, LOW);
  } else{
    digitalWrite(LED_BUILTIN, HIGH);
  }
  delay(100);
}
