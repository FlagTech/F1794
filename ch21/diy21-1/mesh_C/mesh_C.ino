#include <painlessMesh.h>

#define   MESH_PREFIX     "ESP32_Mesh"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555
#define   IS_ROOT         false

String nodeName = "笑CC";
Scheduler userScheduler;
painlessMesh  mesh;

void sendMsg();

Task taskSendMsg( TASK_SECOND * 1 , TASK_FOREVER, sendMsg );

void sendMsg() {
  String msg = "我是" + nodeName + "，ID: " + mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMsg.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("收到來自 %u 的訊息：%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("新的連線，nodeId: %u\n", nodeId);
  Serial.printf("新的連線，JSON結構:\n%s\n", mesh.subConnectionJson(true).c_str());
}

void setup() {
  Serial.begin(115200);
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(receivedCallback);
  mesh.onNewConnection(newConnectionCallback);
  mesh.setRoot(IS_ROOT);
  mesh.setContainsRoot(true);

  userScheduler.addTask( taskSendMsg );
  taskSendMsg.enable();
  Serial.printf("我是%s，ID：%u\n", nodeName, mesh.getNodeId());
}

void loop() {
  mesh.update();
}
