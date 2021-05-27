#include <ESP32_Servo.h>

#define IN_X 32        // 可變電阻X（水平搖桿）的輸入腳
#define IN_Y 33        // 可變電阻Y（垂直搖桿）的輸入腳
#define OUT_X 12       // 伺服馬達X的輸出腳
#define OUT_Y 13       // 伺服馬達Y的輸出腳
#define size 5         // 資料陣列元素數量
#define middle size/2  // 資料陣列中間索引

Servo servoX, servoY;   // 宣告兩個伺服馬達程式物件

int valX[size] = {0, 0, 0, 0, 0};  // X軸資料陣列
int valY[size] = {0, 0, 0, 0, 0};  // Y軸資料陣列

int cmp (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void filter() {
  static byte i = 0;  // 陣列索引

  valX[i] = analogRead(IN_X);  // 水平（X）搖桿的輸入值
  valY[i] = analogRead(IN_Y);  // 垂直（Y）搖桿的輸入值  

  qsort(valX, size, sizeof(int), cmp);
  qsort(valY, size, sizeof(int), cmp);

  if (++i % size == 0) i = 0;
}

void control() {
   int posX, posY;       // 暫存類比輸入值的變數

   posX = map(valX[middle], 0, 1023, 0, 180);
   posY = map(valY[middle], 0, 1023, 0, 180);
 
   servoX.write(posX);    // 設定伺服馬達的旋轉角度
   servoY.write(posY);

   delay(15);             // 延遲一段時間，讓伺服馬達轉到定位。
}

void setup() {
  analogSetAttenuation(ADC_11db);  // 設定類比輸入電壓上限3.6V
  analogSetWidth(10);
  servoX.attach(OUT_X, 500, 2400);    // 設定伺服馬達的接腳
  servoY.attach(OUT_Y, 500, 2400);
}

void loop() {
   filter();
   control(); 
}
