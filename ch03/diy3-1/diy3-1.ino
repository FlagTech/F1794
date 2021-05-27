const byte LED1 = 22;
const byte LED2 = LED_BUILTIN;
const byte SW = 23;

bool LED1State = false;
bool LED2State = false;

const bool ON = LOW;
uint32_t pressTime = 0;
uint8_t debounceTime = 30;
uint16_t longPressTime = 500;
uint32_t lastHoldTime = 0;
uint8_t holdTime = 200;

bool isPressed = false;
bool isLongPressed = false;

void checkSwitch() {
  if (digitalRead(SW) == LOW) {
    if (isPressed == false) {
      isPressed = true;
      pressTime = millis();
    }

    if ((millis() - pressTime > longPressTime) && (isLongPressed == false)) {
      isLongPressed = true;
      LED1State = !LED1State;
      digitalWrite(LED1, LED1State);
    }

    if (isLongPressed == true && (millis() - lastHoldTime >= holdTime)) {
      Serial.println("hold on!");
      lastHoldTime = millis();
    }

  } else {
    if (isPressed == true) {
      if (isLongPressed == true) {
        isLongPressed = false;
        lastHoldTime = 0;
      } else if ((millis() - pressTime) > debounceTime) {
        LED2State = !LED2State;
        digitalWrite(LED2, LED2State);
      }
      isPressed = false;
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(SW, INPUT_PULLUP);
}

void loop() {
  checkSwitch();
}
