#include <painlessMesh.h>
#include <DHT.h>

#define   MESH_PREFIX     "ESP32_Mesh"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555
#define   ROOT_ID         2935586441
#define   IS_ROOT         false     // 不是根節點
#define   LED_BUILTIN     2
#define   DHTPIN 16              // DHT11的資料接腳
#define   DHTTYPE         DHT11  // 感測器類型

String nodeName = "阿B";
uint16_t pwmVal = 0;
DHT dht(DHTPIN, DHTTYPE);



Scheduler userScheduler;
painlessMesh  mesh;
SimpleList<uint32_t> nodes;
void sendDHT();
// 每隔5秒傳送DHT11感測值
Task taskDHT( TASK_SECOND * 5 , TASK_FOREVER, sendDHT );

void sendDHT() {
  String json;
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  StaticJsonDocument<100> doc;
  doc["name"] = nodeName;
  doc["type"] = "DHT";
  doc["temp"] = t;
  doc["humid"] = h;

  serializeJson(doc, json);
  mesh.sendSingle( ROOT_ID, json );
  Serial.printf("送出訊息：%s\n", json.c_str());
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("收到訊息：%s\n", msg.c_str());
  DynamicJsonDocument doc(512);
  DeserializationError err = deserializeJson(doc, msg.c_str());
  if (err) {
    Serial.printf("JSON資料解析錯誤：%s\n", err.c_str());
    return;  // 離開函式
  }

  if (doc["type"] == "dimmer") {
    ledcWrite(0, doc["val"]);
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
    Serial.printf(" %u", *node);  // 顯示Mesh裡面的每個裝置的ID
    node++;
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  ledcSetup(0, 5000, 10);  // 通道0, 5KHz, 10位元
  ledcAttachPin(LED_BUILTIN, 0);    // 腳15, 通道0
  dht.begin();

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(receivedCallback);
  mesh.onNewConnection(newConnectionCallback);
  mesh.onChangedConnections(changedConnectionCallback);
  mesh.setRoot(IS_ROOT);
  mesh.setContainsRoot(true);

  userScheduler.addTask( taskDHT );
  taskDHT.enable();
  Serial.printf("我是%s，ID：%u\n", nodeName, mesh.getNodeId());
}

void loop() {
  mesh.update();
}
