#include <DHT.h>
#include <SD.h>
#include <WiFi.h>
#include <time.h>

#define DHTPIN 16      // DHT11的接腳
#define DHTTYPE DHT11  // DHT感測器類型
#define WAKE_TIME 10*60*1000000L  // 10分鐘後喚醒

const char* ssid = "Wi-Fi網路名稱";
const char* password = "Wi-Fi網路密碼";

const char* ntpServer = "time.windows.com";
const uint16_t utcOffest = 28800;   // 台北時間（UTC+8）
const uint8_t daylightOffset = 0;

DHT dht(DHTPIN, DHTTYPE);

void saveData() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  time_t now = time(NULL);
  struct tm *localtm = localtime(&now);
  Serial.println(localtm, "時間：%Y/%m/%d %H:%M:%S");
  Serial.printf("溫度：%.1f°C、濕度：%.1f%%\n", t, h);

  File file = SD.open("/logo.txt", "a");
  if (!file) {
    Serial.println("無法開啟檔案～");
    return;
  } 
  file.print(localtm, "%Y/%m/%d %H:%M:%S");
  file.printf(",%.1f°C,%.1f%%\n", t, h);
  file.close();
}

void setup() {
  Serial.begin(115200);

  if (!SD.begin()) {
    Serial.println("無法掛載SD記憶卡～");
    return;
  }
  
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("沒有插入SD記憶卡～");
    return;
  }
  
  WiFi.mode(WIFI_STA);      // 設定成STA模式
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP位址：");
  Serial.println(WiFi.localIP());

  // 設置並取得網路時間
  configTime(utcOffest, daylightOffset, ntpServer);
  dht.begin();
  delay(2000);
  saveData();
  delay(100);
  esp_sleep_enable_timer_wakeup(WAKE_TIME);
  Serial.println("睡覺了～");
  delay(100);
  esp_deep_sleep_start();
}

void loop() { }
