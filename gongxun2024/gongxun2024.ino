#include "PinDefine.hpp"
#include "ESPNOW.hpp"
#include "EMMC42V5.hpp"
#include "Servo.hpp"
#include "Chassis.hpp"


bool _light = true;

int color_data[6];

int state = 0;
void setup() {
  // Espnow_Init();
  delay(1000);
  //系统状态灯初始化
  analogWrite(LIGHT, 100);

  //风扇初始化
  analogWrite(FAN, 100);

  //调试串口初始化
  21USBSerial.begin(115200);

  //树莓派串口初始化
  Serial.begin(115200);

  //备用串口初始化
  // Serial2.begin(115200, SERIAL_8N1, RX2, TX2);

  // 底盘初始化
  Chassis_Init();

  //舵机初始化
  Servo_Init();

  //灯光初始化
  Light_Init();


  //线程
  Task_Init();

  //启动按键初始化
  pinMode(4, INPUT_PULLUP);

  //系统按键初始化
  pinMode(0, INPUT_PULLUP);
  
  pinMode(LIGHT, OUTPUT);
  
  digitalWrite(LIGHT,_light);//开灯

  Servo_Put();
  //   delay(1000);
}

void loop() {

  //   test();


  if (!digitalRead(4)) {
    delay(1000);
    Action();
  } else {
    step_test();
  }


  // if (USBSerial.available() > 0) {
  //   // USBSerial.println("start");
  //   Chassis_Incremental_Position(USBSerial.parseInt(), 0, 0);
  //   // delay(1000);
  // }

  delay(50);
}


//抓取放车上
void Grab_And_Put(int id, bool isGround = true) {
  int _waitTime = 400;
  Chassis_Lift_To_Wait();
    // Chassis_Lift_To_Top();
  //   Servo_Turn_to_Front();
  Servo_Select_Goods_ID(id);
  Servo_Put();
  if (isGround) {
    Chassis_Lift_To_Grab(isGround);
  } else {
    Chassis_Lift_To_RAW();
  }

  // delay(_waitTime);
  Servo_Grab();
  delay(_waitTime);
  Chassis_Lift_To_Top();
  // delay(_waitTime);
  Servo_Turn_to_Back();
  // Servo_Select_Goods_ID(id);
  Chassis_Lift_To_Back_Put();
  // delay(_waitTime);
  Servo_Put();
  delay(_waitTime);
  Chassis_Lift_To_Top();
  // delay(_waitTime);
  Servo_Turn_to_Front();
  Chassis_Lift_To_Wait();
}


//从车上抓取放置
//false 放码垛上， true 放地上
void Get_And_Put(int id, bool isground = false) {
  int _waitTime = 400;
  Servo_Select_Goods_ID(id);

  Chassis_Lift_To_Top();
  Servo_Put();
  Servo_Turn_to_Back();
  Chassis_Lift_To_Back_Put();
  Servo_Grab();
  delay(_waitTime);
  Chassis_Lift_To_Top();
  delay(_waitTime);
  Servo_Turn_to_Front();
  Chassis_Lift_To_Put(isground);
  delay(_waitTime);
  Servo_Put();
  delay(_waitTime);
  Chassis_Lift_To_Wait();
}


