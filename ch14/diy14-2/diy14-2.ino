#include <WiFi.h>
#include <Audio.h>
#include <DHT.h>

#define I2S_DOUT      26  // 接模組的DIN
#define I2S_BCLK      27  // 接模組的BCK
#define I2S_LRC       25  // 接模組的LCK
#define SW            22  // 接開關
#define LED           5   // 內建的LED
#define DHTPIN        13  // 接DHT11
#define DHTTYPE       DHT11

Audio audio;
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Wi-Fi名稱";
const char* pwd = "Wi-Fi密碼";

uint8_t debounceDelay = 20;
bool activate = false;

char* texts[] = {"讀書", "睡覺", "聽音樂", "滑手機", "外出走走"};
uint8_t totalTexts = sizeof(texts)/sizeof(texts[0]);

void audio_eof_speech(const char *info) {
  digitalWrite(LED, HIGH);
  activate = false;
  printf("「%s」播放結束\n", info);
}

void setup() {
  Serial.begin(115200);
  pinMode(SW, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  dht.begin();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWi-Fi已連線！");
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(12); // 0...21
}

void loop() {
  bool swState = digitalRead(SW);
  String msg;

  if (!activate && swState == LOW) {
    delay(debounceDelay);
    // 確認兩次開關值是否一致
    if (swState == digitalRead(SW)) {
      activate = true;
      digitalWrite(LED, LOW);
      // float h = dht.readHumidity();
      uint16_t t = dht.readTemperature();
      if (isnan(t)) {
        msg = "無法取得溫度值。";
      } else {
        uint8_t index = random(totalTexts);
        msg = "現在溫度攝氏" + String(t) + "度，真是" +
              texts[index] + "的好天氣啊！";
      }

      audio.connecttospeech(msg, "zh"); 
    }
  }

  audio.loop();
}
