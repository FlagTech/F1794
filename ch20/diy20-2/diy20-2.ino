#include <motor_esp32.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define UART_SERVICE  "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define RX_UUID       "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
#define BATT_SERVICE (uint16_t)0x180f // 電池服務UUID

bool deviceConnected = false;
uint8_t battLevel = 100; // 電量百分比
BLECharacteristic *pCharact_TX;
BLECharacteristic *pCharactBatt;  // 電量的特質

// Motor (ENA, ENB, IN1, IN2, IN3, IN4)
Motor motor(14, 27, 13, 12, 25, 26);

class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      // 中斷連線時停止馬達
      motor.drive(STOP);
    }
};

class RXCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharact) {
      std::string rxVal = pCharact->getValue();
      Serial.printf("收到輸入值：%s\n", rxVal.c_str());
      if (rxVal == "w") {           // 直行
        motor.drive(FORWARD, 50, 50);
      } else if (rxVal == "a") {    // 左轉
        motor.drive(LEFT, 60, 30);
      } else if (rxVal == "d") {    // 右轉
        motor.drive(RIGHT, 30, 60);
      } else if (rxVal == "x") {    // 後退
        motor.drive(BACKWARD, 50, 50);
      } else {                      // 停止
        motor.drive(STOP);
      }
    }
};

void setup() {
  Serial.begin(115200);

  BLEDevice::init("ESP32藍牙小車");  // 建立BLE裝置
  // 建立BLE伺服器
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  // 建立BLE服務
  BLEService *pService = pServer->createService(UART_SERVICE);
  BLECharacteristic *pCharact_RX = pService->createCharacteristic(
                                     RX_UUID,
                                     BLECharacteristic::PROPERTY_WRITE
                                   );

  pCharact_RX->setCallbacks(new RXCallbacks());

  BLEDescriptor *pDesc = new BLEDescriptor((uint16_t)0x2901);
  pDesc->setValue("控制小車的馬達");
  pCharact_RX->addDescriptor(pDesc);

  // 新建電池的BLE服務Service
  BLEService* pBattService = pServer->createService(BATT_SERVICE);

  pCharactBatt = pBattService->createCharacteristic((uint16_t)0x2A19,
                 BLECharacteristic::PROPERTY_READ |
                 BLECharacteristic::PROPERTY_NOTIFY);

  BLEDescriptor* pBattDesc = new BLEDescriptor((uint16_t)0x2901);
  pBattDesc->setValue("電量0~100");
  pCharactBatt->addDescriptor(pBattDesc);
  pCharactBatt->addDescriptor(new BLE2902());

  pService->start();  // 啟動服務
  pBattService->start();

  pServer->getAdvertising()->start();  // 開始廣告
  Serial.println("等待用戶端連線…");
}

void loop() {
  if (deviceConnected) {  // 若裝置已連線，則傳遞（虛構的）電量值。
    pCharactBatt->setValue(&battLevel, 1);
    pCharactBatt->notify();
    delay(1000);
    Serial.printf("電量：%d%%\n", battLevel);

    battLevel--;
    if (int(battLevel) == 0) battLevel = 100;
  }
}
