// 參閱12-29頁

class Display
{ // 建立「顯示器」類別
protected:
  int _x = 0;
  int _y = 0;

public:
  void setCursor(int x, int y)
  {
    _x = x;
    _y = y;
  };

  void print(String txt)
  {
    Serial.printf("在(%u, %u)顯示：%s\n", _x, _y, txt.c_str());
  };
};

class OLED : public Display
{ // 繼承「顯示器」的OLED類別
public:
  void drawLine()
  {
    Serial.println("繪製線條～");
  }
  void createBitmap()
  {
    Serial.println("輸出點陣圖～");
  }
};

class LCD : public Display
{ // 繼承「顯示器」的LCD類別
public:
  void createChar()
  {
    Serial.println("產生自訂符號～");
  }
  void backlight()
  {
    Serial.println("已開啟背光～");
  }
};

void setup()
{
  Serial.begin(115200);

  OLED oled;
  oled.setCursor(12, 34);
  oled.print("追憶似水年華");
  oled.drawLine();
  oled.createBitmap();
}

void loop() {}
