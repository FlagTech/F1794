#ifndef ESP32_BLE_KEYBOARD_MOUSE_H
#define ESP32_BLE_KEYBOARD_MOUSE_H
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "BleConnectionStatus.h"
#include "BLEHIDDevice.h"
#include "BLECharacteristic.h"
#include "Print.h"

#define MOUSE_LEFT 1     // 滑鼠左鍵
#define MOUSE_RIGHT 2    // 滑鼠右鍵
#define MOUSE_MIDDLE 4   // 滑鼠中鍵
#define MOUSE_BACK 8     // 滑鼠「回上一頁」
#define MOUSE_FORWARD 16 // 滑鼠「到下一頁」
#define MOUSE_ALL (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE) #For compatibility with the Mouse library

const uint8_t KEY_LEFT_CTRL = 0x80;   // 左CTRL鍵
const uint8_t KEY_LEFT_SHIFT = 0x81;  // 左Shift鍵
const uint8_t KEY_LEFT_ALT = 0x82;    // 左ALT鍵
const uint8_t KEY_LEFT_GUI = 0x83;    // 左GUI鍵（Windows, Command）
const uint8_t KEY_RIGHT_CTRL = 0x84;  // 右Ctrl鍵
const uint8_t KEY_RIGHT_SHIFT = 0x85; // 右Shift鍵
const uint8_t KEY_RIGHT_ALT = 0x86;   // 右Alt鍵
const uint8_t KEY_RIGHT_GUI = 0x87;   // 右GUI鍵

const uint8_t KEY_UP_ARROW = 0xDA;    // 上方向鍵
const uint8_t KEY_DOWN_ARROW = 0xD9;  // 下方向鍵
const uint8_t KEY_LEFT_ARROW = 0xD8;  // 左方向鍵
const uint8_t KEY_RIGHT_ARROW = 0xD7; // 右方向鍵
const uint8_t KEY_BACKSPACE = 0xB2;   // 退格鍵
const uint8_t KEY_TAB = 0xB3;         // Tab鍵
const uint8_t KEY_RETURN = 0xB0;      // Return/Enter鍵
const uint8_t KEY_ESC = 0xB1;         // ESC鍵
const uint8_t KEY_INSERT = 0xD1;      // 「插入」鍵
const uint8_t KEY_DELETE = 0xD4;      // 「刪除」鍵
const uint8_t KEY_PAGE_UP = 0xD3;     // 「上翻頁」鍵
const uint8_t KEY_PAGE_DOWN = 0xD6;   // 「下翻頁」鍵
const uint8_t KEY_HOME = 0xD2;        // home鍵
const uint8_t KEY_END = 0xD5;         // end鍵
const uint8_t KEY_CAPS_LOCK = 0xC1;   // 「大寫鎖定」鍵
const uint8_t KEY_F1 = 0xC2;          // F1功能鍵
const uint8_t KEY_F2 = 0xC3;
const uint8_t KEY_F3 = 0xC4;
const uint8_t KEY_F4 = 0xC5;
const uint8_t KEY_F5 = 0xC6;
const uint8_t KEY_F6 = 0xC7;
const uint8_t KEY_F7 = 0xC8;
const uint8_t KEY_F8 = 0xC9;
const uint8_t KEY_F9 = 0xCA;
const uint8_t KEY_F10 = 0xCB;
const uint8_t KEY_F11 = 0xCC;
const uint8_t KEY_F12 = 0xCD;
const uint8_t KEY_F13 = 0xF0;
const uint8_t KEY_F14 = 0xF1;
const uint8_t KEY_F15 = 0xF2;
const uint8_t KEY_F16 = 0xF3;
const uint8_t KEY_F17 = 0xF4;
const uint8_t KEY_F18 = 0xF5;
const uint8_t KEY_F19 = 0xF6;
const uint8_t KEY_F20 = 0xF7;
const uint8_t KEY_F21 = 0xF8;
const uint8_t KEY_F22 = 0xF9;
const uint8_t KEY_F23 = 0xFA;
const uint8_t KEY_F24 = 0xFB;