/*
分步调试6
*/
void step_test() {
  static int mode_switch = 0;

  if (mode_switch == 0) {
    mode_switch = 1;
    USBSerial.println("请输入指令t/T");
    Chassis_Enable(false);
  }
  if (USBSerial.available()) {
    mode_switch = 0;
    char _M = USBSerial.read();
    float _Position;
    if (_M == 't' || _M == 'T') {
      USBSerial.println("选择模式：\r\n1、夹爪状态\r\n2、抬升机构位置\r\n3、抬升机构角度\r\n4、物料ID\r\n5、X移动\r\n6、Y移动\r\n7、Z旋转\r\n8、电机状态\r\n9、色环定位\r\n10、z轴补偿\r\n");
      //       if (USBSerial.available()) {

      //   }
      while (!USBSerial.available())
        ;
      int id = USBSerial.parseInt();
      USBSerial.println(id);
      switch (id) {
        case 1:
          USBSerial.println("夹爪状态\r\n1、夹爪打开\r\n2、夹爪抓取\r\n3、夹爪初始化\r\n4、角度\r\n");
          while (!USBSerial.available())
            ;
          id = USBSerial.parseInt();
          USBSerial.println(id);
          switch (id) {
            case 1:
              Servo_Put();

              break;
            case 2:
              Servo_Grab();

              break;
            case 3:
              Servo_Init();

              break;
            case 4:
              while (!USBSerial.available())
                ;
              id = USBSerial.parseInt();
              USBSerial.println(id);
              Servo_Write(1, id);
            default:
              break;
          }
          break;
        case 2:
          USBSerial.println("抬升机构位置\r\n1、顶部\r\n2、定位位置\r\n3、车物料盘位置\r\n4、地面放置/夹取位置\r\n5、等待位置\r\n6、原料区夹取\r\n7、高度值\r\n");
          while (!USBSerial.available())
            ;
          id = USBSerial.parseInt();
          USBSerial.println(id);
          switch (id) {
            case 1:
              Chassis_Lift_To_Top();
              break;
            case 2:
              Chassis_Lift_To_Grab(false);
              break;
            case 3:
              Chassis_Lift_To_Put();
              break;
            case 4:
              Chassis_Lift_To_Grab(true);
              break;
            case 5:
              Chassis_Lift_To_Wait();
              break;
            case 6:
              Chassis_Lift_To_RAW();
              break;
            case 7:
              USBSerial.println("请输入高度值");
              while (!USBSerial.available())
                ;
              id = USBSerial.parseInt();
              USBSerial.println(id);
              Chassis_Lift_Space(id);
              break;
            default:
              break;
          }
          break;
        case 3:
          USBSerial.println("抬升机构角度\r\n1、前方\r\n2、后方\r\n3、角度\r\n");
          while (!USBSerial.available())
            ;
          id = USBSerial.parseInt();
          USBSerial.println(id);
          switch (id) {
            case 1:
              Servo_Turn_to_Front();
              break;
            case 2:
              Servo_Turn_to_Back();
              break;
            case 3:
              USBSerial.println("请输入角度");
              while (!USBSerial.available())
                ;
              int _angle = USBSerial.parseInt();
              Servo_Write(SERVO_BASE_ID, _angle);
              break;
              // default:
              //   break;
          }
          break;
        case 4:
          USBSerial.println("物料ID\r\n1、ID1\r\n2、ID2\r\n3、ID3\r\n4、角度\r\n");
          while (!USBSerial.available())
            ;
          id = USBSerial.parseInt();
          USBSerial.println(id);
          if (id != 4) {
            Servo_Write(SERVO_GOODS_ID, id);
          } else {
            USBSerial.println("请输入角度");
            while (!USBSerial.available())
              ;
            int _angle = USBSerial.parseInt();
            Servo_Write(SERVO_GOODS_ID, _angle);
          }
          break;
        case 5:
          USBSerial.println("X移动距离,单位mm\r\n>--\r\n");
          while (!USBSerial.available())
            ;
          _Position = USBSerial.parseFloat();
          USBSerial.println(_Position);
          Chassis_Incremental_Position(_Position, 0, 0);
          break;
        case 6:
          USBSerial.println("Y移动距离,单位mm\r\n>--\r\n");
          while (!USBSerial.available())
            ;
          _Position = USBSerial.parseFloat();
          USBSerial.println(_Position);
          Chassis_Incremental_Position(0, _Position, 0);
          break;
        case 7:
          USBSerial.println("Z旋转角度,单位度\r\n>--\r\n");
          while (!USBSerial.available())
            ;
          _Position = USBSerial.parseFloat();
          USBSerial.println(_Position);
          Chassis_Incremental_Position(0, 0, _Position);
          break;
        case 8:
          USBSerial.println("底盘电机状态\r\n1、使能\r\n2、失能\r\n");
          while (!USBSerial.available())
            ;
          id = USBSerial.parseInt();
          USBSerial.println(id);
          switch (id) {
            case 1: Chassis_Enable(true); break;
            case 2: Chassis_Enable(false); break;
          }
        case 9:
          USBSerial.println("圆环定位\r\n1、红色\r\n2、绿色\r\n3、蓝色\r\n");
          while (!USBSerial.available())
            ;
          id = USBSerial.parseInt();
          USBSerial.println(id);
          if (id >= 0 && id <= 3)
            positioning(id);
          break;
        case 10:
          USBSerial.println("执行z校准\r\n");
          positioning(2);
          Calibrate_z();
          positioning(2);
          break;
        default:
          break;
      }
    }
  }
}


