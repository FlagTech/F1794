#include <painlessMesh.h>
#include <ArduinoJson.h>

#define   MESH_PREFIX     "ESP32_Mesh"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555
#define   ROOT_ID         2935586441
#define   IS_ROOT         false     // 不是根節點
#define   LED_BUILTIN     2
#define   BITS 10        // 類比取樣位元
#define   ADC_RES 1023   // 10位元解析度

String nodeName = "笑CC";
uint16_t pwmVal = 0;

painlessMesh  mesh;
Scheduler userScheduler;

void checkLDR();
// 每隔10秒傳送光敏感測值
Task taskLDR( TASK_SECOND * 5 , TASK_FOREVER, checkLDR );

void checkLDR() {
  int16_t val = analogRead(A0);  // 讀取LDR

  String json = "{\"type\":\"LDR\",\"val\":" + String(val) + "}";
  mesh.sendSingle( ROOT_ID, json );
  Serial.printf("送出訊息：%s\n", json.c_str());
}

void receivedCallback( uint32_t from, String &msg ) {
  DynamicJsonDocument doc(512);
  DeserializationError err = deserializeJson(doc, msg.c_str());
  if (err) {
    Serial.printf("JSON資料解析錯誤：%s\n", err.c_str());
    return;  // 離開函式
  }

  if (doc["type"] == "dimmer") {  // "調光器"訊息
  #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    ledcWrite(LED_BUILTIN, doc["val"]);
  #else
    ledcWrite(0, doc["val"]);
  #endif
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("新的連線，nodeId: %u\n", nodeId);
  Serial.printf("新的連線，JSON結構:\n%s\n", mesh.subConnectionJson(true).c_str());
}

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);  // 設定類比輸入
  analogSetWidth(BITS);

#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcAttachChannel(LED_BUILTIN, 5000, BITS, 0);  // 接腳, 頻率, 解析度, 通道
#else
  ledcSetup(0, 5000, BITS);        // 設定PWM，通道0、5KHz、10位元
  ledcAttachPin(LED_BUILTIN, 0);   // 指定內建的LED接腳成PWM輸出
#endif

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(receivedCallback);
  mesh.setRoot(IS_ROOT);
  mesh.setContainsRoot(true);

  userScheduler.addTask( taskLDR );
  taskLDR.enable();
  Serial.printf("我是%s，ID：%u\n", nodeName, mesh.getNodeId());
}

void loop() {
  mesh.update();
}
