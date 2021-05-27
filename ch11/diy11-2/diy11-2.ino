#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include "sleepTimer.h"  // 引用自訂的睡眠定時器

#define DHTPIN 16      // DHT11的資料接腳
#define DHTTYPE DHT11  // 感測器類型
#define MIN 30         // 整點喚醒的「分」
#define SEC 0           // 整點喚醒的「秒」
 
const char* ssid = "Wi-Fi網路ID";
const char* password = "Wi-Fi密碼";
String apiKey = "你的ThingSpeak Read API Key";

SleepTimer st(5);           // 建立睡眠定時物件，容許誤差5秒。
DHT dht(DHTPIN, DHTTYPE);  // 建立DHT11物件

void sendData() {
  float t = dht.readTemperature();  // 讀取溫度
  float h = dht.readHumidity();     // 讀取濕度

  HTTPClient http;
  String urlStr = "http://api.thingspeak.com/update?api_key=";
  urlStr += apiKey + "&field1=";
  urlStr += String(t);   // 溫度
  urlStr += "&field2=";
  urlStr += String(h);   // 濕度
  urlStr += " HTTP/1.1\n";

  http.begin(urlStr);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();
    Serial.printf("HTTP回應碼：%d、回應本體：%s\n", httpCode, payload);
  } else {
    Serial.println("HTTP請求出錯啦～");
  }
  http.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("IP位址：");
  Serial.println(WiFi.localIP());  // 顯示IP位址

  dht.begin();
  st.init(28800, 0);
  st.start(MIN, SEC, sendData);
}

void loop() { }
