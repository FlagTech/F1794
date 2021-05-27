typedef enum
{
  RELEASED,
  PRESSED,
  LONG_PRESSED,
  PRESSING,
  RELEASED_FROM_PRESS,
  RELEASED_FROM_LONGPRESS
} Status;

class Switch
{
private:
  byte _pin;
  bool _ON;
  long _pressTime = 0;
  long _debounceTime = 30;
  long _longPressTime = 500;
  long _lastHoldTime = 0;
  long _holdTime = 200;
  boolean _isPressed = false;
  boolean _isLongPressed = false;

public:
  Switch(byte pin, bool ON = HIGH, bool pullup = false)
  {
    _pin = pin;
    _ON = ON;

    if (pullup)
    {
      pinMode(_pin, INPUT_PULLUP);
    }
    else
    {
      pinMode(_pin, INPUT);
    }
  }

  Status check()
  {
    Status status = RELEASED;

    if (digitalRead(_pin) == _ON)
    {
      if (!_isPressed)
      {
        _isPressed = true;
        status = PRESSED;
        _pressTime = millis();
      }

      if (_isLongPressed && (millis() - _lastHoldTime >= _holdTime))
      {
        status = PRESSING;
        _lastHoldTime = millis();
      }

      if (!_isLongPressed && (millis() - _pressTime > _longPressTime))
      {
        _isLongPressed = true;
        status = LONG_PRESSED;
      }
    }
    else
    {
      if (_isPressed)
      {
        if (_isLongPressed == true)
        {
          _isLongPressed = false;
          _lastHoldTime = 0;
          status = RELEASED_FROM_LONGPRESS;
        }
        else if ((millis() - _pressTime) > _debounceTime)
        {
          status = RELEASED_FROM_PRESS;
        }
        _isPressed = false;
      }
    }

    return status;
  }
};
