#include <ArduinoJson.h>
#include <WiFi.h>  // 要引用此程式庫
#include <HTTPClient.h>

const char* ssid = "Wi-Fi名稱";
const char* password = "Wi-Fi密碼";
String API_KEY = "你的openWeather API Key";
String city = "Taipei,TW";

HTTPClient http;

String openWeather() {
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" +
               city + "&appid=" + API_KEY;
  String payload = "";

  if ((WiFi.status() == WL_CONNECTED)) {

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      payload = http.getString();
      Serial.printf("回應本體：%s\n", payload.c_str());
    } else {
      Serial.println("HTTP請求出錯了～");
    }

    http.end();
    return payload;
  }
}

void parseWeather(String json) {
  DynamicJsonDocument doc(800);

  deserializeJson(doc, json);
  JsonObject weather = doc["weather"][0];
  const char* weather_icon = weather["icon"];  // 取得圖示名稱
  JsonObject main = doc["main"];
  float temp = (float)main["temp"] - 273.15;

  Serial.printf("天氣圖示：%s\n", weather_icon);
  Serial.printf("攝氏溫度：%.1f\n", temp);
}


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nIP位址：");
  Serial.println(WiFi.localIP());

  String payload = openWeather();

  if (payload != "") {
    parseWeather(payload);
  }
}

void loop() {
}
