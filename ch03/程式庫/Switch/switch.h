/*
檢測開關被「按一下」和「長按」的行為
作者：小趙
版本：1.0.0
*/
#ifndef SWITCH_H
#define SWITCH_H
#include <Arduino.h>

class Switch
{
private:
  byte _pin;
  bool _ON;
  uint32_t _pressTime = 0;
  uint8_t _debounceTime = 30;
  uint16_t _longPressTime = 500;
  uint32_t _lastHoldTime = 0;
  uint16_t _holdTime = 200;
  bool _isPressed = false;
  bool _isLongPressed = false;

public:
  typedef enum : uint8_t
  {
    RELEASED,
    PRESSED,
    LONG_PRESSED,
    PRESSING,
    RELEASED_FROM_PRESS,
    RELEASED_FROM_LONGPRESS
  } Status;

  Switch(byte pin, bool ON, bool pullup);
  Switch::Status check();
};
#endif
