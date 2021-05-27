// 使用ESP32 D1 mini板
// 搭配Wemos官方的Motor馬達控制板
// 馬達控制板程式庫下載：https://docs.wemos.cc/en/latest/d1_mini_shiled/motor.html

#include <WEMOS_Motor.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define UART_SERVICE_UUID      "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_RX "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
// #define CHARACTERISTIC_UUID_TX "6e400003-b5a3-f393-e0a9-e50e24dcca9e"
#define BATT_SERVICE_UUID (uint16_t)0x180f // 電池服務UUID

bool deviceConnected = false;
uint8_t battLevel = 100; // 電量百分比
BLECharacteristic *pCharact_TX;
BLECharacteristic *pCharactBatt;  // 電量的特質

// 馬達驅動板的I2C位址：0x30、PWM頻率：1000Hz
Motor motorL(0x30, _MOTOR_A, 1000);
Motor motorR(0x30, _MOTOR_B, 1000);

class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      // 中斷連線時停止馬達
      motorR.setmotor(_STOP);
      motorL.setmotor(_STOP);
    }
};

class RXCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharact) {
      std::string rxVal = pCharact->getValue();
      Serial.printf("收到輸入值：%s\n", rxVal.c_str());

      if (rxVal == "w") {           // 直行
          motorR.setmotor(_CW, 50);
          motorL.setmotor(_CW, 50);
      } else if (rxVal == "a") {    // 左轉
          motorR.setmotor(_CW, 60);
          motorL.setmotor(_CW, 30);
      } else if (rxVal == "d") {    // 右轉
          motorR.setmotor(_CW, 30);
          motorL.setmotor(_CW, 60);
      } else if (rxVal == "x") {    // 後退
          motorR.setmotor(_CCW, 50);
          motorL.setmotor(_CCW, 50);
      } else {                      // 停止
          motorR.setmotor(_STOP);
          motorL.setmotor(_STOP);
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
  BLEService *pService = pServer->createService(UART_SERVICE_UUID);
  BLECharacteristic *pCharact_RX = pService->createCharacteristic(
                                     CHARACTERISTIC_UUID_RX,
                                     BLECharacteristic::PROPERTY_WRITE
                                   );

  pCharact_RX->setCallbacks(new RXCallbacks());

  BLEDescriptor *pDesc = new BLEDescriptor((uint16_t)0x2901);
  pDesc->setValue("控制小車的馬達");
  pCharact_RX->addDescriptor(pDesc);

  // 新建電池的BLE服務Service
  BLEService* pBattService = pServer->createService(BATT_SERVICE_UUID);
  
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
  if (deviceConnected) {
    pCharactBatt->setValue(&battLevel, 1);
    pCharactBatt->notify();
    delay(1000);
    Serial.printf("電量：%d%%\n", battLevel);

    battLevel--;
    if (int(battLevel) == 0) battLevel = 100;
  }
}
