// source： https://gitlab.com/painlessMesh/painlessMesh
#include "rotary_switch.h"
#include <painlessMesh.h>
#include <HTTPClient.h>   // 引用HTTP前端

#define   HOSTNAME        "HTTP_BRIDGE" // 自訂的AP連線名稱
#define   MESH_PREFIX     "ESP32_Mesh"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555
#define   IS_ROOT         true        // 根節點
#define   DIM_STEP        10

String thingspeak_key = "你的API KEY";
const char* ssid = "WiFi名稱";
const char* password = "WiFi密碼";
const char* server = "api.thingspeak.com";

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

// 傳送DHT11資料到ThingSpeak
void sendData(int8_t t, int8_t h) {
  HTTPClient http;

  String urlStr = "http://api.thingspeak.com/update?";
  urlStr += "api_key=" + thingspeak_key +
            "&field1=" + String(t) +   // 溫度
            "&field2=" + String(h) +   // 濕度
            " HTTP/1.1\n";

  http.begin(urlStr);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();
    printf("HTTP回應碼：%d、回應本體：%s\n", httpCode, payload);
  } else {
    Serial.println("HTTP請求出錯啦～");
  }
  http.end();
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

    sendData(t, h);  // 傳送DHT11資料到ThingSpeak
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

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION);  // set before init() so that you can see startup messages
  // mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
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
}

void loop() {
  mesh.update();

  if (myIP != getlocalIP()) {
    myIP = getlocalIP();
    Serial.println("網站伺服器IP位址：" + myIP.toString());
  }
}
