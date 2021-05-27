// 參閱14-36頁

#include <driver/i2s.h>
#include "sound.h"

const unsigned char *pcmData = rawData + 44;
uint32_t pcmIndex = 0;
const i2s_port_t i2s_num = I2S_NUM_0;

struct wavStruct
{
  // RIFF識別區
  char riffTag[4];    // 包含"RIFF"
  uint32_t riffSize;  // PCM資料大小+36
  char riffFormat[4]; // 包含"WAVE"

  // 聲音格式區
  char fmtTag[4];       // 包含"fmt "
  uint32_t fmtSize;     // 聲音格式區的大小
  uint16_t audioFormat; // 聲音格式，1代表PCM
  uint16_t numChannels; // 聲道數
  uint32_t sampleRate;  // 取樣頻率
  uint32_t byteRate;    // 每秒取樣的位元組數
  uint16_t blockAlign;  // 區塊對齊
  uint16_t bitDepth;    // 每秒取樣的位元數

  // 資料區
  char dataTag[4];   // 包含"data"
  uint32_t dataSize; // PCM資料大小
} wavHeader;

const i2s_pin_config_t i2s_pins = {
    .bck_io_num = 27,
    .ws_io_num = 25,
    .data_out_num = 26};

// I2S configuration structures
const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 16000,
    .bits_per_sample = (i2s_bits_per_sample_t)16,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // 中斷優先等級1
    .dma_buf_count = 2,
    .dma_buf_len = 256};

void setup()
{
  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
  i2s_set_pin(i2s_num, &i2s_pins);

  memcpy(&wavHeader, rawData, 44);
  printf("聲道數：%d\n", wavHeader.numChannels);
  printf("位元對齊：%d\n", wavHeader.blockAlign);
  printf("資料大小：%d\n", wavHeader.dataSize);
}

void loop()
{
  uint8_t dual[4];
  uint8_t dataLen = 4; // 資料長度
  const unsigned char *pt;
  size_t bytesWritten;

  if (wavHeader.numChannels)
  {
    dual[0] = *(pcmData + pcmIndex);
    dual[1] = *(pcmData + pcmIndex + 1);
    dual[2] = dual[0];
    dual[3] = dual[1];
    pt = dual;
  }
  else
  {
    pt = pcmData + pcmIndex;
  }

  i2s_write(i2s_num, pt, dataLen, &bytesWritten, 1000);
  pcmIndex += wavHeader.blockAlign;
  if (pcmIndex >= wavHeader.dataSize)
  {
    pcmIndex = 0;
  }
}
