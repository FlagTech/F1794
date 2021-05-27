#include <WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "Wi-Fi網路名稱";
const char* password = "Wi-Fi密碼";

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);  // 內建的LED腳設成輸出模式
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println(".");
    delay(500);
  }
  Serial.print("IP位址：");
  Serial.println(WiFi.localIP());

  ArduinoOTA.setHostname("ESP32 IoT");
  ArduinoOTA.setPassword("12345");
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  // 加入閃爍LED的程式
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}
