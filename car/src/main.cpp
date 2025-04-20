#include "Arduino.h"
// 定义 LED 连接的引脚
const int ledPin = 36; 

void setup() {
  // 将 ledPin 配置为输出模式
  pinMode(ledPin, OUTPUT); 
}

void loop() {
  digitalWrite(ledPin, HIGH);  // 点亮 LED（HIGH 表示高电平）
  delay(1000);                // 延迟 1 秒
  digitalWrite(ledPin, LOW);   // 熄灭 LED（LOW 表示低电平）
  delay(1000);                // 延迟 1 秒
}