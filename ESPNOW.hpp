#ifndef ESPNOW_HPP_
#define ESPNOW_HPP_

#include <esp_now.h>
#include <WiFi.h>

// 接收端的MAC地址
uint8_t broadcastAddress[] = {0x30, 0xAE, 0xA4, 0x07, 0x0D, 0x64};

// 发送结构体类型
typedef struct struct_message {
 char DebugMes[32];
 char Num[8];
 float x;
 float y;
 float z;
 float Angle_Car;
 float Angle_Head;
 uint8_t Goods_ID;
 uint8_t Status_Claw;
 uint8_t Status_System;
} struct_message;

// 创建一个结构体变量
struct_message Esp_Data;

// 回调函数,函数将在发送消息时执行。此函数告诉我们信息是否成功发送;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // USBSerial.print("\r\nLast Packet Send Status:\t");
  // USBSerial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void Espnow_Init() { 
  // 设置WIFI模式为STA模式，即无线终端
  WiFi.mode(WIFI_STA);

  //  初始化ESP-NOW
  if (esp_now_init() != ESP_OK) {
    // USBSerial.println("Error initializing ESP-NOW");
    return;
  }

 //注册回调函数
  esp_now_register_send_cb(OnDataSent);
  
  // 注册通信频道
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  //通道
  peerInfo.encrypt = false;//是否加密为False
         
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    // USBSerial.println("Failed to add peer");
    return;
  }

  esp_now_send(broadcastAddress, (uint8_t *) &Esp_Data, sizeof(Esp_Data)); 
}
 
esp_err_t Espnow_Loop() {
  //设置要发送的值
//   strcpy(Data.a, "THIS IS A CHAR");

  //发送信息到指定ESP32上
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Esp_Data, sizeof(Esp_Data)); 
  return result;
}

#endif