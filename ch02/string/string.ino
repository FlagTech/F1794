// 參閱2-40頁
using namespace std;
string s = "hello world!";
string sn = "123.45"; // 字串格式數字

void setup()
{
  Serial.begin(115200);
  Serial.printf("字串長度：%d\n", s.length());
  string txt = s.substr(7, 2); // 取出部份字串
  Serial.printf("取出子字串：%s\n", txt.c_str());
  s.insert(6, "new "); // 插入字串
  Serial.printf("插入字串之後：%s\n", s.c_str());
  s.erase(6, 4); // 從索引6開始刪除4個字元
  Serial.printf("刪除一段字之後：%s\n", s.c_str());
  s.replace(0, 6, "cyber"); // 從索引0開始，用“cyber”替換6個字。
  Serial.printf("替換字串之後：%s\n", s.c_str());
  int n = s.find("world"); // 搜尋字串“world”
  Serial.printf("'world'的起始位置：%d\n", n);
  n = s.find("@"); // 看看字串是否包含 '@'
  if (n == string::npos)
  {
    Serial.println("找不到'@'");
  }
  else
  {
    Serial.printf("'@'的位置：%d\n", n);
  }
  n = atoi(sn.c_str()); // 把字串轉成整數，記得先取得C風格字串。
  Serial.printf("sn轉成整數：%d\n", n);
  float f = atof(sn.c_str()); // 把字串轉成浮點數
  Serial.printf("sn轉成浮點數：%f\n", f);
}

void loop() {}
