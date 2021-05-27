// 參閱3-16頁

const byte LED1 = 22;
const byte LED2 = LED_BUILTIN; // 內建的LED
const byte SW_PIN = 23;        // 開關接腳

boolean LED1State = false;
boolean LED2State = false;

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
  long _pressTime = 0;
  long _debounceTime = 30;
  long _longPressTime = 500;
  long _lastHoldTime = 0;
  long _holdTime = 200;
  boolean _isPressed = false;
  boolean _isLongPressed = false;

public:
  byte swPin;
  bool ONState;
  bool pullup;

  Switch(byte pin, bool ON = HIGH, bool pullup = false)
  {
    swPin = pin;
    ONState = ON;

    if (pullup)
    {
      pinMode(pin, INPUT_PULLUP);
    }
    else
    {
      pinMode(pin, INPUT);
    }
  }

  Status check()
  {
    Status status = RELEASED;

    if (digitalRead(swPin) == ONState)
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

Switch sw(SW_PIN, LOW, true);

void setup()
{
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop()
{
  switch (sw.check())
  {
  case RELEASED_FROM_PRESS:
    LED2State = !LED2State;
    digitalWrite(LED2, LED2State);
    break;
  case LONG_PRESSED:
    LED1State = !LED1State;
    digitalWrite(LED1, LED1State);
    break;
  case PRESSING:
    Serial.println("hello!");
    break;
  }
}
