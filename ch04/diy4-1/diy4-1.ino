const byte INT_PIN = 19;
bool running = true;
volatile byte counter = 0;
volatile bool state = LOW;

void IRAM_ATTR ISR() {
  state = !state;
  counter ++;
  digitalWrite(LED_BUILTIN, state);
  Serial.println("是在哈囉？");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(INT_PIN, INPUT_PULLUP);
  attachInterrupt(INT_PIN, ISR, FALLING);
}

void loop() {
  if (running && counter >= 10) {
    running = false;
    detachInterrupt(INT_PIN);
    Serial.println("收工啦～");
  }
}
