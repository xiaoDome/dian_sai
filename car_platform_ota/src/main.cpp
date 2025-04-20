#include <Arduino.h>
//#include <TFT_eSPI.h>
#include <math.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
#include "EMMC42V5.hpp"
#include "Chassis.hpp"
// #include <ArduinoMDNS.h> // 引入mDNS库
#define RX1 18
#define TX1 8

const char* ssid = "ESP32-c3_OTAdemo"; // AP的名称
const char* password = "123456789"; // AP的密码

IPAddress local_IP(192, 168, 4, 1); // 静态IP地址
IPAddress gateway(192, 168, 4, 1); // 网关IP地址


WebServer server(80);

//TFT_eSPI tft;

const char* updateIndex =
  "<html>"
  "<head>"
  "<meta charset=\"UTF-8\">"
  "<title>ESP32 OTA 更新</title>"
  "<style>"
  "body { font-family: Arial, sans-serif; text-align: center; }"
  "h1 { color: #333; }"
  "form { margin-top: 20px; }"
  "input[type=file] { display: block; margin: 20px auto; }"
  "input[type=submit] { margin-top: 20px; padding: 10px 20px; font-size: 18px; }"
  "</style>"
  "</head>"
  "<body>"
  "<h1>欢迎使用 ESP32 OTA 更新</h1>"
  "<form method='POST' action='/update' enctype='multipart/form-data'>"
  "<input type='file' name='update' accept='.bin'>"
  "<input type='submit' value='上传固件'>"
  "</form>"
  "</body>"
  "</html>";


void handleRoot() {
  server.sendHeader("Location", "/update");
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(9600); // 初始化串口，波特率为115200
  Serial.println("Booting...");

  // 将 ESP32 设置为 AP 模式并指定静态 IP 地址
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_IP, gateway, IPAddress(255, 255, 255, 0));
  
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP()); // 打印 ESP32 的 AP IP 地址

  // 设置服务器处理函数
  server.on("/", HTTP_GET, handleRoot); // 根路由重定向到 OTA 页面

  server.on("/update", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", updateIndex);
  });

  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");

    //动态显示结果
  String message = Update.hasError() ? "更新失败" : "更新成功。重新启动…";
  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  server.send(200, "text/html", "<span style='font-size: 24px;'>" + message + "</span>");


    delay(1000);
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload(); //用于处理上传的文件数据
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { // 以最大可用大小开始
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      // 将接收到的数据写入Update对象
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { // 设置大小为当前大小
        Serial.printf("Update Success: %u bytes\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });

  server.begin();
  Serial.println("HTTP server started");


  //程序逻辑 版本1.1
  //Serial.println();
  //Serial.println("NEW ESP32C3!!");
  // tft.begin();
  // tft.setRotation(3);
  // tft.setTextFont(2);
  // tft.fillScreen(TFT_BLACK);
  // tft.drawString("NEW ESP32C3!!", 0, 0);
  // tft.drawRect(2, 20, 100, 20, TFT_BROWN);

  motor_ser.begin(115200, SERIAL_8N1, RX1, TX1);
  
  delay(100);
  Chassis_Enable(true);
  delay(2000);  
  Chassis_Incremental_Position(300,300,0);
  delay(200);
  // Chassis_Incremental_Position(300,0,0);  
  // delay(200);
  // Chassis_Incremental_Position(300,0,0);  
  // delay(200);  
  // Chassis_Incremental_Position(0,0,360);
  // delay(200);  
  // Chassis_Incremental_Position(300,0,0);
  // delay(200);  
  // Chassis_Incremental_Position(300,0,0);  
  // delay(200);  
  // Chassis_Incremental_Position(300,0,0);  

}

void loop() {
  server.handleClient();

}

