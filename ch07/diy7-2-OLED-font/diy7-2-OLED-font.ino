// 參閱7-15頁

#include <map>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <U8g2lib.h>
#include "weatherFont.h"

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

std::map<String, char> icon_map{
    {"01d", 'B'}, {"02d", 'H'}, {"03d", 'N'}, {"04d", 'Y'}, {"09d", 'R'}, {"10d", 'Q'}, {"11d", 'P'}, {"13d", 'W'}, {"50d", 'J'}, {"01n", 'C'}, {"02n", 'I'}, {"03n", '5'}, {"04n", '%'}, {"09n", '8'}, {"10n", '7'}, {"11n", '6'}, {"13n", '#'}, {"50n", 'K'}};

const char *ssid = "Wi-Fi名稱";
const char *password = "Wi-Fi密碼";
String API_KEY = "你的openWeather API Key";
String city = "Taipei,TW";

HTTPClient http;

void connectWiFi()
{
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nIP位址：");
  Serial.println(WiFi.localIP());
}

String openWeather()
{
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" +
               city + "&appid=" + API_KEY;
  String payload = "";

  if ((WiFi.status() != WL_CONNECTED))
  {
    connectWiFi();
  }
  else
  {
    http.begin(url);
    int httpCode = http.GET(); //Make the request

    if (httpCode > 0)
    {
      payload = http.getString();
      Serial.printf("回應本體：%s\n", payload.c_str());
    }
    else
    {
      Serial.println("HTTP請求出錯了~");
    }

    http.end();
  }
  return payload;
}

void displayWeather(String json)
{
  DynamicJsonDocument doc(1024);

  deserializeJson(doc, json);
  JsonObject weather = doc["weather"][0];
  const char *icon = weather["icon"];
  const char *city = doc["name"];

  JsonObject main = doc["main"];
  // 取得絕對溫度，然後轉成攝氏溫度。
  float temp = (float)main["temp"] - 273.15;
  int humid = (int)main["humidity"];

  Serial.printf("天氣圖示：%s\n", icon);
  Serial.printf("攝氏溫度：%.1f\n", temp);

  // 在OLED螢幕顯示天氣資訊
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_profont12_mr);
    u8g2.drawUTF8(0, 8, city);
    u8g2.setFont(u8g2_font_inr16_mf);
    u8g2.setCursor(60, 36);
    u8g2.print(String(temp, 1) + "\xb0");
    u8g2.setCursor(60, 62);
    u8g2.print(String(humid) + "%");
    u8g2.setFont(weatherFont);
    u8g2.setCursor(0, 62);
    u8g2.print(icon_map[icon]);
  } while (u8g2.nextPage());
}

void setup()
{
  Serial.begin(115200);
  connectWiFi();
  u8g2.begin();
}

void loop()
{
  String payload = openWeather();

  if (payload != "")
  {
    displayWeather(payload);
  }

  delay(50000);
}
