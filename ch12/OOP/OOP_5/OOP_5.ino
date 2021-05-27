// 參閱12-38頁

class Shape
{
public:
  virtual float area() = 0;
  virtual float perimeter() = 0;
};

class Rect : public Shape
{
private:
  float _w, _h;

public:
  Rect(float w)
  {
    _w = w;
    _h = w;
  }
  Rect(float w, float h)
  {
    _w = w;
    _h = h;
  }
  float area()
  {
    return _w * _h; // 寬×高
  }

  float perimeter()
  {
    return (_w + _h) * 2; // (寬+高)×2
  }
};

class Circle : public Shape
{
private:
  float _r; // 私有的「半徑」屬性
public:
  Circle(float r)
  { // 建構式，接收一個「半徑」參數。
    _r = r;
  }
  float area()
  {                      // 計算圓面積
    return _r * _r * PI; // 半徑×半徑×圓周率
  }
  float perimeter()
  {                     // 計算圓周長
    return _r * 2 * PI; // 半徑×2×圓周率
  }
};

void setup()
{
  Serial.begin(115200);

  Shape *pt;
  Rect rect(12, 34); // 建立寬12, 高34的矩形物件
  Circle circle(56); // 建立半徑56的圓形物件

  pt = &rect; // 參照到矩形物件
  Serial.printf("矩形面積：%.1f\n", pt->area());
  Serial.printf("矩形周長：%.1f\n", pt->perimeter());

  pt = &circle; // 參照到圓形物件
  Serial.printf("圓形面積：%.1f\n", pt->area());
  Serial.printf("圓形周長：%.1f\n", pt->perimeter());
}

void loop() {}
