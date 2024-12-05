#include <ESP_I2S.h>
#include <BluetoothA2DPSink.h>

const uint8_t I2S_SCK = 27;     // 時派
const uint8_t I2S_WS = 26;      // 左右聲道資料切換
const uint8_t I2S_SDOUT = 25;   // 資料輸出
I2SClass i2s;

BluetoothA2DPSink a2dp_sink(i2s);

void setup() {
    i2s.setPins(I2S_SCK, I2S_WS, I2S_SDOUT);
    if (!i2s.begin(I2S_MODE_STD, 44100, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO, I2S_STD_SLOT_BOTH)) {
      Serial.println("無法初始化I2S！");
      while (1); 
    }

    a2dp_sink.start("ESP32高傳真音響");
}
void loop() {}