typedef uint8_t MediaKeyReport[2];

const MediaKeyReport KEY_MEDIA_NEXT_TRACK = {1, 0};                      // 下一首
const MediaKeyReport KEY_MEDIA_PREVIOUS_TRACK = {2, 0};                  // 上一首
const MediaKeyReport KEY_MEDIA_STOP = {4, 0};                            // 停止
const MediaKeyReport KEY_MEDIA_PLAY_PAUSE = {8, 0};                      // 播放/暫停
const MediaKeyReport KEY_MEDIA_MUTE = {16, 0};                           // 靜音
const MediaKeyReport KEY_MEDIA_VOLUME_UP = {32, 0};                      // 音量調高
const MediaKeyReport KEY_MEDIA_VOLUME_DOWN = {64, 0};                    // 音量降低
const MediaKeyReport KEY_MEDIA_WWW_HOME = {128, 0};                      // WWW首頁
const MediaKeyReport KEY_MEDIA_LOCAL_MACHINE_BROWSER = {0, 1};           // 開啟「我的電腦」
const MediaKeyReport KEY_MEDIA_CALCULATOR = {0, 2};                      // 計算機
const MediaKeyReport KEY_MEDIA_WWW_BOOKMARKS = {0, 4};                   // WWW書籤
const MediaKeyReport KEY_MEDIA_WWW_SEARCH = {0, 8};                      // WWW搜尋
const MediaKeyReport KEY_MEDIA_WWW_STOP = {0, 16};                       // WWW停止載入
const MediaKeyReport KEY_MEDIA_WWW_BACK = {0, 32};                       // WWW回上頁
const MediaKeyReport KEY_MEDIA_CONSUMER_CONTROL_CONFIGURATION = {0, 64}; // 選曲
const MediaKeyReport KEY_MEDIA_EMAIL_READER = {0, 128};                  // 開啟電郵軟體

//  Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;

class BleKeyboardMouse : public Print
{
private:
  uint8_t _buttons; // --- Mouse
  BleConnectionStatus *connectionStatus;
  BLEHIDDevice *hid;
  BLECharacteristic *inputKeyboard;
  BLECharacteristic *outputKeyboard;
  BLECharacteristic *inputMediaKeys;
  BLECharacteristic *inputMouse; // ---Mouse
  KeyReport _keyReport;
  MediaKeyReport _mediaKeyReport;
  void buttons(uint8_t b); // ---mouse
  void rawAction(uint8_t msg[], char msgSize);
  static void taskServer(void *pvParameter);

public:
  BleKeyboardMouse(std::string deviceName = "ESP32 BLE Keyboard Mouse", std::string deviceManufacturer = "Espressif", uint8_t batteryLevel = 100);
  void begin(void);
  void end(void);
  void sendReport(KeyReport *keys);
  void sendReport(MediaKeyReport *keys);
  size_t press(uint8_t k);
  size_t press(const MediaKeyReport k);
  size_t release(uint8_t k);
  size_t release(const MediaKeyReport k);
  size_t write(uint8_t c);
  size_t write(const MediaKeyReport c);
  size_t write(const uint8_t *buffer, size_t size);
  void releaseAll(void);
  //---------Mouse
  void click(uint8_t b = MOUSE_LEFT);
  void move(signed char x, signed char y, signed char wheel = 0, signed char hWheel = 0);
  void mousePress(uint8_t b = MOUSE_LEFT);   // press LEFT by default
  void mouseRelease(uint8_t b = MOUSE_LEFT); // release LEFT by default
  bool isPressed(uint8_t b = MOUSE_LEFT);    // check LEFT by default
  //---------Mouse
  bool isConnected(void);
  void setBatteryLevel(uint8_t level);
  uint8_t batteryLevel;
  std::string deviceManufacturer;
  std::string deviceName;
};

#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_KEYBOARD_MOUSE_H
