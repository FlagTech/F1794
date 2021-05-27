#include <painlessMesh.h>
#include <ArduinoJson.h>

#define   MESH_PREFIX     "ESP32_Mesh"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555
#define   ROOT_ID         2935586441
#define   IS_ROOT         false     // 不是根節點
#define   LED_BUILTIN     2

String nodeName = "迪D";
uint16_t pwmVal = 0;

painlessMesh  mesh;
Scheduler userScheduler;

void receivedCallback( uint32_t from, String &msg ) {
  DynamicJsonDocument doc(512);
  DeserializationError err = deserializeJson(doc, msg.c_str());
  if (err) {
    Serial.printf("JSON資料解析錯誤：%s\n", err.c_str());
    return;  // 離開函式
  }

  if (doc["type"] == "dimmer") {  // "調光器"訊息
    ledcWrite(0, doc["val"]);   // 通道0, PWM輸出
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("新的連線，nodeId: %u\n", nodeId);
  Serial.printf("新的連線，JSON結構:\n%s\n", mesh.subConnectionJson(true).c_str());
}

void setup() {
  Serial.begin(115200);
  ledcSetup(0, 5000, 10);  // 通道0, 5KHz, 10位元
  ledcAttachPin(LED_BUILTIN, 0);

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(receivedCallback);
  mesh.setRoot(IS_ROOT);
  mesh.setContainsRoot(true);

  Serial.printf("我是%s，ID：%u\n", nodeName, mesh.getNodeId());
}

void loop() {
  mesh.update();
}
