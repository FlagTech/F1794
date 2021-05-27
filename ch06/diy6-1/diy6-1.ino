#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.begin("WiFi網路名稱", "密碼");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("\nIP位址：");
  Serial.println(WiFi.localIP());
  Serial.print("WiFi RSSI: ");
  Serial.println(WiFi.RSSI());
}

void loop() {
}
