#include <WiFi.h>
#include <Audio.h>

#define I2S_DOUT      25  // 接模組的DIN
#define I2S_BCLK      27  // 接模組的BCK
#define I2S_LRC       26  // 接模組的LCK
#define MAX_VOL       21  // 最高音量
#define VOL_UP        22  // 音量（升）腳
#define VOL_DOWN      19  // 音量（降）腳

Audio audio;

const char* ssid = "Wi-Fi名稱";
const char* pwd = "Wi-Fi密碼";

uint8_t volume = 10;    // 初始音量

void setup() {
  Serial.begin(115200);
  pinMode(VOL_UP, INPUT_PULLUP);
  pinMode(VOL_DOWN, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("已連接Wi-Fi");
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(volume); // 0...21
  audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.mp3");
  // ICRT電台
  // audio.connecttohost("https://live.leanstream.co/ICRTFM-MP3");
  // TED Talks
  //audio.connecttohost("https://tunein.streamguys1.com/TEDTalks");
  // 新加坡Yes 93.3 FM
  //audio.connecttohost("https://19183.live.streamtheworld.com/YES933_PREM.aac");
  // BBC Radio 1
  //audio.connecttohost("http://bbcmedia.ic.llnwd.net/stream/bbcmedia_radio1_mf_p");
}

void loop()
{
  audio.loop();

  if (digitalRead(VOL_DOWN) == LOW) {
    delay(20);
    if (digitalRead(VOL_DOWN) == LOW) {
      if (volume > 0) {
        volume--;
      }
      audio.setVolume(volume);
    }
  }

  if (digitalRead(VOL_UP) == LOW) {
    delay(20);
    if (digitalRead(VOL_DOWN) == LOW) {
      if (volume < MAX_VOL) {
        volume++;
      }
      audio.setVolume(volume);
    }
  }
}

// 事件處理函式
void audio_info(const char *info){
    printf("聲音資訊：%s\n", info);
}
void audio_showstation(const char *info){
    printf("電台   ：%s\n", info);
}

void audio_showstreamtitle(const char *info){
    printf("串流標題：%s\n", info);
}

void audio_bitrate(const char *info){
    printf("位元率：%s\n", info);
}