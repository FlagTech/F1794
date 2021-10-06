#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#define BITS 10

const char *ssid = "Wi-Fi名稱";
const char *password = "Wi-Fi密碼";
const uint16_t ADC_RES = 1023;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);

  if (!SPIFFS.begin(true)) {
    Serial.println("掛載SPIFFS時發生錯誤～");
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("IP位址：");
  Serial.println(WiFi.localIP()); // 顯示IP位址

  server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
  server.serveStatic("/favicon.ico", SPIFFS, "/www/favicon.ico");

  server.on("/sensors.json", HTTP_GET, [](AsyncWebServerRequest *req) {
    StaticJsonDocument<50> data;
    data["sens1"] = analogRead(A0);
    data["sens2"] = analogRead(A3);

    String resp;
    serializeJson(data, resp);
    req->send(200, "application/json", resp);
  });

  server.begin();
  Serial.println("HTTP伺服器開工了～");
}

void loop() {
}
