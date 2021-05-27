#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#define BITS 10

const char *ssid = "Wi-Fi網路名稱";
const char *password = "Wi-Fi密碼";
const uint16_t ADC_RES = 1023;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("IP位址：");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "text/html", "hello!");  // 回應首頁文件的請求
  });

  server.on("/sensors.json", HTTP_GET, [](AsyncWebServerRequest *req) {
    StaticJsonDocument<50> doc;
    doc ["sens1"] = analogRead(A0);
    doc ["sens2"] = analogRead(A3);
    String resp;
    serializeJson(doc, resp);
    req->send(200, "application/json", resp);
  });

  server.begin();
}

void loop() { }
