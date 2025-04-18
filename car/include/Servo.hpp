#ifndef SERVO_HPP_
#define SERVO_HPP_

#include "PinDefine.hpp"
#include "ESP32SERVO.hpp"

_SERVO Servo_Arm(SERVO_ARM, 0, 270, 500, 2500);
_SERVO Servo_Goods(SERVO_GOODS, 1, 270, 500, 2500);
_SERVO Servo_Base(SERVO_BASE, 2, 270, 500, 2500);

#define _Servo_Arm_Start_Angle 180
#define _Servo_Arm_Grab_Angle 225
#define _Servo_Arm_Put_Angle 140
#define _Servo_Arm_Open_Angle 120

#define _Servo_Goods_Start_Angle 130
#define _Servo_Goods_ID1_Angle 72
#define _Servo_Goods_ID2_Angle 167
#define _Servo_Goods_ID3_Angle 255

#define _Servo_Base_Start_Angle 220
#define _Servo_Base_Front_Angle 25
#define _Servo_Base_Back_Angle 197

#define SERVO_ARM_ID 1
#define SERVO_GOODS_ID 2
#define SERVO_BASE_ID 3
void Servo_Write(int id, int angle);

/*
舵机初始化
*/
void Servo_Init() {
  Servo_Arm.setup();
  Servo_Goods.setup();
  Servo_Base.setup();
  delay(100);
  Servo_Arm.write(_Servo_Arm_Start_Angle);
  Servo_Goods.write(_Servo_Goods_Start_Angle);
  Servo_Base.write(_Servo_Base_Start_Angle);
  // Servo_Write(SERVO_BASE_ID, _Servo_Base_Start_Angle);
  delay(100);
}

// void Servo_Test(){
//   if(USBSerial.available()){
//     char _id = USBSerial.read();
//     int angle = USBSerial.parseInt();
//     switch(_id){
//       case 'a': Servo_Arm.write(angle); break;
//       case 'g': Servo_Goods.write(angle); break;
//       case 'b': Servo_Base.write(angle); break;
//     }
//   }
// }

/*
舵机控制函数
*/
void Servo_Write(int id, int angle) {
  int _delayTime = 7;
  switch (id) {
    case SERVO_ARM_ID:
      Servo_Arm.write(angle);
      break;
    case SERVO_GOODS_ID:
      Servo_Goods.write(angle);
      break;
    case SERVO_BASE_ID:
      static int last_angle = _Servo_Base_Start_Angle;
      if (last_angle <= angle) {
        for (int i = last_angle; i < angle; i++) {
          Servo_Base.write(i);
          delay(_delayTime);
        }
      } else if (last_angle > angle) {
        for (int i = last_angle; i > angle; i--) {
          Servo_Base.write(i);
          delay(_delayTime);
        }
      }
      last_angle = angle;
      // Servo_Base.write(angle);
      break;
      // default:
      //   break;
  }
}

/*
物料转盘舵机控制
id：转盘对应的id号
*/
void Servo_Select_Goods_ID(int id) {
  switch (id) {
    case 1:
      Servo_Goods.write(_Servo_Goods_ID1_Angle);
      break;
    case 2:
      Servo_Goods.write(_Servo_Goods_ID2_Angle);
      break;
    case 3:
      Servo_Goods.write(_Servo_Goods_ID3_Angle);
      break;
    default:
      break;
  }
}

/*
夹爪舵机抓取
*/
void Servo_Grab() {
  Servo_Arm.write(_Servo_Arm_Grab_Angle);
  delay(100);
}

/*
夹爪舵机放置
*/
void Servo_Put() {
  Servo_Arm.write(_Servo_Arm_Put_Angle);
  delay(50);
}

/*
抬升机构向前
*/
void Servo_Turn_to_Front() {
  Servo_Write(SERVO_BASE_ID, _Servo_Base_Front_Angle);
  delay(50);
}

/*
抬升机构向后
*/
void Servo_Turn_to_Back() {
  Servo_Write(SERVO_BASE_ID, _Servo_Base_Back_Angle);
  delay(50);
}

#endif