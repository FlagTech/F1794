#include <WiFi.h>  // 要引用此程式庫
#include <HTTPClient.h>

const char* ssid = "Wi-Fi名稱";
const char* password = "Wi-Fi密碼";
String API_KEY = "你的openWeather API Key";
String city = "Taipei,TW";

HTTPClient http;

void openWeather() {
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" +
               city + "&appid=" + API_KEY;
               
  if ((WiFi.status() == WL_CONNECTED)) {
    http.begin(url); // 指定連結網址
    int httpCode = http.GET();  // 發起連結請求

    if (httpCode > 0) { // 檢查HTTP回應代碼
      String payload = http.getString();
      Serial.printf("回應本體：%s\n", payload.c_str());
    } else {
      Serial.println("HTTP請求出錯了~");
    }

    http.end();
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nIP位址：");
  Serial.println(WiFi.localIP() );
  
  openWeather();
}

void loop() {
}
