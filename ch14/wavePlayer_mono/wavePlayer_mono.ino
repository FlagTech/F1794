// 請參閱14-26頁

#include <driver/i2s.h>
#include "sound.h"

const unsigned char *pcmData = rawData + 44;
const uint32_t pcmSize = sizeof(rawData) - 44;
uint32_t pcmIndex = 0;
const i2s_port_t i2s_num = I2S_NUM_0;

const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 16000,
    .bits_per_sample = (i2s_bits_per_sample_t)16,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // 中斷優先等級1
    .dma_buf_count = 2,
    .dma_buf_len = 256};

const i2s_pin_config_t i2s_pins = {
    .bck_io_num = 27,
    .ws_io_num = 25,
    .data_out_num = 26};

void setup()
{
  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
  i2s_set_pin(i2s_num, &i2s_pins);
}

void loop()
{
  uint8_t align = 2; // 區塊對齊值
  const unsigned char *pt;
  size_t bytesWritten;

  pt = pcmData + pcmIndex;

  i2s_write(i2s_num, pt, 2, &bytesWritten, 1000);
  pcmIndex += align;
  if (pcmIndex >= pcmSize)
  {
    pcmIndex = 0;
  }
}
