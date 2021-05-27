#include "WiFi.h"
const char *ssid = "ESP32AP";
const char *password = "12345678";

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  Serial.print("\nWi-Fi基地台的IP位址：");
  Serial.println(WiFi.softAPIP());   // 在序列埠監控視窗顯示IP位址
}

void loop() {}
