#include <WiFi.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WebSocketsServer.h>

#define BITS 10
#define LED_PIN 26

const char *ssid = "Wi-Fi名稱";
const char *password = "Wi-Fi密碼";
const uint16_t ADC_RES = 1023;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void onSocketEvent(AsyncWebSocket *server,
                   AsyncWebSocketClient *client,
                   AwsEventType type,
                   void *arg,
                   uint8_t *data,
                   size_t len)
{

  switch (type) {
    case WS_EVT_CONNECT:
      printf("來自%s的用戶%u已連線\n", client->remoteIP().toString().c_str(), client->id());
      break;
    case WS_EVT_DISCONNECT:
      printf("用戶%u已離線\n", client->id());
      break;
    case WS_EVT_ERROR:
      printf("用戶%u出錯了：%s\n", client->id(), (char *)data);
      break;
    case WS_EVT_DATA:
      printf("用戶%u傳入資料：%s\n", client->id(), (char *)data);
      const size_t capacity = JSON_OBJECT_SIZE(2) + 20;
      DynamicJsonDocument doc(capacity);
      deserializeJson(doc, data);

      const char *device = doc["device"]; // "LED"
      int val = doc["val"];               // 資料值
      if (strcmp(device, "LED") == 0)
      {
        printf("PWM: %d\n", val);
      #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
        ledcWrite(LED_PIN, 1023 - val); // 輸出PWM
      #else
        ledcWrite(0, 1023 - val); // 輸出PWM
      #endif
      }
      break;
  }
}

void notifyClients() {
  const size_t capacity = JSON_OBJECT_SIZE(2);
  DynamicJsonDocument doc(capacity);

  doc["device"] = "LDR";
  doc["val"] = analogRead(A0);

  char data[30];
  serializeJson(doc, data);
  ws.textAll(data);
}

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);
  pinMode(LED_PIN, OUTPUT);

#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcAttachChannel(LED_PIN, 5000, BITS, 0);  // 接腳, 頻率, 解析度, 通道
#else
  ledcSetup(0, 5000, BITS);   // PWM預設為5KHz，10位元解析度。
  ledcAttachPin(LED_PIN, 0);
#endif

  digitalWrite(LED_PIN, HIGH);

  if (!SPIFFS.begin(true)) {
    Serial.println("無法載入SPIFFS記憶體");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  printf("\nIP位址：%s\n", WiFi.localIP().toString().c_str());

  server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
  server.serveStatic("/favicon.ico", SPIFFS, "/www/favicon.ico");
  server.onNotFound([](AsyncWebServerRequest * req) {
    req->send(404, "text/plain", "Not found");
  });

  ws.onEvent(onSocketEvent); // 附加事件處理程式
  server.addHandler(&ws);
  server.begin();
  Serial.println("HTTP伺服器開工了～");
}

void loop() {
  ws.cleanupClients();
  notifyClients();
  delay(5000);
}
