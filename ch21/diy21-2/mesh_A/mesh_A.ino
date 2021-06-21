#include <painlessMesh.h>
#include "rotary_switch.h"

#define   MESH_PREFIX     "ESP32_Mesh"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555
#define   IS_ROOT         true
#define   DIM_STEP        10

uint16_t  dimVal = 0;
String nodeName = "小A";

// 建立「旋轉開關」物件（CLK腳, DT腳, SW腳）
RotarySwitch rsw(19, 21, 22);

Scheduler userScheduler;
painlessMesh  mesh;
SimpleList<uint32_t> nodes;  // 節點結構串列
//std::list<uint32_t> nodes;

void checkDial();

Task taskDimmer( 0 , TASK_FOREVER, checkDial );

void sendDimmer(uint16_t val) {
  String json = "{\"type\":\"dimmer\",\"val\":" + String(val) + "}";
  mesh.sendBroadcast( json );
  Serial.printf("送出訊息：%s\n", json.c_str());
}

void checkDial() {
  switch (rsw.check()) {
    case RotarySwitch::TURN_RIGHT:
      if (dimVal+DIM_STEP <= 1023) dimVal += DIM_STEP;
      sendDimmer(dimVal);
      break;
    case RotarySwitch::TURN_LEFT:
      if (dimVal-DIM_STEP >= 0) dimVal -= DIM_STEP;
      sendDimmer(dimVal);
      break;
  }
}

// 接收其他節點的訊息
void receivedCallback( uint32_t from, String &msg ) {
  DynamicJsonDocument doc(200);  // 預留記憶體解析JSON
  DeserializationError err = deserializeJson(doc, msg.c_str());
  if (err) {
    Serial.printf("JSON資料解析錯誤：%s\n", err.c_str());
    return;  // 若解析JSON時出錯，則退出此函式。
  }

  if (doc["type"] == "DHT") {     // 溫濕度的訊息
    int8_t t = doc["temp"];
    int8_t h = doc["humid"];
    const char * n = doc["name"];
    Serial.printf("溫度：%u°C, 濕度：%u%%, 節點：%s, ID：%u\n", t, h, n, from);
  }

  if (doc["type"] == "LDR") {     // 光敏的訊息
    int16_t val = doc["val"];
    const char * n = doc["name"];
    Serial.printf("亮度：%u, 節點：%s, ID：%u\n", val, n, from);
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("新的連線，nodeId: %u\n", nodeId);
  Serial.printf("新的連線，JSON結構:\n%s\n", mesh.subConnectionJson(true).c_str());
}

void changedConnectionCallback() {
  Serial.printf("連線產生變化了\n");

  nodes = mesh.getNodeList();
  Serial.printf("節點數: %d\n", nodes.size());
  Serial.printf("連線列表: ");
  SimpleList<uint32_t>::iterator node = nodes.begin();
  while (node != nodes.end()) {
    Serial.printf(" %u", *node);
    node++;
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(receivedCallback);
  mesh.onNewConnection(newConnectionCallback);
  mesh.onChangedConnections(changedConnectionCallback);
  mesh.setRoot(IS_ROOT);
  mesh.setContainsRoot(true);

  userScheduler.addTask( taskDimmer );
  taskDimmer.enable();
  Serial.printf("我是%s，ID：%u\n", nodeName, mesh.getNodeId());
}

void loop() {
  mesh.update();
}