/*
运行
*/
void Action() {
  float _z = 86.7;
  //***************************************************启动区前往二维码**********************************************//


  Chassis_Incremental_Position(0, 150, 0);
  Chassis_Incremental_Position(515, 0, 0);

  //识别二维码
  wait_QRcode();


  //***************************************************二维码前往原料区**********************************************//


  //二维码区到原料区
  Chassis_Incremental_Position(920, 0, 0);


  //夹爪置前，准备识别物料
  Servo_Turn_to_Front();

  //下降夹爪
  Chassis_Lift_To_Wait();

  //***************************************************原料区抓取************************************************//


  for (int _a = 0; _a < 2; _a++) {
    Chassis_Lift_To_Wait();

    Chassis_Incremental_Position(0, -60, 0);  //向前抓取

    // //第一次识别并抓取
    if (_a == 0) wati_color(true);

    else wati_color(false);  //第二次识别抓取

    Servo_Goods.write(_Servo_Goods_Start_Angle);  //转盘回中

    Chassis_Incremental_Position(0, 60, 0);  //退回赛道

    digitalWrite(LIGHT,!_light);//开灯
    //***************************************************原料区前往粗加工区************************************************//

    //原料区到粗加工区
    // if (_a == 0) {
    Chassis_Incremental_Position(-410, 0, 0);  //退回赛道中部
    Chassis_Lift_To_Grab(true);                //放下夹爪以降低重心
    Chassis_Incremental_Position(0, 0, _z);    //旋转车头方向
    Chassis_Incremental_Position(1690, 0, 0);  //前进至粗加工区

    Chassis_Incremental_Position(0, 0, _z);  //旋转车头方向，使夹爪朝向码垛

    Chassis_Incremental_Position(0, -60, 0);  //靠近色环

    //***************************************************定位校准************************************************//

    Chassis_Lift_To_Grab(false);  //抬升夹爪

    positioning(2);  //色环定位
    Calibrate_z();   //z矫正
    // positioning(2);  //二次定位

    //***************************************************粗加工区从车上放置到色环上************************************************//
    for (int grab_i = 0; grab_i < 3; grab_i++) {
      switch (color_data[grab_i + _a * 3]) {
        case 1:
          Chassis_Incremental_Position(-150, 0, 0);
          positioning(1);  //色环定位
          Get_And_Put(1, true);
          Chassis_Incremental_Position(150, 0, 0);
          break;
        case 2:
          positioning(2);  //色环定位
          Get_And_Put(2, true);
          break;
        case 3:
          Chassis_Incremental_Position(150, 0, 0);
          positioning(3);  //色环定位
          Get_And_Put(3, true);
          Chassis_Incremental_Position(-150, 0, 0);
          break;
      }
    }

    //***************************************************粗加工区从地面夹取到车上************************************************//
    for (int grab_i = 0; grab_i < 3; grab_i++) {
      switch (color_data[grab_i + _a * 3]) {
        case 1:
          Chassis_Incremental_Position(-150, 0, 0);
          Grab_And_Put(1, true);
          Chassis_Incremental_Position(150, 0, 0);
          break;
        case 2:
          Grab_And_Put(2, true);
          break;
        case 3:
          Chassis_Incremental_Position(150, 0, 0);
          Grab_And_Put(3, true);
          Chassis_Incremental_Position(-150, 0, 0);
          break;
      }
    }

    Servo_Turn_to_Front();

    //***************************************************定位校准************************************************//
    Chassis_Lift_To_Grab(false);  //抬升夹爪

    // positioning(2);  //色环定位
    // Calibrate_z();//z矫正
    // positioning(2); //二次定位

    Chassis_Incremental_Position(0, 100, 0);  //退回赛道
    Chassis_Lift_To_Grab(true);
    // delay(500);

    Servo_Goods.write(_Servo_Goods_Start_Angle);  //物料盘回中

    // Chassis_Lift_To_Top();
    // Servo_Turn_to_Back();

    //***************************************************粗加工区前往精加工区************************************************//

    //粗加工区到精加工区
    Chassis_Incremental_Position(-785, 0, 0);
    // delay(5000);
    Chassis_Incremental_Position(0, 0, -_z);
    // delay(2000);
    Chassis_Incremental_Position(-785, 0, 0);
    // delay(5000);
    Chassis_Lift_To_Wait();

    Chassis_Incremental_Position(0, -100, 0);

    //***************************************************定位校准************************************************//
    Chassis_Lift_To_Grab(false);  //抬升夹爪

    positioning(2);  //色环定位
    Calibrate_z();   //z矫正
    // positioning(2);  //二次定位/


    //***************************************************精加工区从车上放置到色环上************************************************//
    for (int grab_i = 0; grab_i < 3; grab_i++) {
      switch (color_data[grab_i + _a * 3]) {
        case 1:
          Chassis_Incremental_Position(-150, 0, 0);
          positioning(1);  //色环定位
          Get_And_Put(1, !_a);
          Chassis_Incremental_Position(150, 0, 0);
          break;
        case 2:
          positioning(2);  //色环定位
          Get_And_Put(2, !_a);
          break;
        case 3:
          Chassis_Incremental_Position(150, 0, 0);
          positioning(3);  //色环定位
          Get_And_Put(3, !_a);
          Chassis_Incremental_Position(-150, 0, 0);
          break;
      }
    }

    //***************************************************定位校准************************************************//


    // Chassis_Incremental_Position(0, 110, 0);

    if (_a == 0) {
      Servo_Turn_to_Front();
      Chassis_Lift_To_Grab(false);  //抬升夹爪

      positioning(2);  //色环定位/

      Servo_Goods.write(_Servo_Goods_Start_Angle);

      Chassis_Incremental_Position(0, 110, 0);
      // Servo_Turn_to_Front();
      Chassis_Lift_To_Grab(true);

      //***************************************************粗加工区前往原料区************************************************//
      // Servo_Turn_to_Back();
      //精加工区到原料区

      Chassis_Incremental_Position(-870, 0, 0);
      // delay(5000);
      Chassis_Incremental_Position(0, 0, -_z);
      // delay(2000);
      Chassis_Lift_To_Wait();

      Chassis_Incremental_Position(-365, 0, 0);
    }
    // delay(5000);
  }

  //***************************************************回家************************************************//
  Chassis_Incremental_Position(0, 110, 0);
  Chassis_Incremental_Position(-50, 0, 0);
  Chassis_Incremental_Position(0, 0, -_z);
  Chassis_Lift_To_Grab(true);
  Chassis_Incremental_Position(-1620, 0, 0);
  Chassis_Incremental_Position(0, -900, 0);
  // delay(7000);
  Chassis_Incremental_Position(-195, -130, 0);
  // delay(2000);
  while (1)
    ;
}