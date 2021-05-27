#include "rotary_switch.h"
#include <painlessMesh.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WebSocketsServer.h>
#define   HOSTNAME        "HTTP_BRIDGE" // 自訂的AP連線名稱
#define   MESH_PREFIX     "ESP32_Mesh"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555
#define   IS_ROOT         true        // 根節點
#define   DIM_STEP        10

const char* ssid = "Wi-Fi名稱";
const char* password = "Wi-Fi密碼";

uint16_t  dimVal = 0;
String nodeName = "小A";

// 建立「旋轉開關」物件（CLK腳, DT腳, SW腳）
RotarySwitch rsw(19, 21, 22);

Scheduler userScheduler;
painlessMesh  mesh;
SimpleList<uint32_t> nodes;
IPAddress myIP(0, 0, 0, 0);
IPAddress myAPIP(0,0,0,0);
IPAddress getlocalIP();

AsyncWebServer server(80); // 建立HTTP伺服器物件
AsyncWebSocket ws("/ws");  // 建立WebSocket物件

void onSocketEvent(AsyncWebSocket *server,
                   AsyncWebSocketClient *client,
                   AwsEventType type,
                   void *arg,
                   uint8_t *data,
                   size_t len)
{

  switch (type)
  {
    case WS_EVT_CONNECT:
      Serial.printf("來自%s的用戶%u已連線\n", client->remoteIP().toString().c_str(), client->id());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("用戶%u已離線\n", client->id());
      break;
    case WS_EVT_ERROR:
      Serial.printf("用戶%u出錯了：%s\n", client->id(), (char *)data);
      break;
    case WS_EVT_DATA:
      //Serial.printf("用戶%u傳入資料：%s\n", client->id(), (char *)data);
      data[len] = 0;
      String json = "{\"type\":\"dimmer\",\"val\":" + String((char *)data) + "}";
      mesh.sendBroadcast( json );  // 廣播調光器數值
      Serial.printf("送出訊息：%s\n", json.c_str());
      break;
  }
}

void checkDial();
Task taskDimmer( 0 , TASK_FOREVER, checkDial );

void sendDimmer(uint16_t val) {
  String json = "{\"type\":\"dimmer\",\"val\":" + String(val) + "}";
  mesh.sendBroadcast( json );  // 廣播調光器數值
  Serial.printf("送出訊息：%s\n", json.c_str());
}

void checkDial() {
  switch (rsw.check()) {
    case RotarySwitch::TURN_RIGHT:
      if (dimVal + DIM_STEP <= 1023) dimVal += DIM_STEP;
      sendDimmer(dimVal);
      break;
    case RotarySwitch::TURN_LEFT:
      if (dimVal - DIM_STEP >= 0) dimVal -= DIM_STEP;
      sendDimmer(dimVal);
      break;
  }
}

void receivedCallback( uint32_t from, String &msg ) {
  DynamicJsonDocument doc(100);
  DeserializationError err = deserializeJson(doc, msg.c_str());
  if (err) {
    Serial.printf("JSON資料解析錯誤：%s\n", err.c_str());
    return;  // 離開函式
  }

  if (doc["type"] == "DHT") {  // 溫濕度的訊息
    int8_t t = doc["temp"];
    int8_t h = doc["humid"];
    const char * n = doc["name"];
    ws.textAll(msg.c_str());     // 轉傳JSON字串給Web前端
    Serial.printf("溫度：%u°C, 濕度：%u%%, 節點：%s, ID：%u\n", t, h, n, from);
  }

  if (doc["type"] == "LDR") {  // 亮度的訊息
    int8_t light = doc["light"];
    Serial.printf("亮度：%u, 節點：%u\n", light, from);
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

IPAddress getlocalIP() {
  return IPAddress(mesh.getStationIP());
  //return IPAddress(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("無法載入SPIFFS記憶體");
    return;
  }
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION);  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(receivedCallback);
  mesh.onNewConnection(newConnectionCallback);
  mesh.onChangedConnections(changedConnectionCallback);
  mesh.stationManual(ssid, password);
  mesh.setHostname(HOSTNAME);
  myAPIP = IPAddress(mesh.getAPIP());
  Serial.println("AP模式的IP位址：" + myAPIP.toString());

  mesh.setRoot(IS_ROOT);
  mesh.setContainsRoot(true);

  userScheduler.addTask( taskDimmer );
  taskDimmer.enable();
  Serial.printf("我是%s，ID：%u\n", nodeName, mesh.getNodeId());

  server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
  server.serveStatic("/favicon.ico", SPIFFS, "/www/favicon.ico");
  // 查無此頁
  server.onNotFound([](AsyncWebServerRequest * req) {
    req->send(404, "text/plain", "Not found");
  });

  ws.onEvent(onSocketEvent); // 附加事件處理程式
  server.addHandler(&ws);
  server.begin(); // 啟動網站伺服器
}

void loop() {
  mesh.update();

  if (myIP != getlocalIP()) {
    myIP = getlocalIP();
    Serial.println("網站伺服器IP位址：" + myIP.toString());
  }
}
