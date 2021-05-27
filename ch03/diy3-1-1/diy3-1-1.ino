const byte LED1 = 22;
const byte LED2 = LED_BUILTIN;
const byte SW = 23;  // 開關腳

bool LED1State = false;
bool LED2State = false;

const bool ON = LOW;
long pressTime = 0;
long debounceTime = 30;
long longPressTime = 500;
long lastHoldTime = 0;
long holdTime = 200;

boolean isPressed = false;
boolean isLongPressed = false;

typedef enum {
  RELEASED,                 // 放開或「未按下」
  PRESSED,                  // 按下
  LONG_PRESSED,            // 長按
  PRESSING,                 // 持續按著
  RELEASED_FROM_PRESS,     // 從「按下」狀態放開，亦即「快按一下」。
  RELEASED_FROM_LONGPRESS // 從「長按」狀態放開
} SW_Status;

SW_Status checkSwitch(byte pin, bool ON=HIGH, bool pullup=false) {
  SW_Status status = RELEASED;  // 開關狀態，預設為「放開」。
 // 設定開關接腳的模式
 if (pullup) {
    pinMode(pin, INPUT_PULLUP);
 } else {
    pinMode(pin, INPUT);
 }

  if (digitalRead(pin) == ON) {
    if (!isPressed) {
      isPressed = true;
      status = PRESSED;   // 開關狀態設成「按下」
      pressTime = millis();
    }

    if (isLongPressed && (millis() - lastHoldTime >= holdTime)) {
      status = PRESSING;   // 開關狀態設成「持續按著」
      lastHoldTime = millis();
    }

    if (!isLongPressed && (millis() - pressTime > longPressTime)) {
      isLongPressed = true;
      status = LONG_PRESSED;  // 開關狀態設成「長按」
    }
  } else {
    if (isPressed) {
      if (isLongPressed == true) {
        isLongPressed = false;
        lastHoldTime = 0;
        status = RELEASED_FROM_LONGPRESS;  // 從「長按」狀態放開
      } else if ((millis() - pressTime) > debounceTime) {
        status = RELEASED_FROM_PRESS;      // 從「按下」狀態放開
      }
      isPressed = false;
    }
  }

  return status;   // 傳回開關狀態
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  //pinMode(SW, INPUT_PULLUP);
}

void loop() {
  // 檢查開關（接腳, 按下是低電位, 啟用上拉電阻）
  switch (checkSwitch(SW, LOW, true)) {
    case RELEASED_FROM_PRESS:  // 若是「按一下」
      LED2State = !LED2State;
      digitalWrite(LED2, LED2State); // 點亮或熄滅LED2
      break;
    case LONG_PRESSED:         // 若是「長按」
      LED1State = !LED1State;
      digitalWrite(LED1, LED1State); // 點亮或熄滅LED1
      break;
  }
}
