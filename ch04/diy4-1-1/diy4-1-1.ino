const byte INT_PIN = 19;
bool running = true;
volatile byte counter = 0;
volatile bool state = LOW;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR ISR(){
  portENTER_CRITICAL(&mux);
  state = !state;
  counter ++;
  digitalWrite(LED_BUILTIN, state);
  portEXIT_CRITICAL(&mux);
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
